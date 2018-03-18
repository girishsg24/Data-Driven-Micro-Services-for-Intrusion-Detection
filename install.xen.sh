#!/bin/bash
#------------------------------------------------------------------------------
#   This scripts is used for installing Xen under Ubuntu14.04. 
#   This scirpts is expected to be placed in the same directory with $XEN_ROOT.
#
#   For diffreent distros of Linux, change 'post installation' correspondingly
#   to get Xen boot correctly.
#
#   This is an unstable version. Report any issues to: 
#       hongdal@clemson.edu
#
#   07/13/2015
#------------------------------------------------------------------------------

# Build Xen dependencies. 
echo "+--------------------------------------------------+"
echo "|             BUILDING DEPENDENCIES ...            |"
echo "+--------------------------------------------------+"
./setup.xen.sh

# setting up environment parameters.
if [[ $# -eq 1 ]];  then
    export XEN_ROOT=$1 
elif [[ $# -eq 0 ]]; then 
    export XEN_ROOT=$(pwd)"/xen-4.4.1"
else
    exit 1
fi
echo "Set XEN_ROOT to "$XEN_ROOT
cd $XEN_ROOT
if [[ $? -ne 0 ]]; then 
    echo "Could not find xen-root directory. "
    exit 1
fi 

# find core number. 
cpuNumber=$(lscpu | awk -F ':' '{if(NR==4){print$2}}' | sed -e 's/^[ /t ]*//')
if [[ $cpuNumber -gt 1 ]]; then 
    let "cpuNumber=$cpuNumber-1"
fi

# ./configure
echo "+--------------------------------------------------+"
echo "|                 ./configure ...                   |"
echo "+--------------------------------------------------+"
./configure
if [[ $? -ne 0 ]]; then
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    echo "   CONFIGURE FIALED! NOT GOING TO INSTALL.  "
    echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" 
    exit 1
fi

# make world = make clean ; make dist
echo "+--------------------------------------------------+"
echo "|                 MAKE WORLD ...                   |"
echo "+--------------------------------------------------+"
make -j $cpuNumber world

if [[ $? -ne 0 ]]; then
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    echo "      MAKE FIALED! NOT GOING TO INSTALL. "
    echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" 
    exit 1
fi

# install 
echo "+--------------------------------------------------+"
echo "|         INSTALL XEN TO LOCAL MACHINE...          |"
echo "+--------------------------------------------------+"
make -j $cpuNumber install 
if [[ $? -ne 0 ]]; then
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    echo "            INSTALL FAILED!                 "
    echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" 
    exit 1
fi 

# post installation 
echo "+--------------------------------------------------+"
echo "|             POST INSTALLATION ...                |"
echo "+--------------------------------------------------+"
/sbin/ldconfig
update-rc.d xencommons defaults 19 18
update-rc.d xend defaults 20 21 
update-rc.d xendomains defaults 21 20 
update-rc.d xen-watchdog defaults 22 23

# set default boot entry. 
# sed -i 's/GRUB_DEFAULT=.*/GRUB_DEFAULT=2/' /etc/default/grub
# config the momory.
sed -i 's/GRUB_CMDLINE_XEN=.*//' /etc/default/grub 
#echo 'GRUB_CMDLINE_XEN="dom0_mem=16G,max:16G"' >> /etc/default/grub
update-grub 


# post installation 
echo "+--------------------------------------------------+"
echo "|             XEN INSTALL COMPLETED ...            |"
echo "+--------------------------------------------------+"
echo ""
echo "Back verything necessary before you reboot. Good Luck!"
echo ""
echo ""


