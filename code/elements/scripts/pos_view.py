import os
import sys
from PyQt4 import QtCore, QtGui
import numpy as np
import tempfile
import subprocess

record_template = [
    'Account',
    'Symbol',
    'SecurityID',
    'GMV',
    'SymbolMV',
    'TotalCapital',
    'OO',
    'SymbolLongOO',
    'SymbolShortOO',
    'bid',
    'offer',
    'last',
    'lastUpdateTime',
    'SymbolPosition']


class Window(QtGui.QWidget):

    def __init__(self, file_name='/logs_throne/Greyjoy/Limits_Greyjoy.log', search = 'CurrPosInfo', strategy='POSSERV', max_symbols=10000):
        self.file_name = file_name
        self.f = open(self.file_name)
        self.f.seek(0,1)
        self.search_string = search
        self.max_symbols = max_symbols

        QtGui.QWidget.__init__(self)
        
        self.setStyleSheet("border: 0px");
 
        self.table = QtGui.QTableWidget()

        self.table.setColumnCount(len(record_template))
        self.table.setRowCount(0)
        self.table.setHorizontalHeaderLabels( record_template)
        self.table.setShowGrid(False)
        self.table.verticalHeader().setVisible(False)
        self.table.horizontalHeader().setDefaultSectionSize(120)
        self.table.verticalHeader().setDefaultSectionSize(16)

        self.resize(1400,200) 

        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.table)
        self.setLayout(layout)

        self.offset_file = tempfile.mktemp()
        
        self.timer = QtCore.QTimer()        
        self.timer.timeout.connect(self.on_timer)
        self.timer.start(100)
        
        self.symbols = {}
        
        self.setWindowTitle(strategy)

    def on_timer(self):
        for i in xrange(self.max_symbols):
            where = self.f.tell()
            line = self.f.readline()
            if not line:
                self.f.seek(where)
                break
            else:
                self.parse_line(line)

    def parse_line(self, line):
        if line.count(self.search_string) == 0:
            return
        record = {}

        ll = line.split('[')[1].split(']')[0].split(':')[1]
        toks = ll.split(',')
        
        for tok in toks:
            t = tok.split('=')
            if len(t) != 2:
                continue

            k = t[0].strip() 
            v = t[1].strip()
            record[k] = v

        if record['last'] != '0':       
            self.update_table(record)    

    def get_symbol_index(self, s):
        if not  self.symbols.has_key(s):
            self.symbols[s] = self.table.rowCount()
            self.table.setRowCount(self.table.rowCount() + 1)
        return self.symbols[s]

    def update_table(self, record):
        if not record.has_key('Symbol'):
            return 

        j =  self.get_symbol_index(record['Symbol'])
        i = 0
        for e in record_template:
            if record.has_key(e):
                if e == 'Symbol':
                    item = QtGui.QTableWidgetItem(record[e]) 
                    self.table.setItem(j,i,item)
                    item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                else:
                    item = QtGui.QTableWidgetItem(record[e]) 
                    self.table.setItem(j,i,item)
            i += 1


if __name__ == "__main__":

    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
