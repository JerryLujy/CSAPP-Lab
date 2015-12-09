#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define verbose
#ifdef verbose
# define printdetail(...) printf(__VA_ARGS__)
#else
# define printdetail(...)
#endif

/* You won't lose style points for including this long line in your code */
static const char * user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3";

void serve(int fd);
int  get_request(rio_t * rio, char * method, char * uri, char * version);
void parse_uri(char * uri, char * hostname, char * port, char * query);
void build_request(rio_t * rio, char * request, char * hostname,
		   char * port, char * query);
int  send_request(char * hostname, char * port, char * request, int clientfd);
void send_response(rio_t * rio, int connfd);
void proxyerror(int fd, char * cause, int code,
		char * shortmsg, char * longmsg);

int main(int argc, char * * argv)
{
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  struct sockaddr_storage clientaddr;
  socklen_t clientlen = sizeof(clientaddr);

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
  }
  Signal(SIGPIPE, SIG_IGN);

  listenfd = Open_listenfd(argv[1]);

  while (1) {
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
		port, MAXLINE, 0);
    printf("****************************************\n");
    printf("Accepted connection from (%s:%s)\n", hostname, port);
    serve(connfd);
    printf("Connection closed with (%s:%s)\n\n", hostname, port);
    Close(connfd);
  }
  return 0;
}

void serve(int fd)
{
  char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char hostname[MAXLINE], port[MAXLINE], query[MAXLINE];
  char request[MAXBUF];
  rio_t rio;
  int serverfd;

  /* Read client request line */
  Rio_readinitb(&rio, fd);
  if (!get_request(&rio, method, uri, version))
    return;
  /* Get hostname and query string from uri */
  parse_uri(uri, hostname, port, query);
  /* Build the request to be send out to server */
  build_request(&rio, request, hostname, port, query);
  /* Send request to server */
  if ((serverfd = send_request(hostname, port, request, fd)) < 0)
    return;
  /* Read reponse from server and forward to client */
  Rio_readinitb(&rio, serverfd);
  send_response(&rio, fd);
  Close(serverfd);
}

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
  printdetail("-----Send to server-----\n%s", request);
}

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
  printdetail("Connected to server %s on port %s\n", hostname, port);
  Rio_writen(serverfd, request, strlen(request));
  printf("Sent request to server %s on port %s\n", hostname, port);
  return serverfd;
}

void send_response(rio_t * rio, int connfd)
{
  int contentlen = 0;
  int istext = 0;
  char buf[MAXBUF];
  /* Read response headers and extract content information */
  do {
    Rio_readlineb(rio, buf, MAXBUF);
    Rio_writen(connfd, buf, strlen(buf));
    printdetail("%s", buf);
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
    printf("Received response from server %d bytes\n", contentlen);
    while (contentlen > 0) {
      size_t n = (contentlen > MAXBUF) ? MAXBUF : contentlen;
      ssize_t s = Rio_readnb(rio, buf, n);
      if (s <= 0) break;
      contentlen -= s;
      Rio_writen(connfd, buf, n);
    }
  } else if (istext) {
    printf("Received response from server in text format, length unknown\n");
    while (Rio_readlineb(rio, buf, MAXBUF) > 0) {
      Rio_writen(connfd, buf, strlen(buf));
    }
  }
}

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
}
