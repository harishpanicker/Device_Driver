#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/signal.h>
#include <linux/device.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#endif

int delay = HZ;

static struct task_struct *thread_wait;

static int my_thread_fun(void *wait)
{
	unsigned long jif1, jif2;
	wait_queue_head_t wq_head;

	//Initilize the wair queue
	init_waitqueue_head(&wq_head);
	
	while(!kthread_should_stop())
	{
		jif1 = jiffies; // couter start at system boots
	
		wait_event_interruptable_timeout(wq_head,0,delay);
		
		jif2 = jiffies;
		printk(KERN_INFO"jiffies start = %ul and jiffies end = %lu\n",jif1,jif2);		
		
	}
	printk(KERN_INFO"thread stopped");
	thread_wait = NULL;
	//do_exit(0);
}

static int __init my_timer_func(void)
{
	printk(KERN_INFO"creating thread");
	thread_wait = kthread_run(my_thread_fun,NULL,"mythread");
	printk(KERN_INFO"waitr queue  success");
	return 0;
}


static void __exit my_timer_exit(void)
{
	if(thread_wait!=NULL)
	{
		kthread_stop(thread_wait);
		printk(KERN_INFO"stoping the thread");
	}
	printk(KERN_INFO "wait queue function");
}


module_init(my_timer_func);
module_exit(my_timer_exit);