#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
static struct cdev chr_dev;
static dev_t ndev;
static int chr_open(struct inode *nd, struct file *filp)
{
	int major, minor;

	major = MAJOR(nd->i_rdev);
	minor = MINOR(nd->i_rdev);

	printk("%s: major=%d, minor=%d\n", __func__, major, minor);

	return 0;
}

static ssize_t chr_read(struct file *f, char __user *u, size_t sz, loff_t *off)
{
	printk("In the chr_read() function!\n");
	return 0;
}

struct file_operations chr_ops = {
	.owner = THIS_MODULE,
	.open = chr_open,
	.read = chr_read,
};

static int demo_init(void)
{
	int ret;

	cdev_init(&chr_dev, &chr_ops);

	ret = alloc_chrdev_region(&ndev, 0, 1, "chr_dev");

	if (ret < 0)
		return ret;

	printk("%s:major=%d, minor:%d\n", __func__, MAJOR(ndev), MINOR(ndev));

	ret = cdev_add(&chr_dev, ndev, 1);
	if (ret < 0)
		return ret;

	return 0;
}

static void demo_exit(void)
{
	printk("Removing chr_dev module...\n");

	cdev_del(&chr_dev);
	unregister_chrdev_region(ndev, 1);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dennis@AMDLinuxFGL");
MODULE_DESCRIPTION("A char device driver as an example");
