#!/bin/bash
echo '[*] Checking python3'
sudo apt-get install python3 -y
echo '[*] Checking PyQt5'
sudo apt-get install python3-pyqt5
echo '[*] Checking libpcap'
sudo apt-get install libpcap0.8-dev
echo '[*] Checking scapy'
wget https://github.com/secdev/scapy/archive/v2.3.3.zip
unzip v2.3.3.zip
rm -f v2.3.3.zip
cd scapy-2.3.3
sudo python setup.py install
cd ..
rm -rf scapy-2.3.3
echo 'finish'
