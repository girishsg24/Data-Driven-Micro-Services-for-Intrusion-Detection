#!/bin/bash
#------------------------------------------------------------------------------
#   This script is used for making clickos image. 
#   Run this script, with Xen source version later than 4.4.1.
#   
#   When running this script, you ought to be the Dom0 of Xen, which means you 
#   have already get Xen installed. 
#
#   Please report any issues to: 
#       hongdal@clemson.edu
#   
#   Happy coding! 
#   07/13/2015
#------------------------------------------------------------------------------
export XEN_ROOT=$(pwd)"/xen-4.4.1/" 
export MINIOS_ROOT=$XEN_ROOT"extras/mini-os/" 
export CLICKOS_ROOT=$(pwd)"/clickos/"
export TOOLCHAIN_ROOT=$(pwd)"/toolchain/"
export COSMOS_ROOT=$(pwd)"/cosmos/"

if [[ ! -d $XEN_ROOT ]]; then 
    echo "Not found: "$XEN_ROOT
    exit 1
fi
if [[ ! -d $MINIOS_ROOT ]]; then 
    echo "Not found: "$MINIOS_ROOT
    exit 1
fi
if [[ ! -d $CLICKOS_ROOT ]]; then 
    echo "Not found: "$CLICKOS_ROOT
    exit 1
fi
if [[ ! -d $TOOLCHAIN_ROOT ]]; then 
    echo "Not found: "$TOOLCHAIN_ROOT
    exit 1
fi
if [[ ! -d $COSMOS_ROOT ]]; then 
    echo "Not found: "$COSMOS_ROOT
    exit 1
fi

echo "----------------------------------------"
echo "WARNING: about to delete: "$MINIOS_ROOT
echo "----------------------------------------"
counter=5
while [[ counter -gt 0 ]]; do
    echo $counter" seconds left! "
    let "counter=$counter-1"
    sleep 1
done
rm -rf $MINIOS_ROOT
cp -rf $(pwd)"/minios/" $MINIOS_ROOT

# build toolchain 
cd $TOOLCHAIN_ROOT
echo "+-------------------------------------------------------------+"
echo "|                 BUILDING TOOLCHAINS ...                     |" 
echo "+-------------------------------------------------------------+"
make 
if [[ $? -ne 0 ]]; then
    echo "Make toolchain failed"
    exit 1
fi
export NEWLIB_ROOT=$TOOLCHAIN_ROOT"/x86_64-root/x86_64-xen-elf"
export LWIP_ROOT=$TOOLCHAIN_ROOT"/x86_64-root/x86_64-xen-elf"

# build clickos 
cd $CLICKOS_ROOT
echo "+-------------------------------------------------------------+"
echo "|                 CONFIGURE FOR CLICKOS ...                   |" 
echo "+-------------------------------------------------------------+"
./configure --enable-minios --with-xen=$XEN_ROOT --with-minios=$MINIOS_ROOT
if [[ $? -ne 0 ]]; then
    echo "Configure failed!"
    exit 1
fi
echo "+-------------------------------------------------------------+"
echo "|                     MAKE MINIOS ...                         |" 
echo "+-------------------------------------------------------------+"
make minios
if [[ $? -ne 0 ]]; then 
    echo "Make minios failed!"
    exit 1
fi

# build cosmos 
echo "+-------------------------------------------------------------+"
echo "|                     MAKE COSMOS ...                         |" 
echo "+-------------------------------------------------------------+"
cd $XEN_ROOT
./configure 
if [[ $? -ne 0 ]]; then 
    echo "./configure for xen failed!"
    exit 1
fi
cd tools 
make -C include
if [[ $? -ne 0 ]]; then 
    echo "make in xen/tools failed!"
    exit 1
fi
cd $COSMOS_ROOT
make DOMLIB=xl
if [[ $? -ne 0 ]]; then 
    echo "make cosmos failed!"
    exit 1
fi


