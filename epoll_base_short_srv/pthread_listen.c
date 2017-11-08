/*
 * pthread_listen.c
 *
 *  Created on: Oct 17, 2016
 *      Author: adan
 */

#include "pthread_listen.h"


//listen pthread 线程初始化函数(创建epoll->创建listen socket->add li_sock->epoll)
int fpthread_listen_init(struct static_val *s_val){
  //创建epoll
  s_val->epoll_fd = epoll_create(epoll_max_sfd);
  if(s_val->epoll_fd == -1){
	printf("listen pthread create epoll fail, errno: %d\n",errno);
    s_val->sig_exit++;
    return -1;
  }

  s_val->sfd_li = cre_sock_li(srv_ip, srv_port);//绑定端口,开始监听
  if(s_val->sfd_li == -1){
    printf("*function inet_listen() fail, program has quit*\n");
    s_val->sig_exit++;
    return -1;
  }

  //加入是将listen_fd 和listen_ev 兴趣加入到epoll_fd 中
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET ;		//!> 读+边缘触发ET
  ev.data.fd = s_val->sfd_li;			//!> 将listen_fd 加入
  if(epoll_ctl(s_val->epoll_fd, EPOLL_CTL_ADD, s_val->sfd_li, &ev) < 0){
    printf("listen pthread epoll_ctl fail, add listen socket %d into epoll fail, errno: %d\n", s_val->sfd_li, errno);
    s_val->sig_exit++;
    return -1;
  }

  printf("~.~.~.~.~.~.init listen pthread success~.~.~.~.~.~.~.\n");
  return 0;//初始化成功
}


//listen pthread 线程函数
void* fpthread_listen(struct static_val *s_val){
  int tmp = fpthread_listen_init(s_val);//启动线程初始化函数

  //无论初始化是否成功, 马上激活主线程继续工作, 如果错误, 就会拉高sig_exit
  pthread_mutex_lock(&s_val->tmp_mutex);
  pthread_cond_signal(&s_val->tmp_cond);
  pthread_mutex_unlock(&s_val->tmp_mutex);
  if(tmp == -1)
     return NULL;//监听线程init 错误, 退出
  ///////////////////
  //监听线程初始化成功//
  ///////////////////
  struct as_info as;
  as_init(&as);//创造私有临时栈, 并初始化
  s_val->cur_fds++;//因为epoll 已经有listen sfd 了, 所以先+1, epoll cur_fds 计数必须准确

  ///////////////////////////////////////////////////////////////////////////
  printf("~.~.~.~.~.~.inet tcp socket: li-pthread into loops now~.~.\n\n");
  for(;;){
    if_event(s_val,&as);		//调用判断事件函数

    if(s_val->sig_exit != 0)	//quit?
      break;

    if(aq_is_empty(s_val->paq_io) == 0){//任务队列为空?
      //当任务队列不为空时
      for(tmp = 0;tmp < pool_max;tmp++){//找出空闲的io 线程
	    if(s_val->sig_io[tmp] == 0){//io 线程状态 = 0 ?
	      //减少冲突--只对空闲线程发信号
	      pthread_mutex_lock(&s_val->mutex_io[tmp]);
	      pthread_cond_signal(&s_val->cond_io[tmp]);
	      pthread_mutex_unlock(&s_val->mutex_io[tmp]);
	      break;//一旦启动了一条线程，即可返回
	    }
      }
      //如果都没有空闲的IO 线程, 即任务队列开始积压任务--> 但每次都会取空所有任务
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  //listen pthread sig_exit 退出 -- 回收线程自身的资源
  shutdown(s_val->sfd_li,2);//关闭监听socket
  close(s_val->sfd_li);
  s_val->sfd_li = 0;
  //关闭epoll
  close(s_val->epoll_fd);
  s_val->epoll_fd = 0;

  printf("listen pthread has quit, clean model is good.\n");
  s_val->sig_exit++;//拉起退出标志
  return NULL;
}


//监听线程启动函数(主函数调用)
int start_plisten(struct static_val *s_val){
  //创建 listen pthread
  pthread_t pthread;
  if(pthread_create(&pthread,NULL,(void*)fpthread_listen,s_val) != 0){
    printf("create listen pthread fail, errno: %d\n",errno);
    return -1;
  }
  //detach 模式, 线程退出内核会回收一切, 不保留信息记录, 不会成为僵尸线程
  if(pthread_detach(pthread) != 0){
    s_val->sig_exit++;
    return -1;
  }
  //等待监听线程创建完毕才返回
  pthread_mutex_lock(&s_val->tmp_mutex);
  pthread_cond_wait(&s_val->tmp_cond,&s_val->tmp_mutex);
  pthread_mutex_unlock(&s_val->tmp_mutex);

  if(s_val->sig_exit != 0)//判断listen 线程是否创建成功
    if(s_val->sig_exit == 1){
      shutdown(s_val->sfd_li,2);//创建失败,回收本函数调用的资源
      close(s_val->sfd_li);
      return -1;
    }

  s_val->pid_li = pthread;//记录pid
  printf("listen pthread has started\n\n");
  return 0;
}
