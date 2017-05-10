#!/bin/bash
cd Background
make clean
make
cd ..
python3 GUI/GUI.py &
sleep 1
sudo ./Background/netdump
