#!/bin/bash 

wget http://openvswitch.org/releases/openvswitch-2.3.1.tar.gz
tar -xzf openvswitch-2.3.1.tar.gz 
rm openvswitch-2.3.1.tar.gz
cd openvswitch-2.3.1/

./configure --with-linux=/lib/modules/`uname -r`/build
make -j10
sudo make install

sudo make modules_install
sudo /sbin/modprobe openvswitch

sudo mkdir -p /usr/local/etc/openvswitch
sudo ovsdb-tool create /usr/local/etc/openvswitch/conf.db vswitchd/vswitch.ovsschema
sudo ovsdb-server --remote=punix:/usr/local/var/run/openvswitch/db.sock --remote=db:Open_vSwitch,Open_vSwitch,manager_options --private-key=db:Open_vSwitch,SSL,private_key --certificate=db:Open_vSwitch,SSL,certificate --bootstrap-ca-cert=db:Open_vSwitch,SSL,ca_cert --pidfile --detach
sudo ovs-vsctl --no-wait init
sudo ovs-vswitchd --pidfile --detach

cd ..

