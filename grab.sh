#!/bin/bash

# grab xen
scp hongdal@access1.cs.clemson.edu:~/Backend/resource/xen-4.4.1.tar.gz .
if [[ ! -e xen-4.4.1.tar.gz ]]; then
    echo "Downlaod xen failed."
    exit 1
fi
tar -zxf xen-4.4.1.tar.gz
rm -f xen-4.4.1.tar.gz

# grab clickos
scp hongdal@access1.cs.clemson.edu:~/Backend/resource/clickos.tar .
if [[ ! -e clickos.tar ]]; then
    echo "Downlaod clickos failed."
    exit 1
fi
tar -xf clickos.tar 
rm -f clickos.tar

# grab minios 
scp hongdal@access1.cs.clemson.edu:~/Backend/resource/minios.tar .
if [[ ! -e minios.tar ]]; then
    echo "Downlaod minios failed."
    exit 1
fi
tar -xf minios.tar
rm -f minios.tar 


# grab toolchain 
scp hongdal@access1.cs.clemson.edu:~/Backend/resource/toolchain.tar .
if [[ ! -e toolchain.tar ]]; then
    echo "Downlaod toolchain failed."
    exit 1
fi
tar -xf toolchain.tar 
rm -f toolchain.tar 


# grab cosmos 
scp hongdal@access1.cs.clemson.edu:~/Backend/resource/cosmos.tar .
if [[ ! -e cosmos.tar ]]; then
    echo "Downlaod cosmos failed."
    exit 1
fi
tar -xf cosmos.tar 
rm -f cosmos.tar 




