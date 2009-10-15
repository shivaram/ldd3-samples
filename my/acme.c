/* Dummy character device driver from Free Electrons
 * Referenced in http://free-electrons.com/training/drivers
 *
 * Copyright 2006, Free Electrons <http://free-electrons.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>

// Physical address of the imaginary acme device
// #define ACME_PHYS 0x12345678  // Not working, using kmalloc/kfree just for test instead

static void *acme_buf;
static int acme_bufsize=8192;

static int acme_count=1;
static dev_t acme_dev = MKDEV(202,128);

static struct cdev acme_cdev;

static ssize_t
acme_read(struct file *file, char __user *buf, size_t count, loff_t * ppos)
{
   /* The acme_buf address corresponds to a device I/O memory area */
   /* of size acme_bufsize, obtained with ioremap() */
   int remaining_size, transfer_size;

   remaining_size = acme_bufsize - (int) (*ppos); // bytes left to transfer
   if (remaining_size == 0) { /* All read, returning 0 (End Of File) */
	  return 0;
   }

   /* Size of this transfer */
   transfer_size = min(remaining_size, (int) count);
   
   if (copy_to_user(buf /* to */, acme_buf + *ppos /* from */, transfer_size)) {
      return -EFAULT;
   } else { /* Increase the position in the open file */
      *ppos += transfer_size;
      return transfer_size;
   }
}

static ssize_t
acme_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
   int remaining_bytes;

   /* Number of bytes not written yet in the device */
   remaining_bytes = acme_bufsize - (*ppos);
   
   if (count > remaining_bytes) {
	  /* Can't write beyond the end of the device */
	  return -EIO;
   }

   if (copy_from_user(acme_buf + *ppos /* to */, buf /* from */, count)) {
      return -EFAULT;
   } else {
	  /* Increase the position in the open file */
      *ppos += count;
      return count;
   }
}

static struct file_operations acme_fops =
{
	.owner = THIS_MODULE,
	.read = acme_read,
	.write = acme_write,
};

static int __init acme_init(void)
{
    int err;
    acme_buf = kmalloc (acme_bufsize, GFP_KERNEL);

    if (!acme_buf) {
       err = -ENOMEM;
       goto err_exit;
    }

    if (register_chrdev_region(acme_dev, acme_count, "acme")) {
       err=-ENODEV;
       goto err_free_buf;
    }

    cdev_init(&acme_cdev, &acme_fops);

    if (cdev_add(&acme_cdev, acme_dev,
                 acme_count)) {
       err=-ENODEV;
       goto err_dev_unregister;
    }

    return 0;

    err_dev_unregister:
        unregister_chrdev_region(
           acme_dev, acme_count);
    err_free_buf:
        iounmap(acme_buf);
    err_exit:
        return err;
}

static void __exit acme_exit(void)
{
    cdev_del(&acme_cdev);
    unregister_chrdev_region(acme_dev,
                       acme_count);
    kfree(acme_buf);
}

module_init(acme_init);
module_exit(acme_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example character driver");
MODULE_AUTHOR("Free Electrons");


