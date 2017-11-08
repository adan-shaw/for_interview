/*
 * pthread_recv.c
 *
 *  Created on: Oct 17, 2016
 *      Author: adan
 */

#include "pthread_recv.h"

//互交一次
int recv_once(struct static_val *s_val, int _pid, int sock_tmp){
  struct data_frame buf;
  memset(&buf,0,sizeof(struct data_frame));

  //接收数据--epoll 水平触发,这里本来就已经recv 完毕,一次recv 肯定能马上获取全部数据
  //因为已经被提出了epoll, 所以这里用非异步接收, 保证数据接受安全性--其实MSG_DONTWAIT 也是可以的
  int data_size = recv(sock_tmp, &buf, sizeof(struct data_frame), MSG_DONTWAIT);
  if(data_size < 0){//接收数据错误
    printf("io pthread.%d ->socket %d recv an data from client fail, errno:%d\n", _pid, sock_tmp, errno);
    return -1;
  }
  if(data_size == 0){//传输时客户端已经关闭
    printf("client has close when io pthread.%d ->socket %d receive client data, errno:%d\n", _pid, sock_tmp, errno);
    return -1;
  }

  //数据解密

  //显示收到的数据

  //处理数据

  //打包回发结果

  //执行回发
  int tmp = send(sock_tmp, &buf, sizeof(struct data_frame), MSG_DONTWAIT);
  if(tmp < 0){
    printf("socket %d send an data to client fail, errno:%d\n", sock_tmp, errno);
    return -1;
  }
  if(tmp == 0){
    printf("socket %d send an data to client fail, client has closed, errno:%d\n", sock_tmp, errno);
    return -1;
  }

  return 0;//正确返回
}


//IO work pthread - 线程函数
void* fpthread_recv(struct static_val *s_val){
  struct aq_info *pm_recv = s_val->paq_io;//继承任务队列
  int _pid = s_val->pid_tmp;//继承pid

  pthread_mutex_lock(&s_val->tmp_mutex);//继承参数完毕, 再开锁
  pthread_cond_signal(&s_val->tmp_cond);
  pthread_mutex_unlock(&s_val->tmp_mutex);
  //////////////////////
  //io 工作线程初始化成功//
  //////////////////////
  struct as_info as;//创建并初始化私有栈
  as_init(&as);

  ///////////////////////////////////////////////////////////////////////////
  printf("~.~.~.~.~.~.pthread pool: io-pthread-%d into loops now~.~.\n", _pid);
  for(;;){//work loop
   if(s_val->sig_exit != 0)//quit?
      break;
    //不是退出,马上暂停,等待listen pthread 呼叫,listen pthread 没呼叫一次,检查任务队列一次
    //如果检查到任务队列非空,则执行任务--另外listen pthread 线程也要加入检查任务队列的模块
    //listen pthread 也要自动检查任务队列,当队列不为空时,无论epoll_wait 有没有事件都要进行检查
    if(as_cur_size(&as) == 0){//私有栈已取空??
       //如果任务已经做完->停止, 否则继续工作
	  s_val->sig_io[_pid] = 0;//标志改为空闲
	  pthread_mutex_lock(&s_val->mutex_io[_pid]);
	  pthread_cond_wait(&s_val->cond_io[_pid],&s_val->mutex_io[_pid]);
	  pthread_mutex_unlock(&s_val->mutex_io[_pid]);
    }

    s_val->sig_io[_pid] = 1;//标志改为繁忙

    if(aq_is_empty(pm_recv) == 0){//当任务队列有IO 任务
      //用私有栈的as_pbuf 缓冲区, 批量取出任务队列中的所有任务, 并返回mission 的个数
      int mission = aq_pop(pm_recv, &as.as_tmp_buf);

      pthread_mutex_unlock(&pm_recv->aq_tmp_mutex);//先解锁, 再判断结果
      if(mission == pm_recv->aq_sig_empty){//如果取出的队列元素个数=0
        //pthread_mutex_unlock(&pm_recv->tmp_mutex);//解开队列锁
	    continue;//捞到空队列->不作为
      }
      else{
        //正常取出队列元素,拷贝出pop缓冲区-->这个可以省略掉
    	//memcpy(&as.as_pbuf,pm_recv->AS_pop,mission);
	    //解开队列锁
	    //pthread_mutex_unlock(&pm_recv->tmp_mutex);
	    //整个私有栈的pbuf copy ->私有栈的buf, 这步肯定不能省
	    if(as_pushN(&as,mission) != as.as_sig_push_ok){
	      //如果栈满，则执行128个任务之后，再压入
	      int tmp, tmp_val;
	      for(tmp = 0;tmp < 128;tmp++){
	        tmp_val = as_pop(&as);
	        if(tmp_val < 0 || tmp_val > 65536)
	          continue;//元素少于0, 跳过(防止任务栈里面的socket 变坏拖死程序)

	        //执行一次互交
		    if(recv_once(s_val, _pid, tmp_val) == -1){
		      s_val->err_io[_pid]++;
		      printf("io pthread %d once interaction error, err_count: %d\n",_pid,s_val->err_io[_pid]);
		    }

		    //*********************************************
		    //因为无论IO 是否正常,sock_fd 总是被关闭,这是短链接服务器
		    close(tmp_val);
		    //*********************************************

		    s_val->test_io[_pid]++;//线程服务次数累加
	      }

	      //如果栈未满直接插入栈
	      if(as_pushN(&as,mission) != as.as_sig_push_ok){
	        printf("always full stack,awful !!");
	        break;//这样都还是满栈--退出
	      }
	    }
	    else{
	      //正常压入--正常互交0-20次再返回检查任务总队列，防止喂饱私有栈, 喂饱io 线程
	      int tmp_cur = as_cur_size(&as);
	      if(tmp_cur > 20)
	        tmp_cur = 20;
	      //执行互交任务的loops
	      int tmp_loop;
	      for(tmp_loop = 0;tmp_loop < tmp_cur;tmp_loop++){
	         int tmp_val = as_pop(&as);
	         if(tmp_val < 0 || tmp_val > 65536)
	    	   continue;//元素少于0, 跳过(防止任务栈里面的socket 变坏拖死程序)

	         //执行一次互交
	         if(recv_once(s_val, _pid, tmp_val) == -1){
		       s_val->err_io[_pid]++;
		       printf("io pthread %d once interaction error, err_count: %d\n",_pid,s_val->err_io[_pid]);
	         }

	         //*********************************************
		     //因为无论IO 是否正常,sock_fd 总是被关闭,,这是短链接服务器
		     close(tmp_val);
		     //*********************************************

		     s_val->test_io[_pid]++;//线程服务次数累加
	       }
	     }
      }
    }
  }
  ///////////////////////////////////////////////////////////////////////////
  //退出IO 线程, 回收自身资源
  pthread_mutex_destroy(&s_val->mutex_io[_pid]);
  pthread_cond_destroy(&s_val->cond_io[_pid]);
  s_val->pid_io[_pid] = 0;

  //IO 线程成功退出
  printf("+++++++++++io pthread had quit+++++++++++\n");
  return NULL;
}


//io 线程池-启动函数
int start_precv(struct static_val *s_val){
  pthread_t pthread;
  for(;s_val->pid_tmp < pool_max;){//创建io 线程池
    if(pthread_create(&pthread,NULL,(void*)fpthread_recv,s_val) != 0){
	  printf("create io pthread fail in start_precv(), errno: %d\n", errno);
	  s_val->sig_exit++;
	  return -1;
    }
    //detach 模式,线程退出内核会回收一切,不保留信息记录,不会成为僵尸线程
    if(pthread_detach(pthread) != 0){
	  printf("io pthread set pthread_detach fail, errno: %d", errno);
	  s_val->sig_exit++;
	  return -1;
    }
    //等待线程创建完毕
    pthread_mutex_lock(&s_val->tmp_mutex);
    pthread_cond_wait(&s_val->tmp_cond,&s_val->tmp_mutex);
    pthread_mutex_unlock(&s_val->tmp_mutex);
    //记录线程PID
    s_val->pid_io[s_val->pid_tmp] = pthread;
    printf("precv-No.%d pthread has started\n", s_val->pid_tmp);
    s_val->pid_tmp++; //堆数组序号+1
  }

  //创建完毕后销毁tmp 锁和条件变量（io pthread 专属, 也因为这样, 它会被安排到最后才创建）
  int tmp = -1;
  tmp = pthread_mutex_destroy(&s_val->tmp_mutex);
  if(tmp != 0)
    printf("pthread_mutex_destroy fail in start_precv(), errno: %d\n", errno);

  tmp = pthread_cond_destroy(&s_val->tmp_cond);
  if(tmp != 0)
    printf("pthread_cond_destroy fail in start_precv(), errno: %d\n", errno);

  s_val->pid_tmp = pool_max;//未知操作, 但先不要改, 忘记了
  printf("[[[[[[ all precv pthread init success ]]]]]]]\n");
  return 0;
}

