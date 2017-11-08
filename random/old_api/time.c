/*
 * time.c
 *
 *  Created on: Mar 18, 2017
 *      Author: adan
 */

#include <time.h>
//
//获取机械时间
void get_clock(void){
  clock_t clock_tmp;
  clock_tmp = clock();//clock_t 相当于unsigned int, 只有数字, 一般只有5位数左右
  //
  //转换成真实机械时间
  double d_tmp;
  d_tmp = (double)(clock_tmp) / CLOCKS_PER_SEC;
}
//
//获取时间time()
void get_time(void){
  time_t t;
  t = time(NULL);//只是原始码--只有数字--一共10位, 月日时分秒, 2位一组
  //
  //转换成格林威治(GMT)时间格式
  struct tm *gmt;
  gmt = gmtime(&t);
  printf("year=%d mon=%d mday=%d \n", gmt->tm_year, gmt->tm_mon, gmt->tm_mday);
  printf("hour=%d min=%d sec=%d \n", gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
  printf("wday=%d yday=%d isdst=%d\n", gmt->tm_wday, gmt->tm_yday, gmt->tm_isdst);
  //
  //转换成本地时间
  struct tm *loc;
  loc = localtime(&t);
  printf("year=%d mon=%d mday=%d \n", loc->tm_year, loc->tm_mon, loc->tm_mday);
  printf("hour=%d min=%d sec=%d \n", loc->tm_hour, loc->tm_min, loc->tm_sec);
  printf("tm_wday=%d tm_yday=%d tm_isdst=%d \n", loc->tm_wday, loc->tm_yday, loc->tm_isdst);
  //
  //未知转换
  printf("asctime() formats the gmtime() value as: %s", asctime(&gmt));
  printf("ctime() formats the time() value as: %s", ctime(&t));

  printf("mktime() of gmtime() value: %ld secs\n", (long) mktime(&gmt));
  printf("mktime() of localtime() value: %ld secs\n", (long) mktime(&loc));

  //直接转换为本地时间
  char buf_64[64];
  sprintf(&buf_64,"%s",asctime(t));
}

