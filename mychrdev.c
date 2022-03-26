#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");
#define DRIVER_NAME "mychrdev"
dev_t device_number;

static char msg[101] = {0};
 
static struct cdev my_cdev;
 
static int mydevice_open(struct inode *inode, struct file *file)
{
        pr_info("%s\n", __func__);
        return 0;
}
 
static int mydevice_release(struct inode *inode, struct file *file)
{
        pr_info("%s\n", __func__);
        return 0;
}
 
ssize_t mydevice_read(struct file *file, char __user *user_buffer,
                      size_t count, loff_t *offset)
{
    uint8_t *data = msg;
    size_t datalen = strlen(msg);

    if (*offset >= datalen) return 0;
    if (count > datalen - *offset) count = datalen - *offset;
    if (copy_to_user(user_buffer, data, count)) return -EFAULT;

    *offset += count;

    pr_info("%s\n", __func__);
    return count;
 
}
 
ssize_t mydevice_write(struct file *file, const char __user *user_buffer,
                       size_t count, loff_t *offset)
{
	short maxdatalen = 100;
	size_t ncopied;
	
	if (count < maxdatalen) maxdatalen = count;

	ncopied = copy_from_user(msg, user_buffer, maxdatalen);

	msg[maxdatalen] = 0;

    pr_info("%s\n", __func__);

	printk("Data from the user: %s\n", msg);

    return count;
}
 
struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = mydevice_open,
        .release = mydevice_release,
        .read = mydevice_read,
        .write = mydevice_write
};
 
 
static int mychrdev_device_init(void)
{
    int retval;
    pr_info("%s: In init\n", __func__);
    retval = alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME);
    if (!retval) {
        pr_info("%s: Major Number:%d\t Minor Number:%d\n",
                __func__, MAJOR(device_number), MINOR(device_number));
        cdev_init(&my_cdev, &fops);
        retval = cdev_add(&my_cdev, device_number, 1);
        if (retval)
            pr_info("%s: Failed in adding cdev to subsystem "
                            "retval:%d\n", __func__, retval);
    }
    else
        pr_err("%s: Failed in allocating device number "
                "Error:%d\n", __func__, retval);
    return retval;
}
 
static void mychrdev_device_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info("%s: In exit\n", __func__);
}
 
module_init(mychrdev_device_init);
module_exit(mychrdev_device_exit);
