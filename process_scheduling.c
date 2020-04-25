#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
typedef struct child{
	char name[100];
	int rd_t;
	int ex_t;
	int left_t;
	pid_t pid;
}child;

child child_q[100];

int setcore(int core){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
	if(sched_setaffinity(0, sizeof(mask), &mask)<0)
	{	
		perror("sched_setaffinity error");
		exit(EXIT_FAILURE);
	}	
	return 0;
}

void unittime()
{
	volatile unsigned long i;
	for(i=0;i<1000000UL;i++);
}

int compare(const void *a, const void *b)
{
	child* a1 = (child *)a;
	child* b1 = (child *)b;
	return (a1->rd_t - b1->rd_t);
}
void setIDLE(pid_t pid){
	struct sched_param param;
	param.sched_priority=0;
	if(sched_setscheduler(pid,SCHED_IDLE,&param)<0)
	{
		perror("setscheduler fail");
		exit(EXIT_FAILURE);
	}
}
void setACTIVE(pid_t pid){
	struct sched_param param;
	param.sched_priority=0;
	if(sched_setscheduler(pid,SCHED_OTHER,&param)<0)
	{
		perror("setscheduler fail");
		exit(EXIT_FAILURE);
	}
}
void fork_child(int who)
{
	pid_t pid = fork();
	if(pid<0){
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		setcore(1);
		setIDLE(getpid());
		long long start_time, end_time, d_time;
		int exec_time = child_q[who].ex_t;
		for(int j = 0;j<exec_time;j++)
			unittime();
		fprintf(stderr,"%s %d\n", child_q[who].name, getpid());
		exit(0);
	}
	
	child_q[who].pid = pid;
}
int main(int argc, char* argv[])
{
	char policy_name[10];
	int child_num;
	int policy = 0;
	scanf("%s",policy_name);
	scanf("%d",&child_num);
	for(int i = 0;i < child_num;i++){
		scanf("%s%d%d",child_q[i].name, &child_q[i].rd_t, &child_q[i].ex_t);
		child_q[i].left_t = -1;
	}
	if(strcmp(policy_name, "FIFO") == 0)
		policy = 0;
	else if(strcmp(policy_name, "RR") == 0)
		policy = 1;
	else if(strcmp(policy_name, "SJF") == 0)
		policy = 2;
	else
		policy = 3;
	fprintf(stderr,"%s\n",policy_name);
	setcore(0);
	int finish_child = 0;
	int counter = 0;
	int now_run = -1;
	qsort(child_q, child_num, sizeof(child),compare);
	while(finish_child != child_num)
	{
		if(now_run != -1 && child_q[now_run].left_t == 0)
		{
			int status;
			waitpid(child_q[now_run].pid,&status,0);
			now_run = -1;
			finish_child++;
		}


		for(int i = 0;i<child_num;i++)
		{
			if(child_q[i].rd_t == counter)
			{
				fork_child(i);
			}
		}
		if(now_run == -1)
		{
			if(policy == 0)//FIFO
			{ 
				for(int j = 0;j < child_num;j++)
				{
					if(child_q[j].left_t == 0||child_q[j].rd_t > counter)
						continue;
					now_run = j;
					child_q[j].left_t = child_q[j].ex_t;
					setACTIVE(child_q[now_run].pid);
					fprintf(stderr,"now child = %s\n",child_q[j].name);
					break;
				}
			}
			
				
		}
		
		unittime();
		if(now_run != -1)
			child_q[now_run].left_t--;
		counter++;
	}
	return 0;

}
