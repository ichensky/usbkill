# usbkill 
`usbkill` is an anti-forensic LKM kill-switch that waits for a change on your usb ports and turns off your computer.

To run:

```shell
make
sudo insmod usbkill.ko
```

You will need to have the `linux-headers` package installed. If you haven't:

```shell
apt-get install linux-headers-$(uname -r)
```

List usb devices:

```shell
lsusb
```
