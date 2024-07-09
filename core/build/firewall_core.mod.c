#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x85df9b6c, "strsep" },
	{ 0x2d39b0a7, "kstrdup" },
	{ 0xcf2a6966, "up" },
	{ 0xe2c17b5d, "__SCT__might_resched" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x66b6fe77, "misc_deregister" },
	{ 0xc3920a5a, "init_net" },
	{ 0xc40e93d3, "nf_unregister_net_hook" },
	{ 0x37a0cba, "kfree" },
	{ 0x3ef70737, "filp_close" },
	{ 0x122c3a7e, "_printk" },
	{ 0x67543840, "filp_open" },
	{ 0x7b4da6ff, "__init_rwsem" },
	{ 0x6e1510b2, "misc_register" },
	{ 0xb88db70c, "kmalloc_caches" },
	{ 0x4454730e, "kmalloc_trace" },
	{ 0xb1b2127c, "nf_register_net_hook" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x11089ac7, "_ctype" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0xfff5afc, "time64_to_tm" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x57bc19d2, "down_write" },
	{ 0x2a968edb, "kernel_write" },
	{ 0xce807a25, "up_write" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xbb4f4766, "simple_write_to_buffer" },
	{ 0xfb578fc5, "memset" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0x2fa5cadd, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EF659CF6C83A8A775300EE7");
