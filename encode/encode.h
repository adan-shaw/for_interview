/*
 * encode.h
 *
 *  Created on: May 30, 2017
 *      Author: adan
 */

//**********************************************************************
//仅在encode2 里面有, 到了真正项目的时候定义在static_struct.h and static_val.h 里面
//
#define frame_buf_size 256//for lua table 512-256 = 256

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
  double xkey[8];      //4*8
  double comm;		//4
  double uid;		//4
  double age;		//4
  double sex;		//4
  double country;	//电话号码前缀: china 086
  double city;		//大沙镇 526200
  double code;		//4
  double ts;		//4

  double d_backup1;	//4
  double d_backup2;	//4
  double d_backup3;	//4
  double d_backup4;	//4
  double d_backup5;	//4
  double d_backup6;	//4
  double d_backup7;	//4
  double d_backup8;	//4

  char uname[16];	//16
  char upw[16];		//16

  char s_backup1[32];//32
  char s_backup2[32];//32
  char s_backup3[32];//32
  char s_backup4[32];//32
  char buf[frame_buf_size];	//frame_buf_size
};
//********************************************************************

#include "../random/xrandom.h"

//获取-装载 加密钥匙
void get_key(struct encode_key* xtmp);
//加密-double 部分
void encode_uint(struct data_frame* pdata,struct encode_key* pxkey);
//加密字符部分--char
void encode_char(struct data_frame* pdata,struct encode_key* pxkey);
//解密-double 部分
void decode_uint(struct data_frame* pdata,struct encode_key* pxkey);
//解密字符部分--char
void decode_char(struct data_frame* pdata,struct encode_key* pxkey);
//隐藏钥匙
void hide_key(struct data_frame* pdata,struct encode_key* pxkey);
//恢复钥匙
void restore_key(struct data_frame* pdata,struct encode_key* pxkey);


//加密-data_frame(单帧 512)
void encode_all(struct data_frame* pdata);
//解码密-data_frame(单帧 512)
void decode_all(struct data_frame* pdata);



//测试模块
void test_encode(void);

