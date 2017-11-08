/*
 * array_queue.h
 *
 *  Created on: Nov 13, 2016
 *      Author: adan
 */

#include <pthread.h>

//*****
//运行宏
//*****
#define type_element int	//4个字节的type (固定)
//#define type_element void*//*(即指针变量): 32-4/64-8  个字节

#define aq_max_buf 409600
#define aq_tmp_max_push 40960
#define aq_tmp_max_pop 40960//每次取出最大256--因为要直接倒入栈, 但是又不能直接用栈的全部缓冲区, 否则就不能每次操作20个了, 因为如果有剩余, 导入全部缓冲区会被清洗所有数据


//队列结构体--一个队列一个结构体
struct aq_info{
  //*******
  //运行变量
  //*******
  type_element aq_buf[aq_max_buf];
  int aq_pos_front;//2个字节(固定)
  int aq_pos_rear;
  int aq_cur_size;

  pthread_mutex_t aq_tmp_mutex;//自带锁--唯一

  type_element aq_tmp_buf_push[aq_tmp_max_push];//批量push 临时缓冲区-使用者：只有epoll_listen 线程


  //(这样的缓冲区, 必须由io 线程自己独占, array_queue 容器不负责读出后的缓冲区的数据安全)
  //其实这个缓冲区可以去掉的, 直接让操作者自己提供缓冲区, 这里只是示范使用
  //type_element aq_tmp_buf_pop[aq_tmp_max_pop];//批量pop 临时缓冲区-使用者：多个IO 线程共享


  //*******
  //运行常量
  //*******
  int aq_sig_pos_reset;//aq_pos_front or aq_pos_rear 重置位(aq_max_buf - 1)
  int aq_sig_full;
  int aq_sig_empty;
  int aq_sig_push_ok;
};

//***********
//函数前置声明
//***********
//容器aq_info 初始化
void aq_init(struct aq_info* paq);
//压入数据push
int aq_push(struct aq_info* paq, type_element* pdata, int N);
//弹出数据，尽量弹出全部，但是最多弹出4096，返回弹出的数量
//需要手动释放锁--详情请看头文件
//还要传入一个缓冲区
int aq_pop(struct aq_info* paq, type_element* pdata);
//返回栈的当前的元素个数(成功返回当前数量, 不会失败)
int aq_cur_size(struct aq_info* paq);
//判断aq 是否为空，是空返回1,非空返回0
int aq_is_empty(struct aq_info* paq);
//判断aq 是否队满，是空返回1,非空返回0
int aq_is_full(struct aq_info* paq);

//模块自测函数
void aq_test(void);
