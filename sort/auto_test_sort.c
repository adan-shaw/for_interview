/*
 * auto_test_sort.c
 *
 *  Created on: Nov 4, 2017
 *      Author: adan
 */


#include "auto_test_sort.h"//随机数产生器


//获取一个当前时间的字符串到实参中(实参应为64 bit 的char)
void get_xtime(char* buf_64){
  time_t t = time(NULL);
  struct tm *_tm;
  _tm = localtime(&t);
  //asctime(_tm);//转为localtime string
  sprintf(buf_64,"%s",asctime(_tm));
}

//打印数组
void parray(int* array, int count){
  int tmp = 0;
  int x = 10;
  for(;tmp < count;tmp++){
    printf("%d-",array[tmp]);
    if(x == 0){
      printf("\n");
      x = 10;
    }
    else
      x++;
  }
  return;
}

//检查排序的正确性(统一都是从小到大) -- 排序正确返回0, 失败返回-1
int check_sort(int* array, int count){
  int tmp = 0;
  int xcount = 1;
  for(;tmp < count-1;tmp++){
    if(array[tmp] > array[tmp+1]){
      printf("%d--%d,%d\n",tmp,array[tmp],array[tmp+1]);
      return -1;
    }
    else
      xcount++;
  }

  if(xcount == count)
    return 0;//一次都没有错, 返回成功
  else
	return -1;
}


//测试主函数--由于排序算法不是独立的个体, 所以需要依赖一个文件进行统测
int main(void){
  //double_bubble_sort_test();//双向冒泡测试入口
  quick_sort_test();//快速排序测试入口
  return 0;
}

//双向冒泡测试
void double_bubble_sort_test(void){
  int xx[20] = {9,3,5,7,8,1,2,6,4,11,24,36,45,12,12,12,16,48,57,69};
  //测试数量级
  int *x100 = make_random_array(100);
  int *x1k = make_random_array(1000);
  int *x1w = make_random_array(10000);//1秒内--看来1w 是临界点
  int *x10w = make_random_array(100000);//几十秒-26s 左右
  //int *x100w = make_random_array(1000000);//噩梦-注释掉了
  //耗时统计
  char start[64];
  char end[64];

  //双向冒泡测试
  get_xtime(&start);
  double_bubble_sort(xx,20);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  parray(xx,20);//可以不打印
  if(check_sort(xx,20) == 0)
	printf("double_bubble_sort(xx) success !\n");
  else
	printf("double_bubble_sort(xx) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  double_bubble_sort(x100,100);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x100,100) == 0)
	printf("double_bubble_sort(x100) success !\n");
  else
	printf("double_bubble_sort(x100) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  double_bubble_sort(x1k,1000);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x1k,1000) == 0)
    printf("double_bubble_sort(x1k) success !\n");
  else
  	printf("double_bubble_sort(x1k) fail !\n");


 printf("\n\n");
  get_xtime(&start);
  double_bubble_sort(x1w,10000);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x1w,10000) == 0)
	printf("double_bubble_sort(x1w) success !\n");
  else
	printf("double_bubble_sort(x1w) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  double_bubble_sort(x10w,100000);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x10w,100000) == 0)
    printf("double_bubble_sort(x10w) success !\n");
  else
    printf("double_bubble_sort(x10w) fail !\n");

  /*
  printf("\n\n");
  get_xtime(&start);
  double_bubble_sort(x100w,1000000);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x100w,1000000) == 0)
    printf("double_bubble_sort(x100w) success !\n");
  else
    printf("double_bubble_sort(x100w) fail !\n");
  */

  free(x100);
  free(x1k);
  free(x1w);
  free(x10w);
  //free(x100w);
  return;
}


//快速排序测试
void quick_sort_test(void){
  int xx[20] = {9,3,5,7,8,1,2,6,4,11,24,36,45,12,12,12,16,48,57,69};
  //测试数量级
  int *x100 = make_random_array(100);
  int *x1k = make_random_array(1000);
  int *x1w = make_random_array(10000);
  int *x10w = make_random_array(100000);
  int *x100w = make_random_array(1000000);//仅用1秒
  //耗时统计
  char start[64];
  char end[64];

  //开始测试
  get_xtime(&start);
  quick_sort(xx,20,0,19);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  parray(xx,20);//可以不打印
  if(check_sort(xx,20) == 0)
	printf("quick_sort(xx) success !\n");
  else
	printf("quick_sort(xx) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  quick_sort(x100,100,0,99);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x100,100) == 0)
	printf("quick_sort(x100) success !\n");
  else
	printf("quick_sort(x100) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  quick_sort(x1k,1000,0,999);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x1k,1000) == 0)
    printf("quick_sort(x1k) success !\n");
  else
  	printf("quick_sort(x1k) fail !\n");


 printf("\n\n");
  get_xtime(&start);
  quick_sort(x1w,10000,0,9999);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x1w,10000) == 0)
	printf("quick_sort(x1w) success !\n");
  else
	printf("quick_sort(x1w) fail !\n");

  printf("\n\n");
  get_xtime(&start);
  quick_sort(x10w,100000,0,99999);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x10w,100000) == 0)
    printf("quick_sort(x10w) success !\n");
  else
    printf("quick_sort(x10w) fail !\n");


  printf("\n\n");
  get_xtime(&start);
  quick_sort(x100w,1000000,0,999999);
  get_xtime(&end);
  printf("exec start and finish time: \n%s%s", start, end);
  //parray(x100,100);//可以不打印
  if(check_sort(x100w,1000000) == 0)
    printf("quick_sort(x100w) success !\n");
  else
    printf("quick_sort(x100w) fail !\n");


  free(x100);
  free(x1k);
  free(x1w);
  free(x10w);
  free(x100w);
  return;
}

