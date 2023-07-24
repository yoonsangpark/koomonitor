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

#include "koomonitor.h"

#define DRIVER_NAME "koomonitor"
#define DRIVER_VERSION "0.1"

static int koomonitor_probe(struct platform_device *dev)
{
	pr_info("koomonitor probe\n");
	
	return 0;
}

static int koomonitor_remove(struct platform_device *dev)
{
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

	return 0;
}

static void koomonitor_exit(void)
{
	pr_info("koomonitor exit\n");
	platform_driver_unregister(&koomonitor_driver);
}

module_init(koomintor_init);
module_exit(koomonitor_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_NAME);
MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR("yoonsangpark");
