#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>

static struct device dev_demo = {
	.init_name = "dev-demo",
};

static int device_demo_regiser(struct device *dev_demo)                                                                                                         
{                    
	int ret;

	ret = device_register(dev_demo);
	if (ret)
		return ret;

	return 0;
} 

static int __init device_demo_init(void)
{

	device_demo_regiser(&dev_demo);

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
