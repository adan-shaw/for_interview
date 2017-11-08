/*
 * xrandom.h
 *
 *  Created on: May 30, 2017
 *      Author: adan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//获取复杂随机数-无字典
unsigned int get_xrandom(void);

//获取一个当前时间的字符串到实参中(实参应为64 bit 的char)
void get_xtime(char* buf_64);


//测试模块
void test_xrandom(void);
