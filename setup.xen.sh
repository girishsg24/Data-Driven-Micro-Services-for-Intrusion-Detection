#!/bin/bash

apt-get update

apt-get -y install build-essential

apt-get -y install bcc bin86 gawk bridge-utils iproute libcurl3 libcurl4-openssl-dev bzip2 module-init-tools transfig tgif 
apt-get -y install texinfo texlive-latex-base texlive-latex-recommended 
apt-get -y install texlive-fonts-extra texlive-fonts-recommended pciutils-dev mercurial
apt-get -y install make gcc libc6-dev zlib1g-dev python python-dev python-twisted 
apt-get -y install libncurses5-dev patch libvncserver-dev libsdl-dev libjpeg62-dev
apt-get -y install iasl libbz2-dev e2fslibs-dev git-core uuid-dev ocaml ocaml-findlib 
apt-get -y install libx11-dev bison flex xz-utils libyajl-dev
apt-get -y install gettext libpixman-1-dev

apt-get -y build-dep xen
apt-get -y install libglib2.0-dev 
apt-get -y install libyajl-dev 

