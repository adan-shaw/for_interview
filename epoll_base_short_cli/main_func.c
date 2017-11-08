/*
 * main_func.c
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */

#include "static_val.h"


//客户端--全局变量初始化
void init_s_val(void* arg){
  struct static_val* s_val = arg;
  s_val->test_count = 0;
  s_val->test_err = 0;
  return;
}

//客户端--客户端退出函数
void quit_main(void* arg){
  struct static_val* s_val = arg;
  free(s_val);
  printf("good bye\n");
  return;
}

//客户端--客户端状态信息
void show_cli_status(void* arg){
  struct static_val* s_val = arg;
  printf("test count: %d, test error: %d\n",s_val->test_count,s_val->test_err);
  return;
}

//客户端--命令列表
void echo_comm(void){
 printf("commmand list:\n");
  printf("No.1: test box(max = 40960--online socket--short server has no max~))\n");
  printf("No.2: stop client and quit\n");
  printf("No.3: about us\n");
  printf("No.4: show the client status\n");
  printf("No.0: show command list\n");
  return;
}

//客户端--测试盒子
void test_loop(void* arg){
  struct static_val* s_val = arg;
  int Y, X;
  printf("this is a loop test,test interaction count X finish then wait for 1 second\n");
  printf("and then keeping test Y times till 'Y = 0'\n");
  printf("please input a test number X*6\n");
  scanf("%d",&X);
  printf("please input a test number Y\n");
  scanf("%d",&Y);

  int tmp;
  int tmp2;
  int test_count = 0;
  int sfd_tmp;
  //局部记录变量
  int tmp_err = 0,tmp_count = 0;
  //start time
  printf("start at: \n");
  get_time_echo(1);
  //****************************************
  /*
  int sock_buf[40960];
  memset(&sock_buf,-1,40960);
  int sock_buf_pos = 0;//游标
  if(X*Y > 40960){//防止越界
	X = 6800;//6800*6 ~= 40960
	Y = 1;
  }
  */
  //****************************************

  for(tmp = 0;tmp<Y;tmp++){
    for(tmp2 = 0;tmp2<X;tmp2++){
	  int db_all;
	  for(db_all = 0;db_all < 6;db_all++){//反正db 操作数据请求都是1-8,方便递归,1和8是清表，不用
	    sfd_tmp = inet_connect(srv_ip,srv_port);//由于每次操作都要链接一次
	    if(sfd_tmp < 0){
		  s_val->test_err++;
		  tmp_err++;
	    }//****************************************
	    //else{
	      //sock_buf[sock_buf_pos] = sfd_tmp;//用int 数组记录这个socket
	      //sock_buf_pos++;//游标向后移动
	    //}
	    //****************************************
	    if(test_once(sfd_tmp,db_all,999) == -1){//所以不能链接一次就遍历8次
		  s_val->test_err++;
		  tmp_err++;
	    }
	  }
	  //错误累加 >20
	  if(tmp_err > 20)
	      break;
	  s_val->test_count+=6;
	}
    if(tmp_err > 20){
	  tmp_count = tmp*X;
	  printf("test box > 20,client has stop test mession,test count: %d\n",tmp_count);
	  break;
	}
  }

  printf("test Y*X times finish\n");
  printf("finsh at: \n");
  get_time_echo(0);
  printf("client status count:\n");
  show_cli_status(s_val);
  //****************************************
  /*
  //长链接的socket 回收
  printf("start close all socket connection\n");
  for(tmp = 0;tmp<Y*X;tmp++){
    shutdown(sock_buf[sock_buf_pos]);
    close(sock_buf[sock_buf_pos]);
    sock_buf_pos--;
  }
  printf("all test socket has close !!\n");
  */
  //****************************************
  echo_comm();
  return;
}
