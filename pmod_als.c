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
 * ALS sends two bytes: 
 * First Byte: 
 *
 * Second Byte:
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
#include <linux/miscdevice.h>
#include <linux/spi/spi.h>


#define PMOD_ALS_SPI_MODE 			SPI_MODE_3   /*CPOL: 1 and CPHA: 1*/
#define PMOD_ALS_SPI_BITS 			8  
#define PMOD_ALS_SPI_MAX_CLK		3200000	 	 /* 3.2 MHz, from ADC datasheet */ 


/*

  Driver State.
  spi_device*, is owned by the kernel.

*/

struct* pmod_als_spi_device = {
	struct spi_device *spi_device;
	struct misc_device misc_device;
};




/* 

file → The open file object (kernel's tracking of an open FD).

buf → Pointer in user space where we copy our result.

count → How many bytes the user requested.

file_pos_ptr → "File position pointer". Even in device files, Linux tracks offsets.

*/

static ssize_t pmod_als_read(struct file *file,
							char __user *buf
							size_t count,
							loff_t *file_pos_ptr)
{
	struct pmod_als_device *als_device = container_of(file -> private_data,
													  struct pmod_als_device,
													  misc_device)

			u8 rx[2] = {0};
			struct spi_transfer spi_t = {
				.tx_buf = NULL,
				.rx_buf = rx,
				.len = 2,
				.speed_hz = PMOD_ALS_SPI_MAX_CLK,
				.bits_per_word = PMOD_ALS_SPI_BITS,
			};

			/*
			- Create empty message
			- Check EOF for the read
			*/
			struct spi_message spi_msg;
			u16 raw_value; 
			u8 pared_value;
			char data_str[8];
			int len;

			if(*file_pos_ptr > 0)
			{
				return 0;
			}

}


// File operations



// Probe function



// Remove function


// Device tree match table


// SPI driver definition 

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
MODULE_DESCRIPTION("SPI Driver for Digilent PMOD ALS");
MODULE_AUTHOR("MA");

