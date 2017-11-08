/*
 * auto_test_sort.h
 *
 *  Created on: Nov 4, 2017
 *      Author: adan
 */

#include <stdio.h>
#include "random_int.h"//随机数产生器
#include "double_bubble_sort.h"//双向排序
#include "quick_sort.h"//快速排序

//获取一个当前时间的字符串到实参中(实参应为64 bit 的char)
void get_xtime(char* buf_64);

//打印数组
void parray(int* array, int count);

//检查排序的正确性(统一都是从小到大) -- 排序正确返回0, 失败返回-1
int check_sort(int* array, int count);

//测试主函数--由于排序算法不是独立的个体, 所以需要依赖一个文件进行统测
int main(void);

//双向冒泡测试
void double_bubble_sort_test(void);

//快速排序测试
void quick_sort_test(void);
