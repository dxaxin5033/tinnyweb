/*************************************************************************
    > File Name: get_line.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2016年08月18日 星期四 21时40分34秒
 ************************************************************************/

#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/socket.h>
int get_line(int sock, char*buf, int size)                                         
{
    int i=0;
    char c='\0';
    int n;
    while((i<size-1)&&(c!='\n'))
    {
        n= recv(sock,&c,1,0);
        if(n>0)
        {
            if(c=='\r')
            {
                n= recv(sock,&c,1,MSG_PEEK);                                       
                if((n>0)&&(c=='\n'))
                    recv(sock,&c,1,0);
                else
                    c='\n';
            }
            buf[i]=c;
            i++;
        }else
            c='\n';
    }
    buf[i]='\0';
    return i;
}
