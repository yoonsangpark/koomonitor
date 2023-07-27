/*
 * Driver for the Koomonitor 
 *
 * Copyright (C) 2023 KOO Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/nvt-gpio.h>

#include "koomonitor.h"

#define DRIVER_NAME "koomonitor"
#define DRIVER_VERSION "0.1"


#define MGPIO P_GPIO(11)  /* 11 + 32 = 43 */
//#define P_GPIO(pin)  (pin + 0x20)

static int koo_misc_open(struct inode *inode, struct file *file)
{
	pr_info("KOO misc device open\n");

	return 0;
}
/*
** This function will be called when we close the Misc Device file
*/
static int koo_misc_close(struct inode *inodep, struct file *filp)
{
	pr_info("KOO misc device close\n");

	return 0;
}
/*
** This function will be called when we write the Misc Device file
*/
static ssize_t koo_misc_write(struct file *file, const char __user *buf,
               size_t len, loff_t *ppos)
{
	pr_info("KOO misc device write\n");
    
	return len; 
}
 
/*
** This function will be called when we read the Misc Device file
*/
static ssize_t koo_misc_read(struct file *filp, char __user *buf,
                    size_t count, loff_t *f_pos)
{
	uint32_t value = 0;
	pr_info("KOO misc device read\n");

	gpio_direction_input(MGPIO); 
	value = gpio_get_value(MGPIO);
	pr_info("MGPIO = %d\n", value);

	value = gpio_to_irq(MGPIO);
	pr_info("MGPIO IRQ = %d\n", value);

	return 0;
}

//File operation structure
static const struct file_operations koo_misc_fops = {
	.owner          = THIS_MODULE,
	.write          = koo_misc_write,
	.read           = koo_misc_read,
	.open           = koo_misc_open,
	.release        = koo_misc_close,
};

//Misc device structure
struct miscdevice koo_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "koo_misc",
	.fops = &koo_misc_fops,
};

static int koomonitor_probe(struct platform_device *dev)
{
	int err;
	err = gpio_request(MGPIO, "gpio_test");

	if (err)
		pr_err("#### failed to request MGPIO\n");
	
	pr_info("koomonitor probe\n");
	
	return 0;
}

static int koomonitor_remove(struct platform_device *dev)
{
	gpio_free(MGPIO);

	pr_info("koomonitor removed\n");
	
	return 0;
}

static int koomonitor_suspend(struct device *dev)
{
	pr_info("koomonitor suspend\n");
	
	return 0;
}

static int koomonitor_resume(struct device *dev)
{
	pr_info("koomonitor resume\n");
	
	return 0;
}

static const struct dev_pm_ops koomonitor_pm_ops = {
	.suspend = koomonitor_suspend,
	.resume = koomonitor_resume,
	.poweroff = koomonitor_suspend,
	.freeze = koomonitor_suspend,
	.thaw = koomonitor_resume,
	.restore = koomonitor_resume,
};

static struct platform_driver koomonitor_driver = {
	.driver =
		{
			.name = "koomonitor",
			.pm = &koomonitor_pm_ops,
		},
	.probe = koomonitor_probe,
	.remove = koomonitor_remove,
};

static int koomintor_init(void)
{
	int ret;

	pr_info("koomonitor init\n");

	ret = platform_driver_register(&koomonitor_driver);

	if (ret) {
		pr_err("Unable to register driver\n");
		return ret;
	}

	pr_info("misc_register init done!!!\n");
	ret = misc_register(&koo_misc_device);
	if (ret) {
		pr_err("misc_register failed!!!\n");
		return ret;
	}

	return 0;
}

static void koomonitor_exit(void)
{
	pr_info("koomonitor exit\n");
	platform_driver_unregister(&koomonitor_driver);

	pr_info("misc_register exit done!!!\n");
	misc_deregister(&koo_misc_device);
}

module_init(koomintor_init);
module_exit(koomonitor_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_NAME);
MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR("yoonsangpark");
