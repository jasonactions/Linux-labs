
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int dolphi;
int bobcat;

module_param(dolphi, int, 0);
module_param(bobcat, int ,0);

static int hello_export(void)
{
	printk(KERN_ERR "Hello from another module, dolphi:%d,bobcat:%d\n", 
	       dolphi, bobcat);

	return 0;
}
EXPORT_SYMBOL(hello_export);

static int __init hello_start(void)
{
	printk(KERN_INFO "Hello World\n");
	return 0;
}

static void __exit hello_end(void)
{
	    printk(KERN_ERR "Go to Hell\n");
}

module_init(hello_start);
module_exit(hello_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("blog.topspeedsnail.com");
MODULE_DESCRIPTION("My First LKM!");
