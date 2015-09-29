/*
 * ASoC driver for DABPi using a SI468X FM/FMHD/DAB+ receiver
 * connected to a Raspberry Pi
 *
 * Author:      Bjoern Biesenbach, <bjoern@bjoern-b.de>
 * Copyright 2014
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/jack.h>

static int snd_rpi_dabpi_startup(struct snd_pcm_substream *substream)
{
	return 0;
}

static int snd_rpi_dabpi_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	return 0;
}

/* machine stream operations */
static struct snd_soc_ops snd_rpi_dabpi_ops = {
	.startup = snd_rpi_dabpi_startup,
	.hw_params = snd_rpi_dabpi_hw_params,
};

static struct snd_soc_dai_link snd_rpi_dabpi_dai[] = {
{
	.name		= "DABPi",
	.stream_name	= "DABPi Hifi",
	.cpu_dai_name	= "bcm2708-i2s.0",
	.codec_dai_name	= "si468x-hifi",
	.platform_name	= "bcm2708-i2s.0",
	.codec_name	= "si468x-codec",
	.dai_fmt	= SND_SOC_DAIFMT_I2S
				| SND_SOC_DAIFMT_NB_NF
				| SND_SOC_DAIFMT_CBS_CFS, // rpi is clock and frame sync master
	.ops		= &snd_rpi_dabpi_ops,
},
};

/* audio machine driver */
static struct snd_soc_card snd_rpi_dabpi = {
	.name		= "snd_rpi_dabpi",
	.dai_link	= snd_rpi_dabpi_dai,
	.num_links	= ARRAY_SIZE(snd_rpi_dabpi_dai),
};

static struct platform_device snd_rpi_dabpi_device = {
        .name = "snd-dabpi",
        .id = 0,
        .num_resources = 0,
};

static void snd_rpi_dabpi_platform_release(struct device *dev) {
        printk(KERN_INFO "rpi-dabpi snd_rpi_dabpi_platform_release\n");
        printk(KERN_INFO "rpi-dabpi snd_rpi_dabpi_platform_release = void\n");
}

static int snd_rpi_dabpi_probe(struct platform_device *pdev)
{
	int ret = 0;

	printk(KERN_INFO "snd_rpi_dabpi_probe\n");

	pdev->dev.release = snd_rpi_dabpi_platform_release;
	snd_rpi_dabpi.dev = &pdev->dev;
	ret = snd_soc_register_card(&snd_rpi_dabpi);
	if (ret) {
		dev_err(&pdev->dev,
				"snd_soc_register_card() failed: %d\n", ret);
	}

	printk(KERN_INFO "snd_rpi_dabpi_probe = %d\n", ret);

	return ret;
}


static int snd_rpi_dabpi_remove(struct platform_device *pdev)
{
	int ret;

	printk(KERN_INFO "snd_rpi_dabpi_remove\n");
	ret = snd_soc_unregister_card(&snd_rpi_dabpi);
	printk(KERN_INFO "snd_rpi_dabpi_remove = %d\n", ret);

	return ret;
}

static struct platform_driver snd_rpi_dabpi_driver = {
	.driver = {
		.name   = "snd-dabpi",
		.owner  = THIS_MODULE,
	},
	.probe	  = snd_rpi_dabpi_probe,
	.remove	 = snd_rpi_dabpi_remove,
};

//module_platform_driver(snd_rpi_dabpi_driver);

static char *cpu_dai_name = "bcm2708-i2s.0";

module_param(cpu_dai_name, charp, 0000);
MODULE_PARM_DESC(cpu_dai_name, "cpu_dai_name example: bcm2708-i2s.0");

static int __init rpi_dabpi_init(void) {
	int ret;

	snd_rpi_dabpi_dai[0].cpu_dai_name = cpu_dai_name;
	snd_rpi_dabpi_dai[0].platform_name = cpu_dai_name;

	printk(KERN_INFO "rpi_dabpi_init cpu_dai_name=%s\n", cpu_dai_name);

	printk(KERN_INFO "rpi_dabpi platform_device_register\n");
	ret = platform_device_register(&snd_rpi_dabpi_device);
	if(ret) {
		printk(KERN_INFO "rpi_dabpi Unable to register platform device '%s': %d\n", snd_rpi_dabpi_device.name, ret);
	}
	printk(KERN_INFO "rpi_dabpi platform_device_register = %d\n", ret);

	if(!ret) {
		printk(KERN_INFO "rpi_dabpi platform_driver_register\n");
		ret = platform_driver_register(&snd_rpi_dabpi_driver);
		printk(KERN_INFO "rpi_dabpi platform_driver_register = %d\n", ret);

		if(ret) {
			printk(KERN_INFO "rpi_dabpi platform_device_unregister\n");
			platform_device_unregister(&snd_rpi_dabpi_device);
			printk(KERN_INFO "rpi_dabpi platform_device_unregister = void\n");
		}
	}

	printk(KERN_INFO "rpi_dabpi_init = %d\n", ret);

	return ret;
}
module_init(rpi_dabpi_init);

static void __exit rpi_dabpi_exit(void) {
	printk(KERN_INFO "rpi_dabpi_exit\n");

	printk(KERN_INFO "rpi_dabpi platform_driver_unregister\n");
	platform_driver_unregister(&snd_rpi_dabpi_driver);
	printk(KERN_INFO "rpi_dabpi platform_driver_unregister = void\n");

	printk(KERN_INFO "rpi_dabpi platform_device_unregister\n");
	platform_device_unregister(&snd_rpi_dabpi_device);
	printk(KERN_INFO "rpi_dabpi platform_device_unregister = void\n");

	printk(KERN_INFO "rpi_dabpi_exit = void\n");
}
module_exit(rpi_dabpi_exit);

MODULE_AUTHOR("Bjoern Biesenbach");
MODULE_DESCRIPTION("ASoC Driver for Raspberry Pi connected to DABPi board (SI4688)");
MODULE_LICENSE("GPL");

