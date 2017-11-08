/*
 * encode.c
 *
 *  Created on: 2017/5/18
 *      Author: adan shaw
 */

#include "encode.h"

//
//获取-装载 加密钥匙--unsigned int
void get_key(struct encode_key* xtmp){
  xtmp->key1 = get_xrandom();
  xtmp->key2 = get_xrandom();
  xtmp->key3 = get_xrandom();
  xtmp->key4 = get_xrandom();
  xtmp->key5 = get_xrandom();
  xtmp->key6 = get_xrandom();
  xtmp->key7 = get_xrandom();
  xtmp->key8 = get_xrandom();
}
//
//拆分单个随机数, 用来加密字符, 因为字符都是1bit, 而钥匙是4 bit
void encode_uint(struct data_frame* pdata, struct encode_key* pxkey){
  pdata->comm = pxkey->key1 + pdata->comm - 123;
  pdata->uid = pxkey->key2 + pdata->uid - 456;
  pdata->age = pxkey->key3 + pdata->age - 789;
  pdata->sex = pxkey->key4 + pdata->sex - 147;
  pdata->country = pxkey->key5 + pdata->country - 258;
  pdata->city = pxkey->key6 + pdata->city - 369;
  pdata->code = pxkey->key7 + pdata->code - 900;
  pdata->ts = pxkey->key8 + pdata->ts - 933;

  pdata->d_backup1 = pxkey->key1 + pdata->d_backup1 - 321;
  pdata->d_backup2 = pxkey->key2 + pdata->d_backup2 - 654;
  pdata->d_backup3 = pxkey->key3 + pdata->d_backup3 - 701;
  pdata->d_backup4 = pxkey->key4 + pdata->d_backup4 - 233;
  pdata->d_backup5 = pxkey->key5 + pdata->d_backup5 - 611;
  pdata->d_backup6 = pxkey->key6 + pdata->d_backup6 - 439;
  pdata->d_backup7 = pxkey->key7 + pdata->d_backup7 - 199;
  pdata->d_backup8 = pxkey->key8 + pdata->d_backup8 - 111;

}
//
//加密字符部分--char
void encode_char(struct data_frame* pdata, struct encode_key* pxkey){
  int tmp;
  int count = 0;
  for(tmp = 0;tmp < 2;tmp++){
	pdata->uname[count] = pdata->uname[count] + pxkey->key1 + 1;//高位全部被截取了而已, 其实还是可行的
	count++;												    //4 bit 有前3 bit 被截取了, 但是直接相加并没有错
	pdata->uname[count] = pdata->uname[count] + pxkey->key2 + 2;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key3 + 3;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key4 + 4;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key5 + 5;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key6 + 6;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key7 + 7;
	count++;
	pdata->uname[count] = pdata->uname[count] + pxkey->key8 + 8;
	count++;
  }
  //
  count = 0;
  for(tmp = 0;tmp < 2;tmp++){
	pdata->upw[count] = pdata->upw[count] + pxkey->key4 + 9;//高位全部被截取了而已, 其实还是可行的
	count++;												//4 bit 有前3 bit 被截取了, 但是直接相加并没有错
	pdata->upw[count] = pdata->upw[count] + pxkey->key3 + 8;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key2 + 7;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key1 + 6;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key5 + 5;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key6 + 4;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key7 + 3;
	count++;
	pdata->upw[count] = pdata->upw[count] + pxkey->key8 + 2;
	count++;
  }
  //
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key8 + 2;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key1 + 3;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key7 + 4;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key2 + 5;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key6 + 6;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key3 + 7;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key5 + 8;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] + pxkey->key4 + 9;
	count++;
  }
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key8 + 8;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key1 + 7;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key7 + 6;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key2 + 5;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key6 + 4;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key3 + 3;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key5 + 2;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] + pxkey->key4 + 1;
	count++;
  }
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key8 + 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key1 + 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key7 + 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key2 + 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key6 + 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key3 + 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key5 + 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] + pxkey->key4 + 4;
	count++;
  }
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key8 + 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key1 + 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key7 + 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key2 + 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key6 + 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key3 + 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key5 + 8;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] + pxkey->key4 + 8;
	count++;
  }
  //256 = 32 * 8
  count = 0;
  for(tmp = 0;tmp < 32;tmp++){
	pdata->buf[count] = pdata->buf[count] + pxkey->key5 + 1;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key1 + 8;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key8 + 7;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key6 + 9;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key2 + 5;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key3 + 4;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key4 + 2;
	count++;
	pdata->buf[count] = pdata->buf[count] + pxkey->key7 + 2;
	count++;
	//printf("%d\n",count);//for test
  }
}
//
//解密-unsigned int 部分
void decode_uint(struct data_frame* pdata, struct encode_key* pxkey){
  pdata->comm = pdata->comm - pxkey->key1  + 123;
  pdata->uid = pdata->uid - pxkey->key2 + 456;
  pdata->age = pdata->age - pxkey->key3 + 789;
  pdata->sex = pdata->sex - pxkey->key4 + 147;
  pdata->country = pdata->country - pxkey->key5 + 258;
  pdata->city = pdata->city - pxkey->key6 + 369;
  pdata->code = pdata->code - pxkey->key7 + 900;
  pdata->ts = pdata->ts - pxkey->key8 + 933;

  pdata->d_backup1 = pdata->d_backup1 - pxkey->key1 + 321;
  pdata->d_backup2 = pdata->d_backup2 - pxkey->key2 + 654;

  pdata->d_backup3 = pdata->d_backup3 - pxkey->key3 + 701;
  pdata->d_backup4 = pdata->d_backup4 - pxkey->key4 + 233;
  pdata->d_backup5 = pdata->d_backup5 - pxkey->key5 + 611;
  pdata->d_backup6 = pdata->d_backup6 - pxkey->key6 + 439;
  pdata->d_backup7 = pdata->d_backup7 - pxkey->key7 + 199;
  pdata->d_backup8 = pdata->d_backup8 - pxkey->key8 + 111;
  //printf("decode_uint is okay\n");
}
//
//解密字符部分--char
void decode_char(struct data_frame* pdata,struct encode_key* pxkey){
  int tmp;
  int count = 0;
  for(tmp = 0;tmp < 2;tmp++){
	pdata->uname[count] = pdata->uname[count] - pxkey->key1 - 1;//高位全部被截取了而已, 其实还是可行的
	count++;												    //4 bit 有前3 bit 被截取了, 但是直接相加并没有错
	pdata->uname[count] = pdata->uname[count] - pxkey->key2 - 2;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key3 - 3;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key4 - 4;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key5 - 5;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key6 - 6;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key7 - 7;
	count++;
	pdata->uname[count] = pdata->uname[count] - pxkey->key8 - 8;
	count++;
  }
  //printf("first");
  //
  count = 0;
  for(tmp = 0;tmp < 2;tmp++){
	pdata->upw[count] = pdata->upw[count] - pxkey->key4 - 9;//高位全部被截取了而已, 其实还是可行的
	count++;												//4 bit 有前3 bit 被截取了, 但是直接相加并没有错
	pdata->upw[count] = pdata->upw[count] - pxkey->key3 - 8;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key2 - 7;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key1 - 6;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key5 - 5;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key6 - 4;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key7 - 3;
	count++;
	pdata->upw[count] = pdata->upw[count] - pxkey->key8 - 2;
	count++;
  }
  //
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key8 - 2;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key1 - 3;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key7 - 4;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key2 - 5;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key6 - 6;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key3 - 7;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key5 - 8;
	count++;
	pdata->s_backup1[count] = pdata->s_backup1[count] - pxkey->key4 - 9;
	count++;
  }
  //printf("1");
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key8 - 8;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key1 - 7;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key7 - 6;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key2 - 5;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key6 - 4;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key3 - 3;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key5 - 2;
	count++;
	pdata->s_backup2[count] = pdata->s_backup2[count] - pxkey->key4 - 1;
	count++;
  }
  //printf("2");
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key8 - 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key1 - 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key7 - 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key2 - 3;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key6 - 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key3 - 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key5 - 4;
	count++;
	pdata->s_backup3[count] = pdata->s_backup3[count] - pxkey->key4 - 4;
	count++;
  }
  //printf("3");
  count = 0;
  for(tmp = 0;tmp < 4;tmp++){
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key8 - 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key1 - 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key7 - 6;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key2 - 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key6 - 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key3 - 7;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key5 - 8;
	count++;
	pdata->s_backup4[count] = pdata->s_backup4[count] - pxkey->key4 - 8;
	count++;
  }
  //printf("4");
  //256 = 32 * 8
  count = 0;
  for(tmp = 0;tmp < 32;tmp++){
	pdata->buf[count] = pdata->buf[count] - pxkey->key5 - 1;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key1 - 8;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key8 - 7;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key6 - 9;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key2 - 5;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key3 - 4;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key4 - 2;
	count++;
	pdata->buf[count] = pdata->buf[count] - pxkey->key7 - 2;
	count++;
	//printf("%d\n",count);//for test
  }
}
//
//隐藏钥匙
void hide_key(struct data_frame* pdata, struct encode_key* pxkey){
  //钥匙换位
  pdata->xkey[1] = pdata->comm;
  pdata->xkey[2] = pdata->uid;
  pdata->xkey[3] = pdata->age;
  pdata->xkey[4] = pdata->sex;
  pdata->xkey[5] = pdata->d_backup1;
  pdata->xkey[6] = pdata->d_backup3;
  pdata->xkey[7] = pdata->d_backup4;
  pdata->xkey[0] = pdata->d_backup7;
  //钥匙赋值 并硬性掩码
  pdata->comm = pxkey->key1 + 357;
  pdata->uid = pxkey->key2 + 369;
  pdata->age = pxkey->key3 + 25;
  pdata->sex = pxkey->key4 + 18;
  pdata->d_backup1 = pxkey->key5 + 9;
  pdata->d_backup3 = pxkey->key6;
  pdata->d_backup4 = pxkey->key7 + 1020;
  pdata->d_backup7 = pxkey->key8 + 999;
  //printf("hide_key is okay\n");
}
//
//恢复钥匙
void restore_key(struct data_frame* pdata, struct encode_key* pxkey){
  //恢复获取
  pxkey->key1 = pdata->comm - 357;
  pxkey->key2 = pdata->uid - 369;
  pxkey->key3 = pdata->age - 25;
  pxkey->key4 = pdata->sex - 18;
  pxkey->key5 = pdata->d_backup1 - 9;
  pxkey->key6 = pdata->d_backup3;
  pxkey->key7 = pdata->d_backup4 - 1020;
  pxkey->key8 = pdata->d_backup7 - 999;
  //恢复uint 数据
  pdata->comm = pdata->xkey[1];
  pdata->uid = pdata->xkey[2];
  pdata->age = pdata->xkey[3];
  pdata->sex = pdata->xkey[4];
  pdata->d_backup1 = pdata->xkey[5];
  pdata->d_backup3 = pdata->xkey[6];
  pdata->d_backup4 = pdata->xkey[7];
  pdata->d_backup7 = pdata->xkey[0];
  //printf("restore_key is okay\n");
}
//***********************************************
//加密-data_frame(单帧 512)
void encode_all(struct data_frame* pdata){
  struct encode_key xxkey;
  get_key(&xxkey);//获取8个int 密钥
  encode_uint(pdata,&xxkey);
  encode_char(pdata,&xxkey);
  hide_key(pdata,&xxkey);//隐藏钥匙frist
  //printf("encode_all is okay!!!!!!!!!!!!!!!!!!!!!!!1\n");//for test
}
//
//解码密-data_frame(单帧 512)
void decode_all(struct data_frame* pdata){
  struct encode_key xxkey;
  restore_key(pdata,&xxkey);//还原钥匙frist
  decode_uint(pdata,&xxkey);
  decode_char(pdata,&xxkey);
  //printf("decode_all is okay!!!!!!!!!!!!!!!!!!!!!!!1\n");//for test
}
//**********************************************
//*******************
//测试模块
//*******************
void test_encode(void){
//加密测试
struct data_frame xtest;
double d1 = 1.2;
double d2 = 1.33333333;
double d3 = 99999999.111;
xtest.comm = d1;
xtest.uid = d2;
xtest.age = d3;
xtest.sex = 4;
xtest.country = 5;
xtest.city = 6;
xtest.code = 7;
xtest.ts = 8;

xtest.d_backup8 = d1;
xtest.d_backup7 = d2;
xtest.d_backup6 = d3;
xtest.d_backup5 = 4;
xtest.d_backup4 = 5;
xtest.d_backup3 = 6;
xtest.d_backup2 = 7;
xtest.d_backup1 = 8;


sprintf(xtest.uname,"%s","a");
sprintf(xtest.upw,"%s","b");
sprintf(xtest.s_backup1,"%s","c");
sprintf(xtest.s_backup2,"%s","d");
sprintf(xtest.s_backup3,"%s","e");
sprintf(xtest.s_backup4,"%s","f");
sprintf(xtest.buf,"%s","g");




printf("原文～～～～～～～first\n");
//
//打印原文
printf("%lf",xtest.comm);
printf("\n");
printf("%lf",xtest.uid);
printf("\n");
printf("%lf",xtest.age );
printf("\n");
printf("%lf",xtest.sex);
printf("\n");
printf("%lf",xtest.country);
printf("\n");
printf("%lf",xtest.city);
printf("\n");
printf("%lf",xtest.code);
printf("\n");
printf("%lf",xtest.ts);
printf("\n\n");
printf("%lf",xtest.d_backup1);
printf("\n");
printf("%lf",xtest.d_backup2);
printf("\n");
printf("%lf",xtest.d_backup3);
printf("\n");
printf("%lf",xtest.d_backup4);
printf("\n");
printf("%lf",xtest.d_backup5);
printf("\n");
printf("%lf",xtest.d_backup6);
printf("\n");
printf("%lf",xtest.d_backup7);
printf("\n");
printf("%lf",xtest.d_backup8);
printf("\n\n");
//
printf("%s",xtest.uname);
printf("\n");
printf("%s",xtest.upw);
printf("\n");
printf("%s",xtest.s_backup1);
printf("\n");
printf("%s",xtest.s_backup2);
printf("\n");
printf("%s",xtest.s_backup3);
printf("\n");
printf("%s",xtest.s_backup4);
printf("\n");
printf("%s",xtest.buf);
printf("\n");

//
encode_all(&xtest);
/*
struct encode_key xxkey;
get_key(&xxkey);
encode_uint(&xtest,&xxkey);
encode_char(&xtest,&xxkey);
hide_key(&xtest,&xxkey);//隐藏钥匙frist
*/
//
//打印密文
printf("密文～～～～～～～second\n");
printf("%lf",xtest.comm);
printf("\n");
printf("%lf",xtest.uid);
printf("\n");
printf("%lf",xtest.age );
printf("\n");
printf("%lf",xtest.sex);
printf("\n");
printf("%lf",xtest.country);
printf("\n");
printf("%lf",xtest.city);
printf("\n");
printf("%lf",xtest.code);
printf("\n");
printf("%lf",xtest.ts);
printf("\n\n");
printf("%lf",xtest.d_backup1);
printf("\n");
printf("%lf",xtest.d_backup2);
printf("\n");
printf("%lf",xtest.d_backup3);
printf("\n");
printf("%lf",xtest.d_backup4);
printf("\n");
printf("%lf",xtest.d_backup5);
printf("\n");
printf("%lf",xtest.d_backup6);
printf("\n");
printf("%lf",xtest.d_backup7);
printf("\n");
printf("%lf",xtest.d_backup8);
printf("\n\n");
//
printf("%s",xtest.uname);
printf("\n");
printf("%s",xtest.upw);
printf("\n");
printf("%s",xtest.s_backup1);
printf("\n");
printf("%s",xtest.s_backup2);
printf("\n");
printf("%s",xtest.s_backup3);
printf("\n");
printf("%s",xtest.s_backup4);
printf("\n");
printf("%s",xtest.buf);
printf("\n");
//
decode_all(&xtest);
/*
restore_key(&xtest,&xxkey);//还原钥匙frist
decode_uint(&xtest,&xxkey);
decode_char(&xtest,&xxkey);
*/
//
//打印结果
printf("解密文～～～～～～～last\n");
printf("%lf",xtest.comm);
printf("\n");
printf("%lf",xtest.uid);
printf("\n");
printf("%lf",xtest.age );
printf("\n");
printf("%lf",xtest.sex);
printf("\n");
printf("%lf",xtest.country);
printf("\n");
printf("%lf",xtest.city);
printf("\n");
printf("%lf",xtest.code);
printf("\n");
printf("%lf",xtest.ts);
printf("\n\n");
printf("%lf",xtest.d_backup1);
printf("\n");
printf("%lf",xtest.d_backup2);
printf("\n");
printf("%lf",xtest.d_backup3);
printf("\n");
printf("%lf",xtest.d_backup4);
printf("\n");
printf("%lf",xtest.d_backup5);
printf("\n");
printf("%lf",xtest.d_backup6);
printf("\n");
printf("%lf",xtest.d_backup7);
printf("\n");
printf("%lf",xtest.d_backup8);
printf("\n\n");
//
printf("%s",xtest.uname);
printf("\n");
printf("%s",xtest.upw);
printf("\n");
printf("%s",xtest.s_backup1);
printf("\n");
printf("%s",xtest.s_backup2);
printf("\n");
printf("%s",xtest.s_backup3);
printf("\n");
printf("%s",xtest.s_backup4);
printf("\n");
printf("%s",xtest.buf);
printf("\n");


//1-2s 10W 还可以啊 -- 这种方法不考虑钥匙是否能还原, 先考虑加密和密文是否能够还原, 再想办法解决key 还原问题
//是设计加密算法的一些基本要素...先考虑能不能还原, 再考虑key 还原与隐藏
printf("big data test start!!\n");
int Xstart = time(NULL);
int xxxtmp = 0;
for(;xxxtmp < 100000;xxxtmp++){
	encode_all(&xtest);
	//
	decode_all(&xtest);
}
int Xend = time(NULL);
printf("takes time : %d---test count = %d\n",Xend - Xstart,xxxtmp);

return;
}

int main(void){
	test_encode();
	//test_xrandom();
	return 0;
}

