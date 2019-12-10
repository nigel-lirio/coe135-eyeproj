#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/ioport.h>

#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/kfifo.h>

#include<linux/slab.h> 
#include <linux/fs.h> 
#include <linux/errno.h> 
#include <linux/types.h> 
#include <linux/proc_fs.h>
#include <linux/fcntl.h>

#include <asm/irq.h>
#include <asm/io.h>

#include <asm/spinlock.h>
#include <linux/random.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/signal.h>
#endif


#define OURMOUSE_BASE   0x300
#define OURMOUSE_MINOR 0
#define OURMOUSE_MAJOR 250
#define MOUSE_IRQ 12

MODULE_LICENSE("GPL");

static ssize_t mouse_read(struct file *f, char *buffer,size_t count, loff_t
*pos);
static ssize_t write_mouse(struct file *file, const char *buffer, size_t
count, loff_t *ppos);
static unsigned int mouse_poll(struct file *file, poll_table *wait);
static int open_mouse(struct inode *inode, struct file *file);
static int close_mouse(struct inode *inode,struct file *file);

/*
int the_fifo()
{


    /*void arraystore(int x, int y, int arr[])
    {
        arr[0] = x;
        arr[1] = y;


    }
    */

//FIFO

/*
   int fd1; 
  
    // FIFO file path 
    char * myfifo = "/tmp/myfifo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>,<permission>) 
    mkfifo(myfifo, 0666); 
  
    //char str1[80], str2[80]; 
    int x_fifo, y_fifo;
    //int arr[2];

    //arraystore(x_fifo, y_fifo, arr);
    while (1) 
    { 
        // First open in read only and read 
        fd1 = open(myfifo,O_RDONLY); 
        read(fd1, &x_fifo, sizeof(x_fifo)); 
  
        // Print the read string and close 
        printf("User1: %d\n", x_fifo); 
        close(fd1);


        open(myfifo,O_RDONLY); 
        read(fd1, &y_fifo, sizeof(y_fifo)); 
        printf("User1: %d\n", y_fifo);
         close(fd1);
    }
    return x_fifo        /*  <--- basta the coords  */



    //FIFOEND




//KERNEL FIFO



//Listing 4: Managing Interrupts
static DEFINE_SPINLOCK(mouse_lock);

static DECLARE_WAIT_QUEUE_HEAD(mouse_wait);

static int mouse_buttons = 0;
static int mouse_users = 0; /* User count */

static int mouse_dx = 0;    // Position changes

static int mouse_dy = 0;
static int mouse_event = 0;  // Mouse has moved
static int mouse_intr = MOUSE_IRQ;

//DECLARE_KFIFO(fifo1, int, 2)
//INIT_KFIFO(fifo1)
/*static int the_fifo(void)
{
    unsigned char buf[4];
    int x_fifo, y_fifo;
    static DEFINE_KFIFO(fifo1, signed int, 2);
    kfifo_initialized(fifo1);
//kfifo_out(&fifo1, buf, 4);
    kfifo_peek(fifo1, &x_fifo);

    static DEFINE_KFIFO(fifo2, signed int, 2);
    kfifo_initialized(fifo2);
    kfifo_peek(fifo2, &y_fifo);
    return x_fifo;

}
*/

// #define FIFO_SIZE	32

// //FIFO IMPLEMETATION 1

// static struct kfifo fifo;
// static int __init example_init(void)
// {
// 	int			i;
// 	unsigned int		ret;
// 	unsigned int		nents;
//     unsigned char fifox, fifoy;
// 	struct scatterlist	sg[10];
// 	printk(KERN_INFO "DMA fifo test start\n");
// 	if (kfifo_alloc(&fifo, FIFO_SIZE, GFP_KERNEL)) {
// 		printk(KERN_WARNING "error kfifo_alloc\n");
// 		return -ENOMEM;
// 	}
// 	printk(KERN_INFO "queue size: %u\n", kfifo_size(&fifo));
// 	kfifo_in(&fifo, "test", 4);
// 	for (i = 0; i != 9; i++)
// 		kfifo_put(&fifo, i);
// 	/* kick away first byte */
// 	kfifo_skip(&fifo);
// 	printk(KERN_INFO "queue len: %u\n", kfifo_len(&fifo));
// 	/*
// 	 * Configure the kfifo buffer to receive data from DMA input.
// 	 *
// 	 *  .--------------------------------------.
// 	 *  | 0 | 1 | 2 | ... | 12 | 13 | ... | 31 |
// 	 *  |---|------------------|---------------|
// 	 *   \_/ \________________/ \_____________/
// 	 *    \          \                  \
// 	 *     \          \_allocated data   \
// 	 *      \_*free space*                \_*free space*
// 	 *
// 	 * We need two different SG entries: one for the free space area at the
// 	 * end of the kfifo buffer (19 bytes) and another for the first free
// 	 * byte at the beginning, after the kfifo_skip().
// 	 */
// 	sg_init_table(sg, ARRAY_SIZE(sg));
// 	nents = kfifo_dma_in_prepare(&fifo, sg, ARRAY_SIZE(sg), FIFO_SIZE);
// 	printk(KERN_INFO "DMA sgl entries: %d\n", nents);
// 	if (!nents) {
// 		/* fifo is full and no sgl was created */
// 		printk(KERN_WARNING "error kfifo_dma_in_prepare\n");
// 		return -EIO;
// 	}
// 	/* receive data */
// 	printk(KERN_INFO "scatterlist for receive:\n");
// 	for (i = 0; i < nents; i++) {
// 		printk(KERN_INFO
// 		"sg[%d] -> "
// 		"page %p offset 0x%.8x length 0x%.8x\n",
// 			i, sg_page(&sg[i]), sg[i].offset, sg[i].length);
// 		if (sg_is_last(&sg[i]))
// 			break;
// 	}
// 	/* put here your code to setup and exectute the dma operation */
// 	/* ... */
// 	/* example: zero bytes received */
// 	ret = 0;
// 	/* finish the dma operation and update the received data */
// 	kfifo_dma_in_finish(&fifo, ret);
// 	/* Prepare to transmit data, example: 8 bytes */
// 	nents = kfifo_dma_out_prepare(&fifo, sg, ARRAY_SIZE(sg), 8);
// 	printk(KERN_INFO "DMA sgl entries: %d\n", nents);
// 	if (!nents) {
// 		/* no data was available and no sgl was created */
// 		printk(KERN_WARNING "error kfifo_dma_out_prepare\n");
// 		return -EIO;
// 	}
// 	printk(KERN_INFO "scatterlist for transmit:\n");
// 	for (i = 0; i < nents; i++) {
// 		printk(KERN_INFO
// 		"sg[%d] -> "
// 		"page %p offset 0x%.8x length 0x%.8x\n",
// 			i, sg_page(&sg[i]), sg[i].offset, sg[i].length);
// 		if (sg_is_last(&sg[i]))
// 			break;
// 	}
// 	/* put here your code to setup and exectute the dma operation */
// 	/* ... */
//     kfifo_peek(&fifo, &fifox);
//     printk(KERN_INFO "%d\n", fifox)
//     kfifo_peek(&fifo, &fifoy);
//     printk(KERN_INFO "%d\n", fifoy)

// 	/* example: 5 bytes transmitted */
// 	ret = 5;
// 	/* finish the dma operation and update the transmitted data */
// 	kfifo_dma_out_finish(&fifo, ret);
// 	ret = kfifo_len(&fifo);
// 	printk(KERN_INFO "queue len: %u\n", kfifo_len(&fifo));
// 	if (ret != 7) {
// 		printk(KERN_WARNING "size mismatch: test failed");
// 		return -EIO;
// 	}
// 	printk(KERN_INFO "test passed\n");
// 	return 0;
// }




#define FIFO_SIZE	32

/* name of the proc entry */
#define	PROC_FIFO	"bytestream-fifo"

/* lock for procfs read access */
static DEFINE_MUTEX(read_lock);

/* lock for procfs write access */
static DEFINE_MUTEX(write_lock);

/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */
#if 0
#define DYNAMIC
#endif

#ifdef DYNAMIC
static struct kfifo test;
#else
static DECLARE_KFIFO(test, unsigned char, FIFO_SIZE);
#endif

static const unsigned char expected_result[FIFO_SIZE] = {
	 3,  4,  5,  6,  7,  8,  9,  0,
	 1, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42,
};

static int __init testfunc(void)
{
	unsigned char	buf[6];
	unsigned char	i, j;
	unsigned int	ret;

	printk(KERN_INFO "byte stream fifo test start\n");

	/* put string into the fifo */
	kfifo_in(&test, "hello", 5);

	/* put values into the fifo */
	for (i = 0; i != 10; i++)
		kfifo_put(&test, &i);

	/* show the number of used elements */
	printk(KERN_INFO "fifo len: %u\n", kfifo_len(&test));

	/* get max of 5 bytes from the fifo */
	i = kfifo_out(&test, buf, 5);
	printk(KERN_INFO "buf: %.*s\n", i, buf);

	/* get max of 2 elements from the fifo */
	ret = kfifo_out(&test, buf, 2);
	printk(KERN_INFO "ret: %d\n", ret);
	/* and put it back to the end of the fifo */
	ret = kfifo_in(&test, buf, ret);
	printk(KERN_INFO "ret: %d\n", ret);

	/* skip first element of the fifo */
	printk(KERN_INFO "skip 1st element\n");
	kfifo_skip(&test);

	/* put values into the fifo until is full */
	for (i = 20; kfifo_put(&test, &i); i++)
		;

	printk(KERN_INFO "queue len: %u\n", kfifo_len(&test));

	/* show the first value without removing from the fifo */
	if (kfifo_peek(&test, &i))
		printk(KERN_INFO "%d\n", i);

	/* check the correctness of all values in the fifo */
	j = 0;
	while (kfifo_get(&test, &i)) {
		printk(KERN_INFO "item = %d\n", i);
		if (i != expected_result[j++]) {
			printk(KERN_WARNING "value mismatch: test failed\n");
			return -EIO;
		}
	}
	if (j != ARRAY_SIZE(expected_result)) {
		printk(KERN_WARNING "size mismatch: test failed\n");
		return -EIO;
	}
	printk(KERN_INFO "test passed\n");

    i = mouse_dx;
	return i;
    
}

static ssize_t fifo_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	int ret;
	unsigned int copied;

	if (mutex_lock_interruptible(&write_lock))
		return -ERESTARTSYS;

	ret = kfifo_from_user(&test, buf, count, &copied);

	mutex_unlock(&write_lock);

	return ret ? ret : copied;
}

static ssize_t fifo_read(struct file *file, char __user *buf,
						size_t count, loff_t *ppos)
{
	int ret;
	unsigned int copied;

	if (mutex_lock_interruptible(&read_lock))
		return -ERESTARTSYS;

	ret = kfifo_to_user(&test, buf, count, &copied);

	mutex_unlock(&read_lock);

	return ret ? ret : copied;
}

static const struct file_operations fifo_fops = {
	.owner		= THIS_MODULE,
	.read		= fifo_read,
	.write		= fifo_write,
	.llseek		= noop_llseek,
};

// static int __init example_init(void)
// {
// #ifdef DYNAMIC
// 	int ret;

// 	ret = kfifo_alloc(&test, FIFO_SIZE, GFP_KERNEL);
// 	if (ret) {
// 		printk(KERN_ERR "error kfifo_alloc\n");
// 		return ret;
// 	}
// #else
// 	INIT_KFIFO(test);
// #endif
// 	if (testfunc() < 0) {
// #ifdef DYNAMIC
// 		kfifo_free(&test);
// #endif
// 		return -EIO;
// 	}

// 	if (proc_create(PROC_FIFO, 0, NULL, &fifo_fops) == NULL) {
// #ifdef DYNAMIC
// 		kfifo_free(&test);
// #endif
// 		return -ENOMEM;
// 	}
// 	return 0;
// }

// static void __exit example_exit(void)
// {
// 	remove_proc_entry(PROC_FIFO, NULL);
// #ifdef DYNAMIC
// 	kfifo_free(&test);
// #endif
// }

// module_init(example_init);
// module_exit(example_exit);
// MODULE_LICENSE("GPL");





#define FIFO_SIZE	32

/* name of the proc entry */
#define	PROC_FIFO	"bytest2ream-fifo"

// /* lock for procfs read access */
// static DEFINE_MUTEX(read_lock);

// /* lock for procfs write access */
// static DEFINE_MUTEX(write_lock);

/*
 * define DYNAMIC in this example for a dynamically allocated fifo.
 *
 * Otherwise the fifo storage will be a part of the fifo structure.
 */
#if 0
#define DYNAMIC
#endif

#ifdef DYNAMIC
static struct kfifo test2;
#else
static DECLARE_KFIFO(test2, unsigned char, FIFO_SIZE);
#endif

// static const unsigned char expected_result[FIFO_SIZE] = {
// 	 3,  4,  5,  6,  7,  8,  9,  0,
// 	 1, 20, 21, 22, 23, 24, 25, 26,
// 	27, 28, 29, 30, 31, 32, 33, 34,
// 	35, 36, 37, 38, 39, 40, 41, 42,
// };

static int __init test2func(void)
{
	unsigned char	buf[6];
	unsigned char	fifoy, j;
	unsigned int	ret;

	printk(KERN_INFO "byte stream fifo test2 start\n");

	/* put string into the fifo */
	kfifo_in(&test2, "hello", 5);

	/* put values into the fifo */
	for (fifoy = 0; fifoy != 10; fifoy++)
		kfifo_put(&test2, &fifoy);

	/* show the number of used elements */
	printk(KERN_INFO "fifo len: %u\n", kfifo_len(&test2));

	/* get max of 5 bytes from the fifo */
	fifoy = kfifo_out(&test2, buf, 5);
	printk(KERN_INFO "buf: %.*s\n", fifoy, buf);

	/* get max of 2 elements from the fifo */
	ret = kfifo_out(&test2, buf, 2);
	printk(KERN_INFO "ret: %d\n", ret);
	/* and put it back to the end of the fifo */
	ret = kfifo_in(&test2, buf, ret);
	printk(KERN_INFO "ret: %d\n", ret);

	/* skip first element of the fifo */
	printk(KERN_INFO "skip 1st element\n");
	kfifo_skip(&test2);

	/* put values into the fifo until is full */
	for (fifoy = 20; kfifo_put(&test2, &fifoy); fifoy++)
		;

	printk(KERN_INFO "queue len: %u\n", kfifo_len(&test2));

	/* show the first value without removing from the fifo */
	if (kfifo_peek(&test2, &fifoy))
		printk(KERN_INFO "%d\n", fifoy);

	/* check the correctness of all values in the fifo */
	j = 0;
	while (kfifo_get(&test2, &fifoy)) {
		printk(KERN_INFO "item = %d\n", fifoy);
		if (fifoy != expected_result[j++]) {
			printk(KERN_WARNING "value mismatch: test2 failed\n");
			return -EIO;
		}
	}
	if (j != ARRAY_SIZE(expected_result)) {
		printk(KERN_WARNING "size mismatch: test2 failed\n");
		return -EIO;
	}
	printk(KERN_INFO "test2 passed\n");

    fifoy = mouse_dy;
	return fifoy;
    
}

// static ssize_t fifo_write(struct file *file, const char __user *buf,
// 						size_t count, loff_t *ppos)
// {
// 	int ret;
// 	unsigned int copied;

// 	if (mutex_lock_interruptible(&write_lock))
// 		return -ERESTARTSYS;

// 	ret = kfifo_from_user(&test2, buf, count, &copied);

// 	mutex_unlock(&write_lock);

// 	return ret ? ret : copied;
// }

// static ssize_t fifo_read(struct file *file, char __user *buf,
// 						size_t count, loff_t *ppos)
// {
// 	int ret;
// 	unsigned int copied;

// 	if (mutex_lock_interruptible(&read_lock))
// 		return -ERESTARTSYS;

// 	ret = kfifo_to_user(&test2, buf, count, &copied);

// 	mutex_unlock(&read_lock);

// 	return ret ? ret : copied;
// }

// static const struct file_operations fifo_fops = {
// 	.owner		= THIS_MODULE,
// 	.read		= fifo_read,
// 	.write		= fifo_write,
// 	.llseek		= noop_llseek,
// };




































struct file_operations our_mouse_fops = {
        owner: THIS_MODULE,            /* Automatic usage management */
        read:  mouse_read,             /* You can read a mouse */
        write: write_mouse,            /* This won't do a lot */
        poll:  mouse_poll,             /* Poll */
        open:  open_mouse,             /* Called on open */
        release: close_mouse,          /* Called on close */
};

static struct miscdevice our_mouse = {
    OURMOUSE_MINOR, "ourmouse", &our_mouse_fops
};

__init ourmouse_init(void)
{

    //if(check_region(OURMOUSE_BASE, 3))
	//return -ENODEV;
    request_region(OURMOUSE_BASE, 3,"ourmouse");
    register_chrdev(OURMOUSE_MAJOR,"ourmouse",&our_mouse_fops);
    misc_register(&our_mouse);
    //the_fifo();                               /* <--- calling the fifo? */
    testfunc();
    test2func();
    printk(KERN_INFO "ourmouse_init\n");
    return 0;
}

#ifdef MODULE

int __init init_mouse(void)
{
    printk(KERN_INFO "initializing...\n");

   /* static DEFINE_KFIFO(fifo1, int, 2);
    kfifo_initialized(fifo1);

    kfifo_peek(fifo1, &x_fifo);

    static DEFINE_KFIFO(fifo2, int, 2);
    kfifo_initialized(fifo2);

    kfifo_peek(fifo2, &y_fifo);
    */

    if(ourmouse_init()<0)
	return -ENODEV;
    
    return 0;
}

void __exit cleanup_mouse(void)
{
    misc_deregister(&our_mouse);
    release_region(OURMOUSE_BASE, 3);
    printk(KERN_INFO "cleaning up\n");
}

#endif

//Listing 7: The Interrupt Handler

irqreturn_t ourmouse_interrupt(int irq, void *dev_id)
{
    char delta_x;
    char delta_y;
    unsigned char new_buttons;

    delta_x = inb(OURMOUSE_BASE);
    printk(KERN_INFO "x pos %c\n", delta_x);
    delta_y = inb(OURMOUSE_BASE+1);
    printk(KERN_INFO "y pos %c\n", delta_y);
    new_buttons = inb(OURMOUSE_BASE+2);

    if(delta_x || delta_y || new_buttons != mouse_buttons)
    {
        /* Something happened */
        spin_lock(&mouse_lock);
        mouse_event = 1;
        mouse_dx += delta_x;
        mouse_dy += delta_y;
        mouse_buttons = new_buttons;
        spin_unlock(&mouse_lock);

        wake_up_interruptible(&mouse_wait);
    }
}


//Listing 4: Managing Interrupts


static int open_mouse(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "open_mouse\n");
    if(mouse_users++)
	return 0;
    if(request_irq(mouse_intr, ourmouse_interrupt, 0, "ourmouse", NULL))
    {
	mouse_users--;
	return -EBUSY;
    }
    mouse_dx = 0;
    mouse_dy = 0;
    mouse_buttons = 0;
    mouse_event = 0;
    //MOD_INC_USE_COUNT;
}

//Listing 5: The close_mouse Function
static int close_mouse(struct inode *inode, struct file *file)

{
    if(--mouse_users)
	return 0;

    free_irq(ourmouse_interrupt, NULL);

    //MOD_DEC_USE_COUNT;

    return 0;

}

//Listing 6: Filling in the Write Handler
static ssize_t write_mouse(struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
    return -EINVAL;
}


//Listing 8: The poll function

static unsigned int mouse_poll(struct file *file, poll_table *wait)

{
    poll_wait(file, &mouse_wait, wait);
    if(mouse_event)
	return POLLIN | POLLRDNORM;
    return 0;
}

//Listing 9: Waiting for an Event

static ssize_t mouse_read(struct file *file, char *buffer, size_t count, loff_t *pos)
{
    int dx, dy;
    unsigned char button;
    unsigned long flags;
    int n;

    if(count<3)
	return -EINVAL;

/*
* Wait for an event
*/
    while(!mouse_event)
    {
	if(file->f_flags&O_NDELAY)
	    return -EAGAIN;

	wait_event_interruptible(mouse_wait, 1);

	if(signal_pending(current))
	    return -ERESTARTSYS;
    }


//Listing 10: Reading the Event

/* mouse_read continued */

/* Grab the event */

    spin_lock_irqsave(&mouse_lock, flags);

    dx = mouse_dx;
    dy = mouse_dy;
    button = mouse_buttons;
    
    if(dx<=-127)
        dx=-127;
    if(dx>=127)
        dx=127;
    if(dy<=-127)
        dy=-127;
    if(dy>=127)
        dy=127;

    mouse_dx -= dx;
    mouse_dy -= dy;

    if (mouse_dx == 0 && mouse_dy == 0)
        mouse_event = 0;

    spin_unlock_irqrestore(&mouse_lock, flags);

//Listing 11: Copying Results to the Buffer

/* mouse_read continued */

    if(put_user(button|0x80, buffer))
        return -EFAULT;
    if(put_user((char)dx, buffer+1))
        return -EFAULT;
    if(put_user((char)dy, buffer+2))
        return -EFAULT;
    
    for(n=3; n < count; n++)
        if(put_user(0x00, buffer+n))
            return -EFAULT;
    
    return count;
}



module_init(init_mouse);
module_exit(cleanup_mouse);




