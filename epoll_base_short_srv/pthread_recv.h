/*
 * pthread_recv.h
 *
 *  Created on: Sep 26, 2017
 *      Author: adan
 */

#include <sys/socket.h>

#include "static_struct.h"
#include "./array_struct/array_queue.h"
#include "./array_struct/array_stack.h"

//io 线程池-启动函数
int start_precv(struct static_val *s_val);

//IO work pthread - 线程函数
void* fpthread_recv(struct static_val *s_val);

//互交一次
int recv_once(struct static_val *s_val, int _pid, int sock_tmp);
