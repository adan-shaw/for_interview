/*
 * random.c
 *
 *  Created on: Mar 18, 2017
 *      Author: adan
 */

#include <stdlib.h>
#include <time.h>//使用当前时钟做种子

void test_random(void){
  //!> <1>
  //!> 用  法: void rand(void);
  int rand_num = 0;
  srand((unsigned)time(NULL));/*定义这个可以产生不同的随机数*/
  rand_num =  rand() ;//随机数取值

  printf("%d\n",rand());				//打出随机生产出来的数字(全部是数字,正数)
  printf( "%5.2f\n", rand()/32767.0);	//2.产生0~1之间的随机数
  printf( "%d\n", rand()%100+1);  		//3.产生1~100之间的随机数

  //!> <2>
  //!> 用  法: int random(int num);
  int random_num = 0;
  srandom((unsigned)time(NULL));
  random_num = random();
  printf("Random number in the 0-99 range: %d\n", random()%100);

}
