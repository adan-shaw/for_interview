/*
 * random_int.h
 *
 *  Created on: Nov 4, 2017
 *      Author: adan
 */

//这个模块是为了测试算法而设计的中间产物, 专门制造随机数-array...
#include <stdio.h>
#include <time.h>

//输入要产生的随机数个数count, 返回一个malloc 指针
int* make_random_array(int count);

//获取随机数-无字典
int get_random_sort(void);
