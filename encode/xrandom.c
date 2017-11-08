/*
 * xrandom.c
 *
 *  Created on: 2017/5/18
 *      Author: adan shaw
 */

#include "xrandom.h"//防止字典卡死编辑器而设计的

//获取复杂随机数-无字典
unsigned int get_xrandom(void){
  unsigned int _clock = clock();//获取CPU 时钟	-- 取4位
  unsigned int _time = time(NULL);//获取时间		-- 取4位
  unsigned int _clock4 = _clock % 10000;
  unsigned int _time4 = _time % 10000;
  unsigned int result = _clock4*_time4%10000000;
  srand(result);//设置随机数种子
  //if(rand() + result - 2056 < 0)
  //printf("%d",rand() + result - 2056);//for test
  return rand() % 10000000 + result - 7294;
}

//**********************************************
//*******************
//测试模块
//*******************
void test_xrandom(void){
  int tmp;
  //字典随机数测试--百万测试
  int test_count = 0;
  int test_err = 0;
  int big = 0;
  for(tmp = 0;tmp < 2000000;tmp++){
	unsigned int xtmp = get_xrandom();//可以更换测试函数达到遍历测试目的
	test_count++;
	if(xtmp < 0){
	  printf("%d\n",xtmp);
	  test_err++;

	  scanf("%d",&xtmp);
	}
	if(xtmp > 100000000){//>10 亿
	  printf("%d-%d\n",xtmp,test_count);
	  big++;
	  scanf("%d",&xtmp);
	}
  }
  printf("%d test finish,error count = %d\n",test_count,test_err,big);
}






