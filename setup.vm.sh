#!/bin/bash

name=$1
ip=$2
bridge=$3 
method=$4
image=$5

if [[ $# -lt 3 ]]; then 
    echo "Usage: $0 <vm_name> <vm_IP> <bridge to connect> [new|copy] [path-to-exsisted-image]"
    echo " If 'copy' is specified, then 'path-to-exsisting-image' refers to /dev/openstack-volumes/XenUbuntu-disk"
    exit 1
fi

if [[ -z $method ]]; then
    method="new"
fi
if [[ -z $image ]]; then
    image="/dev/openstack-volumes/XenUbuntu-disk"
fi

# back up /etc/xen-tools/xen-tools.conf 
if [[ ! -f "/etc/xen-tools/xen-tools.conf.orig" ]]; then 
    cp /etc/xen-tools/xen-tools.conf /etc/xen-tools/xen-tools.conf.orig
fi

if [[ $method == "copy" ]]; then 
    cp ./xen-tools.conf.copy /etc/xen-tools/xen-tools.conf
    if [[ ! -d "/tmp/mnt/" ]]; then 
        mkdir /tmp/mnt/ 
    fi
    umount /tmp/mnt/ 
    mount $image /tmp/mnt/
    sed -i "s/^install-source.*\+/install-source = \/tmp\/mnt\//" /etc/xen-tools/xen-tools.conf
else 
    cp ./xen-tools.conf.new /etc/xen-tools/xen-tools.conf
fi

# make image. 
xen-create-image --hostname=$name --password=123

if [[ $? -ne 0 ]]; then 
    echo ""
    echo ""
    echo "WARNING: xen-create-image failed! "
    echo ""
    echo ""
    umount /tmp/mnt/ > /dev/null 2>&1 
    exit 1
else
    echo ""
    echo ""
    echo "CONGRATULATIONS: xen-create-image successed! "
    echo "Use:  xl create /etc/xen/"$name".vm.cfg; xl console $name to start the MV."
    echo ""
    echo ""
    umount /tmp/mnt/ > /dev/null 2>&1 
fi

# setup net works.
MAC=`grep "vif" /etc/xen/$name.vm.cfg | awk -F ',' '{print $2}' | cut -d '=' -f 2`
sed -i "s/^vif.*\+/vif = [ 'script=vif-openvswitch,ip="$ip", mac="$MAC", bridge="$bridge"' \]/" /etc/xen/$name.vm.cfg

