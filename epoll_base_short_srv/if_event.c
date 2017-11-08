/*
 * if_event.c
 *
 *  Created on: Oct 29, 2016
 *      Author: adan
 */

#include "if_event.h"


//判断epoll 中触发的fd事件类型, 是IO 任务则返回触发集合
void if_event(struct static_val *s_val,struct as_info *pas){
  struct aq_info* pm_recv;//继承IO 任务队列
  pm_recv = s_val->paq_io;

  int tmp;
  int sfd_acc;//如果有accept 事件, 需要用到的临时变量
  struct epoll_event ev_tmp;//epoll 感兴趣的事件临时变量(单个)
  struct epoll_event evs[epoll_max_sfd];//epoll 感兴趣的事件(所有)--这个变量很大！
  int wait_fds;//!> epoll_wait 返回的->已发生事件的fd 计数

  //**********************
  struct sockaddr_in addr;
  char ip_tmp[4];
  socklen_t addrlen;
  char ip_cli[16];
  int port_cli;
  //**********************
  //获取epoll 中被触发的fd, 返回到evs 事件数组容器中
  wait_fds = epoll_wait(s_val->epoll_fd, evs, s_val->cur_fds, timeout_epoll_wait);//阻塞方式:第四参数为-1
  if(wait_fds > 0){//判断触发fd 数量是否 > 0
	//循环-处理-所有被触发的fd
    for(tmp = 0;tmp < wait_fds;tmp++){
      //如果是listen fd 发生事件
      if(evs[tmp].data.fd == s_val->sfd_li && s_val->cur_fds < epoll_max_sfd){//!>监听fd 触发的事件
    	//if((sfd_acc = accept(s_val->sfd_li, NULL, NULL)) == -1){//接受客户端
    	if((sfd_acc = accept(s_val->sfd_li, &addr, &addrlen)) == -1){
	      s_val->err_li++;
	      printf("listen pthread accept client error in epollwait(), errno: %d|||error count: %d\n", errno,s_val->err_li);
	      continue;
    	}
        //添加client sfd 到epoll 中
	    if(add_epoll(s_val,sfd_acc) == 0){
	      ++s_val->cur_fds;//epoll 句柄计数+1
	      s_val->count_test++;//服务器次数+1

		  //**************
		  //strcpy(&ip_cli, inet_ntoa(&addr.sin_addr));//返回点分十进制的字符串..char *inet_ntoa (struct in_addr);
          memset(&ip_tmp,addr.sin_addr.s_addr,4);
          //printf("%d.%d.%d.%d\n",ip_tmp[0],ip_tmp[1],ip_tmp[2],ip_tmp[3]);
          //IP地址: 127.127.127.127保留地址127.0.0.1 都是一样的, 0.0.0.0
          //系统自带inet_ntoa 简直垃圾不能用
          sprintf(&ip_cli,"%d.%d.%d.%d",ip_tmp[0],ip_tmp[1],ip_tmp[2],ip_tmp[3]);

          port_cli = addr.sin_port;

		  printf("new client from: %s:%d\n", ip_cli, port_cli);
		  //**************

	      continue;
	    }
	    else{//添加client 到epoll失败, 错误次数+1
	     s_val->err_li++;
	     printf("new accept sfd add_epoll() fail in listen pthread, error count: %d",s_val->err_li);
	    }
      }
      else{//如果是client fd 事件触发-->有io 数据

    	//***********************************************
        //短连接服务器需要直接剔除client 出epoll
    	//直接从epoll 中移除client
	    if(epoll_ctl(s_val->epoll_fd, EPOLL_CTL_DEL, evs[tmp].data.fd, NULL) < 0 ){
	      s_val->err_li++;
	      printf("listen pthread sfd_acc epoll_ctl EPOLL_CTL_DEL error, errno: %d, error count: %d\n", errno,s_val->err_li);
	      continue;
	    }
	    --s_val->cur_fds;//维系 cur_fds的正确性
	    //**********************************************

	    //将client sfd压入临时栈
	    as_push(pas,evs[tmp].data.fd);
      }
    }
    //获取临时栈中的所有io 任务计数
    int as_size = as_cur_size(pas);//int as_pos = as_size -1;//保持先进后出

    //执行批量压入队列(这里必须是批量push queue 操作)
    if(aq_push(pm_recv, pas->as_buf, as_size) != pm_recv->aq_sig_push_ok){
       s_val->err_li += as_size;
       printf("listen pthread batch aq_push() error in if_event()...\n");
       printf("listen pthread error count: %d",s_val->err_li);
    }
    pas->as_cur_size = -1;//复原栈

  }
  else{//epoll_wait timeout
    if(wait_fds == 0)
      return;
    //epoll_wait 异常错误
    printf("listen pthread epoll_wait() error in if_event.c, errno: %d\n", errno);
    s_val->sig_exit++;
    printf("epoll_wait() error, the server is shutting down\n");
  }
  return;
}


//添加sfd 进epoll(设置重用, 设置nonblocking, 设置如果一端主动关闭, 另一端也必须关闭)
int add_epoll(struct static_val *s_val, int sfd_acc){
  //设置重用本地地址和端口--SO_REUSEADDR 重用地址
  int nREUSEADDR = 1;
  if(setsockopt(sfd_acc,SOL_SOCKET,SO_REUSEADDR,(const char*)&nREUSEADDR,sizeof(int)) == -1){
    printf("setsockopt = nREUSEADDR = 1 fail, errno: %d\n",errno);
    return -1;
  }

  //设置:如果服务器主动关闭剔除客户端,客户端强制关闭
  struct linger m_sLinger;
  m_sLinger.l_onoff = 1;//(在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
  m_sLinger.l_linger = 0;//(容许逗留的时间为0秒)
  if(setsockopt(sfd_acc,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(struct linger)) == -1){
    printf("setsockopt = l_linger = 0 fail, errno: %d\n",errno);
    return -1;
  }

  //通用ctl 设置 socket nonblocking
  if(fcntl(sfd_acc, F_SETFL, fcntl(s_val->epoll_fd, F_GETFD, 0) &~ O_NONBLOCK) == -1){
    printf("listen pthread sfd_acc fcntl() error, errno: %d\n",errno);
    return -1;
  }

  //将sfd 加入epoll 中
  struct epoll_event ev_tmp;
  ev_tmp.events = EPOLLIN | EPOLLET;
  ev_tmp.data.fd = sfd_acc;
  if(epoll_ctl(s_val->epoll_fd, EPOLL_CTL_ADD, sfd_acc, &ev_tmp) < 0 ){
    printf("listen pthread sfd_acc epoll_ctl() EPOLL_CTL_ADD error, errno: %d\n", errno);
    return -1;
  }

  return 0;//add_epoll() 成功
}
