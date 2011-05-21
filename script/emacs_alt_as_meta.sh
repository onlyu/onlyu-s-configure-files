#!/bin/sh

kbdcontrol -d | sed 's/lalt/meta/g' | sed 's/ralt/meta/g' > /usr/share/syscons/keymaps/local.kbd
#echo keymap=\"local\" >> /etc/rc.conf 
