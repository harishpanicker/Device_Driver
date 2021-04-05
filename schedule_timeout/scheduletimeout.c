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

	while(!kthread_should_stop())
	{
		jif1 = jiffies; // couter start at system boots
		/* set task's state to interruptible sleep */
		set_current_state(TASK_INTERRUPTIBLE);//it wakes up prematurely if it receives a signal, code does not want to process signals, you can use TASK_UNINTERRUPTIBLE
		
		/* take a nap and wake up in "s" seconds */
		schedule_timeout(delay);  // this function internally create a timer ,call puts your task to sleep until at least the specified time has elapsed. 
			
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
	printk(KERN_INFO"my_timer_func success");
	return 0;
}


static void __exit my_timer_exit(void)
{
	if(thread_wait!=NULL)
	{
		kthread_stop(thread_wait);
		printk(KERN_INFO"stoping the thread");
	}
	printk(KERN_INFO "my_timer_func_exit function");
}


module_init(my_timer_func);
module_exit(my_timer_exit);