/*
 * static_val.h
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */


#define frame_buf_size 320//for lua table 512-192 = 320(320 可变)

//#define srv_ip "192.168.56.1"
#define srv_ip "127.0.0.1"
#define srv_port 9213

//加密钥匙头
struct encode_key{
  unsigned int key1;
  unsigned int key2;
  unsigned int key3;
  unsigned int key4;
  unsigned int key5;
  unsigned int key6;
  unsigned int key7;
  unsigned int key8;
};


//数据帧格式(唯一格式, 不再精简了...)
struct data_frame{
  struct encode_key encode;	//4*8
  double comm;		//4
  double uid;		//4
  double age;		//4
  double sex;		//4
  double country;	//电话号码前缀: china 086
  double city;		//大沙镇 526200
  double d_backup1;	//4
  double d_backup2;	//4

  double d_backup3;	//4
  double d_backup4;	//4
  double d_backup5;	//4
  double d_backup6;	//4

  char uname[16];	//16
  char upw[16];		//16

  char s_backup1[16];//16
  char s_backup2[32];//32
  char s_backup3[32];//32
  char buf[frame_buf_size];	//frame_buf_size
};


//客户端--全局变量
struct static_val{
  unsigned int test_err;
  unsigned int test_count;
};




