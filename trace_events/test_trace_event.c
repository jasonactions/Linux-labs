#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/tick.h>
#include <linux/types.h>
#include <linux/cpu.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/cpu.h>

/*must define,or build error*/
#define CREATE_TRACE_POINTS
#include <trace/events/test_driver.h>

#define TEST_DRIVER_DELAY          (50 * HZ)
#define THREAD_BIND_CPU             (0)

struct timer_list test_timer;

struct test_data {
    unsigned long num;
    unsigned long cpu_freq;
    unsigned long cpu_max_freq;
};

struct test_data *global;

static unsigned long i = 0;

static void test_driver_func(unsigned long data)
{
    unsigned long expires;

    global->num = (i + 1) % NR_CPUS;
    global->cpu_freq = cpufreq_quick_get((i + 1) % NR_CPUS);
    global->cpu_max_freq = cpufreq_quick_get_max((i + 1) % NR_CPUS);
    trace_get_test_driver_data(global->num, global->cpu_freq, global->cpu_max_freq);
    /*dynamic modify timer expires time*/
    expires = jiffies + msecs_to_jiffies(2000 + i * 200);
    mod_timer(&test_timer, expires);
    if(i > 100)
        i = 0;
    i++;
}

static int __init test_driver_init(void)
{
    init_timer_deferrable(&test_timer);
    test_timer.function = test_driver_func;
    test_timer.data = 0;
    test_timer.expires = jiffies + TEST_DRIVER_DELAY;
    add_timer_on(&test_timer, THREAD_BIND_CPU);
    global = kzalloc(sizeof(struct test_data), GFP_KERNEL);
    if(!global) {
        printk(KERN_INFO "alloc space is fail\n");
        return -1;
    }
    return 0;
}

static void __exit test_driver_exit(void)
{
    struct test_data *test = global;
    kfree(test);
    del_timer_sync(&test_timer);
}

module_init(test_driver_init);
module_exit(test_driver_exit);

MODULE_AUTHOR("samarxie");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TEST DRIVER FOR TRACE USE");
