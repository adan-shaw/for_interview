/*
 * xfile.h
 *
 *  Created on: Aug 19, 2017
 *      Author: adan
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>//pread and pwrite lseek
#include <sys/uio.h>//readv and writev
#include <sys/types.h>//lseek

#define max_filepath 256

//把FILE *的文件描述符转换成int型的fd函数是：int fileno(FILE *tream)
//把int 型的fd 文件描述符转换成FILE *的文件描述符函数是：FILE *fdopen(int fd, const char *mode)
struct xfile{
  //文件描述符
  FILE* xfd;
  int xfdi;
  //每次操作都检查这个标志, 0 表示没有错误但没有打开fd, -1 表示已经出错, 1 表示正常并已经打开fd
  int xfd_stat;//不用记录错误次数, 但是要记录工作状态, 正常工作就继续工作
};


//写操作自动加锁, 读操作需要传入参数确定是否加锁(加锁默认都是从SEEK_SET 文件开始位置开始数起)
//文件支持行操作, 一个文件根据pos 的不同可以被多个程序同时读写, 等待加锁还需要do_lock_file_wait()
//加读锁(弃用-复杂度太高)
//int xlock(int rw, off_t xpos, ssize_t xlen);
//解读锁
//int xunlock(off_t xpos, ssize_t xlen);

//根据file path 和 mode 打开file...(带缓冲区工作)--成功返回0, 失败返回-1
int xfopen(struct xfile* xfile_body, char* fpath, char* mode);//二进制or 字符读写都following mode 变量
//关闭file
void xclose(struct xfile* xfile_body);
//冲刷缓冲区
void xfflush(struct xfile* xfile_body);

//读需要传入int lock 1 对文件加锁读, 0 不加锁读(弃用, 复杂度太高)
//***目前这个类的file 是假设为独占的, 每个fd 独占一个file, 共享会出现未知错误***
//字符串读--返回读的size, 失败返回-1...请传入缓冲区指针pdata, xpos = 0, 当前pos 开始读
int read_str(struct xfile* xfile_body, off_t xpos, void* pdata, ssize_t xlen);
//当前pos读
int read_cur(struct xfile* xfile_body, void* pdata, ssize_t xlen);

//写默认加锁(现在并没有实现无锁)
//字符串写--返回写的size, 失败返回-1...请传入缓冲区指针pdata, xpos = 0, 当前pos 开始写
int write_str(struct xfile* xfile_body, off_t xpos, void* pdata, ssize_t xlen);
//当前pos写
int write_cur(struct xfile* xfile_body, void* pdata, ssize_t xlen);

//加锁的主要对象是文件, 不是fd 本身, 每个fd 对不同的file 中的seek_pos 都是唯一的
//修改当前pos--where_start = SEEK_SET | SEEK_CUR | SEEK_END
void xpos(struct xfile* xfile_body, int where_start, off_t xpos);//lseek 每个文件描述符都不一样, 不需要加锁

//多块缓冲区聚合读(暂时弃用)
//int read_bp(off_t xpos, void* pdata, ssize_t xlen);
//多块缓冲区聚合写(暂时弃用)
//int write_bp(off_t xpos, void* pdata, ssize_t xlen);

void test_xfile(void);//模块测试函数(外人不准用)
