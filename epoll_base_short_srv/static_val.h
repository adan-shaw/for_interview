/*
 * static_val.h
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#define pool_max 4//pthread_pool 4条io 线程(其实一般2 条也可以)

#define frame_buf_size 256//for lua table 512-256 = 256(256 可变)



//#define srv_ip "192.168.56.1"
#define srv_ip "127.0.0.1"
#define srv_port 9213



#define	epoll_max_sfd 20480//ulimit -n 值一样

#define timeout_epoll_wait 1000//epoll_wait timeout 1s
