#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/fs.h>
#include <sys/uio.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define FILEPATH_MAX 256


void general_io(void){
  char buf[BUF_SIZE];

  char file_path[FILEPATH_MAX];//文件路径
  //
  int flags = O_CREAT | O_WRONLY | O_TRUNC;
  // user:rw-group:rw-other:rw-
  mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  //打开file, open 一般是直接写盘的函数(详细解析请翻到最下面)
  int fd = open(file_path, flags, perms);
  if (fd == -1)
    printf("open file '%s' fail, errno:%d\n", errno);

  //读取函数, 读取出来是字符还是二进制代码, 根据打开时候的方式判断
  ssize_t ret_r = read(fd, buf, BUF_SIZE);
  if (ret_r == -1)
    printf("read file '%s' fail, errno:%d\n", errno);

  //写入函数, 读取出来是字符还是二进制代码, 根据打开时候的方式判断
  ssize_t ret_w = write(fd, buf, BUF_SIZE);
  if (ret_w == -1)
    printf("write file '%s' fail, errno:%d\n", errno);

  if (close(fd) == -1)//关闭文件fd
    printf("close file '%s' fail, errno:%d\n", errno);

  /*
  FILE *fopen(char *filename, char *mode)
  mode 打开模式：
  r   只读方式打开一个文本文件
  rb  只读方式打开一个二进制文件
  w   只写方式打开一个文本文件
  wb  只写方式打开一个二进制文件
  a   追加方式打开一个文本文件
  ab  追加方式打开一个二进制文件
  r+  可读可写方式打开一个文本文件
  rb+ 可读可写方式打开一个二进制文件
  w+  可读可写方式创建一个文本文件
  wb+ 可读可写方式生成一个二进制文件
  a+  可读可写追加方式打开一个文本文件
  ab+ 可读可写方式追加一个二进制文件
  */
  fd = fopen(&file_path, "rb+");//只读二进制打开file...用FILE * 指针更好

  //冲刷io 缓冲区(fopen 特有)
  fflush(fd);

  //关闭fd(fopen 特有, 最好是FILE * 指针)
  fclose(fd);


  //当前文件偏移量修改...
  int seek_whence = SEEK_SET | SEEK_CUR | SEEK_END;
  off_t offset;
  int ret_seek = lseek(fd, offset, seek_whence);
  if (ret_seek == -1)
    printf("seek place on file '%s' fail, errno:%d\n", errno);

  // int fcntl(int fd, int cmd, ... /* arg */ );
  int cmd = F_DUPFD | F_GETFD | F_SETFD |
		F_GETFL | F_SETFL | F_GETLK |
		F_SETLK | F_SETLKW | F_SETLK ;
  int ret_fcntl = fcntl(fd, cmd);
  if (ret_fcntl < 0)
    printf("file '%s' open fcntl fail, errno:%d\n",errno);

  //!>通用IO 操作配置
  //!> int ioctl(int d, unsigned long request, ...);
  int attr = FS_APPEND_FL | FS_NOATIME_FL |  FS_COMPR_FL |
		 FS_NODUMP_FL | FS_DIRSYNC_FL |  FS_IMMUTABLE_FL |
		 FS_JOURNAL_DATA_FL | FS_SECRM_FL | FS_SYNC_FL |
		 FS_NOTAIL_FL | FS_TOPDIR_FL | FS_UNRM_FL ;
  int ret_ioctl = ioctl(fd, attr);
  if (ret_ioctl == -1)
    printf("file '%s' open file '%s' open ret_fcntl  fail, errno:%d\n", errno);

  //flock 锁初始化
  //文件锁的用法之一, 锁住程序不允许第二次调用...
  struct flock lock;//文件锁
  lock.l_type = F_RDLCK;//加读锁
  lock.l_whence = SEEK_SET;//从文件的开始位置
  lock.l_start = 10;//加锁的开始位置为10
  lock.l_len = 100;//长度为100

  //执行加锁
  int res = fcntl(fd,F_SETLK,&lock);
  if(-1 == res)
  	;//加锁失败
  else
  	;//加锁成功

  //执行解锁
  lock.l_type = F_UNLCK;
  res = fcntl(fd,F_SETLK,&lock);
  if(-1 == res)
	;//解锁失败
  else
	;//解锁成功

  //*************************************************
  //其他的读写方式
  //*************************************************
  //pread 直接移动到目标位置读取
  // ssize_t pread(intfd, void *buf, size_t count, off_t offset);
  size_t count_pr;
  off_t offset_pr;
  int ret_pr = pread(fd, buf, count_pr, offset_pr);
  if(ret_pr ==-1)
    printf("file pread fail, errno:%d\n",errno);

  //pwrite 直接移动到目标位置写入
  //ssize_t pwrite(intfd, const void *buf, size_tcount, off_toffset)；
  size_t count_pw;
  off_t offset_pw;
  int ret_pw = pwrite(fd, buf, count_pw, offset_pw);
  if(pwrite ==-1)
    printf("file pwrite fail, errno:%d\n",errno);

  //散布读,即将文件中若干连续的数据块读入内存分散的缓冲区中
  // ssize_t readv(int filedes,const struct iovec *iov,int iovcnt);
  //块内存读(可以读类)
  struct iovec *iov_r;
  int ret_rv = readv(fd, iov_r ,3);
  if (ret_rv == -1)
    printf("file readv fail, errno:%d\n",errno);

  //聚集写,即收集内存中分散的若干缓冲区中的数据写至文件的连续区域中
  // ssize_t writedv(int filedes,const struct iovec *iov,int iovcnt);
  //块内存写(可以写类)
  struct iovec *iov_w;
  int ret_wv = writev(fd, iov_w ,3);
  if (ret_wv == -1)
    printf("file writev fail, errno:%d\n",errno);

  //截断文件: 将参数path指定的文件大小改为参数length指定的大小
  //如果原来的文件大小比参数length大,则超过的部分会被删除
  // int truncate(const char *path, off_t length);
  off_t offset_df;
  int ret_df = truncate("file path", offset_df);
  if (ret_df == -1)
    printf("file truncate() fail, errno:%d\n",errno);

  //截断写入: 以写入模式打开的文件,如果原来的文件大小比参数length 大,则超过的部分会被删去
  // int ftruncate(int fd, off_t length);
  off_t offset_dw;
  int ret_dw = ftruncate(fd, offset_dw);
  if (ret_dw == -1)
    printf("file ftruncate() fail, errno:%d\n",errno);

  //创建临时文件-临时文件使用完成后应及时删除,只有当前用户才能访问
  // int mkstemp(char *template);
  fd = mkstemp("/tmp/temp.txt");
  if (fd == -1)
    printf("create temp file fail--mkstemp(), errno:%d\n",errno);

}


/*
 * //读取int 数据时, 记得用rb ~~ 读取b, 二进制数据
 *

open和fopen的区别：
一般用fopen, 因为open 没有缓冲拼命写磁盘, 除了性能地狱, 设备损坏之外, 延时, 冲突, 简直就是渣...
文件io 是需要加锁的, 即使是用fopen 也需要加锁...flock 文件锁比较专业

前者属于低级IO, 后者是高级IO
前者返回一个文件描述符, 后者返回一个文件指针
前者无缓冲, 后者有缓冲
前者与 read, write 等配合使用, 后者与 fread, fwrite等配合使用
后者是在前者的基础上扩充而来的, 在大多数情况下, 用后者

下面是open 与fopen 的一些详细参数
//打开文件-O_WRONLY=只读，O_APPEND=追加，O_NONBLOCK=非阻塞，O_CREAT=文件不存在自动创建
//第三参数是权限控制：默认属主打开
int fd = open(file_path, O_WRONLY|O_APPEND|O_NONBLOCK|O_CREAT);
int open(const char *path, int access,int mode) --  path 要打开的文件路径和名称
													access 访问模式，宏定义和含义如下：
        O_RDONLY         1    只读打开
        O_WRONLY         2    只写打开
        O_RDWR           4    读写打开
        还可选择以下模式与以上3种基本模式相与：
            O_CREAT     0x0100   创建一个文件并打开
            O_TRUNC     0x0200   打开一个已存在的文件并将文件长度设置为0，其他属性保持
            O_EXCL      0x0400   未使用
            O_APPEND    0x0800   追加打开文件
            O_TEXT      0x4000   打开文本文件翻译CR-LF控制字符
            O_BINARY    0x8000   打开二进制字符，不作CR-LF翻译
    mode 该参数仅在access=O_CREAT方式下使用，其取值如下：
        S_IFMT      0xF000   文件类型掩码
        S_IFDIR     0x4000   目录
        S_IFIFO     0x1000   FIFO 专用
        S_IFCHR     0x2000   字符专用
        S_IFBLK     0x3000   块专用
        S_IFREG     0x8000   只为0x0000
        S_IREAD     0x0100   可读
        S_IWRITE    0x0080   可写
        S_IEXEC     0x0040   可执行


 */
