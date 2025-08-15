#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcec1d106, "__spi_register_driver" },
	{ 0x9f9a8a99, "misc_deregister" },
	{ 0xd41cd16a, "_dev_info" },
	{ 0x7eb87600, "devm_kmalloc" },
	{ 0x11f82e34, "spi_setup" },
	{ 0x3f4c09e, "misc_register" },
	{ 0x6a409ad6, "_dev_err" },
	{ 0xd51bf3d7, "driver_unregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x3364cb49, "spi_sync" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cdigilent,pmod-als");
MODULE_ALIAS("of:N*T*Cdigilent,pmod-alsC*");

MODULE_INFO(srcversion, "5BAB308A1E1B78B494EF212");
