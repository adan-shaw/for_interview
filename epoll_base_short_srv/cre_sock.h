/*
 * cre_sock.h
 *
 *  Created on: Sep 24, 2017
 *      Author: adan
 */

//**************************************
//这是一个对socket 创建的封装体
//**************************************

#include <errno.h>
#include <sys/socket.h>//socket 需要
#include <netinet/in.h>//struct sockaddr_in 需要
#include <fcntl.h>//nonblacking 需要

#define max_listen_queue 1024//listen fd accept 等待队列最大值

//创建一个srv 监听socket 并返回fd, 失败返回-1
int cre_sock_li(char* srv_ip, int srv_port);

//创建一个cli fd 并自动conn srv, 成功返回fd, 失败返回-1
int cre_sock_cli(char* srv_ip, int srv_port);

//通用ctl 设置 socket nonblocking
int fcntl_nonblocking(int fd);
