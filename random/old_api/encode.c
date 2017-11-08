/*
 * encode.c
 *
 *  Created on: Apr 9, 2017
 *      Author: adan
 */

#include <rpc/des_crypt.h>//关键头文件
#include <stdio.h>

//这是一个公版加密算法

/*
des_crypt, ecb_crypt, cbc_crypt, des_setparity, DES_FAILED
都是用于DES快速加密
调用格式
  #include <rpc/des_crypt.h>

  int ecb_crypt(char *key, char *data, unsigned datalen, unsigned mode);

  int cbc_crypt(char *key, char *data, unsigned datalen, unsigned mode, char *ivec);

  void des_setparity(char *key);

  int DES_FAILED(int status);

ecb_crypt()  和  cbc_crypt()遵循NBS DES, 这些加密函数比crypt更快、更加通用
如果DES硬件有效, 那么这些加密函数也可以使用它们

ecb_crypt()工作于ECB模式(电子密码本模式, 把要加密的数据分成一块块独立的数据
然后再进行加密, 并行), 加密每一个独立数据块。

cbc_crypt()工作于CBC模式（密码块链接模式，把数据分成多个块,
每个块加密前与前一个块的密文进行亦或操作, 然后再加密，串行）, CBC比ECB更安全

使用说明
参数key是指8字节的密钥, 使用之前需对密钥进行奇偶性设置des_setparity(char *key)
参数data指需要加密或者解密的源数据
参数datalen指源数据长度, 必须是8的倍数, 不是8倍数，则datalen += 8 - datalen%8等其它操作让其变为8的倍数
参数mode 可以用OR来制定多个模式, 包括加密DES_ENCRYPT, 解密DES_DECRYPT
软件加密或硬件加密是DES_HW, DES_SW. 如果指定了DES_HW, 但是相应的硬件不支持,
你那么默认会使用软件加密并且返回DESERR_NOHWDEVICE


返回值:
  DESERR_NONE         成功
  DESERR_NOHWDEVICE   加密成功，但是使用的是软件加密而不是硬件加密
  DESERR_HWERROR      硬件或者驱动出现错误
  DESERR_BADPARAM     参数错误

linux中使用DES加密解密
key要使用des_setparity


*/


//用公版的钥匙，然后一定要定期更新密钥
//用生成随机数的方式，用随机数做密钥--密钥是8位--不要用两种加密方案，不合理
//但是人家仍然可以截获然后破解遍历
//只是如果截获的数据破解后，下一次又要不同的密钥，又要重新算过
//但是一次频繁发送和接受，就可以分析大量的数据了，在同一时间点内
//所以密钥
//先用离线比对时间的方式，如果通过的话，就用离线实时比对的方式发送第一次密钥
//如果失败就用默认密钥对比通信
//接获系统时间，比较差值--用一个变量存放差值，然后还是用离线比对的手法来现实
//计算密钥--时间因子，自动更换密钥--client 自动更换，服务器就不用每次都下发，全体下发
//当然还有算法=数字，还有字符和标点符号
void encode()
{ int b_loop, b_len;
  long long b_begin, b_end;
  char b_key[100] = {0};
  char * b_test_data = NULL;

  sprintf(b_key, "%s", "123");

  b_test_data = (char *)malloc(100);

  memset(b_test_data, 0 , 100);
  b_len = sprintf(b_test_data, "%s", "abcdefghijklmnopqrstuvwxyz");

  if(b_len % 8){
    b_len += 8 - b_len % 8;
  }

  //参数决定加密还是解密
  ecb_crypt(b_key, b_test_data, b_len, DES_ENCRYPT);//加密--DES_ENCRYPT

  printf("time = %lu, val = %s\n", b_end - b_begin, b_test_data);

  ecb_crypt(b_key, b_test_data, b_len, DES_DECRYPT);//解密--DES_DECRYPT

  printf("data = %s\n", b_test_data);

  return ;

}
