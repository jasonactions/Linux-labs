#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>


static struct device dev_demo;

static int device_demo_init_and_add(struct device *dev_demo,
		char *init_name, struct device *parent, struct class *cls)
{
	int ret;
	
	dev_demo->init_name = init_name;
	dev_demo->parent = parent;
	dev_demo->class = cls;

	device_initialize(dev_demo);
	ret = device_add(dev_demo);
	if (ret) {
		printk(KERN_ERR "devcie add fail,ret(%d)!\n", ret);
		return ret;
	}


	return 0;
}

static int __init device_demo_init(void)
{
	device_demo_init_and_add(&dev_demo, "dev-demo", NULL, NULL);

	return 0;
}

static void __exit device_demo_exit(void)
{
	device_del(&dev_demo);

	return;
}


module_init(device_demo_init);
module_exit(device_demo_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
MODULE_DESCRIPTION("device demo");
