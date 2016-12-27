// chardev.h 
// character device routines
// copied and adapted from:
// https://github.com/skuhl/sys-prog-examples/blob/master/kernel-module/chardev-ioctl/chardev.c
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/ioctl.h>

#define MAJOR_NUM 100
#define IOCTL_RUN_TEST0 _IOR(MAJOR_NUM, 0, unsigned int)
#define IOCTL_RUN_TEST1 _IOR(MAJOR_NUM, 1, unsigned int)
#define IOCTL_RUN_TEST2 _IOR(MAJOR_NUM, 2, unsigned int)

#define SUCCESS 0
#define DEVICE_NAME "fpmod_dev"

static int runTest(unsigned int count);

/* 
 * Is the device open right now? Used to prevent
 * concurent access into the same device 
 */
static int Device_Open = 0;

static int device_open(struct inode *inode, struct file *file)
{
//#ifdef DEBUG
	printk(KERN_INFO "device_open(%p)\n", file);
//#endif

	/* 
	 * We don't want to talk to two processes at the same time 
	 */
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
//#ifdef DEBUG
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
//#endif

	/* 
	 * We're now ready for our next caller 
	 */
	Device_Open--;

	module_put(THIS_MODULE);
	return SUCCESS;
}

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
long device_ioctl(struct file *file,	/* see include/linux/fs.h */
                  unsigned int ioctl_num,	/* number and param for ioctl */
                  unsigned long ioctl_param)
{
	unsigned int param = (unsigned int) ioctl_param;
	/* 
	 * Switch according to the ioctl called 
	 */
	switch (ioctl_num) {
	case IOCTL_RUN_TEST0:
	case IOCTL_RUN_TEST1:
	case IOCTL_RUN_TEST2:
        	//printk(KERN_INFO "IOCTL_RUN_TEST0: %d", param);
			runTest(param);

	}

	return SUCCESS;
}

struct file_operations Fops = {
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,	/* a.k.a. close */
};

// to use the device: mknod DEVICE_FILE_NAME c MAJOR_NUM 0
