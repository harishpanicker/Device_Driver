#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>

/* fifo size in elements (bytes) */
#define FIFO_SIZE	32
/* name of the proc entry */
#define	PROC_FIFO	"bytestream-fifo"

#if 0
#define DYNAMIC
#endif


#ifdef DYNAMIC
	static struct kfifo test;
#else 
	static DECLARE_KFIFO(test, unsigned char, FIFO_SIZE);
#endif 



/* lock for procfs read access */
static DEFINE_MUTEX(read_lock);
/* lock for procfs write access */
static DEFINE_MUTEX(write_lock);
/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */

static int __init testfunc(void)
{
	unsigned char	buf[6];
	unsigned char	i;
	unsigned int	ret;
	printk(KERN_INFO "byte stream fifo test start\n");
	/* put string into the fifo */
	kfifo_in(&test, "hello", 5);
	/* put values into the fifo */
	for (i = 0; i != 10; i++)
		kfifo_put(&test, &i);
		
	/* show the number of used elements */
	printk(KERN_INFO "fifo len: %u\n", kfifo_len(&test));
	
	/* get max of 5 bytes from the fifo */
	i = kfifo_out(&test, buf, 5);
	printk(KERN_INFO "buf: %d.*s\n", i, buf);
	
	/* get max of 2 elements from the fifo */
	ret = kfifo_out(&test, buf, 2);
	printk(KERN_INFO "ret: %d\n", ret);
	printk(KERN_INFO "buf: %d.*s\n", ret, buf);
	
	/* and put it back to the end of the fifo */
	ret = kfifo_in(&test, buf, ret);
	printk(KERN_INFO "ret: %d\n", ret);
	/* put values into the fifo until is full */
	for (i = 20; kfifo_put(&test, &i); i++)
		;
	printk(KERN_INFO "queue len: %u\n", kfifo_len(&test));
	/* print out all values in the fifo */
	while (kfifo_get(&test, &i))
		printk("%d ", i);
	printk("\n");
	return 0;
}
static int __init fifo_example_init(void)
{
#ifdef DYNAMIC
	int ret;
	ret = kfifo_alloc(&test,FIFO_SIZE,GFP_KERNEL);
	if(ret)
	{
		printk(KERN_ERR"error kfifo_alloc");
		return ret;
	}	
#else
	INIT_KFIFO(test);
#endif
	testfunc();
}
static void __exit kfifo_example_exit(void)
{
#ifdef DYNAMIC
	kfifo_free(&test); 
#endif
}
module_init(fifo_example_init)
module_exit(kfifo_example_exit)