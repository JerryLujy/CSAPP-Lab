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
int get_request(rio_t * rio, char * method, char * uri, char * version);
void parse_uri(char * uri, char * hostname, char * query);
void build_request(rio_t * rio, char * request, char * hostname, char * query);
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

  listenfd = Open_listenfd(argv[1]);

  while (1) {
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
		port, MAXLINE, 0);
    printf("Accepted connection from (%s:%s)\n\n", hostname, port);
    serve(connfd);
    printdetail("Connection closed with (%s:%s)\n\n", hostname, port);
    Close(connfd);
  }
  return 0;
}

void serve(int fd)
{
  char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char hostname[MAXLINE], query[MAXLINE];
  char request[MAXBUF];
  rio_t rio;

  Rio_readinitb(&rio, fd);
  /* Read client request line */
  if (!get_request(&rio, method, uri, version))
    return;
  /* Get hostname and query string from uri */
  parse_uri(uri, hostname, query);
  /* Build the request to be send out to server */
  build_request(&rio, request, hostname, query);
}

int get_request(rio_t * rio, char * method, char * uri, char * version)
{
  char buf[MAXLINE];
  do {
    Rio_readlineb(rio, buf, MAXLINE);
  } while (strcmp(buf, "\r\n") == 0);
  printdetail("%s", buf);
  /* Check request line validity */
  if (sscanf(buf, "%s %s %s", method, uri, version) != 3 ||
      strstr(version, "HTTP/1.") == NULL) {
    proxyerror(rio->rio_fd, buf, 400, "Bad request",
	       "The proxy cannot understand the request");
    return 0;
  }
  if (strcasecmp(method, "GET") != 0) {
    proxyerror(rio->rio_fd, method, 501, "Not Implemented",
	       "Proxy does not support the request method");
    return 0;
  }
  return 1;
}

void build_request(rio_t * rio, char * request, char * hostname, char * query)
{
  char buf[MAXLINE];
  sprintf(request, "GET %s HTTP/1.0\r\n", query);
  sprintf(request, "%sHost: %s\r\n", request, hostname);
  sprintf(request, "%s%s\r\n", request, user_agent_hdr);
  sprintf(request, "%sConnection: close\r\n", request);
  sprintf(request, "%sProxy-connection: close\r\n", request);
  do {
    Rio_readlineb(rio, buf, MAXLINE);
    printdetail("%s", buf);
    /* These headers intercepted by proxy */
    if (strncasecmp(buf, "User-Agent", 10) == 0 ||
	strncasecmp(buf, "Connection", 10) == 0 ||
	strncasecmp(buf, "Proxy-connection", 16) == 0)
      continue;
    sprintf(request, "%s%s", request, buf);
  } while (strcmp(buf, "\r\n"));
  printdetail("-----Send to server-----\n%s", request);
}

void parse_uri(char * uri, char * hostname, char * query)
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
