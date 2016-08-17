
#include<stdio.h>                                                                  
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include"accept_request.h"
#define MAXLINK 382
#define SERVER_PORT 8000
#define BUFSIZE 1024
int main()
{
    struct sockaddr_in serveraddr,clientaddr;
    int sockfd ,clientfd, addrlen, rlen;
    pthread_t tid;
    int i,fd; 
    info safd[MAXLINK];
    sockfd= socket(PF_INET,SOCK_STREAM,0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family= AF_INET;
    serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
    serveraddr.sin_port=htons(SERVER_PORT);
    bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    i=0;
    listen(sockfd, 18);
    printf("http runing on port %d\n",SERVER_PORT);
    while(i<MAXLINK)
    {

        
        addrlen= sizeof(clientaddr);
        clientfd = accept(sockfd,(struct sockaddr*) &clientaddr,&addrlen);
        if(clientfd== -1)
        {
            perror("accept");
            exit(1);
        }
        safd[i].skaddr=clientaddr;
        safd[i].cfd=clientfd;
        if(pthread_create(&tid,NULL,accept_request,(void *)&safd[i])!=0)
        {
            perror("pthread_create");
            exit(1);
                                                                                   
        }
      i++;
    }
    close(sockfd);
    return 0;

}   
                                               
