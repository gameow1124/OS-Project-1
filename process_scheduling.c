#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
typedef struct child{
	int id;
	char name[100];
	int rd_t;
	int ex_t;
}child;






int main(int argc, char* argv[])
{
	char policy_name[10];
	int child_num;
	int policy = 0;
	child child_q[100];
	scanf("%s",policy_name);
	scanf("%d",&child_num);
	for(int i = 0;i < child_num;i++){
		child_q[i].id = i;
		scanf("%s%d%d",child_q[i].name, &child_q[i].rd_t, &child_q[i].ex_t);
	}
	if(strcmp(policy_name, "FIFO") == 0)
		policy = 0;
	else if(strcmp(policy_name, "RR") == 0)
		policy = 1;
	else if(strcmp(policy_name, "SJF") == 0)
		policy = 2;
	else
		policy = 3;
	printf("%d",policy);
		
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if(sched_setaffinity(0, sizeof(mask), &mask)<0)
	{	
		perror("sched_setaffinity error");
		return -1;
	}	

	return 0;

}
