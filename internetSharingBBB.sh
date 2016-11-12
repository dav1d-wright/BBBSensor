#!/bin/bash

ifconfig usb0 192.168.7.2
route add default gw 192.168.7.1
echo "nameserver 8.8.8.8" >> /etc/resolv.conf
