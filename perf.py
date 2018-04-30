#!/usr/bin/env python
#
#  Requires:
#    - scapy
#    - tcpreplay

import sys
from scapy.all import *

if len(sys.argv) != 6:
    print "usage: %s dst_ip dst_port iface pps repeat" % sys.argv[0]
    print "example: %s 1.2.3.4 5001 eth0 100000 300"
    exit(1)

payload = 'a' * 100
pkt = IP(dst=sys.argv[1])/UDP(dport=[sys.argv[2]])/payload

sendpfast(pkt * sys.argv[4], pps=sys.argv[4], mbps=1000, loop=sys.argv[5], iface=sys.argv[3])
