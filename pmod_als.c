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

struct pmod_als_dev {
	struct spi_device *spi;
	struct miscdevice misc_device;
};




/* 

file → The open file object (kernel's tracking of an open FD).

buf → Pointer in user space where we copy our result.

count → How many bytes the user requested.

file_pos_ptr → "File position pointer". Even in device files, Linux tracks offsets.

*/

static ssize_t pmod_als_read(struct file *file,
							char __user *buf,
							size_t count,
							loff_t *file_pos_ptr)
{
	struct pmod_als_dev *als_device = container_of(file -> private_data,
													  struct pmod_als_dev,
													  misc_device);

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
			u8 parsed_value;
			char data_str[8];
			int len;

			if(*file_pos_ptr > 0)
			{
				return 0;
			}


	spi_message_init(&spi_msg);
	spi_message_add_tail(&spi_t, &spi_msg);

	if (spi_sync(als_device->spi, &spi_msg)) {
		pr_err("pmod_als: SPI transfer failed\n");
		return -EIO; // I/O error (e.g., SPI transaction failed).
	}

	/**
	 * ADC081S021 output format: 
	 * [0000 D7 D6 D5 D4 D3 D2 D1] [D0 0000000]
	 * leading 4 bits and trailing 3 bits
	 * 
	 */

	 // Extract value
	raw_value = ((rx[0] << 8) | rx[1]) >> 3;
	parsed_value = raw_value & 0xFF;

	// Copy value to userspace
	len = snprintf(data_str, sizeof(data_str), "%u\n", parsed_value);

	if (count < len)
		return -EINVAL; // Invlid parameter, too short

	if (copy_to_user(buf, data_str, len))
		return -EFAULT; // Bad user-space pointer in copy.

	*file_pos_ptr = len; // mark EOF for 'cat' command.
	return len;
}

static const struct file_operations pmod_als_fops = {
	.owner = THIS_MODULE,
	.read  = pmod_als_read,
};

static int pmod_als_probe(struct spi_device *spi)
{
	struct pmod_als_dev *als_device;
	int ret;

	// Allocate per device state
	als_device = devm_kzalloc(&spi->dev, sizeof(*als_device), GFP_KERNEL);
	if (!als_device)
		return -ENOMEM; // Could not allocate memory.


	// Configure spi bus
	als_device->spi = spi;
	spi->mode = PMOD_ALS_SPI_MODE;
	spi->bits_per_word = PMOD_ALS_SPI_BITS;
	spi->max_speed_hz = PMOD_ALS_SPI_MAX_CLK;

	ret = spi_setup(spi);
	if (ret) {
		dev_err(&spi->dev, "Failed to setup SPI: %d\n", ret);
		return ret;
	}

	// Expose inteface
	als_device->misc_device.minor = MISC_DYNAMIC_MINOR;
	als_device->misc_device.name = "pmod_als"; // appears to us as: /dev/pmod_als
	als_device->misc_device.fops = &pmod_als_fops;

	ret = misc_register(&als_device->misc_device);
	if (ret) {
		dev_err(&spi->dev, "Failed to register misc device\n");
		return ret;
	}

	// Store state
	spi_set_drvdata(spi, als_device);
	dev_info(&spi->dev, "PMOD ALS driver registered\n");

	return 0;
}

static void pmod_als_remove(struct spi_device *spi)
{
	struct pmod_als_dev *als_device = spi_get_drvdata(spi);

	misc_deregister(&als_device->misc_device);
	dev_info(&spi->dev, "PMOD ALS driver removed\n");
}

static const struct of_device_id pmod_als_of_match[] = {
	{ .compatible = "digilent,pmod-als" },
	{ }
};
MODULE_DEVICE_TABLE(of, pmod_als_of_match);

static struct spi_driver pmod_als_driver = {
	.driver = {
		.name           = "pmod_als",
		.of_match_table = pmod_als_of_match,
	},
	.probe  = pmod_als_probe,
	.remove = pmod_als_remove,
};

module_spi_driver(pmod_als_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SPI Driver for Digilent PMOD ALS");
MODULE_AUTHOR("MA");

