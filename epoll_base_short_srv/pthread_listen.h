/*
 * pthread_listen.h
 *
 *  Created on: Sep 26, 2017
 *      Author: adan
 */

#include <sys/socket.h>
#include <sys/resource.h>	/* 设置最大的连接数需要setrlimit */
#include <sys/epoll.h>		/* epoll头文件 */
#include <fcntl.h>	        /* nonblocking需要 */

#include "static_struct.h"
#include "./array_struct/array_stack.h"


//监听线程启动函数(主函数调用)
int start_plisten(struct static_val *s_val);

//listen pthread 线程初始化函数(创建epoll->创建listen socket->add li_sock->epoll)
int fpthread_listen_init(struct static_val *s_val);

//listen pthread 线程函数(listen pthread main loops)
void* fpthread_listen(struct static_val *s_val);
