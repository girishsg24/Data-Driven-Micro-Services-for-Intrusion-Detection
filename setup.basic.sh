#!/bin/bash
# install vim, if it is not installed.
apt-get update
apt-get -y install vim

tar -xf xen-4.4.1.tar.gz

tar -xf toolchain.tar; tar -xf minios.tar; tar -xf clickos.tar; tar -xf cosmos.tar;
