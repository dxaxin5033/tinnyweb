#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include"server_dynamic.h"
#include"get_line.h"
void server_dynamic(int fd, char *filename, char *cgiargs,const char * method)
{
	int rlen, content_len, tmpfd;
	char buf[1024],*emptylist[]={NULL};
	int cgi_out[2],cgi_in[2];
	sprintf(buf,"HTTP/1.0 200 OK\r\n");
	send(fd, buf,strlen(buf),0);
	
	buf[0]='A'; buf[1]='\0';
	rlen =1;
	int i;
	int c;
	if(strcmp(method,"GET")==0)
	{
		while((rlen>0)&&strcmp("\n",buf))
			rlen=get_line(fd, buf,sizeof(buf));
	}else
	{
		rlen= get_line(fd, buf, sizeof(buf));
		while((rlen>0)&&strcmp("\n",buf))
		{
			buf[15]='\0';
			if(strcasecmp(buf,"Content-Length:")==0)
				content_len=atoi(&buf[16]);
			rlen= get_line(fd, buf, sizeof(buf));
		}
		if(content_len<0){
			perror("content_len<0");
			exit(1);
		}
	}
	if((pipe(cgi_out)<0)||(pipe(cgi_in)<0))
	{
		perror("pipe");
		exit(1);
	}

	if(fork()==0)
	{
		int tmpfd_in, tmpfd_out;
		dup2(STDOUT_FILENO,tmpfd_out);
		dup2(cgi_out[1],STDOUT_FILENO);
		dup2(STDIN_FILENO,tmpfd_in);
		dup2(cgi_in[1],STDIN_FILENO);
		close(cgi_out[0]);
		close(cgi_in[0]);
		setenv("REQUEST_METHOD",method,1);
		if(strcasecmp(method,"GET")==0)
		{
			setenv("QUERY_STRING",cgiargs,1);
		}else if(strcmp(method,"POST")==0)
		{
			setenv("CONTENT_LENGTH",content_len,1);

		}
		execl(filename,filename,NULL);
		dup2(tmpfd_out,STDOUT_FILENO);
		dup2(tmpfd_in,STDIN_FILENO);
		exit(0);
	}else{
		close(cgi_out[1]);
		close(cgi_in[1]);
		if(strcasecmp(method,"POST")==0)
		{
			for(i=0;i<content_len;i++)
			{
				recv(fd,&c,1,0);
				write(cgi_in[0],&c,1);
			}
		}
		while(read(cgi_out[0],&c,1)>0)
			send(fd,&c,1,0);
		close(cgi_out[0]);
		close(cgi_in[0]);
		wait(NULL);
	}

}
