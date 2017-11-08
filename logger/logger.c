/*
 * logger.c
 *
 *  Created on: 2017/5/18
 *      Author: adan shaw
 */

#include "logger.h"


//初始化日志模块, 成功返回0, 失败返回-1
int xlog_init(struct logger* logger_body){
  //初始化变量
  sprintf(logger_body->file_path,"%s",xlog_path);
  sprintf(logger_body->xfile_path,"%s",xlog_path);

  logger_body->full_out_count = 0;//模块坏死次数
  logger_body->file_size_max = 10240000;//1000W 字节一换 =
  logger_body->file_size_cur = 0;
  logger_body->fd = NULL;//需要初始化, 免得经常fopen file

  logger_body->xcount = 0;//temp 计数
  logger_body->xt = 0;//temp 计时
  logger_body->xsig = 0;//是否已经被重置, 0 是未被重置; 1 是已经重置, 请等待

  //open fd(打开文件)
  logger_body->fd = fopen(logger_body->file_path,"a+");//以追+模式打开, 有+号表示没有文件时会自动创建
  if(logger_body->fd != NULL){
	logger_body->file_size_cur = 0;//重置时这个必须被重置
	printf("xlog_init is good ...\n");
	return 0;
  }
  else
	return -1;
}
//
//强行执行一次冲刷, 并关闭fd
void xlog_close(struct logger* logger_body){
  fflush(logger_body->fd);
  if(logger_body->fd > 0){
    fclose(logger_body->fd);
    logger_body->fd = NULL;
  }
  else
	printf("xlog_close fail !!logger_body->fd is error !~~\n");
  logger_body->fd = 0;
}
//
//写入零散变量的日志记录(很少用吧)
//日志模块实体, 日志要刷如的变量的表, 变量的个数, 如果传入时间则自身不再初始化时间
void xlog(struct logger* logger_body, struct log_val in_val, int n, char* xtime){
  //判断是否需要冲刷内存数据到磁盘了
  if(logger_body->xsig == 0){
    logger_body->xt = time(NULL);
    logger_body->xcount = 0;
    logger_body->xsig = 1;
    //强行执行一次冲刷
    fflush(logger_body->fd);
  }

  memset(&logger_body->xlog_buf,'\0',xlog_buf_len);//格式化缓冲区

  //获取时间和本地时间
  time_t t = time(NULL);
  struct tm *_tm;
  _tm = localtime(&t);

  //写入时间
  if(xtime != NULL)
	sprintf(logger_body->xlog_buf,"%s",xtime);//传入时间
  else
	sprintf(logger_body->xlog_buf,"%s",asctime(_tm));//没有传入时间

  //写入变量
  int tmp = 0;
  for(;tmp < n;tmp++){
	switch(tmp){
	case 0: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf0); break;
	case 1: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf1); break;
	case 2: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf2); break;
	case 3: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf3); break;
	case 4: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf4); break;
	case 5: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf5); break;
	case 6: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf6); break;
	case 7: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf7); break;
	case 8: sprintf(logger_body->xlog_buf,"%s%s, ",logger_body->xlog_buf,in_val.xbuf8); break;
	case 9: sprintf(logger_body->xlog_buf,"%s%s\n",logger_body->xlog_buf,in_val.xbuf9); break;
	}
  }
  int len = strlen(logger_body->xlog_buf) + 1;
  //printf("%s\n",logger_body->xlog_buf);printf("len = %d\n",len);//for test
  //tmp = fwrite(logger_body->xlog_buf,len,xlog_buf_len,logger_body->fd);//fwrite 按照二进制来写入--遇到数字变整形
  //tmp = fwrite(logger_body->xlog_buf,sizeof(char),len,logger_body->fd);//整形可能是4bit
  tmp = fprintf(logger_body->fd,"%s\n",logger_body->xlog_buf);//fprintf 是纯字符写入, 同样是缓存再写
  if(tmp != len){
	printf("xlog_fix start because fprintf error??tmp = %d, len = %d\n",tmp,len);
	xlog_fix(logger_body);
  }
  else{
    //超出1000w bit 就换文件
	printf("%s\n",logger_body->xlog_buf);
    logger_body->file_size_cur = logger_body->file_size_cur + len;
    if(logger_body->file_size_cur > logger_body->file_size_max){
      printf("xlog_fix start because logger_body->file_size_cur reset??logger_body->file_size_cur = %d, logger_body->file_size_max = %d\n",logger_body->file_size_cur,logger_body->file_size_max);
      xlog_fix(logger_body);
    }
  }
  //超出20 次写入就冲刷一次,  超过20s 就冲刷一次
  if(logger_body->xcount == log_max_fflush_write)
	logger_body->xsig = 0;
  if(t - logger_body->xt > log_max_fflush_timeout)
	logger_body->xsig = 0;
}
//
//修复日志模块
void xlog_fix(struct logger* logger_body){
  xlog_close(logger_body);
  int tmp;
  //尝试修复--重置资源
  printf("awful !!logger module is not working, now we trying to help it back to work !!\n");
  time_t t = time(NULL);//只是原始码--只有数字--一共10位, 月日时分秒, 2位一组
  struct tm *loc = localtime(&t);//转换成本地时间
  logger_body->full_out_count = logger_body->full_out_count + 2;
  sprintf(logger_body->file_path,"%s_%d-%d-%d_%d",logger_body->xfile_path,loc->tm_year, loc->tm_mon, loc->tm_mday,logger_body->full_out_count);//换个文件名, 可能文件太大
  if(xlog_init(logger_body) != 0){
    //坏死
    printf("logger module is dead, where is the manager ?? please take a look !!!important !!!\n");
    for(tmp = 0;tmp < 10;tmp++)
	  printf("******************************\n");
  }
  else
    printf("logger module is fixed, now it's good working ...logger_body->file_path = %s\n",logger_body->file_path);//修复成功
}
//
//error log错误信息日志记录
void xlog_err(struct logger* logger_body, char* err_msg, char* xtime){
  //判断是否需要冲刷内存数据到磁盘了
  if(logger_body->xsig == 0){
    logger_body->xt = time(NULL);
    logger_body->xcount = 0;
    logger_body->xsig = 1;
    //强行执行一次冲刷
    fflush(logger_body->fd);
  }

  memset(&logger_body->xlog_buf,'\0',xlog_buf_len);//格式化缓冲区

  //获取时间和本地时间
  time_t t = time(NULL);
  struct tm *_tm;
  _tm = localtime(&t);

  //写入时间
  if(xtime != NULL)
    sprintf(logger_body->xlog_buf,"%s",xtime);//传入时间
  else
    sprintf(logger_body->xlog_buf,"%s",asctime(_tm));//没有传入时间

  int tmp = 0;
  sprintf(logger_body->xlog_buf,"%s%s\n",logger_body->xlog_buf,err_msg);
  int len = strlen(logger_body->xlog_buf) + 1;
  //printf("%s\n",logger_body->xlog_buf);printf("len = %d\n",len);//for test
  //tmp = fwrite(logger_body->xlog_buf,len,xlog_buf_len,logger_body->fd);//fwrite 按照二进制来写入--遇到数字变整形
  //tmp = fwrite(logger_body->xlog_buf,sizeof(char),len,logger_body->fd);//整形可能是4bit
  tmp = fprintf(logger_body->fd,"%s\n",logger_body->xlog_buf);//fprintf 是纯字符写入, 同样是缓存再写
  if(tmp != len){
	printf("xlog_fix start because fprintf error??tmp = %d, len = %d\n",tmp,len);
	xlog_fix(logger_body);
  }
  else{
    //超出100w bit 就换文件
	printf("%s\n",logger_body->xlog_buf);
    logger_body->file_size_cur = logger_body->file_size_cur + len;
    if(logger_body->file_size_cur > logger_body->file_size_max){
      printf("xlog_fix start because logger_body->file_size_cur reset??logger_body->file_size_cur = %d, logger_body->file_size_max = %d\n",logger_body->file_size_cur,logger_body->file_size_max);
      xlog_fix(logger_body);
    }
  }
  //超出20 次写入就冲刷一次,  超过20s 就冲刷一次
  if(logger_body->xcount == log_max_fflush_write)
	logger_body->xsig = 0;
  if(t - logger_body->xt > log_max_fflush_timeout)
	logger_body->xsig = 0;
}
//
//**********************************************
//*******************
//测试模块
//*******************
void test_logger(void){
 //测试日志系统
  struct logger logger_body;
  xlog_init(&logger_body);
  struct log_val test;
  int tmp = 0,n=xlog_val_max;
  //变量赋值到容器里面, 打日志的变量
  for(;tmp < n;tmp++){
	switch(tmp){
	case 0: sprintf(test.xbuf0,"fuck you"); break;
	case 1: sprintf(test.xbuf1,"fuck you"); break;
	case 2: sprintf(test.xbuf2,"fuck you"); break;
	case 3: sprintf(test.xbuf3,"fuck you"); break;
	case 4: sprintf(test.xbuf4,"fuck you"); break;
	case 5: sprintf(test.xbuf5,"fuck you"); break;
	case 6: sprintf(test.xbuf6,"fuck you"); break;
	case 7: sprintf(test.xbuf7,"fuck you"); break;
	case 8: sprintf(test.xbuf8,"fuck you"); break;
	case 9: sprintf(test.xbuf9,"fuck you"); break;
	}
  }

  //单条测试
  xlog(&logger_body, test, 10, NULL);//打变量日志 实参-10 表示有10 个变量需要打日志
  xlog_err(&logger_body, "error logger", NULL);//打错误日志
  sleep(1);


  //暴力测试--2W 5S, 性能还可以...
  int start = time(NULL);
  for(;tmp < 20000;tmp++){
	xlog(&logger_body, test, 10, NULL);
	xlog_err(&logger_body, "if you seek amy tonight !!", NULL);
  }
  printf("%d\n",start-time(NULL));
  printf("\n19000 times\n");
  xlog_close(&logger_body);
  return;
}

int main(void){
	test_logger();
	return 0;
}
