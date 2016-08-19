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
#include"accept_request.h"
#include"server_dynamic.h"
#include"get_line.h"
#define BUFSIZE 1024
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
void serve_file(int client, const char *filename);
void * accept_request(void * arg);
void cat(int client, FILE *file);
void headers(int client,const char *filename);
void not_found(int client);
void unimplemented(int client);


void serve_file(int client, const char *filename)
{   
    FILE *source=NULL;
    int rlen=1;
    char buf[1024];
    buf[0] ='A'; buf[1]='\0';
    while((rlen >0)&&strcmp("\n",buf))
    {
        rlen=get_line(client,buf,sizeof(buf));

    }
    source=fopen(filename,"r");
    if(source==NULL)
        not_found(client);
    else
    {
        headers(client,filename);
        cat(client,source);
    }
    fclose(source);
}
void cat(int client, FILE *file)
{
    char buf[1024];
    fgets(buf,sizeof(buf),file);
    while(!feof(file))
    {
        send(client,buf,strlen(buf),0);
        fgets(buf,sizeof(buf),file);
    }
}                                                                                  
void * accept_request(void * arg)
{
    int rlen, i, j,cgi=0;
 info * clientinfo=(info *) arg;
    char method[255];
    char uri[255];
    char path[255];
    char strip[128];
    char buf[BUFSIZE];
    char *clientip, *query_string='\0';
    struct stat st;
    clientip=inet_ntop(AF_INET,&clientinfo->skaddr.sin_addr.s_addr,strip,sizeof(strip));
    printf("client ip %s\t port %d\n",clientip,ntohs(clientinfo->skaddr.sin_port));     
    rlen=get_line(clientinfo->cfd,buf,sizeof(buf));
    i=0; j=0;
    while((!isspace(buf[i]))&&(j<sizeof(method)-1))
    {
        method[j]=buf[i];
        i++; j++;
    }
    method[j]='\0';
    printf("mothod: %s\n",method);
    if(strcasecmp(method, "GET")&&strcasecmp(method, "POST"))
    {
        unimplemented(clientinfo->cfd);                                            
        return;
    }
	if(strcasecmp(method,"POST")==0)
		cgi=1;
    j=0;
    while(isspace(buf[i])&&(i<sizeof(buf)-1))
        i++;
    
    while((!isspace(buf[i]))&&(j<sizeof(uri)-1))
    {
        uri[j]=buf[i];
        i++; j++;
    }
    uri[j]='\0';
	/***********************
	 *
	 * read request
	 * *******************/

	if(strcasecmp(method,"GET")==0)
	{
		query_string=uri;
		while(*query_string != '?'&&*query_string != '\0')
			query_string++;
		if(*query_string=='?')
		{
			cgi=1;
			*query_string='\0';
			query_string++;
		}
	}
	/*************************
	 *
	 *read cgi argvs
	 ****************************/


    sprintf(path,"htdocs%s",uri);
    printf("path:%s\n",path);
    if((path[strlen(path)-1])=='/')
        strcat(path,"index.html");
    if(stat(path,&st)==-1)
    {
        while((rlen>0)&&strcmp("\n",buf))
            rlen=recv(clientinfo->cfd,buf,sizeof(buf),0);
        not_found(clientinfo->cfd);
    }else
    {
        if((st.st_mode & S_IFMT)==S_IFDIR)
            strcat(path,"/index.html");
		if((st.st_mode & S_IXUSR)||(st.st_mode & S_IXGRP)||
				(st.st_mode & S_IXOTH))
			cgi=1;
		if(!cgi)
			serve_file(clientinfo->cfd,path);
		else
			server_dynamic(clientinfo->cfd,path,query_string,method);
    }                                                                              

    close(clientinfo->cfd);
    return (void *)0;

}


void headers(int client,const char *filename)
{
    char buf[1024];
    (void)filename;
    strcpy(buf,"HTTP/1.0 200 OK\r\n");
    send(client,buf,strlen(buf),0);
    strcpy(buf,SERVER_STRING);
    send(client,buf,strlen(buf),0);
    strcpy(buf,"Content-Type: text/html\r\n");
    send(client,buf,strlen(buf),0);

    strcpy(buf,"\r\n");
    send(client,buf,strlen(buf),0);
}

void not_found(int client)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
    send(client, buf,strlen(buf),0);                                               
    sprintf(buf,SERVER_STRING);
    send(client, buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
  send(client, buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client, buf,strlen(buf),0);
    sprintf(buf,"<HTML><TITLE>Not Found</TITLE>\r\n");
    send(client, buf,strlen(buf),0);
    sprintf(buf,"<BODY><P>The server could not fulfill\r\n");
    send(client, buf,strlen(buf),0);
    sprintf(buf,"your request because the resource specified\r\n");
    send(client, buf,strlen(buf),0);
    
    sprintf(buf,"is unavailable or nonexistent\r\n");
    send(client, buf,strlen(buf),0);

    sprintf(buf,"</BODY></HTML>\r\n");
    send(client, buf,strlen(buf),0);
}
void unimplemented(int client)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.0 501 NOT Implemented\r\n");
    send(client, buf,strlen(buf),0);
    sprintf(buf,SERVER_STRING);
    send(client, buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");                                    
    send(client, buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(client, buf,strlen(buf),0);
 sprintf(buf,"<HTML><TITLE>Not Method Not Implemented</TITLE>\r\n");
    send(client, buf,strlen(buf),0);
    sprintf(buf,"<BODY><P>HTTP request mothod not support.\r\n");
    send(client, buf,strlen(buf),0);
    

    sprintf(buf,"</BODY></HTML>\r\n");
    send(client, buf,strlen(buf),0);
}           
