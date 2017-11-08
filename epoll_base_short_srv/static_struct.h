/*
 * static_struct.h
 *
 *  Created on: Sep 26, 2017
 *      Author: adan
 */

#include "static_val.h"
#include "static_db.h"

//加密钥匙头
struct encode_key{
  unsigned int key1;
  unsigned int key2;
  unsigned int key3;
  unsigned int key4;
  unsigned int key5;
  unsigned int key6;
  unsigned int key7;
  unsigned int key8;
};


//数据帧格式(唯一格式, 不再精简了...)
struct data_frame{
  double xkey[8];   //4*8
  double comm;		//4
  double uid;		//4
  double age;		//4
  double sex;		//4
  double country;	//电话号码前缀: china 086
  double city;		//大沙镇 526200
  double code;		//4
  double ts;		//4

  double d_backup1;	//4
  double d_backup2;	//4
  double d_backup3;	//4
  double d_backup4;	//4
  double d_backup5;	//4
  double d_backup6;	//4
  double d_backup7;	//4
  double d_backup8;	//4

  char uname[16];	//16
  char upw[16];		//16

  char s_backup1[32];//32
  char s_backup2[32];//32
  char s_backup3[32];//32
  char s_backup4[32];//32
  char buf[frame_buf_size];	//frame_buf_size
};


//全局变量
struct static_val{
  unsigned int sig_exit;	//退出标志
  unsigned int sig_srv_stat;//标志_服务器运行状态--1启动/停止0=false
           int count_test;	//互交次数总累加
           int count_err;	//互交错误总累加

	       int sfd_li;		//监听socket
  unsigned int pid_li;		//监听线程PID
  unsigned int err_li;		//监听线程错误

	       int epoll_fd;
	       int cur_fds;		//epoll 中的fd 计数

  //io 线程池模块
  void* paq_io;		        //io 任务队列

  unsigned int pid_io[pool_max];		//线程的PID
  unsigned int test_io[pool_max];		//线程的服务次数累加
  unsigned int err_io[pool_max];		//线程的错误次数累加
  pthread_cond_t cond_io[pool_max];	    //线程的同步条件变量
  pthread_mutex_t mutex_io[pool_max];	//线程的同步锁
  unsigned int sig_io[pool_max];		//线程的状态表示-正在使用=+1 | 闲=0(减少冲突)

  int pid_tmp;			    //创建线程组时用到的临时变量
  pthread_cond_t tmp_cond;
  pthread_mutex_t tmp_mutex;

  //********************************************************************
  //redis 模块
  //********************************************************************
  //redisContext *c;
  //pthread_mutex_t redis_mutex;
};
