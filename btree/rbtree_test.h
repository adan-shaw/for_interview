/*
 * rbtree_test.h
 *
 *  Created on: Oct 2, 2017
 *      Author: adan
 */

//由于rbtree 是系统自带的红黑树数据结构, 所以这里对它进行二次封装...
//这个红黑树只能是整型红黑数！！(不过用来存socket uint 够用了)

//红黑树的结构特点是: 查找性能出色(但是不是最好, 哈希查找性能才是比较强), 操作性能也出色,
                 //关键是数据膨胀能力强, 每多一层, 数据容量几乎翻一倍,
                 //而数据容量对查找和操作性能极小, 这是其它如哈希, 链表之类的结构无法比拟的
                 //所以如果你将哈希桶换成红黑树, 根本就是扯蛋, 还不如直接单一棵红黑树, 复杂度更低, 性能更好

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define rbtree_type_key int
//#define rbtree_type_val int
#define rbtree_type_val void*

//查找结果返回(你可以把它看成是迭代器的"结果")
struct rbt_data{
  struct rb_node data_node;//红黑树控制关键-左树, 右树, 红, 黑, 平衡...等
  rbtree_type_key key;//key(键的决定因素在rbt_search and rbt_insert 操作函数里面,
                          //rbt_delete 也是先rbt_search 再删除, 查找本身与红黑树结构无关..
                          //二分法也是在红黑树结构稳定之后, 再划分的)
  rbtree_type_val val;//value
};//sizeof(struct rbt_data) = 32

//**************************************************************
//这三个操作是不用修改的, 直接改节点的元素即可
//**************************************************************
//红黑树查找--成功返回<struct rbt_data>, 失败返回NULL
struct rbt_data* rbt_search(struct rb_root *root, rbtree_type_key key);
//红黑树插入(不能插入已经存在的数据) 成功返回0, 失败返回-1
int rbt_insert(struct rb_root *root, struct rbt_data *data);
//红黑树删除(成功删除返回0, 失败返回-1)
int rbt_delete(struct rb_root *root, rbtree_type_key key);
//**************************************************************


//打印整棵树(从小到大-中序遍历)
void print_rbtree_int_int(struct rb_root *root);
void print_rbtree_int_void(struct rb_root *root);
//测试主函数
int rbtree_test_int_int(void);
int rbtree_test_int_void(void);






//红黑树重用池
//40960*32/1024/1024 = 1.25mb -- 如果做个管理池大概就这么大～～但是ip 黑名单不可能只有4w 个ip
//但是这样只是为了满足临时操作的需求...达到重用的目的...
//线程独占-栈即可--
//对所有插入操作进行重构--原来为动态内存分配

/*
算了, 你都说～～如果别人冲击你, 你就只是为他读内存, 而不读磁盘...
那么肯定是先登录之后再写日志～～ 如果别人冲击你, 没办法的刷内存吧～～刷刷更健康...
反正第一件事就是登录...然后做个socket 表～～记录登录时间```在线表～～超时就踢```回应数据就清除
这样你就可以充分利用自己的加密设计了～～你传上来的数据是有加密的,
那么...做个首包验证永远比那些滑溜溜的技术要牛
 */
