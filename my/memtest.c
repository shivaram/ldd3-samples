/*                                                     
 * $Id: memtest.c,v $
 */                                                    
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
MODULE_LICENSE("GPL");

static char* s;
static char* v;

static int memtest_init(void)
{
	printk(KERN_ALERT "Memtest loaded\n");
    
    s = kmalloc(256 * 1024, GFP_KERNEL);
    memset(s, 0, 1000);
    memset(s, 'A', 10);
    s[10] = '\n';

    v = vmalloc(256 * 1024);

    return 0;
}

static void memtest_exit(void)
{
    printk(KERN_ALERT "kmalloc str (s) at 0x%08X: %s", (unsigned int)s, s);
    kfree(s);

    printk(KERN_ALERT "vmalloc str (s) at 0x%08X: %s", (unsigned int)v, v);
    vfree(v);

	printk(KERN_ALERT "Memtest unloaded\n");
}

module_init(memtest_init);
module_exit(memtest_exit);
