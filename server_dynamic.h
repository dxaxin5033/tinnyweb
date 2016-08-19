/*************************************************************************
    > File Name: server_dynamic.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2016年08月18日 星期四 21时29分00秒
 ************************************************************************/

#ifndef _SERVER_DYNAMIC_H
#define _SERVER_DYNAMIC_H
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include"server_dynamic.h"
void server_dynamic(int fd, char *filnanme, char *cgiargs,const char * method);
#endif
