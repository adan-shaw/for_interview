/*
 * array_queue.c
 *
 *  Created on: Nov 13, 2016
 *      Author: adan
 */

#include "array_queue.h"


//初始化array queue
void aq_init(struct aq_info* paq){
  paq->aq_pos_front = 0;
  paq->aq_pos_rear = 0;
  paq->aq_cur_size = 0;
  pthread_mutex_init(&paq->aq_tmp_mutex,NULL);//初始化队列锁
  //初始化队列标志
  paq->aq_sig_pos_reset = aq_max_buf - 1;
  paq->aq_sig_full = -4000000000;
  paq->aq_sig_empty = -4000000001;
  paq->aq_sig_push_ok = -4000000002;
  return;
}


//压入数据push
int aq_push(struct aq_info* paq, type_element* pdata, int N){
  //判断队列当前是否能装下N 个元素
  pthread_mutex_lock(&paq->aq_tmp_mutex);
  if(paq->aq_cur_size >= aq_max_buf){//判断队列是否满栈
	if(paq->aq_cur_size > aq_max_buf){
	  printf("\n * array queue is fall out, now fix it...aq_cur_size > aq_max_buf *\n\n");
	  paq->aq_cur_size = aq_max_buf;//防止队列损坏
	}
	else
	  printf("array queue has full\n");

    pthread_mutex_unlock(&paq->aq_tmp_mutex);
    return paq->aq_sig_full;//返回队列已满
  }
  else{//队列未满--判断是否能装下N 个元素
	if(paq->aq_cur_size + N > aq_max_buf){//队列+N 后溢出
	  pthread_mutex_unlock(&paq->aq_tmp_mutex);
	  printf("array queue almost full, now is %d, max is %d, push in count is %d\n", paq->aq_cur_size, aq_max_buf, N);
	  return paq->aq_sig_full;//同样返回队列已满
	}
  }

  //队列未满且能装下N 个元素, 即执行插入
  int tmp;
  int D_val = paq->aq_pos_rear + N;//队列+N 个元素后的游标是否溢出??游标溢出要重置
  if(D_val >= aq_max_buf){
    //push 操作后游标已经溢出, 调头重置
	D_val = D_val - aq_max_buf;//求得溢出元素集的容量差

    for(tmp = 0;paq->aq_pos_rear <= paq->aq_sig_pos_reset;paq->aq_pos_rear++){
      //未溢出部分先插入
      paq->aq_buf[paq->aq_pos_rear] = *(pdata + tmp);
      tmp++;
    }

    for(paq->aq_pos_rear = 0;paq->aq_pos_rear < D_val;paq->aq_pos_rear++){
      //溢出部分, 重置到队列头后插入
      paq->aq_buf[paq->aq_pos_rear] = *(pdata + tmp);
      tmp++;
    }

    paq->aq_cur_size = paq->aq_cur_size + N;//总数+N
  }
  else{
    //不用调头
    for(tmp = 0;paq->aq_pos_rear < D_val;paq->aq_pos_rear++){//直接插入
      paq->aq_buf[paq->aq_pos_rear] = *(pdata + tmp);
      tmp++;
    }
    paq->aq_cur_size = paq->aq_cur_size + N;;//总数+N
  }

  pthread_mutex_unlock(&paq->aq_tmp_mutex);//插入完毕解锁
  return paq->aq_sig_push_ok;
}


//弹出数据，尽量弹出全部，但是最多弹出4096，返回弹出的数量
//需要手动释放锁--详情请看头文件
//还要传入一个缓冲区
int aq_pop(struct aq_info* paq, type_element* pdata){
  //检查pop 操作是否有效, 如果队列为空, pop 操作无效, 返回aq_sig_empty
  pthread_mutex_lock(&paq->aq_tmp_mutex);
  if(paq->aq_cur_size <= 0){
    if(paq->aq_cur_size == 0)
	  printf("array queue is empty\n");
    else{
      printf("\n * array queue is fall out, now fix it...aq_cur_size < 0 *\n\n");
      paq->aq_cur_size = 0;//防止队列损坏
    }
    pthread_mutex_unlock(&paq->aq_tmp_mutex);
    return paq->aq_sig_empty;
  }

  int pop_count = paq->aq_cur_size;//获取当前队列容量
  if(pop_count > aq_tmp_max_pop)//队列当前任务的数量大于pop 缓冲区的大小时
    pop_count = aq_tmp_max_pop;//只能一次pop 出缓冲区最大值个元素

  //判断pop 操作之后的游标是否溢出
  int tmp;
  int D_val = paq->aq_pos_front + pop_count;
  if(D_val >= aq_max_buf){
	//pop 操作后, 游标已经溢出, 调头重置
    D_val = D_val - aq_max_buf;//求得溢出元素集的容量差

    for(tmp = 0;paq->aq_pos_front <= paq->aq_sig_pos_reset;paq->aq_pos_front++){
      //未溢出部分先弹出
      pdata[tmp] = paq->aq_buf[paq->aq_pos_front];
      tmp++;
    }

    for(paq->aq_pos_front = 0;paq->aq_pos_front < D_val;paq->aq_pos_front++){
      //溢出部分, 重置到队列头后插入
      pdata[tmp] = paq->aq_buf[paq->aq_pos_front];
      tmp++;
    }

    paq->aq_cur_size = paq->aq_cur_size - pop_count;//总数-pop_count
  }
  else{
    //不用调头
    for(tmp = 0;paq->aq_pos_front < D_val;paq->aq_pos_front++){//直接弹出
      pdata[tmp] = paq->aq_buf[paq->aq_pos_front];
      tmp++;
    }
    paq->aq_cur_size = paq->aq_cur_size - pop_count;//总数-pop_count
  }
  return pop_count;
}


//获取array queue 的cur_size
int aq_cur_size(struct aq_info* paq){
  int tmp;
  pthread_mutex_lock(&paq->aq_tmp_mutex);
  tmp = paq->aq_cur_size;
  pthread_mutex_unlock(&paq->aq_tmp_mutex);
  return tmp;
}


//判断aq 是否为空，是空返回1,非空返回0
int aq_is_empty(struct aq_info* paq){
  int tmp;
  pthread_mutex_lock(&paq->aq_tmp_mutex);
  if(paq->aq_cur_size == 0)
    tmp = 1;
  else
    tmp = 0;
  pthread_mutex_unlock(&paq->aq_tmp_mutex);
  return tmp;
}


//判断aq 是否队满，是空返回1,非空返回0
int aq_is_full(struct aq_info* paq){
  int tmp;
  pthread_mutex_lock(&paq->aq_tmp_mutex);
  if(paq->aq_cur_size == aq_max_buf)
    tmp = 1;
  else
    tmp = 0;
  pthread_mutex_unlock(&paq->aq_tmp_mutex);
  return tmp;
}
