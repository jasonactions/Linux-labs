#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/fs.h>

static struct dentry *seq_file_demo_dir;

static int seq_file_demo_show(struct seq_file *seq, void *v)
{
        seq_printf(seq, "Hello World\n");
        return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
        return single_open(file, &seq_file_demo_show, NULL);
}

static const struct file_operations seq_file_demo_fops = {
        .owner = THIS_MODULE,
        .open = seq_file_demo_open,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = single_release,
};

static int __init seq_file_demo_init(void)
{
        seq_file_demo_dir = debugfs_create_file("seq_file_demo", 0444, NULL,
                NULL, &seq_file_demo_fops);
        return 0;
}

static void __exit seq_file_demo_exit(void)
{
        if (seq_file_demo_dir)
                debugfs_remove(seq_file_demo_dir);
}

module_init(seq_file_demo_init);
module_exit(seq_file_demo_exit);
MODULE_LICENSE("GPL");
