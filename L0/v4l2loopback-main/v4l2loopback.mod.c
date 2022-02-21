#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

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
	{ 0x8e6402a9, "module_layout" },
	{ 0x90ec5eae, "param_ops_charp" },
	{ 0x492a1d21, "param_ops_bool" },
	{ 0xed1f2dff, "param_array_ops" },
	{ 0x4caf37f7, "param_ops_int" },
	{ 0x2e09915e, "video_ioctl2" },
	{ 0xdec1b54d, "v4l2_event_unsubscribe" },
	{ 0x7c4cd9ee, "nonseekable_open" },
	{ 0x5788679a, "noop_llseek" },
	{ 0x6e2db325, "misc_deregister" },
	{ 0x761c978, "misc_register" },
	{ 0xbdff3e7d, "mutex_lock_killable" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0xdee8adbd, "_dev_err" },
	{ 0x7665a95b, "idr_remove" },
	{ 0xc6c1cbdf, "device_create_file" },
	{ 0x9d4458f6, "__video_register_device" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xb2ae6ae3, "v4l2_ctrl_handler_setup" },
	{ 0xb067fb11, "v4l2_ctrl_new_custom" },
	{ 0x66e81a1b, "v4l2_ctrl_handler_init_class" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x1fc5b7d0, "video_device_release" },
	{ 0x14860538, "video_device_alloc" },
	{ 0xe826ac3c, "v4l2_device_register" },
	{ 0xb8f11603, "idr_alloc" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xb283a4ad, "vm_insert_page" },
	{ 0xcb6679a7, "vmalloc_to_page" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x3303335c, "v4l2_fh_add" },
	{ 0x2ec977e1, "v4l2_fh_init" },
	{ 0xe78dfe6d, "kmem_cache_alloc_trace" },
	{ 0x595451f1, "kmalloc_caches" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x40a9b349, "vzalloc" },
	{ 0xa1576a71, "v4l2_event_pending" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x69acdf38, "memcpy" },
	{ 0x800473f, "__cond_resched" },
	{ 0xe46021ca, "_raw_spin_unlock_bh" },
	{ 0xc3690fc, "_raw_spin_lock_bh" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x4bf17765, "v4l2_fh_exit" },
	{ 0x3deed6e2, "v4l2_fh_del" },
	{ 0x8e17b3ae, "idr_destroy" },
	{ 0x1d40b6f3, "idr_for_each" },
	{ 0x5e515be6, "ktime_get_ts64" },
	{ 0x7c74a687, "video_devdata" },
	{ 0x12407a8d, "v4l2_ctrl_subscribe_event" },
	{ 0x360b6b7a, "pv_ops" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x20978fb9, "idr_find" },
	{ 0x5c3c7387, "kstrtoull" },
	{ 0x9257fe65, "v4l2_ctrl_handler_free" },
	{ 0xfffd40b6, "v4l2_device_unregister" },
	{ 0x5fa8bff1, "video_unregister_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x54191821, "device_remove_file" },
	{ 0x999e8297, "vfree" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "videodev");


MODULE_INFO(srcversion, "7E23594B9D4C0E9BFA73CE5");
