/*
 * btree_test_main.c
 *
 *  Created on: Nov 2, 2017
 *      Author: adan
 */

#include "binary_tree.h"

int main(void){
  binary_tree_test();
  rbtree_test_int_int();
  rbtree_test_int_void();

  return 0;
}
