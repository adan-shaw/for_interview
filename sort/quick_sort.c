/*
 * quick_sort.c
 *
 *  Created on: Nov 4, 2017
 *      Author: adan
 */

#include "quick_sort.h"

//快速排序的实现
void quick_sort(int array[], int maxlen, int begin, int end){
  int i, j;
  int xtmp;
  if(begin < end){
    i = begin + 1; // 将array[begin]作为基准数，因此从array[begin+1]开始与基准数比较！
    j = end;       // array[end]是数组的最后一位

    while(i < j){
      if(array[i] > array[begin]){ // 如果比较的数组元素大于基准数，则交换位置
    	xtmp = array[i];
    	array[i] = array[j];
    	array[j] = xtmp;
        j--;
      }
      else
        i++; // 将数组向后移一位，继续与基准数比较
    }

	/* 跳出while循环后，i = j。
	 * 此时数组被分割成两个部分  -->  array[begin+1] ~ array[i-1] < array[begin]
	 *                       -->  array[i+1] ~ array[end] > array[begin]
	 * 这个时候将数组array分成两个部分，再将array[i]与array[begin]进行比较，决定array[i]的位置。
	 * 最后将array[i]与array[begin]交换，进行两个分割部分的排序！以此类推，直到最后i = j不满足条件就退出！
	 */

    if(array[i] >= array[begin]) // 这里必须要取等“>=”，否则数组元素由相同的值时，会出现错误！
      i--;

    xtmp = array[begin];
    array[begin] = array[i];
    array[i] = xtmp;

    quick_sort(array, maxlen, begin, i);
    quick_sort(array, maxlen, j, end);
  }
}
