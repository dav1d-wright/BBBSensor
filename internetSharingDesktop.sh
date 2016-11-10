#!/bin/bash

echo "I'm assuming eth1 is the interface facing the Beaglebone and wlan0 is the internet interface on this computer."
echo "Run ifconfig, if this is not the case then adjust this script."

ifconfig eth1 192.168.7.1
iptables --table nat --append POSTROUTING --out-interface wlan0 -j MASQUERADE
iptables --append FORWARD --in-interface eth1 -j ACCEPT
echo 1 > /proc/sys/net/ipv4/ip_forward
