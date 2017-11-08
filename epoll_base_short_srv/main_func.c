/*
 * main_func.c
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */

#include "main_func.h"
#include "./array_struct/array_queue.h"


//服务器--程序初始化
int init_main(struct static_val* s_val){
  //更改进程拥有的fd 数量极限--jdk 里面不能使用, 必须和修改系统的ulimit -n 值一样
  struct rlimit	rlt;
  rlt.rlim_max = rlt.rlim_cur = epoll_max_sfd;
  if(setrlimit(RLIMIT_NOFILE, &rlt) == -1){
    printf("setrlimit fail, errno: %d\n", errno);
    return -1;
  }

  //创建IO任务队列
  s_val->paq_io = malloc(sizeof(struct aq_info));
  if(s_val->paq_io == NULL)
	return -1;
  aq_init(s_val->paq_io);//初始化IO任务队列

  printf("the server init_main() success\n");
  return 0;
}


//服务器--初始化全局变量结构体
void init_s_val(struct static_val* s_val){
  s_val->sig_exit = 0;		//初始化全局变量
  s_val->sig_srv_stat = 0;
  s_val->count_test = 0;
  s_val->count_err = 0;

  s_val->sfd_li = 0;		//初始化listen 线程变量
  s_val->pid_li = 0;
  s_val->err_li = 0;


  int tmp;					//初始化precv 线程池模块
  for(tmp = 0;tmp < pool_max;tmp++){
    s_val->pid_io[tmp] = 0;
    s_val->test_io[tmp] = 0;
    s_val->err_io[tmp] = 0;
    s_val->sig_io[tmp] = 0;
    pthread_cond_init(&s_val->cond_io[tmp],NULL);//io 线程事件锁初始化
    pthread_mutex_init(&s_val->mutex_io[tmp],NULL);
  }

  s_val->pid_tmp = 0;
  pthread_cond_init(&s_val->tmp_cond,NULL);//同步tmp 模块初始化
  pthread_mutex_init(&s_val->tmp_mutex,NULL);


  //pthread_mutex_init(&s_val->redis_mutex,NULL);//redis 同步模块


  return;//直接返回, 一般不会出错
}


//服务器--命令列表
void echo_comm(void){
  printf("operation command list:\n");
  printf("No.1: start services\n");
  printf("No.2: stop services and quit\n");
  printf("No.3: about us\n");
  printf("No.4: show the server info\n");
  printf("No.0: show command list again\n");
  return;
}


//服务器--开始服务
int start_srv(struct static_val* s_val){
  /*
  //初始化redis 模块--redis 不支持异机启动->不像mysql, 所以本机启动, 默认端口6379
  s_val->c = redisConnect(srv_ip_redis, srv_port_redis);
  if (s_val->c != NULL && s_val->c->err){
    printf("connect redis server %s:%d fail, error: %s\n", srv_ip_redis, srv_port_redis, s_val->c->errstr); // 显示错误信息
	return -1;
  }

  redisReply *reply;
  reply = redisCommand(s_val->c, "SELECT 18");//测试redis 链接是否正常
  if(reply == NULL){
    freeReplyObject(reply);
    return -1;
  }
  freeReplyObject(reply);//释放测试链接

  */
  //启动listen pthread
  if(start_plisten(s_val) == -1){
    //redisFree(s_val->c);//启动监听线程失败, 回收redis db 资源
    return -1;
  }

  //启动recv pthread
  if(start_precv(s_val) == -1){
    //redisFree(s_val->c);//启动监听线程失败, 回收redis db 资源
    return -1;
  }

  return 0;//启动服务成功
}


//服务器--退出函数
void quit_main(struct static_val* s_val){
  s_val->sig_exit++;	//拉高退出标志, 明确退出决心, 同时命令各个线程自己回收部分模块
  printf("starting clean all son pthread ...\n");
  sleep(2);				//等待所有线程回收资源完毕
  printf("all son pthread has quit, go on clean main pthread\n");

  //开始回收全局资源
  //redisFree(s_val->c);  //回收redis
  free(s_val->paq_io);	//回收任务队列
  free(s_val);			//删除全局变量实体
  printf("****************************\n");
  printf("～server has quit, good bye~\n");
  printf("****************************\n");
  return;
}


//服务器--统计信息显示
void show_srv_info(struct static_val* s_val){
  printf("\n***************************************************************************\n");
  //显示全局统计
  printf("  the server ip: %s, port: %d \n  all test count: %d, all error count: %d          power by adan shaw\n", srv_ip, srv_port, s_val->count_test, s_val->count_err);

  //显示listen 模块信息
  if(s_val->pid_li != 0)
    printf("  li-pthread is still working, ");
  else
    printf("  li-pthread is down, ");
  printf("li-error count: %d\n\n", s_val->err_li);

  //IO 线程池 模块信息
  int tmp;
  for(tmp = 0;tmp < pool_max;tmp++)
    if(s_val->pid_io[tmp] != 0){
      printf("io-pthread-%d is still working, ", tmp);
      printf("io-test count: %d, error count is: %d\n", s_val->test_io[tmp], s_val->err_io[tmp]);
    }
    else{
      printf("io-pthread-%d is down, ", tmp);
      printf("io-test count: %d, error count is: %d\n", s_val->test_io[tmp], s_val->err_io[tmp]);
    }

  printf("***************************************************************************\n");
  return;
}

