#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>

asmlinkage int sys_my_printk(char *string)
{
	printk("%s\n",string);
	return 0;
}
