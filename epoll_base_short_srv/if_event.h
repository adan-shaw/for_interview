/*
 * if_event.h
 *
 *  Created on: Sep 26, 2017
 *      Author: adan
 */

#include <sys/socket.h>
#include <sys/resource.h>	/* 设置最大的连接数需要setrlimit */
#include <sys/epoll.h>		/* epoll头文件 */
#include <fcntl.h>	        /* nonblocking需要 */
#include <netdb.h>			/* sockaddr_in 结构体需要 */
#include "static_struct.h"
#include "./array_struct/array_queue.h"
#include "./array_struct/array_stack.h"



//判断epoll 中触发的fd事件类型, 是IO 任务则返回触发集合
void if_event(struct static_val *s_val,struct as_info *pas);

//添加sfd 进epoll(设置重用, 设置nonblocking, 设置如果一端主动关闭, 另一端也必须关闭)
//这样如果服务器主动关闭剔除客户端,客户端强制关闭
int add_epoll(struct static_val *s_val, int sfd_acc);
