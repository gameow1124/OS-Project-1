#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
typedef struct child{
	int id;
	char name[100];
	int rd_t;
	int ex_t;
}child;






int main(int argc, char* argv[])
{
	char policy[10];
	int child_num;
	child child_q[100];
	scanf("%s",policy);
	scanf("%d",&child_num);
	for(int i = 0;i < child_num;i++){
		child_q[i].id = i;
		scanf("%s%d%d",child_q[i].name, &child_q[i].rd_t, &child_q[i].ex_t);
	}
	


	return 0;

}
