/*
 * array_stack.h
 *
 *  Created on: Nov 13, 2016
 *      Author: adan
 */

#define element_type int//int： 4个字节(固定)
//#define element_type void*//*(即指针变量): 32-4/64-8  个字节


#define as_max_buf 40960
#define as_max_tmp_buf 1024

//栈结构体--一个栈一个结构体
struct as_info{
  //*******
  //运行变量
  //*******
  element_type as_buf[as_max_buf];
  element_type as_tmp_buf[as_max_tmp_buf];
  int as_cur_size;

  //*******
  //运行常量
  //*******
  int as_sig_pos_empty;//缓冲区空 -1 (由于堆栈比较特殊所以...)
  int as_sig_pos_full;//缓冲区满 as_buf_max - 1

  int as_sig_full;
  int as_sig_empty;
  int as_sig_push_ok;
};
//附：栈中pos 没有负值


//***********
//函数前置声明
//***********
//容器aq_info 初始化
void as_init(struct as_info* pas);
//压栈一个-成功返回as_push_ok
//实际场景用不上多个push 入,因为stack 就是用来收集epoll fd 后再全部倒出
int as_push(struct as_info* pas,element_type val);
//弹出一个-成功返回 >=0 的值
//实际场景也用不上全部倒出，全部倒出还不如直接COPY 整个stack-缓冲区到新的栈中慢慢操作
element_type as_pop(struct as_info* pas);
//返回栈的当前的元素个数
int as_cur_size(struct as_info* pas);
//压栈N个-成功返回as_push_ok
int as_pushN(struct as_info* pas,int N);


