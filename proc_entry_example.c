/*static inline struct proc_dir_entry *proc_create(const char *name,
													 umode_t mode,
													 struct proc_dir_entry *parent, 
													 const struct file_operations *proc_fops)
*/
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>

int len, temp;
char *msg;
int read_proc(struct file *filp, char *buff, size_t count, loff_t *offp)
{
	if(count>temp)
	{
		count= temp;
	}
	temp = temp -count;
	copy_to_user(buff,msg, count);
	if(count==0)
		temp=len;
	return count;
}

struct file_operations proc_fops =	{
	read : read_proc
};


void create_new_proc_entry(void)
{
	proc_create("Hello", 0, NULL, &proc_fops);
	msg = "Hello world";
	len = strlen(msg);
	temp=len;
	printk(KERN_INFO "1.len=%d", len);
}
int init_module(void)
{
	/*3rd argument NULL indicates it will be in root i.e /proc*/
	create_new_proc_entry();
	return 0;

}
void cleanup_modul(void)	{
	remove_proc_entry("hello", NULL);
}