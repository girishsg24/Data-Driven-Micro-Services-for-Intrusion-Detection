#!/bin/bash

if [[ -z $1 ]]; then 
    echo "Usage: $0 [memory size]"
    echo "$0 16 --> This mean set Xen memory usage as 16G."
    exit 1
fi

update-rc.d xencommons defaults 19 18
update-rc.d xend defaults 20 21 
update-rc.d xendomains defaults 21 20 
update-rc.d xen-watchdog defaults 22 23

# set default boot entry. 
sed -i 's/GRUB_DEFAULT=.*/GRUB_DEFAULT=2/' /etc/default/grub
# config the momory.
sed -i 's/GRUB_CMDLINE_XEN=.*//' /etc/default/grub 
echo "GRUB_CMDLINE_XEN=\"dom0_mem=$1G,max:$1G\"" >> /etc/default/grub

update-grub
