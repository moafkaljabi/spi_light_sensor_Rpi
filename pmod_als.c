/*
 *
 * SPI Driver for the Digilent PMOD Ambient Light Sensor.
 *
 * ALS: Ambient Light Sensor.
 * 
 * ALS only outputs to master, the MOSI pin is unconnected.
 *
 * ALS uses TI ADC081S021.
 *
 * Max Frequency is 4 MHz
 *
 * 
 * CPOL:1 
 * CPHA:1
 * ADC uses SPI Mode 3;
 *
 *
 * */
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/spi/spi.h>

#Define 

staitc int __init driver_init(void)
{

	return 0;
}

static void __exit driver_exit(void)
{

	// Exit
}



module_init(driver_init);
module_exit(driver_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SPI Driver for Digilent ALS");
MODULE_AUTHOR("MA");

