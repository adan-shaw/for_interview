/*
 * inet_connect.h
 *
 *  Created on: Sep 14, 2017
 *      Author: adan
 */

#include <sys/types.h>//socket needs
#include <sys/socket.h>

#include <netinet/in.h>//socket info needs

#include <netdb.h>//gethostbyname needs

#include <arpa/inet.h>//struct in_addr needs -- 没有这个头文件, inet_ntoa 不会报错, 但是会内存溢出, 很坑, 小心

#include <stdio.h>
#include <errno.h>

#define http_get_request_buf_max 4096
#define http_get_request_data_buf_max 3072

//socket connect < 根据ip 字符串和int port创建socket 链接, 成功返回socket fd, 失败返回-1 >
int inet_conn_sock(char* srv_ip, int port);

//host service connect - < 根据host_name 字符串链接host 下的服务, 默认端口是:tcp 方式链接...成功返回socket fd, 失败返回-1>
//注意: 如果你需要直接链接ip , 请直接ip + service_port, 如: http 端口默认 = 80

//service_name list: (如果链接方式不支持, 服务器不会返回正确值)
//1.daytime = url 下面的服务器群的时间服务, 默认用tcp 的方式连接
//2.http = url 下面的服务器群的http 服务, 默认用tcp 的方式连接
//service_name 都是用正式的名字的, 如http, ftp, 之类
int inet_conn_service(char* host_name, char* service_name);

//模块自测函数
int inet_connect_test(void);

