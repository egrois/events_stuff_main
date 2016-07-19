import os
import sys
from PyQt4 import QtCore, QtGui, QtSql, Qt
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import numpy as np
import tempfile
import subprocess
import pandas as pd
from datetime import datetime,time, timedelta


column_colors = {   
                    0 : QColor(Qt.white),
                    1 : QColor.fromRgb(100,100,255),
                    2 : QColor(Qt.white),
                    3 : QColor.fromRgb(255,100,100),
                    4 : QColor(Qt.white)}


class TableModel(QtCore.QAbstractTableModel): 
    def __init__(self, parent=None ): 
        super(TableModel, self).__init__()
        self.datatable = None
        self.header_labels = ['','bid','price','ask','']  
        self.highlight_index = None

    def update(self, dataIn):
        self.datatable = dataIn

    def rowCount(self, parent=QtCore.QModelIndex()):
        return self.datatable.shape[0]

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self.datatable.shape[1]

    def data(self, index, role=QtCore.Qt.DisplayRole):
        i = index.row()
        j = index.column()
        if role == QtCore.Qt.DisplayRole:
            value = self.datatable[i][j]
            if value != 0:
                return '{0:.10g}'.format(value)
            else:
                return ''
        elif role == Qt.BackgroundColorRole:
            bgColor = column_colors[j]
            if (self.highlight_index is not None):
                if index.row() == self.highlight_index.row() and index.column() == self.highlight_index.column():
                    bgColor = QColor.fromRgb(150,150,150) 
            return QVariant(QColor(bgColor))
        elif role == Qt.TextAlignmentRole:
            return QVariant(Qt.AlignCenter)
        elif role == Qt.FontRole:
            font = QFont();
            font.setBold(True)
            return QVariant(font)
        else:
            return QtCore.QVariant()

    def flags(self, index):
        return QtCore.Qt.ItemIsEnabled

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role == Qt.DisplayRole and orientation == Qt.Horizontal:
            return self.header_labels[section]
        return QAbstractTableModel.headerData(self, section, orientation, role)

class Window(QtGui.QWidget):

    def __init__(self, file_name='status_test', strategy='NONE', symbol=''):
        self.file_name = file_name
        self.f = open(self.file_name)
        self.f.seek(0,2)
        self.symbol = symbol
        self.mpi = float(0.0)
        self.ref_price = 0.0 
        self.NUM = 1601
        self.last_order_update = '00:00:00.000000'
        self.orders = []
        self.strategy = strategy

        QtGui.QWidget.__init__(self)
       
        #self.setStyleSheet("border: 3px solid red");
        
        self.model = TableModel(None)
        self.model.datatable = np.zeros((self.NUM, 5))

        self.table = QtGui.QTableView()
        self.table.setModel(self.model)       
        self.table.setAlternatingRowColors(True)
        self.table.resizeColumnsToContents()
        self.table.setColumnWidth(0,30)
        self.table.setColumnWidth(1,60)
        self.table.setColumnWidth(2,90)
        self.table.setColumnWidth(3,60)
        self.table.setColumnWidth(4,30)
        self.table.verticalHeader().setDefaultSectionSize(20)
        self.table.verticalHeader().hide()

        self.button0 = QtGui.QPushButton('0')
        self.button0.clicked.connect(self.handleButton0)
        self.button0.setMaximumWidth(30)
        
        self.button1 = QtGui.QPushButton('1')
        self.button1.clicked.connect(self.handleButton1)
        self.button1.setMaximumWidth(30)
        
        self.button5 = QtGui.QPushButton('5')
        self.button5.clicked.connect(self.handleButton5)
        self.button5.setMaximumWidth(30)
        
        self.button10 = QtGui.QPushButton('10')
        self.button10.clicked.connect(self.handleButton10)
        self.button10.setMaximumWidth(30)
        
        self.button20 = QtGui.QPushButton('20')
        self.button20.clicked.connect(self.handleButton20)
        self.button20.setMaximumWidth(30)
        
        self.button50 = QtGui.QPushButton('50')
        self.button50.clicked.connect(self.handleButton50)
        self.button50.setMaximumWidth(30)
        
        self.button100 = QtGui.QPushButton('100')
        self.button100.clicked.connect(self.handleButton100)
        self.button100.setMaximumWidth(30)
        
        self.buttonX = QtGui.QPushButton('xAll')
        self.buttonX.clicked.connect(self.handleButtonX)
        self.buttonX.setMaximumWidth(30)

        self.size_label = QtGui.QLabel("0")
        self.size_label.setMaximumHeight(20)       
        font = self.size_label.font()
        font.setPointSize(8)
        font.setBold(True)
        self.size_label.setFont(font)
        
        self.strategy_label = QtGui.QLabel(self.strategy)
        self.strategy_label.setMaximumHeight(20)       
        font = self.strategy_label.font()
        font.setPointSize(8)
        font.setBold(True)
        self.strategy_label.setFont(font)
        
        self.symbol_label = QtGui.QLabel(self.symbol)
        self.symbol_label.setMaximumHeight(20)       
        font = self.symbol_label.font()
        font.setPointSize(8)
        font.setBold(True)
        self.symbol_label.setFont(font)
        
        self.pos_label = QtGui.QLabel('0')
        self.pos_label.setMaximumHeight(20)       
        self.pos_label.setFont(font)
 
        self.resize(320,800)
        
        layout = QtGui.QGridLayout()
        
        layout.addWidget(self.size_label, 0, 3)
        layout.addWidget(self.strategy_label, 0, 5)
        layout.addWidget(self.symbol_label, 0, 7)
        layout.addWidget(self.pos_label, 0, 9)

        layout.addWidget(self.button0,1,3)
        layout.addWidget(self.button1,1,4)
        layout.addWidget(self.button5,1,5)
        layout.addWidget(self.button10,1,6)
        layout.addWidget(self.button20,1,7)
        layout.addWidget(self.button50,1,8)
        layout.addWidget(self.button100,1,9)
        layout.addWidget(self.buttonX,1,10)
        layout.addWidget(self.table,2,0,13,13)
        
        self.setLayout(layout)
        
        self.timer = QtCore.QTimer()        
        self.timer.timeout.connect(self.on_timer)
        self.timer.start(100)
    
        self.table.clicked.connect(self.on_click)

    def handleButton0(self):
        self.SIZE = 0 
        self.size_label.setText('0')

    def handleButton1(self):
        self.SIZE = 1 
        self.size_label.setText('1')

    def handleButton5(self):
        self.SIZE = 5
        self.size_label.setText('5')

    def handleButton10(self):
        self.SIZE = 10
        self.size_label.setText('10')

    def handleButton20(self):
        self.SIZE = 20
        self.size_label.setText('20')

    def handleButton50(self):
        self.SIZE = 50
        self.size_label.setText('50')

    def handleButton100(self):
        self.SIZE = 100 
        self.size_label.setText('100')
    
    def handleButtonX(self):
        for order in self.orders:
            self.send_order(self.symbol, 'CANCEL','BUY', 0, 1, order['order_id']) 

    def on_click(self, index):

        i = index.row()
        j = index.column()

        if i <=0 or i > self.NUM:
            return

        price = self.ref_price + float(self.NUM - i) * self.mpi
        
        if j == 0:
            for order in self.orders:
                if order['side'].count('BUY') > 0 and  abs(order['price'] - price) <= 1e-8:
                    self.send_order(self.symbol,'CANCEL','BUY', price, 0, order['order_id']) 
        elif j == 1:
            if self.SIZE > 0:
                self.send_order(self.symbol,'NEW','BUY', price, self.SIZE, -1) 
        elif j == 3:
            if self.SIZE > 0:
                self.send_order(self.symbol,'NEW','SELL', price, self.SIZE, -1) 
        elif j == 4:
            for order in self.orders:
                if order['side'].count('SELL') > 0 and  abs(order['price'] - price) <= 1e-8:
                    self.send_order(self.symbol,'CANCEL','SELL', price, 0, order['order_id']) 

    def send_order(self, symbol, action, side, price, size, order_id):
        str_to_action = {'NEW':0,'MODIFY':2,'CANCEL':1}
        str_to_side = {'BUY':1,'SELL':0}
        str_to_tif = {'DAY':0, 'IOC':1}        
                
        oem_id = str(datetime.now())
        oem_action = str_to_action[action]  
        price = int(price * 1e7)
        is_ioc = 0  
        is_bid = str_to_side[side]  

        ss =  "%s,%s,%s,%s,%s,%s,%s,%s,\n" % (oem_id, oem_action, symbol, price, size, order_id, is_ioc, is_bid)
        f = open('order_entry', 'a')
        f.write(ss)
        f.close()

    def on_timer(self):
        ii = 0
        while True:
            where = self.f.tell()
            line = self.f.readline()
            if not line:
                self.f.seek(where)
                break
            else:
                self.parse_line(line)
                ii += 1
       
        if ii > 0: 
            self.update_orders()
            self.model.update(self.model.datatable)
            self.model.dataChanged.emit(self.model.createIndex(0,0), self.model.createIndex(self.NUM, 5))

    def update_orders(self):

        for i in xrange(self.NUM):
            self.model.datatable[i][0] = 0.0
            self.model.datatable[i][4] = 0.0

        for order in self.orders:
            
            if order['side'].count('SELL') > 0:
                j = int((order['price'] - self.ref_price + 1.e-10) / self.mpi)
                j = self.NUM - j
                self.model.datatable[j][4] += order['size']
            else:
                j = int((order['price'] - self.ref_price + 1.e-10) / self.mpi)
                j = self.NUM - j
                self.model.datatable[j][0] += order['size']

    def parse_line(self, line):
        if line.count('sym=%s' % self.symbol) == 0:
            return
        
        now = datetime.now()
        then = datetime.combine(datetime.today(), datetime.time(datetime.strptime(self.last_order_update.split('.')[0], '%H:%M:%S')))
        diff = now - then       
        if  diff.total_seconds() > 1.5:
            self.orders = [] 
        
        if line.count('MSG_MD_BOOK') > 0:
            self.parse_md_line(line)
        elif line.count('MSG_ORDER_BOOK_ENTRY') > 0:
            self.parse_ob_line(line)

    def parse_ob_line(self,line):
        toks = line.split()
        tm = toks[0]
 
        if self.last_order_update.count(tm) == 0:
            self.orders = [] 
            self.last_order_update = tm
        
        order = {} 
        side = toks[4].split('=')[1] 
        price = float(toks[5].split('=')[1])
        size = int(toks[6].split('=')[1])
        order_id = int(toks[7].split('=')[1])

        order['order_id'] = order_id
        order['price'] = price
        order['side'] = side
        order['size'] = size
        self.orders.append(order)

    def parse_md_line(self, line): 
       
        top_ask = float(0.0)
        top_bid = float(0.0)
        mpi = float(0.0)
        bid = np.zeros(10)
        ask = np.zeros(10)
        pos = int(0)
        last_trade_price = float(0.0)

        toks = line.split()
        for tok in toks:
            tt = tok.split('=')
            if len(tt) < 2:
                continue

            if tt[0].count('mpi') > 0:
                mpi =float( tt[1]) 
            elif tt[0].count('top_bid_price') > 0:
                top_bid = float(tt[1]) 
            elif tt[0].count('top_ask_price') > 0:
                top_ask= float(tt[1])
            
            elif tt[0].count('b0') > 0:
                bid[0] = float(tt[1]) 
            elif tt[0].count('b1') > 0:
                bid[1] = float(tt[1]) 
            elif tt[0].count('b2') > 0:
                bid[2] = float(tt[1]) 
            elif tt[0].count('b3') > 0:
                bid[3] = float(tt[1]) 
            elif tt[0].count('b4') > 0:
                bid[4] = float(tt[1]) 
            elif tt[0].count('b5') > 0:
                bid[5] = float(tt[1]) 
            elif tt[0].count('b6') > 0:
                bid[6] = float(tt[1]) 
            elif tt[0].count('b7') > 0:
                bid[7] = float(tt[1]) 
            elif tt[0].count('b8') > 0:
                bid[8] = float(tt[1]) 
            elif tt[0].count('b9') > 0:
                bid[9] = float(tt[1]) 

            elif tt[0].count('a0') > 0:
                ask[0] = float(tt[1]) 
            elif tt[0].count('a1') > 0:
                ask[1] = float(tt[1]) 
            elif tt[0].count('a2') > 0:
                ask[2] = float(tt[1]) 
            elif tt[0].count('a3') > 0:
                ask[3] = float(tt[1]) 
            elif tt[0].count('a4') > 0:
                ask[4] = float(tt[1]) 
            elif tt[0].count('a5') > 0:
                ask[5] = float(tt[1]) 
            elif tt[0].count('a6') > 0:
                ask[6] = float(tt[1]) 
            elif tt[0].count('a7') > 0:
                ask[7] = float(tt[1]) 
            elif tt[0].count('a8') > 0:
                ask[8] = float(tt[1]) 
            elif tt[0].count('a9') > 0:
                ask[9] = float(tt[1]) 
            elif tt[0].count('pos') > 0:
                pos = int(tt[1]) 
            elif tt[0].count('last_trade_price') > 0:
                last_trade_price = float(tt[1]) 
             
        if self.mpi == float(0.0):
            self.mpi = mpi
            self.ref_price = top_bid - 800.0 * self.mpi
            self.model.datatable = np.zeros((self.NUM, 5))
            for i in xrange(self.NUM):
                self.model.datatable[i][2] = self.ref_price + mpi * (self.NUM - i)
            self.table.scrollTo(self.model.createIndex(820, 0))
 
        for i in xrange(self.NUM):
            self.model.datatable[i][1] = 0.0
            self.model.datatable[i][3] = 0.0

        for i in xrange(10):
            j = int((top_bid - self.ref_price + 1e-10) / self.mpi)
            self.model.datatable[self.NUM - (j - i)][1] = bid[i] 
            j = int((top_ask - self.ref_price + 1e-10) / self.mpi)
            self.model.datatable[self.NUM - (j + i)][3] = ask[i] 
        
        if pos != 0 and abs(last_trade_price) > 1.0-10:
            ii = float((last_trade_price -self.ref_price + 1e-10)/self.mpi)
            self.model.highlight_index = self.model.createIndex(self.NUM - ii + 1, 2)
        else:
            self.model.highlight_index = None

        self.pos_label.setText(str(pos))

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Window( file_name=sys.argv[1], strategy=sys.argv[2], symbol=sys.argv[3])
    window.setStyleSheet('QMainWindow{background-color: red; border: 20px solid blue;}') 
    
    window.show()
    sys.exit(app.exec_())
