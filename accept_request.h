#ifndef _ACCEPT_REQUEST_H
#define _ACCEPT_REQUEST_H

#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
typedef struct {
    struct sockaddr_in skaddr;
    int cfd;
} info;
int get_line(int sock, char*buf, int size);
void serve_file(int client, const char *filename);
void * accept_request(void * arg);
void cat(int client, FILE *file);
void headers(int client,const char *filename);                                     
void not_found(int client);
void unimplemented(int client);
#endif 
