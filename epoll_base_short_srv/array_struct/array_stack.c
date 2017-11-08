/*
 * array_stack.c
 *
 *  Created on: Nov 13, 2016
 *      Author: adan
 */

#include "array_stack.h"


//初始化函数-反正只调用一次,虽然只有一句还是为重用考虑吧
void as_init(struct as_info* pas){
  pas->as_cur_size = -1;//栈比较特殊,这里不能为0
  pas->as_sig_pos_empty = -1;
  pas->as_sig_pos_full = as_max_buf - 1;
  pas->as_sig_full = -4000000000;
  pas->as_sig_empty = -4000000001;
  pas->as_sig_push_ok = -4000000002;
  return;
}


//压栈一个-成功返回as_sig_push_ok
//实际场景用不上多个push 入,因为stack 就是用来收集epoll fd 后再全部倒出
int as_push(struct as_info* pas,element_type val){
  //判断是否栈满
  if(pas->as_cur_size >= pas->as_sig_pos_full){
    if(pas->as_cur_size == pas->as_sig_pos_full)
      printf("array stack has full\n");
    else{
      printf("\n * array stack is fall out, now fix it...aq_cur_size > aq_max_buf *\n\n");
	  pas->as_cur_size = pas->as_sig_pos_full;
    }
    return pas->as_sig_full;
  }

  //栈未满执行push 操作
  pas->as_cur_size++;
  pas->as_buf[pas->as_cur_size] = val;
  return pas->as_sig_push_ok;
}


//弹出一个-成功返回 >=0 的值
//实际场景也用不上全部倒出，全部倒出还不如直接COPY 整个stack-缓冲区到新的栈中慢慢操作
element_type as_pop(struct as_info* pas){
  //判断是否栈空
  if(pas->as_cur_size <= pas->as_sig_pos_empty){
    if(pas->as_cur_size == pas->as_sig_pos_empty)
      printf("array stack is empty\n");
    else{
      printf("\n * array stack is fall out, now fix it...aq_cur_size < 0 *\n\n");
	  pas->as_cur_size = pas->as_sig_pos_empty;
    }
    return pas->as_sig_empty;
  }

  //栈不为空执行pop 操作
  element_type tmp = pas->as_buf[pas->as_cur_size];
  pas->as_cur_size--;
  return tmp;
}


//返回栈的当前的元素个数
int as_cur_size(struct as_info* pas){
  if(pas->as_cur_size == pas->as_sig_pos_empty)
    return 0;
  return pas->as_cur_size+1;
}


//压栈N个-成功返回as_sig_push_ok
int as_pushN(struct as_info* pas,int N){
  //判断是否栈满
  if(pas->as_cur_size >= pas->as_sig_pos_full){
    if(pas->as_cur_size == pas->as_sig_pos_full)
	  printf("array stack has full\n");
    else{
      printf("\n * array stack is fall out, now fix it...aq_cur_size > aq_max_buf *\n\n");
	  pas->as_cur_size = pas->as_sig_pos_full;
    }
    return pas->as_sig_full;
  }

  //安检-防止N > 批量插入缓冲区, 如果N > 批量插入缓冲区的max, 就会指针溢出
  if(N > as_max_tmp_buf){
	printf("bad operation !! push data count > as_max_tmp_buf\n");
	return pas->as_sig_full;//不支持坏操作
  }

  //*****************
  //栈未满, 批量插入数据
  //*****************

  int tmp,tmp2;
  if(pas->as_cur_size == pas->as_sig_pos_empty){
    tmp = N;
    tmp2 = 0;//栈为空时，as_cur_size = -1
  }
  else{
    tmp = pas->as_cur_size + N;
    tmp2 = pas->as_cur_size;
  }

  //执行批量插入操作
  int n;
  for(n = 0;tmp2 < tmp;tmp2++){
    pas->as_buf[tmp2] = pas->as_tmp_buf[n];
    n++;
  }
  pas->as_cur_size = tmp - 1;
  return pas->as_sig_push_ok;
}
