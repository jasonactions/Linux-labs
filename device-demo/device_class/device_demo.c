#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>

struct class *class_demo;

static struct device dev_demo;
static struct device dev_demo3_1;
static struct device dev_demo3_2;
static struct device dev_demo3_3;
static struct device dev_demo3_4;

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
	/* create /sys/class/class_demo */
	class_demo = class_create(THIS_MODULE, "class-demo");
	if (!class_demo) {
		printk(KERN_ERR "create class demo fail!");
		return -1;
	}
	/* create /sys/devices/dev-demo */
	device_demo_init_and_add(&dev_demo, "dev-demo", NULL, NULL);
	/* 1.parent null and class null: /sys/devices/dev-demo_noClass-noParen */
	device_demo_init_and_add(&dev_demo3_1, "dev-demo_noClass-noParent", NULL, NULL);
	/* 2.class null: /sys/devices/dev-demo/dev-demo_parent*/
	device_demo_init_and_add(&dev_demo3_2, "dev-demo_parent", &dev_demo, NULL);
	/* 3. parent null: dev-demo_class -> ../../devices/virtual/class-demo/dev-demo_class under 
	 *   /sys/class/class-demo */
	device_demo_init_and_add(&dev_demo3_3, "dev-demo_class", NULL, class_demo);
	/* 4. class and parent not null, but parent->class null: /sys/devices/dev-demo/class-demo/dev-demo_class-parent*/
	device_demo_init_and_add(&dev_demo3_4, "dev-demo_class-parent", &dev_demo, class_demo);

	return 0;
}

static void __exit device_demo_exit(void)
{
#ifdef USE_DEVICE_CREATE
	if (dev_demo)
		device_del(dev_demo);
#else
	device_del(&dev_demo);
#endif
	device_del(&dev_demo3_1);
	device_del(&dev_demo3_2);
	device_del(&dev_demo3_3);
	device_del(&dev_demo3_4);

	class_destroy(class_demo);

	return;
}


module_init(device_demo_init);
module_exit(device_demo_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
MODULE_DESCRIPTION("device demo");
