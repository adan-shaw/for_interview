/*
 * plan_2_test.c
 *
 *  Created on: Sep 15, 2017
 *      Author: adan
 */


int main(void){
  if(inet_connect_test() == -1){
	printf("plan_2 test fail !!\n");
	return -1;
  }
  return 0;
}
