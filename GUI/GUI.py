#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
import socket,time
import _thread
import json
import operator
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import QtGui

ops = {
    "==": operator.eq,
    "!=": operator.ne,
    ">=": operator.ge,
    ">": operator.gt,
    "<=": operator.le,
    "<": operator.lt,
}
rule_list = []
and_or_list = []

class Rule():
    def __init__(self, rule_str):
        self.src,self.op,self.dst = self.split_rule(rule_str)

    def split_rule(self, rule_str):
        rule_str = ' '.join(rule_str.split())
        astr = rule_str.split(' ')
        cnt = 0
        for i in range(len(rule_str)):
            if rule_str[i] == ' ':
                cnt += 1
                if cnt == 2:
                    index = i
                    break
        return astr[0],astr[1],rule_str[index+1:].strip()

    def fit(self, pack):
        global ops
        key_list = self.src.split('.')
        now = pack
        for key in key_list:
            try:
                now = now[key]
            except:
                return False
        #print("now is :"+now)
        #print("op is :"+self.op)
        #print("dst is :"+self.dst)
        print(ops[self.op](now,type(now)(self.dst)))
        return ops[self.op](now,type(now)(self.dst))

class Lowest_WireShark(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.filter = None
        self.filter_list = []
        self.total_pack = []
        self.initWidget()

        self.resize(250*4,150*4)
        self.center()
        self.setWindowTitle('Lowest WireShark')
        self.setFont(QtGui.QFont("Monospace"))
        self.show()

    def initWidget(self):
        self.table_init()
        #----------test----------#
        #row_count = 0
        #self.table.insertRow(row_count)
        #self.table.setItem(row_count, 0, QTableWidgetItem("id"))
        #self.table.setItem(row_count, 1, QTableWidgetItem("src"))
        #self.table.setItem(row_count, 2, QTableWidgetItem("dst"))
        #self.table.setItem(row_count, 3, QTableWidgetItem("protocol"))
        #self.table.setItem(row_count, 4, QTableWidgetItem("length"))
        #after hbox.addWidget (self.table) , can't addItem
        #self.table.clearContents()
        #self.table_init()
        #row_count = self.table.rowCount()
        #self.table.insertRow(row_count)
        #self.table.setItem(row_count, 0, QTableWidgetItem("id"))
        #self.table.setItem(row_count, 1, QTableWidgetItem("src"))
        #self.table.setItem(row_count, 2, QTableWidgetItem("dst"))
        #self.table.setItem(row_count, 3, QTableWidgetItem("protocol"))
        #self.table.setItem(row_count, 4, QTableWidgetItem("length"))

        #row_count = self.table.rowCount()
        #self.table.insertRow(row_count)
        #self.table.setItem(row_count, 0, QTableWidgetItem("id"))
        #self.table.setItem(row_count, 1, QTableWidgetItem("src"))
        #self.table.setItem(row_count, 2, QTableWidgetItem("dst"))
        #self.table.setItem(row_count, 3, QTableWidgetItem("protocol"))
        #self.table.setItem(row_count, 4, QTableWidgetItem("length"))

        #print(str(self.table.item(0,1).text()))
        #self.table.removeRow(0)
        #self.table.removeRow(1)
        #########################

        self.tree = QTreeView()
        self.model = QStandardItemModel()
        self.tree.header().hide()
        self.tree.setModel(self.model)

        hbox = QHBoxLayout()
        self.detail_qte = QTextEdit(self)
        self.detail_qte.setReadOnly(True)
        hbox.addWidget(self.table)
        hbox.addWidget(self.tree)
        hbox.addWidget(self.detail_qte)
        hbox.setDirection(2)

        vbox = QVBoxLayout()
        vbox.addSpacing(30)
        vbox.addLayout(hbox)

        self.setLayout(vbox)

        filter_lab = QLabel("Filter:",self)
        filter_lab.move(15,10)
        self.filter_qle = QLineEdit(self)
        self.filter_qle.move(80,7)
        self.filter_qle.resize(850,30)
        self.filter_qle.returnPressed.connect(self.setfilter)

    def table_init(self):
        self.table = QTableWidget(0, 5)
        self.table.setHorizontalHeaderLabels(['No', 'Source', 'Destination', 'Protocol', 'Length'])
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.verticalHeader().setVisible(False)
        self.table.itemClicked.connect(self.showdetail)

    def addItem(self,jdata):
        id = str(jdata['id'])
        try:
            src = jdata['ip']['src_ip']
            dst = jdata['ip']['dst_ip']
        except:
            src = jdata['src_mac']
            dst = jdata['dst_mac']
        length = str(jdata['len'])
        protocol = str(jdata['protocol'])
        row_count = self.table.rowCount()

        self.table.insertRow(row_count)
        self.table.setItem(row_count, 0, QTableWidgetItem(id))
        self.table.setItem(row_count, 1, QTableWidgetItem(src))
        self.table.setItem(row_count, 2, QTableWidgetItem(dst))
        self.table.setItem(row_count, 3, QTableWidgetItem(protocol))
        self.table.setItem(row_count, 4, QTableWidgetItem(length))

    def showdetail(self,index):
        row = index.row()
        id = int(str(self.table.item(row,0).text()))
        pack = self.total_pack[id]
        self.model.clear()
        parent = QStandardItem("Details")
        self.model.appendRow(self.drawTree(pack,parent))
        self.detail_qte.setText(pack['data'])

    def drawTree(self,jdata,parent):
        try:
            for key in jdata.keys():
                if key == 'data' or key == 'id':
                    continue
                now_pa = QStandardItem(str(key))
                parent.appendRow(self.drawTree(jdata[key],now_pa))
        except:
            parent.appendRow(QStandardItem(str(jdata)))
        return parent

    def rm_add_AllItem(self):
        while True:
            sitem = self.table.item(0,0)
            if sitem == None:
                break
            self.table.removeRow(0)
        length = len(self.total_pack)
        for i in range(length):
            self.addItem(self.total_pack[i])

    def reshow(self):
        self.rm_add_AllItem()
        # use removeRow to removeRow(0) to ''
        now_row = 0
        while True:
            sitem = self.table.item(now_row,0)
            if sitem == None:
                break
            pack_id = int(str(sitem.text()))
            if self.filter != None and check(self.total_pack[pack_id]) == False:
                self.table.removeRow(now_row)
                now_row -= 1
            now_row += 1

        #for cnt in range(num):
            #pack_id = int(str(self.table.item(cnt,0).text()))
            #if self.filter != None and check(total_pack[pack_id]) == False:
                #print("zhi xing le")
                #self.addItem(pack)
                #self.table.removeRow(cnt)

    def setfilter(self):
        global rule_list,and_or_list
        text = self.filter_qle.text()
        if text == '':
            self.filter = None
        else:
            self.filter = text
        try:
            tmp = self.filter.replace("|","&").split("&")
            self.filter_list = []
            for i in tmp:
                self.filter_list.append(i)
            and_or_list = []
            for i in range(len(self.filter)):
                tmp = self.filter[i]
                if tmp == '&' or tmp == '|':
                    and_or_list.append(tmp)
            rule_list = []
            for i in self.filter_list:
                rule_list.append(Rule(i))
        except:
            pass
        self.reshow()

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

def check(jdata):
    global rule_list,and_or_list
    try:
        flag = rule_list[0].fit(jdata)
        #print(flag)
        #print(len(and_or_list))
        for i in range(len(and_or_list)):
            tmp = rule_list[i+1].fit(jdata)
            if and_or_list[i] == '&':
                flag = flag and tmp
            else:
                flag = flag or tmp
        #print(flag)
        return flag
    except:
        return False

def do_main(ex):
    BUF_SIZE = 2048
    server_addr = ('0.0.0.0', 2333)
    try :
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except (socket.error, msg):
        print ("Creating Socket Failure. Error Code : " + str(msg[0]) + " Message : " + msg[1])
        sys.exit()
    print ("Socket Created!")
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try :
        server.bind(server_addr)
    except (socket.error, msg):
        print ("Binding Failure. Error Code : " + str(msg[0]) + " Message : " + msg[1])
        sys.exit()
    print ("Socket Bind!")
    server.listen(20)
    print ("Socket listening")
    while True:
        client, client_addr = server.accept()
        print ('Connected by'), client_addr
        while True :
            try:
                data = client.recv(BUF_SIZE)
                if data == '':
                    continue
                data = str(data,encoding='utf-8')
                jdata = json.loads(data)
                cnt = len(ex.total_pack)
                jdata["id"] = cnt
                ex.total_pack.append(jdata)
                if ex.filter != None:
                    if check(jdata):
                        ex.addItem(jdata)
                else:
                    ex.addItem(jdata)
            except:
                print("Unexpected error:", sys.exc_info()[0])
                pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Lowest_WireShark()
    try:
        _thread.start_new_thread( do_main,(ex,) )
    except:
        print ("Error")
    sys.exit(app.exec_())
