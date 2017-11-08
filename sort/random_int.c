/*
 * random_int.c
 *
 *  Created on: Nov 4, 2017
 *      Author: adan
 */

#include "random_int.h"

//输入要产生的随机数个数count, 返回一个malloc 指针
int* make_random_array(int count){
  int* buf = malloc(count*sizeof(int));
  int tmp = 0;
  for(;tmp < count;tmp++)
	buf[tmp] = get_random_sort();
  return buf;
}

//获取随机数-无字典
int get_random_sort(void){
  int _clock = clock();//获取CPU 时钟	-- 取4位
  int _time = time(NULL);//获取时间		-- 取4位
  int _clock4 = _clock % 10000;
  int _time4 = _time % 10000;
  int result = _clock4*_time4%1000000;
  srand(result);//设置随机数种子
  //if(rand() + result - 2056 < 0)
  //printf("%d",rand() + result - 2056);//for test
  return rand()%1000000 + result - 7294;
}
