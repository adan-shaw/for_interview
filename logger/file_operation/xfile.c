/*
 * xfile.c
 *
 *  Created on: Aug 19, 2017
 *      Author: adan
 */

#include "xfile.h"

//构造函数
void xfile_init(struct xfile* xfile_body){
  //初始化模块变量
  xfile_body->xfd = NULL;
  xfile_body->xfdi = -1;
  xfile_body->xfd_stat = 0;
  return;
}
//
//根据file path 和 mode 打开file...(带缓冲区工作)
int xfopen(struct xfile* xfile_body, char* fpath, char* mode){
  //参数安检
  if(fpath == NULL || mode == NULL){
	printf("fpath or mode is a empty string, please check xfopen()\n");
	return -1;
  }
  //执行打开操作
  xfile_body->xfd = fopen(fpath, mode);
  if(xfile_body->xfd == NULL){
	printf("xfopen() fopen file fail, errno: %d\n", errno);
	xfile_body->xfd_stat = -1;
	return -1;
  }
  xfile_body->xfdi = fileno(xfile_body->xfd);
  return 0;
}
//
//关闭file(fclose()内部会调用close()来关闭相应的fd)
void xclose(struct xfile* xfile_body){
  if(xfile_body->xfd_stat == 1 && xfile_body->xfd != NULL){//正常模块delete
	if(fclose(xfile_body->xfd) != NULL){
	  printf("xclose fail !!errno: %d\n", errno);
	  xfile_body->xfd_stat = -1;
	}
  }
  if(xfile_body->xfd_stat == -1 && xfile_body->xfd != NULL){//异常情况下模块delete
    if(fclose(xfile_body->xfd) != NULL){
	  printf("xclose fail !!errno: %d\n", errno);
	  xfile_body->xfd_stat = -1;
    }
  }
  xfile_body->xfd_stat = 0;
}
//
//冲刷缓冲区
void xfflush(struct xfile* xfile_body){
  int tmp = fflush(xfile_body->xfd);
  if(tmp != 0){
	printf("fflush fail !!errno: %d\n", errno);
	xfile_body->xfd_stat = -1;
  }
}
//
//字符串读--返回读的size, 失败返回-1...请传入缓冲区指针pdata
int read_str(struct xfile* xfile_body, off_t xpos, void* pdata, ssize_t xlen){
  int tmp = pread(xfile_body->xfdi, pdata, xlen, xpos);
  if(tmp == -1){
    printf("read_str->pread fail, errno: %d\n",errno);
    xfile_body->xfd_stat = -1;
    return -1;
  }
  else
	return tmp;
}
//
//当前pos读
int read_cur(struct xfile* xfile_body, void* pdata, ssize_t xlen){
  ssize_t tmp = read(xfile_body->xfdi, pdata, xlen);
  if (tmp == -1){
	printf("read_cur fail, errno: %d\n", errno);
	xfile_body->xfd_stat = -1;
	return -1;
  }
  else
	return (int)tmp;
}
//
//字符串写--返回写的size, 失败返回-1...请传入缓冲区指针pdata
int write_str(struct xfile* xfile_body, off_t xpos, void* pdata, ssize_t xlen){
  int tmp = pwrite(xfile_body->xfdi, pdata, xlen, xpos);
  if(tmp == -1){
	printf("write_str->pwrite fail, errno: %d\n",errno);
	xfile_body->xfd_stat = -1;
	return -1;
  }
  else
	return tmp;
}
//
//当前pos写
int write_cur(struct xfile* xfile_body, void* pdata, ssize_t xlen){
  ssize_t tmp = write(xfile_body->xfdi, pdata, xlen);
  if (tmp == -1){
	printf("write_cur fail, errno: %d\n", errno);
	xfile_body->xfd_stat = -1;
	return -1;
  }
  else
	return (int)tmp;
}
//
//修改当前pos--where_start = SEEK_SET | SEEK_CUR | SEEK_END
void xpos(struct xfile* xfile_body, int where_start, off_t xpos){
  int tmp = lseek(xfile_body->xfdi, xpos, where_start);
  if(tmp == -1){
	printf("xfile::xpos, errno: %d\n", errno);
	xfile_body->xfd_stat = -1;
  }
}
//
//模块测试函数(外人不准用)
void test_xfile(void){
  printf("test_xfile start now::\n");
  struct xfile xfd;
  xfopen(&xfd,"./test","w+");//打开file, + 号表示文件不存在时自动创建
  char test[] = "i am the test message";
  write_cur(&xfd,&test,sizeof(test));//当前写
  write_str(&xfd,sizeof(test),&test,sizeof(test));//pos 写
  xfflush(&xfd);
  xclose(&xfd);
  //再次使用测试
  xfopen(&xfd,"./test","r");
  char buf[72];
  char buf2[72];
  read_cur(&xfd,&buf,sizeof(buf));
  read_str(&xfd,0,&buf2,sizeof(buf2));
  printf("%s\n%s\n", buf, buf2);
  xclose(&xfd);
}

//*********************************************************
//
/*
//
//加读锁
int xfile::xlock(int rw, off_t xpos, ssize_t xlen){
  struct flock lock;//文件锁
  if(rw == 0)
    lock.l_type = F_RDLCK;//读锁(共享锁)
  else
    lock.l_type = F_WRLCK;//写锁(互斥锁)
  lock.l_pid = getpid();//锁定的pid
  lock.l_whence = SEEK_SET;//从文件的开始位置
  lock.l_start = xpos;//加锁的开始位置为xpos
  lock.l_len = xlen;//长度为xlen
  //执行加锁
  int res = fcntl(xfile_body->xfdi, F_SETLK, &lock);
  if(-1 == res){
    printf("xlock fcntl lock fail, file might now using, errno: %d\n",errno);
    return -1;//加锁失败
  }
  else
	return 0;
}
//
//解读锁
int xfile::xunlock(off_t xpos, ssize_t xlen){
  struct flock lock;//文件锁
  lock.l_type = F_UNLCK;//解锁
  lock.l_pid = getpid();//锁定的pid
  lock.l_whence = SEEK_SET;//从文件的开始位置
  lock.l_start = xpos;//解锁的开始位置为xpos
  lock.l_len = xlen;//长度为xlen
  //执行解锁
  int res = fcntl(xfile_body->xfdi, F_SETLK, &lock);
  if(-1 == res){
	printf("xunlock fcntl lock fail, file might now using, errno: %d\n",errno);
	return -1;//解锁失败
  }
  else
	return 0;
}
*/
//*********************************************************
//
/*
//多块缓冲区聚合写(暂时弃用)
int xfile::read_bp(off_t xpos, void* pdata, ssize_t xlen){
  struct iovec iov;
  iov.iov_base = pdata;
  iov.iov_len = xlen;

  int iov_count = 1;//有多少个缓冲区, 就有多少个iov, 这个是iov_count
  int tmp = readv(xfile_body->xfdi, &iov ,iov_count);
  if(tmp == -1){
	printf("write_bp fail, errno: %d\n",errno);
	return -1;
  }
  else
	return tmp;
}
//
//多块缓冲区聚合写(暂时弃用)
int xfile::write_bp(off_t xpos, void* pdata, ssize_t xlen){
  struct iovec iov;
  iov.iov_base = pdata;
  iov.iov_len = xlen;

  int iov_count = 1;//有多少个缓冲区, 就有多少个iov, 这个是iov_count
  int tmp = writev(xfile_body->xfdi, &iov ,iov_count);
  if(tmp == -1){
    printf("write_bp fail, errno: %d\n",errno);
    return -1;
  }
  else
	return tmp;
}
*/
