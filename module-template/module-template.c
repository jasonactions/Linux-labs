#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init template_init(void)
{
	printk(KERN_INFO "Hello World\n");
	return 0;
}

static void __exit template_exit(void)
{
	    printk(KERN_ERR "Go to Hell\n");
}

module_init(template_init);
module_exit(template_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
MODULE_DESCRIPTION("module demo");
