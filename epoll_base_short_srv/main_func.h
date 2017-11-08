/*
 * main_func.h
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */

#include <sys/resource.h>	/* 设置最大的连接数需要setrlimit */
#include <sys/epoll.h>		/* epoll头文件 */
#include <fcntl.h>	        /* nonblocking需要 */

#include "static_struct.h"

//服务器--程序初始化
int init_main(struct static_val* s_val);

//服务器--初始化全局变量结构体(结构体是唯一的)
void init_s_val(struct static_val* s_val);

//服务器--命令列表
void echo_comm(void);

//服务器--开始服务
int start_srv(struct static_val* s_val);

//服务器--退出函数
void quit_main(struct static_val* s_val);

//服务器--统计信息显示
void show_srv_info(struct static_val* s_val);
