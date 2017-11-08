/*
 * inet_connect.c
 *
 *  Created on: Oct 13, 2016
 *      Author: adan
 */

//学习网络编程看看这片文章
//http://www.cnblogs.com/benxintuzi/p/4589819.html


#include "inet_connect.h"
//************************
//socket connect < 根据ip 字符串和int port创建socket 链接, 成功返回socket fd, 失败返回-1 >
//************************
int inet_conn_sock(char* srv_ip, int port){
  //create socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sfd < 0) {
    printf("<inet_conn_sock> create socket fail, errno: %d\n", errno);
    return -1;
  }

  //init server info
  struct sockaddr_in addr_tmp;
  addr_tmp.sin_addr.s_addr = inet_addr(srv_ip);
  addr_tmp.sin_family = AF_INET;
  addr_tmp.sin_port = htons(port);

  //start connect
  if(connect(sfd, &addr_tmp, sizeof(struct sockaddr_in)) < 0){
    printf("<inet_conn_sock> connect %s:%d fail, errno: %d\n", srv_ip, port, errno);
    close(sfd);
    return -1;
  }

  printf("<inet_conn_sock> connect %s:%d success\n", srv_ip, port);
  return sfd;
}
//************************
/*
  struct servent{
    char* s_name;//服务名称
    char** s_aliases;//服务的别名列表
    int s_port;//端口
    char* s_proto;//服务类型，tcp或udp
  };
  struct hostent{
    char* h_name;//主机名
    char** h_aliases;//主机别名列表
    int h_a_addrtype;//地址类型（地址族）
    int h_length;//地址长度
    char** h_addr_list;//按网络字节序列的主机IP地址列表
  };
*/

/*
  #include <arpa/inet.h>
  void in_addr_example(void){
  in_addr_t x;
  char *z;
  char *y = (char *)&x;

  y[0] = 12;
  y[1] = 34;
  y[2] = 56;
  y[3] = 78;

  z = inet_ntoa(*(struct in_addr *)&x);
  printf("z = %s\n", z);
  return ;
  }
 */
//host service connect - < 根据host_name 字符串链接host 下的服务, 默认端口是:tcp 方式链接...成功返回socket fd, 失败返回-1>
//注意: 如果你需要直接链接ip , 请直接ip + service_port, 如: http 端口默认 = 80

//service_name list: (如果链接方式不支持, 服务器不会返回正确值)
//1.daytime = url 下面的服务器群的时间服务, 默认用tcp 的方式连接
//2.http = url 下面的服务器群的http 服务, 默认用tcp 的方式连接
//service_name 都是用正式的名字的, 如http, ftp, 之类
//************************
int inet_conn_service(char* host_name, char* service_name){
  //create socket
  int sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(sfd < 0) {
    printf("<inet_conn_service> create socket fail, errno: %d\n", errno);
    return -1;
  }

  //get ip by host name(实际只是为了获取ip)
  struct hostent *hostinfo = gethostbyname(host_name);

  //获取服务信息(实际只是为了获取端口)
  struct servent *servinfo = getservbyname(service_name,"tcp");

  //init server info
  struct sockaddr_in addr_tmp;
  addr_tmp.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;//因为h_addr_list本身就是网络字节序的地址列表，故直接赋值
  addr_tmp.sin_family = AF_INET;
  addr_tmp.sin_port = servinfo->s_port;

  //测试获取到的ip and port
  //printf("%d\n",addr_tmp.sin_addr.s_addr);//addr_tmp.sin_addr.s_addr = in_addr_t(是一个指针)
  struct in_addr xtmp;
  //xtmp.s_addr = *(struct in_addr*)*hostinfo->h_addr_list;
  memcpy(&xtmp, &addr_tmp.sin_addr.s_addr, 4);

  //backup ip address string
  char xip[16];
  memcpy(&xip,inet_ntoa(xtmp),16);
  int xport = ntohs(servinfo->s_port);
  //printf("host %s ip: %s:, service %s-port: %d\n", host_name, xip, service_name, xport);
  //printf("host %s ip: %s, service %s-port: %d\n", host_name, inet_ntoa(xtmp), service_name, ntohs(servinfo->s_port));

  //start connect
  if(connect(sfd, &addr_tmp, sizeof(struct sockaddr_in)) < 0){
    printf("<inet_conn_service> connect %s:%d fail, errno: %d\n", xip, xport, errno);
    close(sfd);
    return -1;
  }

  printf("<inet_conn_service> connect host %s ip: %s:, service %s-port: %d success\n", host_name, xip, service_name, xport);
  return sfd;
}
//
//get 请求字符串填充(需要传入buf host, file_path, data(data max = 3072))
//buf 默认4kb, 保留一个1kb 做头数据, 3kb data(尽量控制data 长度和http 头长度)
void http_get_str_init(char* buf, char* host, char* file_path, char* data){
  char data_len_str[32];
  memset(data_len_str, '\0', 32);
  int data_len = strlen(data);
  sprintf(data_len_str, "%d", data_len);//组建data 长度字符串

  memset(buf, '\0', http_get_request_buf_max);
  strcat(buf, "GET ");
  strcat(buf, file_path);
  strcat(buf, " HTTP/1.1\n");
  strcat(buf, "Host: www.baidu.com\n");
  strcat(buf, "Content-Type: text/html\n");
  strcat(buf, "Content-Length: ");
  strcat(buf, &data_len_str);
  strcat(buf, "\n\n");

  strcat(buf, data);
  strcat(buf, "\r\n\r\n");

  printf("%s\n",buf);//for test
  /*
  //组建简单的测试get 请求( 原文：http://www.cnblogs.com/diligenceday/p/6255788.html)
  char str1[4096], str2[4096], *str;
  socklen_t len;

  memset(str2, 0, 4096);
  strcat(str2, "theDataToPost");
  str=(char *)malloc(128);
  len = strlen(str2);
  sprintf(str, "%d", len);

  memset(str1, 0, 4096);
  strcat(str1, "GET / HTTP/1.1\n");
  strcat(str1, "Host: www.baidu.com\n");
  strcat(str1, "Content-Type: text/html\n");
  strcat(str1, "Content-Length: ");
  strcat(str1, str);
  strcat(str1, "\n\n");

  strcat(str1, str2);
  strcat(str1, "\r\n\r\n");
  printf("%s\n",str1);

  int x = send(ssfd_http, &str1, sizeof(str1), 0);//执行上传操作


  char xhtml[4096];
  x = recv(ssfd_http, &xhtml, sizeof(xhtml), 0);//接受服务器下发的数据
  printf("%d\n",x);
  printf("%s\n",xhtml);

  shutdown(ssfd_http,2);
  close(ssfd_http);

  */
  return;
}

//
//测试函数
int inet_connect_test(void){
  //inet_conn_sock 部分需要互测, 而且也简单, 先不测试...
  char host[] = "www.baidu.com";


  //inet_conn_service - get host service socket(阻塞函数)
  int ssfd_http = inet_conn_service(&host, "http");
  printf("http socket fd = : %d\n", ssfd_http);
  //连接上百度之后就阻塞了, www.baidu.com 只是服务器host_name, 你链接上服务器后, 还是需要提交操作请求才能获取到返回信息的
  //否则直接read data, 服务器不会下发任何数据, 你这样只会陷入阻塞

  char data[http_get_request_data_buf_max];
  memset(data, '\0', http_get_request_data_buf_max);
  sprintf(data, "%s", "hello baidu.com");
  char http_get_str[http_get_request_buf_max];
  //执行get 请求组装
  http_get_str_init(&http_get_str, &host, "/", &data);

  int x = send(ssfd_http, &http_get_str, sizeof(http_get_str), 0);//执行上传操作


  char xhtml[4096];
  x = recv(ssfd_http, &xhtml, sizeof(xhtml), 0);//接受服务器下发的数据
  printf("%d\n",x);
  printf("%s\n",xhtml);

  shutdown(ssfd_http,2);
  close(ssfd_http);




/*
  //inet_conn_service - get host service socket(阻塞函数)
  int ssfd_daytime = inet_conn_service(&host, "daytime");
  printf("daytime socket fd = : %d\n", ssfd_daytime);

  char buf[128];
  memset(&buf,0,sizeof(buf));
  x = write(ssfd_http, &str1, sizeof(str1));//执行上传操作

  x = read(ssfd_daytime, &buf, sizeof(buf));//接受服务器下发的数据
  if(x < 0){
	printf("daytime test fail !read socket fail !!");
	shutdown(ssfd_daytime,2);
	close(ssfd_daytime);
	return -1;
  }
  printf("daytime return: %s\n", buf);
  shutdown(ssfd_daytime,2);
  close(ssfd_daytime);
*/

  return 0;
}





