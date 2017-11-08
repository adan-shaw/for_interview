/*
 * main.c
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */

#include "static_val.h"
#include <stdio.h>
#include <stdlib.h>


//客户端主程序函数
int main(int argc,int argv[]){
  printf("******************************************\n");
  printf("welcome you to use our software\n");
  printf("******************************************\n");

  struct static_val* s_val;
  s_val = malloc(sizeof(struct static_val));//动态创建全局变量
  if(s_val == NULL)
    exit(EXIT_FAILURE);

  init_s_val(s_val);//初始化全局变量

  echo_comm();//显示命令列表
  int tmp_loop;
  for(;;){
    printf("now targer server ip:%s ,port:%d\n",srv_ip,srv_port);
    printf("please input a command, need help order number-0\n");
    scanf("%d",&tmp_loop);

    switch(tmp_loop){
      case 1: test_loop(s_val);break;//测试盒子

      case 2: quit_main(s_val);exit(0);//调用退出函数

      case 3:
	    printf("hello stranger, i am adan shaw, thx for you use us software\n");
	    printf("what if you have any questions, please contant us\n");
	    printf("our email:adan_shaw@qq.com\n");
	    break;

      case 4: show_cli_status(s_val);break;//调用退出函数

      case 0: echo_comm();break;//显示命令列表

      default: printf("input a unknow number\n"); break;
    }
  printf("\n");
  }

  //控制loop 内存溢出
  printf("******************************************\n");
  printf("oh my god,main()-pthread out of memory\n");
  printf("the client has stop,input any key to quit\n");
  printf("******************************************\n");
  scanf("%d",&tmp_loop);
  exit(0);
}




