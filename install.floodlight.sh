#!/bin/bash

apt-get update
apt-get -y install build-essential default-jdk ant python-dev eclipse

git clone git://github.com/floodlight/floodlight.git 
cd floodlight
git checkout stable
ant
mkdir /var/lib/floodlight/
chmod 777 /var/lib/floodlight/

cd ..

