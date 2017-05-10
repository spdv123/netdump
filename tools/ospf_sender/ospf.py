#!/usr/bin/env python
import sys
import time
import os

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

try:
    from scapy.all import *
    load_contrib('ospf')
except:
    print bcolors.FAIL + '''
    Scapy not found, see https://github.com/secdev/scapy
    ''' + bcolors.ENDC
    exit(1)


if os.geteuid() != 0:
    print bcolors.FAIL + '''
    Please run as root user.
    ''' + bcolors.ENDC
    exit(1)

try:
    pkts = rdpcap('ospf.pcap')
except:
    print bcolors.FAIL + '''
    Cannot load ospf.pcap, please check the file.
    ''' + bcolors.ENDC
    exit(1)
hello = pkts[0]
dd = pkts[1]
lsr = pkts[2]
lsu = pkts[3]
lsack = pkts[4]
ospf = [hello, dd, lsr, lsu, lsack]
name = ['Hello', 'DD', 'LSR', 'LSU', 'LSAck']


def usage():
    print '''
    Usage: sudo python ospf.py [times]
    '''


def main():
    times = 1
    host = '192.168.33.128'
    if len(sys.argv) > 2:
        usage()
        exit(0)
    if len(sys.argv) == 2:
        if sys.argv[1].strip().lower() == 'help':
            usage()
            exit(0)
        try:
            times = int(sys.argv[1].strip())
        except:
            usage()
            exit(0)
    start_time = time.time()
    for t in range(times):
        cnt = 0
        for packet in ospf:
            packet[IP].src = host
            packet[IP].chksum = None
            packet[OSPF_Hdr].src = host
            packet[OSPF_Hdr].chksum = None
            print '[' + bcolors.OKBLUE + '@' + bcolors.ENDC + '] Sending OSPF %s packet...' % name[cnt % len(ospf)]
            cnt += 1
            sendp(packet, verbose=0)
            print '[' + bcolors.OKGREEN + '*' + bcolors.ENDC + '] Sent success !'
    print 'All tasks finished in %.2f seconds' % (time.time() - start_time)

if __name__ == '__main__':
    main()
