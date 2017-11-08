/*
 * cre_sock.c
 *
 *  Created on: Aug 11, 2017
 *      Author: adan
 */

#include "cre_sock.h"

//创建一个srv 监听socket 并返回fd, 失败返回-1
int cre_sock_li(char* srv_ip, int srv_port){
  //创建inet socket address info
  struct sockaddr_in addr_srv;
  bzero(&addr_srv, sizeof(struct sockaddr_in));//全部置'\0'
  addr_srv.sin_family = AF_INET;
  addr_srv.sin_addr.s_addr = inet_addr(srv_ip);
  addr_srv.sin_port = htons(srv_port);

  //创建inet socket
  int fd_li = -1;
  if((fd_li = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    printf("create inet socket fail at at cre_sock.c, errno: %d\n", errno);
    close(fd_li);
    return -1;
  }

  //设置非阻塞模式
  if(fcntl(fd_li, F_SETFL, fcntl(fd_li, F_GETFD, 0) | O_NONBLOCK) == -1){
    printf("set nonblocking fail at cre_sock.c, errno: %d\n", errno);
    close(fd_li);
    return -1;
  }

  //绑定
  if(bind(fd_li, (struct sockaddr_in *)&addr_srv, sizeof(struct sockaddr_in)) == -1){
    printf("bind inet socket fail at at cre_sock.c, errno: %d\n", errno);
    close(fd_li);
    return -1;
  }

  //开始监听
  if(listen(fd_li, max_listen_queue) == -1){
    printf("listen inet socket fail at at cre_sock.c, errno: %d\n", errno);
    close(fd_li);
    return -1;
  }

  return fd_li;//成功返回fd
}


//创建一个cli fd 并自动conn srv, 成功返回fd, 失败返回-1
int cre_sock_cli(char* srv_ip, int srv_port){
  //创建inet socket address info
  struct sockaddr_in addr_cli;
  bzero(&addr_cli, sizeof(struct sockaddr_in));
  addr_cli.sin_family = AF_INET;
  addr_cli.sin_addr.s_addr = htonl(srv_ip);
  addr_cli.sin_port = htons(srv_port);

  //创建inet socket
  int fd_cli = -1;
  if((fd_cli = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	printf("create inet socket fail at at cre_sock.c, errno: %d\n", errno);
	close(fd_cli);
	return -1;
  }

  //设置非阻塞模式
  if(fcntl(fd_cli, F_SETFL, fcntl(fd_cli, F_GETFD, 0) | O_NONBLOCK) == -1){
	printf("set nonblocking fail at cre_sock.c, errno: %d\n", errno);
	close(fd_cli);
	return -1;
  }

  //链接server
  if(connect(fd_cli, (struct sockaddr_in *)&addr_cli, sizeof(struct sockaddr_in)) < 0){
	printf("connect server fail at cre_sock.c, errno: %d\n", errno);
	close(fd_cli);
	return -1;
  }

  return fd_cli;//成功返回fd
}


//通用ctl 设置 socket nonblocking
int fcntl_nonblocking(int fd){
  if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
    printf("socket %d set blocking fail, errno: %d\n", fd, errno);
    return -1;
  }
  return 0;
}
