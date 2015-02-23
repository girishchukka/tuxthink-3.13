#include <linux/ioctl.h>
#include <linux/ioctl_basic.h>

#define IOC_MAGIC k //defines magic number
#define IOCTL_HELLO _IO(IOC_MAGIC, 0) //defines our ioctl call
int ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)	{
	switch(cmd)	{
		case IOCTL_HELLO:
		printk(KERN_INFO "In the ioctl command\n")
		break;
		default:
		printk(KERN_INFO"Wrong command\n");
		return 1;
	}
	return 0;
}

struct file_operations file_operations=	{
	open: open,
	unlocked_ioctl: ioctl_funcs,
	release: release,
};