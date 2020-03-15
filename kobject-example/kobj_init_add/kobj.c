#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static struct kobject example_kobj;
static struct kobject example_kobj1;

static void example_kobj_release(struct kobject *kobj)
{
	pr_debug("kobject: (%p): %s\n", kobj, __func__);
}

static struct kobj_type example_kobj_ktype = {
	.release	= example_kobj_release,
	.sysfs_ops	= &kobj_sysfs_ops,
};

static int example_init_and_add_seperate(struct kobject *_kobj)
{
	int ret;

	/*must have koby type*/
	kobject_init(_kobj, &example_kobj_ktype);
	/*
	 * 1.build the relationship of kobjects.
	 * 2.create dir under sysfs, as follows is /sys/kobj-init-add-seperate
	 * 3.kobject must be initialized before calling kobj_add. 
	 */
	ret = kobject_add(_kobj, NULL, "kobj-init-add-seperate");
	if (ret)
		return ret;

	return 0;
}

static int example_init_and_add(struct kobject *_kobj)
{
	int ret;

	ret = kobject_init_and_add(_kobj, &example_kobj_ktype, NULL,
			"kobj-init-add");
	if (ret)
		return ret;
	
	return 0;
}

static int __init example_init(void)
{
	int ret;

	ret = example_init_and_add_seperate(&example_kobj);
	if (ret) {
		printk(KERN_ERR "example_init_and_add_seperate failed!");
		return ret;	
	}

	ret = example_init_and_add(&example_kobj1);
	if (ret) {
		printk(KERN_ERR "example_init_and_add failed!");
		return ret;
	}

	return 0;
}

static void __exit example_exit(void)
{
	kobject_del(&example_kobj);
	kobject_del(&example_kobj1);
	return;
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
