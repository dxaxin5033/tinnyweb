/*************************************************************************
    > File Name: get_line.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2016年08月18日 星期四 21时44分16秒
 ************************************************************************/
#ifndef _GET_LINE_H
#define _GET_LINE_H
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/socket.h>
int get_line(int sock, char*buf, int size);
#endif
