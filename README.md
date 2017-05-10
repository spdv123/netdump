## NetDump

A Simplified Wireshark | 一个简化的类Wireshark工具

### About | 说明

* Background: Use C++ with libpcap
* 后台采用C++编写，使用libpcap库
* GUI: Use PyQt5
* GUI采用PyQt5编写
* tools: Send test packets
* 小工具，包含发送测试包的脚本

### Environment | 环境要求
(Tested on Ubuntu 16.04 x86_64)
* python3 (For GUI): ```sudo apt-get install python3```
* PyQt5 (For GUI): ```sudo apt-get install python3-pyqt5```
* libpcap (For Background): ```sudo apt-get install libpcap0.8-dev```
* scapy (For tools): https://github.com/secdev/scapy

_We offerd a bash script to check and setup environment automatically._
_我们提供了一个自动检测配置环境的bash脚本_
```sh
sudo bash setup.sh
```

### Usage | 用法
```sh
sudo ./run.sh
```

### Tools | 小工具

* ospf_sender: ```sudo python ospf_sender.py [times]```
