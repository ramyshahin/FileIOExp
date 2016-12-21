// fpmod.c
// file processing kernel module
// Ramy Shahin
// Dec. 20th 2016
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include "kernelIO.h"

MODULE_LICENSE("GPL");
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

const char* in0 = "/home/ramy/FileIOExp/a.bin";
const char* out = "/home/ramy/FileIOExp/out.bin";

const int IN_FLAGS = O_RDONLY | O_LARGEFILE;// | O_DIRECT;
const int OUT_FLAGS = O_CREAT | O_WRONLY | O_LARGEFILE;// | O_DIRECT;

struct file* fin;
struct file* fout;

static int __init fpmod_init(void)
{
    printk (KERN_INFO "Entering fpmod");
        
    fin = file_open(in0, IN_FLAGS, 0);
    if (!fin) {
        printk(KERN_INFO "Failed to open input file");
        return -1;
    }
    
    fout = file_open(out, OUT_FLAGS, S_IRWXU);
    if (!fout) {
        printk(KERN_INFO "Failed to open output file");
        return -1;
    }

    printk(KERN_INFO "Module initialized");

    return 0;
}

static void __exit fpmod_cleanup(void)
{
    file_close(fout);
    file_close(fin);
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(fpmod_init);
module_exit(fpmod_cleanup);