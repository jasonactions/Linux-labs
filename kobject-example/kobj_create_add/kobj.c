#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static struct kobject *parent;
static struct kobject *child;

static int example_create_and_add_seperate(void)
{
#if 0
	int ret;
	
	/* use default kobj type dynamic_kobj_ktype */
	parent = kobject_create();
	/*
	 * 1.build the relationship of kobjects.
	 * 2.create dir under sysfs, as follows is /sys/kobj-init-add-seperate
	 * 3.kobject must be initialized before calling kobj_add. 
	 */
	ret = kobject_add(parent, NULL, "kobj-create-add-seperate");
	
	if (ret)
		return ret;
#else
	parent = kobject_create_and_add("parent", NULL);
#endif
	return 0;
}

static int example_create_and_add(void)
{

	child = kobject_create_and_add("child", parent);
	if (!child)
		return -1;

	return 0;
}

static int __init example_init(void)
{
	int ret;

	ret = example_create_and_add_seperate();
	if (ret) {
		printk(KERN_ERR "example_create_and_add_seperate failed!");
		return ret;	
	}

	ret = example_create_and_add();
	if (ret) {
		printk(KERN_ERR "example_create_and_add failed!");
		return ret;
	}

	return 0;
}

static void __exit example_exit(void)
{
	if (parent)
		kobject_del(parent);
	
	if (parent)
		kobject_del(parent);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
