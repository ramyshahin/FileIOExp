// fpmod.c
// file processing kernel module
// Ramy Shahin
// Dec. 20th 2016
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("BSD");
MODULE_AUTHOR("Ramy Shahin");
MODULE_DESCRIPTION("Kernel-mode file manipulation module");

#if 0
// read implementation
int fpmod_read(
    char* ,
    char** ,
    off_t ,
    int, 
    int)
{

    return 0;    
}

// proc entry
struct proc_dir_entry fpmod_proc_file = {
    0,                  // indoe #
    5,                  // filename length
    "fpmod",            // filename
    S_IFREG | S_IRUGO,  // file mode
    1,                  // link count
    0,                  // file uid
    0,                  // file gid
    BUF_LEN,            // file size (as reported by ls)
    NULL,               // functions to manipulate inode
    fpmod_read,         // get_info entry point
    NULL                // inode update entry point
    // linked list next pointer
    // parent pointer
    // subdir pointer
    // unused
    // read proc
};
#endif

static int __init fpmod_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    return 0;
}

static void __exit fpmod_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(fpmod_init);
module_exit(fpmod_cleanup);