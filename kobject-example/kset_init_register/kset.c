#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

static struct kset *example_kset;
static struct kobject *parent;
static struct kobject *child;

static unsigned long flag = 1;
static ssize_t child_attr_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	size_t count = 0;
	count += sprintf(&buf[count],"%lu\n", flag);

	return count;
}

static ssize_t child_attr_store(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count)
{
	flag = buf[0] - '0';

	switch(flag) {
	case 0:
		kobject_uevent(kobj, KOBJ_ADD);
		break;
	case 1:
		kobject_uevent(kobj, KOBJ_REMOVE);
		break;
	case 2:
		kobject_uevent(kobj, KOBJ_CHANGE);
		break;
	case 3:
		kobject_uevent(kobj, KOBJ_MOVE);
		break;
	case 4:
		kobject_uevent(kobj, KOBJ_ONLINE);
		break;
	case 5:
		kobject_uevent(kobj, KOBJ_OFFLINE);
		break;
	}
	
	return count;
}
static const struct sysfs_ops child_attr_ops = {
	.show = child_attr_show,
	.store = child_attr_store,
};

static struct kobj_type child_ktype = {
	.sysfs_ops = &child_attr_ops,
};

static struct attribute child_attr = {
	.name = "childattr",
	.mode = S_IRUGO | S_IWUSR,
};

static int __init example_init(void)
{
	int ret;
	
	/* create kset */
	example_kset = kset_create_and_add("kset-example", NULL, NULL);
	if (!example_kset) {
		printk(KERN_ERR "kset create and add example_kset failed!");
		return -1;
	}
	/* create parent kobject */
	parent = kobject_create_and_add("parent", &example_kset->kobj);
	if (!parent) {
		printk(KERN_ERR "kobj create and add parent failed!");
		return -1;
	}
	/* create child object and create attr file under child kobject */
	child = kzalloc(sizeof(*child), GFP_KERNEL);
	ret = kobject_init_and_add(child, &child_ktype, parent, "child");
	if (ret) {
		printk(KERN_ERR "kobj create and add child failed!");
		return ret;
	}
	ret = sysfs_create_file(child, &child_attr);
	if (ret)
		return ret;

	return 0;
}

static void __exit example_exit(void)
{
	
	sysfs_remove_file(child, &child_attr);

	if (child)
		kobject_del(child);

	if (parent)
		kobject_del(parent);

	if (example_kset)
		kset_unregister(example_kset);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
