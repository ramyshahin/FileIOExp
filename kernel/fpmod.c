// fpmod.c
// file processing kernel module
// Ramy Shahin
// Dec. 20th 2016
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include "kernelIO.h"
#include "chardev.h"

#define KERNEL_TEST
#include "../test2.c"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ramy Shahin");
MODULE_DESCRIPTION("Kernel-mode file manipulation module");

static int __init fpmod_init(void)
{
    int ret_val;
    ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

    printk (KERN_INFO "Entering fpmod");
    
	/* 
	 * Negative values signify an error 
	 */
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "Sorry, registering the character device ", ret_val);
		return ret_val;
    }

    return 0;
}

static void __exit fpmod_cleanup(void)
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(fpmod_init);
module_exit(fpmod_cleanup);