## ClickOS
Over the years middleboxes have become a fundamental part of today’s networks. Despite their usefulness, 
they come with a number of problems, many of which arise from the fact that they are hardware-based: they are costly,
difficult to manage, and their functionality is hard or impossible to change, to name a few. To address these issues,
there is a recent trend towards network function virtualization (NFV), in essence proposing to turn these middleboxes into software-based,
virtualized entities. Towards this goal we introduce ClickOS, a high-performance, virtualized software middlebox platform. 
ClickOS virtual machines are small (5MB), boot quickly (about 30 milliseconds), add little delay (45 microseconds) and over one hundred of them can be concurrently run while saturating a 10Gb pipe on a commodity server.
We further implement a wide range of middleboxes including a firewall, a carrier-grade NAT and a load balancer and show that ClickOS can handle packets in the millions per second.

# Reference: http://cnp.neclab.eu/projects/clickos/
====================================================================

This repository contains the useful scripts for setting up ClickOS test bed in CloudLab. 
This scripts have been tested under Ubuntu 14.04 LTS

=================================================================
# Install Dependencies
apt-get update
apt-get install build-essential wget
apt-get install bcc bin86 gawk bridge-utils iproute libcurl3 libcurl4-openssl-dev bzip2 module-init-tools transfig tgif 
apt-get install texinfo texlive-latex-base texlive-latex-recommended 
apt-get install texlive-fonts-extra texlive-fonts-recommended pciutils-dev mercurial
apt-get install make gcc libc6-dev zlib1g-dev python python-dev python-twisted 
apt-get install libncurses5-dev patch libsdl-dev libjpeg62-dev
apt-get install libvncserver-dev
apt-get install iasl libbz2-dev e2fslibs-dev git-core uuid-dev ocaml ocaml-findlib 
apt-get install libx11-dev bison flex xz-utils libyajl-dev
apt-get install gettext libpixman-1-dev
apt-get build-dep xen
apt-get install libglib2.0-dev 
apt-get install libyajl-dev 

===============================================================================
To build the assets, run following commands: 
   
    1) ./setup.basic.sh     # Initialize the environment for next steps.
        (chmod u+x setup.basic.sh if permission denied error)
    
    2) ./install.xen.sh     # Install Xen into this machine and make current OS as Dom0. 
                            # Reboot is required! This step may exit with no-zero (this case,
                            # you may need to install some dependencies that are required by Xen.)
        (chmod u+x install.xen.sh if permission denied error)
        
    3) Grub Step
       sudo grep "menuentry '" /boot/grub/grub.cfg |less -N 
                        # lists all the available images and their number
                        # select the one with xen keyword and use this to update grub file
       edit /etc/default/grub
                        #edit grub default with the (number you found earlier - 1)
                        #Add the below line at the end:
                        GRUB_CMDLINE_XEN="dom0_mem=1024M,max:1024M dom0_max_vcpus=2"

        update-grub
        reboot
       
    4) ./install.clickos.sh # After rebooting, run this script to install clickos. 
        (chmod u+x install.clickos.sh if permission denied error)

    5) ./install.ovs.sh     # Install Open Vswitch. ClickOS needs this. 
        (chmod u+x install.ovs.sh if permission denied error)

    After the four steps, the whole testbed is set up. 
    Then you may need to add some bridges or ports to ovs to support clickos.  
    
===============================================================================
# Refer to the following documents for more details: 
    http://wiki.xenproject.org/wiki/Compiling_Xen_From_Source
    https://github.com/cnplab/clickos 
    http://cnp.neclab.eu/getting-started
