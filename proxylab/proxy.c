/*
 * proxy.c - A simple multithreaded caching web proxy
 *
 * Handles basic HTTP GET requests, and provides 1 Mb caching capability
 *
 * Author: Jieyu Lu      Andrew ID: jieyul1
 */
#include <stdio.h>
#include "csapp.h"
#include "cache.h"

//#define verbose
#ifdef verbose
# define printdetail(...) printf(__VA_ARGS__)
#else
# define printdetail(...)
#endif

/* You won't lose style points for including this long line in your code */
static const char * user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3";

void * serve(void * vargp);
int    get_request(rio_t * rio, char * method, char * uri, char * version);
void   parse_uri(char * uri, char * hostname, char * port, char * query);
void   build_request(rio_t * rio, char * request, char * hostname,
		   char * port, char * query);
int    send_request(char * hostname, char * port, char * request, int clientfd);
size_t send_response(rio_t * rio, int connfd, char * response);
void   proxyerror(int fd, char * cause, int code,
		char * shortmsg, char * longmsg);

/* Clean up when terminating the proxy */
void sigint_handler(int sig)
{
  char * msg = "Proxy has exited\n";
  cache_destroy();
  Rio_writen(STDOUT_FILENO, msg, strlen(msg));
  exit(0);
}

int main(int argc, char * * argv)
{
  int listenfd, * connfd;
  char hostname[MAXLINE], port[MAXLINE];
  struct sockaddr_storage clientaddr;
  socklen_t clientlen = sizeof(clientaddr);
  pthread_t tid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
  }
  Signal(SIGINT, sigint_handler);
  Signal(SIGPIPE, SIG_IGN);
  
  cache_init();
  listenfd = Open_listenfd(argv[1]);

  while (1) {
    connfd = Malloc(sizeof(int));
    *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
		port, MAXLINE, 0);
    Pthread_create(&tid, NULL, serve, (void *)connfd);
    printf("******************************************************\n");
    printf("Thread %lu Accepted connection from (%s:%s)\n",
	   (unsigned long)tid, hostname, port);
  }
  return 0;
}

/*
 * serve - thread routine that provides serice to one client.
 *
 * It reads the request, forwards the request to remote server if content
 * has not been cached, reads the reponse from server, and forwards the 
 * response to the client.
 */
void * serve(void * vargp)
{
  char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char hostname[MAXLINE], port[MAXLINE], query[MAXLINE];
  char request[MAXBUF];
  size_t contentlen;
  char response[MAX_OBJECT_SIZE];
  rio_t rio;
  int serverfd;

  int connfd = *((int *)vargp);
  Pthread_detach(pthread_self());
  Free(vargp);
  /* Read client request line */
  Rio_readinitb(&rio, connfd);
  if (!get_request(&rio, method, uri, version)) {
    Close(connfd);
    return NULL;
  }
  /* Try load from cache */
  contentlen = cache_load(uri, response);
  if (contentlen > 0) {
    /* Cache hit, send content in cache to the client */
    printf("Thread %lu Fetched %zu bytes from cache, transaction finished\n",
	   (unsigned long)pthread_self(), contentlen);
    rio_writen(connfd, response, contentlen);
    Close(connfd);
    return NULL;
  }
  /* Get hostname and query string from uri */
  parse_uri(uri, hostname, port, query);
  /* Build the request to be send out to server */
  build_request(&rio, request, hostname, port, query);
  /* Send request to server */
  if ((serverfd = send_request(hostname, port, request, connfd)) < 0) {
    Close(connfd);
    return NULL;
  }
  /* Read reponse from server and forward to client
     Store the response to cache if it can be stored */
  Rio_readinitb(&rio, serverfd);
  contentlen = send_response(&rio, connfd, response);
  if (contentlen > 0) {
    /* Response can be stored in cache */
    cache_save(uri, response, contentlen);
    printf("Thread %lu Stored %zu bytes into cache\n",
	   (unsigned long)pthread_self(), contentlen);
  }
  Close(serverfd);
  Close(connfd);
  printf("Thread %lu Finished proxy transaction\n",
	 (unsigned long)pthread_self());
  return NULL;
}

/*
 * get_request - reads and parses the GET request line
 *
 * Returns 1 if OK, returns 0 on error
 */
int get_request(rio_t * rio, char * method, char * uri, char * version)
{
  char buf[MAXLINE];
  do {
    Rio_readlineb(rio, buf, MAXLINE);
  } while (strcmp(buf, "\r\n") == 0);
  
  printf("%s", buf);
  /* Check request line validity */
  if (sscanf(buf, "%s %s %s", method, uri, version) != 3 ||
      strstr(version, "HTTP/1.") == NULL) {
    proxyerror(rio->rio_fd, buf, 400, "Bad request",
	       "Proxy cannot understand the request");
    return 0;
  }
  if (strcasecmp(method, "GET") != 0) {
    proxyerror(rio->rio_fd, method, 501, "Not Implemented",
	       "Proxy does not support the request method");
    return 0;
  }
  return 1;
}

/*
 * build_request - build the request to be sent to remote server
 *
 * The function intercepts some of the client request headers and 
 * build the new request string
 */
void build_request(rio_t * rio, char * request, char * hostname,
		   char * port, char * query)
{
  char buf[MAXLINE];
  int flag = 0;// if the request already included host header
  sprintf(request, "GET %s HTTP/1.0\r\n", query);
  sprintf(request, "%s%s\r\n", request, user_agent_hdr);
  do {
    Rio_readlineb(rio, buf, MAXLINE);
    printdetail("%s", buf);
    /* These headers intercepted by proxy */
    if (strncasecmp(buf, "User-Agent", 10) == 0 ||
	strncasecmp(buf, "Connection", 10) == 0 ||
	strncasecmp(buf, "Proxy-connection", 16) == 0)
      continue;
    if (strncasecmp(buf, "Host", 4) == 0)
      flag = 1;
    sprintf(request, "%s%s", request, buf);
  } while (strcmp(buf, "\r\n"));
  /* Append proxy specified header at the end of the request */
  request[strlen(request)-2] = '\0';
  if (!flag) {
    if (strlen(port) == 0)
      sprintf(request, "%sHost: %s\r\n", request, hostname);
    else
      sprintf(request, "%sHost: %s:%s\r\n", request, hostname, port);
  }
  sprintf(request, "%sConnection: close\r\n", request);
  sprintf(request, "%sProxy-connection: close\r\n\r\n", request);
  printdetail("Thread %lu Formatted header, ready to send\n%s",
	      (unsigned long)pthread_self(), request);
}

/* 
 * send_request - sends request to the remote server
 * 
 * Returns the file descriptor of the socket connecting remote server
 */
int send_request(char * hostname, char * port, char * request, int clientfd)
{
  int serverfd;
  if (strlen(port) == 0) {//default to port 80
    strcpy(port, "80");
  }
  if ((serverfd = open_clientfd(hostname, port)) < 0) {
    proxyerror(clientfd, hostname, 400, "Bad request",
	       "Proxy failed to connect to");
    return serverfd;
  }
  printdetail("Thread %lu Connected to server %s on port %s\n",
	      (unsigned long)pthread_self(), hostname, port);
  if (rio_writen(serverfd, request, strlen(request)) < strlen(request)) {
    proxyerror(clientfd, hostname, 400, "Bad request",
	       "Proxy failed to send request to");
    return serverfd;
  }
  printf("Thread %lu Sent request to server %s on port %s\n",
	 (unsigned long)pthread_self(), hostname, port);
  return serverfd;
}

/*
 * send_response - reads response from server and then forwards to the client
 *
 * Stores the server response in memory for caching if the response size 
 * less than maximum size allowed.
 * Returns response size if can be stored in cache. Returns zero otherwise.
 */
size_t send_response(rio_t * rio, int connfd, char * response)
{
  int contentlen = 0;
  size_t responselen = 0;
  int istext = 0;
  char buf[MAXBUF];
  /* A flag to indicate whether or not to cache the response */
  int to_cache = 0;
  memset(response, 0, MAX_OBJECT_SIZE);
  /* Read response headers and extract content information */
  do {
    if (rio_readlineb(rio, buf, MAXBUF) < 0)
      return 0;
    if (rio_writen(connfd, buf, strlen(buf)) != strlen(buf))
      return 0;
    printdetail("%s", buf);
    /* Save to response buffer */
    strcat(response, buf);
    responselen += strlen(buf);
    if (strncasecmp(buf, "Content-Type", 12) == 0) {
      if (strncasecmp(buf + 14, "text", 4) == 0)
	istext = 1;
    }
    if (strncasecmp(buf, "Content-Length", 14) == 0) {
      contentlen = atoi(buf + 16);
    }
  } while (strcmp(buf, "\r\n") != 0);
  /* If content length is specified, read response body with that length.
     Otherwise, read response as text */
  if (contentlen > 0) {
    to_cache = (responselen + contentlen) < MAX_OBJECT_SIZE;
    printf("Thread %lu Received %d byte response from server\n",
	   (unsigned long)pthread_self(), contentlen);
    while (contentlen > 0) {
      size_t n = (contentlen > MAXBUF) ? MAXBUF : contentlen;
      ssize_t s = rio_readnb(rio, buf, n);
      if (s <= 0) break;
      if (to_cache) {
	strncat(response, buf, s);
	responselen += s;
      }
      contentlen -= s;
      if (rio_writen(connfd, buf, n) != n) break;
    }
  } else if (istext) {
    printf("Thread %lu Received response from server in text format\n",
	   (unsigned long)pthread_self);
    while (rio_readlineb(rio, buf, MAXBUF) > 0) {
      size_t s = strlen(buf);
      if (rio_writen(connfd, buf, s) != s) break;
      to_cache = (responselen + s) < MAX_OBJECT_SIZE;
      if (to_cache) {
	strcat(response, buf);
	responselen += s;
      }
    }
  }
  if (to_cache)
    return responselen;
  else
    return 0;
}

/*
 * parse_uri - extracts hostname, port and query string from uri
 */
void parse_uri(char * uri, char * hostname, char * port, char * query)
{
  char * ptr;
  /* Skip the "http://" part */
  if ((ptr = strstr(uri, "//")) != NULL)
    uri = ptr + 2;
  /* Split hostname with query */
  ptr = strchr(uri, '/');
  if (ptr == NULL) {
    strcpy(hostname, uri);
    strcpy(query, "/");
  } else {
    *ptr = '\0';
    strcpy(hostname, uri);
    *ptr = '/';
    strcpy(query, ptr);
  }
  /* Split hostname with port */
  ptr = strchr(hostname, ':');
  if (ptr == NULL) {
    strcpy(port, "");
  } else {
    *ptr = '\0';
    strcpy(port, ptr + 1);
  }
}

/*
 * proxyerror - sends back HTML indicating error during proxy
 */
void proxyerror(int fd, char * cause, int code,
		char * shortmsg, char * longmsg)
{
  char buf[MAXLINE], body[MAXBUF];
  
  sprintf(body, "<!DOCTYPE html>\r\n");
  sprintf(body, "%s<html>\r\n", body);  
  sprintf(body, "%s<head>\r\n", body);
  sprintf(body, "%s  <title>Proxy error</title>\r\n", body);
  sprintf(body, "%s</head>\r\n", body);
  sprintf(body, "%s<body style=\"font-family:palatino\">\r\n", body);
  sprintf(body, "%s  <h2>Error %d: %s</h2>\r\n", body, code, shortmsg);
  sprintf(body, "%s  <p>%s: %s</p>\r\n", body, longmsg, cause);
  sprintf(body, "%s  <hr>\r\n", body);
  sprintf(body, "%s  <p><i>The simple proxy server</i></p>\r\n", body);
  sprintf(body, "%s  <p><small>&copy Jieyu Lu, 2015</small></p>\r\n", body);
  sprintf(body, "%s</body>\r\n", body);
  sprintf(body, "%s</html>\r\n", body);

  sprintf(buf, "HTTP/1.0 %d %s\r\n", code, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %zu\r\n\r\n", strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));

  printf("Thread %lu Terminated transaction with error code %d\n",
	 (unsigned long)pthread_self(), code);
}
