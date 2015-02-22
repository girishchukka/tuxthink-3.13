
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // required for various structures related to files liked fops. 
#include <asm/uaccess.h> // required for copy_from and copy_to user functions 
#include <linux/semaphore.h>
#include <linux/cdev.h> 
#include <linux/mutex.h>
static int Major;
static int written,read;
struct device {
 char array[100];
 struct semaphore sem;
 struct mutex mutex;
}char_arr;

int open(struct inode *inode, struct file *filp)
{
 
 printk(KERN_INFO "Inside open \n");
 if(down_interruptible(&char_arr.sem)) {
  printk(KERN_INFO " could not hold semaphore");
  return -1;
 }
 read = written;
 return 0;
}

int release(struct inode *inode, struct file *filp) {
 printk (KERN_INFO "Inside close \n");
 printk(KERN_INFO "Releasing semaphore");
 up(&char_arr.sem);
 return 0;
}

ssize_t read_char(struct file *filp, char *buff, size_t count, loff_t *offp) {
 unsigned long ret;
 printk("Inside read \n");
 if(count > read) 
 count = read;
 read = read -count;
 ret = copy_to_user(buff, char_arr.array, count);
 return count;
}

ssize_t write_char(struct file *filp, const char *buff, size_t count, loff_t *offp) { 
 unsigned long ret; 
 printk(KERN_INFO "Inside write \n");
 ret = copy_from_user(char_arr.array, buff, count); 
 written += count;
 return count;
}

struct file_operations fops = {
 
 owner:  THIS_MODULE,
 read:  read_char,
 write:  write_char,
 open:   open,
 release: release
};


struct cdev *kernel_cdev; 


int char_arr_init (void) {
 int ret;
 dev_t dev_no,dev;

 kernel_cdev = cdev_alloc(); 
  kernel_cdev->ops = &fops;
 kernel_cdev->owner = THIS_MODULE;
 printk (" Inside init module\n");
  ret = alloc_chrdev_region( &dev_no , 0, 1,"char_arr_dev");
    if (ret < 0) {
  printk("Major number allocation is failed\n");
  return ret; 
 }
    Major = MAJOR(dev_no);
 printk (KERN_INFO" The major number for your device is %d\n", Major); 


 //init_MUTEX(&char_arr.sem);
 /* change in api http://www.linuxquestions.org/questions/blog/frandalla-68463/patching-802-11-linux-sta-driver-for-kernel-2-6-37-3558/*/
 //sema_init(&char_arr.sem, 1);
 /*http://stackoverflow.com/questions/27801529/where-has-init-mutex-gone-in-linux-kernel-version-3-2*/
 mutex_init(&char_arr.mutex);

 ret = cdev_add( kernel_cdev,dev_no,1);
 if(ret < 0 ) 
 {
 printk(KERN_INFO "Unable to allocate cdev");
 return ret;
 }

 return 0;
}

void char_arr_cleanup(void) {
 printk(KERN_INFO " Inside cleanup_module\n");
 cdev_del(kernel_cdev);
 unregister_chrdev_region(Major, 1);
}
MODULE_LICENSE("GPL"); 
module_init(char_arr_init);
module_exit(char_arr_cleanup);


/*	1.insmod
	2.mknod /dev/charr_arr c major_number minor_number
	3.chmod 777 /dev/char_arr
	4.echo "This is the data inside /dev/char_arr" > /dev/char_arr
	5.cat /dev/char_arr
	6.rm /dev/char_arr
	7.rmmod c1.o 
	*/