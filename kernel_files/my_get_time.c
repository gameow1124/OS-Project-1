#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>

asmlinkage int sys_my_get_time(long *sec, long *nsec)
{
	struct timespec now;
	getnstimeofday(&now);
	*sec = now.tv_sec;
	*nsec = now.tv_nsec;
	return 0;
}
