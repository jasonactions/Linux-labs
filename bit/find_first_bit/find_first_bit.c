#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init find_first_bit_init(void)
{
	unsigned long tmp = 0x101;
	int tag;
	/* 
	 * retrun the last bit which was set
	 * if all the bits are not be set, return the position of msb.
	 */
	tag = find_first_bit(&tmp, 32);
	printk(KERN_INFO "find_first_bit_init :%d(%lx)\n", tag, tmp);

	return 0;
}

static void __exit find_first_bit_exit(void)
{
	    printk(KERN_ERR "find_last_bit_eixt\n");
}

module_init(find_first_bit_init);
module_exit(find_first_bit_exit);

MODULE_LICENSE("GPL");

MODULE_DESCRIPTION("My First LKM!");
