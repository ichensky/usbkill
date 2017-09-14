#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/reboot.h>
#include "config.h"

MODULE_LICENSE("GPL");

/*
 * returns 0 if no match, 1 if match
 *
 * Taken from drivers/usb/core/driver.c
 */
static int usb_match_device(struct usb_device *dev,
		const struct usb_device_id *id){
	if ((id->match_flags & USB_DEVICE_ID_MATCH_VENDOR) &&
			id->idVendor != le16_to_cpu(dev->descriptor.idVendor)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_PRODUCT) &&
			id->idProduct != le16_to_cpu(dev->descriptor.idProduct)) { return 0; }

	/* No need to test id->bcdDevice_lo != 0, since 0 is never
	   greater than any unsigned number. */
	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_LO) &&
			(id->bcdDevice_lo > le16_to_cpu(dev->descriptor.bcdDevice))) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_HI) &&
			(id->bcdDevice_hi < le16_to_cpu(dev->descriptor.bcdDevice))) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_CLASS) &&
			(id->bDeviceClass != dev->descriptor.bDeviceClass)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_SUBCLASS) &&
			(id->bDeviceSubClass != dev->descriptor.bDeviceSubClass)) { return 0; }

	if ((id->match_flags & USB_DEVICE_ID_MATCH_DEV_PROTOCOL) &&
			(id->bDeviceProtocol != dev->descriptor.bDeviceProtocol)) { return 0; }

	return 1;
}

static void panic_time(struct usb_device *usb){
	struct device *dev;

	for (dev = &usb->dev; dev; dev = dev->parent){
		mutex_unlock(&dev->mutex);
	}
	printk("Powering off.\n");
	kernel_power_off();
}

static void check_usb_device(struct usb_device *dev) {
	const struct usb_device_id *dev_id;

	unsigned long len = sizeof(whitelist_devices)/
		sizeof(whitelist_devices[0]);
	int i; 
	for(i = 0; i < len; i++) {
		dev_id = &whitelist_devices[i];
		if (usb_match_device(dev, dev_id)) {
			pr_info("Device is ignored\n");
			return;
		}
	}
	panic_time(dev);
}
static void check_usb_device_remove(struct usb_device *dev) {
	const struct usb_device_id *dev_id;

	unsigned long len = sizeof(whitelist_devices_remove)/
		sizeof(whitelist_devices_remove[0]);
	int i; 
	for(i = 0; i < len; i++) {
		dev_id = &whitelist_devices_remove[i];
		if (usb_match_device(dev, dev_id)) {
			panic_time(dev);
			break;
		}
	}
	return;
}

static int notify(struct notifier_block *self, 
		unsigned long action, 
		void *dev) {
	if(action==USB_DEVICE_ADD){
		check_usb_device(dev);
	}
	else if(action==USB_DEVICE_REMOVE){
		check_usb_device(dev);
		check_usb_device_remove(dev);
	}
	return 0;
}
static struct notifier_block usb_notify = {
	.notifier_call = notify,
};
static int __init mod_init(void) {
	pr_info("Watching for USB devices...\n");
	usb_register_notify(&usb_notify);
	return 0;
}
static void __exit mod_exit(void) {
	usb_unregister_notify(&usb_notify);
	pr_info("No longer watching for USB devices.\n");
}
module_init(mod_init);
module_exit(mod_exit);
