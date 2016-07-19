import os
import sys
from PyQt4 import QtCore, QtGui, QtSql
from datetime import datetime

class Window(QtGui.QWidget):
    def __init__(self, file_name='order_entry', log_file=''):
        QtGui.QWidget.__init__(self)

        self.file_name = file_name
        
        symbols = self.get_symbols(log_file)
        self.cb_sym = QtGui.QComboBox(self)
        for sym in sorted(symbols):
            self.cb_sym.addItem(sym)
        
        self.cb_action = QtGui.QComboBox(self)
        self.cb_action.addItem("NEW")
        self.cb_action.addItem("MODIFY")
        self.cb_action.addItem("CANCEL")
        
        self.cb_side = QtGui.QComboBox(self)
        self.cb_side.addItem("BUY")
        self.cb_side.addItem("SELL")

        self.cb_tif = QtGui.QComboBox(self)
        self.cb_tif.addItem("DAY")
        self.cb_tif.addItem("IOC")

 
        self.le_price = QtGui.QLineEdit("0.0")
        self.le_size = QtGui.QLineEdit("1")
        self.le_order_id =  QtGui.QLineEdit("-1")

        l0 = QtGui.QLabel("symbol")
        l1 = QtGui.QLabel("action")
        l2 = QtGui.QLabel("side")
        l3 = QtGui.QLabel("tif")
        l4 = QtGui.QLabel("price")
        l5 = QtGui.QLabel("size")
        l6 = QtGui.QLabel("order_id")
        
        self.bt_submit = QtGui.QPushButton("SUBMIT")
        self.bt_submit.clicked.connect(self.handleSubmit)
 
        layout = QtGui.QGridLayout()

        layout.addWidget(l0,       0,  0)
        layout.addWidget(l1,       1,  0)
        layout.addWidget(l2,       2,  0)
        layout.addWidget(l3,       3,  0)
        layout.addWidget(l4,       4,  0)
        layout.addWidget(l5,       5,  0)
        layout.addWidget(l6,       6,  0)

        layout.addWidget(self.cb_sym,       0,  1)
        layout.addWidget(self.cb_action,    1,  1)
        layout.addWidget(self.cb_side,      2,  1)
        layout.addWidget(self.cb_tif,       3,  1)
        layout.addWidget(self.le_price,     4,  1)
        layout.addWidget(self.le_size,      5,  1)
        layout.addWidget(self.le_order_id,  6,  1)
        layout.addWidget(self.bt_submit,    7,  1)
        self.setLayout(layout)

    def handleSubmit(self):
        str_to_action = {'NEW':0,'MODIFY':2,'CANCEL':1}
        str_to_side = {'BUY':1,'SELL':0}
        str_to_tif = {'DAY':0, 'IOC':1}        

        oem_id = str(datetime.now())
        oem_action = str_to_action[str(self.cb_action.itemText(self.cb_action.currentIndex()))]  
        symbol = self.cb_sym.itemText(self.cb_sym.currentIndex())
        price = int(float(self.le_price.text()) * 1e7)
        size = int(self.le_size.text())
        order_id = int(self.le_order_id.text())
        is_ioc = str_to_tif[str(self.cb_tif.itemText(self.cb_tif.currentIndex()))]  
        is_bid = str_to_side[str(self.cb_side.itemText(self.cb_side.currentIndex()))]  
                
        ss =  "%s,%s,%s,%s,%s,%s,%s,%s,\n" % (oem_id,oem_action,symbol,price,size,order_id, is_ioc,is_bid)
        print ss
        f = open(self.file_name, 'a')
        f.write(ss)
        f.close()

    def get_symbols(self, log_file):
        f = open(log_file)
        syms = set([])
        for line in f.readlines():
            if line.count('INFO') > 0 and line.count('configured'):
                syms.add(line.split()[3].strip())
        return syms

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Window( file_name=sys.argv[1], log_file = sys.argv[2] )
    window.show()
    sys.exit(app.exec_())
