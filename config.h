/* List of all USB devices you want whitelisted (i.e. ignored) */
static const struct usb_device_id whitelist_devices[] = {
	{ USB_DEVICE(0x0000, 0x0000) },
};
/* List of all USB devices you have whitelisted and if any of this device remove: shutdown */
static const struct usb_device_id whitelist_devices_remove[] = { };
