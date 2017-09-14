/* List of all USB devices you want whitelisted (can be inserted into pc) */
static const struct usb_device_id whitelist_add_devices[] = {
	{ USB_DEVICE(0x0000, 0x0000) },
};
/* List of all USB devices you want whitelisted (can be removed from pc) */
static const struct usb_device_id whitelist_remove_devices[] = {
	{ USB_DEVICE(0x0000, 0x0000) },
};
