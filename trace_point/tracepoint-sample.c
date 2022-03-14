/* tracepoint-sample.c
 *
 * Executes a tracepoint when /proc/tracepoint-sample is opened.
 *
 * (C) Copyright 2007 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include "tp-samples-trace.h"

DEFINE_TRACE(subsys_event,
	TP_PROTO(struct inode *inode, struct file *file),
	TP_ARGS(inode, file));

struct proc_dir_entry *pentry_sample;

void my_subsys_event(void *__data, struct inode *inode, struct file *file)
{
	printk("inode: 0x%lx, file: 0x%lx\n", (unsigned long)inode, (unsigned long)file);

}

static int my_open(struct inode *inode, struct file *file)
{
	trace_subsys_event(inode, file);
	
	return 0;
}

static const struct proc_ops mark_ops = {
	.proc_open = my_open,
	.proc_lseek = noop_llseek,
};

static int __init sample_init(void)
{
	printk(KERN_ALERT "sample init\n");
	pentry_sample = proc_create("tracepoint-sample", 0, NULL,
		&mark_ops);
	if (!pentry_sample)
		return -EPERM;

	register_trace_subsys_event(my_subsys_event, NULL);

	return 0;
}

static void __exit sample_exit(void)
{
	printk(KERN_ALERT "sample exit\n");
	remove_proc_entry("tracepoint-sample", NULL);
}

module_init(sample_init)
module_exit(sample_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("Tracepoint sample");
