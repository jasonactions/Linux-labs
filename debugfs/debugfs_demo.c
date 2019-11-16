/*
 * debugfs demo file
 *
 * (C) 2019.11.16 HZero <jasonactions@foxmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>

struct dbg_demo_data {
	u8 str[128];
};


static int debugfs_demo_open(struct inode *inode, struct file *file)
{
	printk("debugfs demo file open!\n");
	
	return 0;
}


static ssize_t debugfs_demo_read(struct file *file, char __user *buf,
		size_t nbytes, loff_t *ppos)
{
	struct inode *inode = file_inode(file);
	struct dbg_demo_data *data; 

	data = inode->i_private;

	printk(KERN_ERR "%s\n", data->str);

	return 0;
}

static int debugfs_demo_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

const struct file_operations debug_demo_fops = {
	.owner		= THIS_MODULE,
	.open		= debugfs_demo_open,
	.read		= debugfs_demo_read,
	.release	= debugfs_demo_release,
};

struct dentry *root, *dbg_file;

static __init int debugfs_demo_init(void)
{
	struct dentry *root, *dbg_file;
	struct dbg_demo_data *data;
	
	data = kmalloc(sizeof(*data), GFP_KERNEL);
	strcpy(data->str, "This is debugfs demo file!\n"); 
	root = debugfs_create_dir("dbg_demo", NULL);
	dbg_file = debugfs_create_file("regs", 0400, root, data, &debug_demo_fops);
	if (!dbg_file)
		goto err_dbg_file;


	return 0;

err_dbg_file:
	debugfs_remove_recursive(root);
	kfree(data);

	return -EINVAL;
}

module_init(debugfs_demo_init);
static __exit void debugfs_demo_exit(void)
{
	debugfs_remove(root);
}
module_exit(debugfs_demo_exit);

MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
MODULE_DESCRIPTION("debugfs demo");
MODULE_LICENSE("GPL");


