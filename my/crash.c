/*                                                     
 * $Id: crash.c,v $
 */                                                    
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
MODULE_LICENSE("GPL");

static char* s = NULL;

static int crash_init(void)
{
	printk(KERN_ALERT "Crash loaded\n");
    
    memset(s, 0, 1000);

    return 0;
}

static void crash_exit(void)
{
	printk(KERN_ALERT "Crash unloaded\n");
}

module_init(crash_init);
module_exit(crash_exit);
