/*
 * logger.h
 *
 *  Created on: Aug 17, 2017
 *      Author: adan
 */

#include "time/xrandom.h"
#include <unistd.h>
#include <errno.h>

#define xlog_buf_len 256//每条日志的最长字符串个数
#define xlog_path "./log.x"//日志路径和名称
#define xlog_val_max 10//打日志的变量个数max(16 bit char)

#define log_max_fflush_write 20
#define log_max_fflush_timeout 20
//
//日志生成格式: 年月日, 时分秒...
//参数可能有: 消息名, 用户名, 房间id, 之类 --log_val 结构体
//其实不用不定参数, 只需要10个缓冲区就足够了, 你也不应该设计一个函数超过10 个参数 !!
//每个参数都为16 char 的buf !!
//
//刷写次数: 每20条一刷, 每20秒且条数不为0一刷(防止晚上还在刷)
//必须传入参数个数--减少格式化缓冲区的次数, 而且还能提醒你这个日志的用法, 但是格式化式必须全部格式化
//
//
struct log_val{
  char xbuf0[16];
  char xbuf1[16];
  char xbuf2[16];
  char xbuf3[16];
  char xbuf4[16];
  char xbuf5[16];
  char xbuf6[16];
  char xbuf7[16];
  char xbuf8[16];
  char xbuf9[16];
};//公开的全局变量--使用logger 时候需要用到的媒介单元

//***********************
//结构体定义(增加错误标志计数)
//***********************
struct logger{
  char file_path[128];
  char xfile_path[32];

  int full_out_count;//模块坏死次数
  int file_size_max;//1000W 字节一换
  int file_size_cur;
  FILE* fd;//需要初始化, 免得经常fopen file

  //执行记录日志, 无返回
  int xcount;//temp 计数
  time_t xt;//temp 计时
  int xsig;//是否已经被重置, 0 是未被重置; 1 是已经重置, 请等待

  char xlog_buf[xlog_buf_len];//256 个字符够了...
};
//***********
//公开的调用方法
//************
//初始化日志模块, 成功返回0, 失败返回-1
int xlog_init(struct logger* logger_body);
//强行执行一次冲刷, 并关闭fd
void xlog_close(struct logger* logger_body);
//写入零散变量的日志记录(很少用吧)
void xlog(struct logger* logger_body, struct log_val in_val, int n, char* xtime);
//修复日志模块
void xlog_fix(struct logger* logger_body);
//error log错误信息日志记录
void xlog_err(struct logger* logger_body, char* err_msg, char* xtime);

//测试模块
void test_logger(void);


