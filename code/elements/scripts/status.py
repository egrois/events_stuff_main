import os
import sys
from PyQt4 import QtCore, QtGui
import numpy as np
import tempfile
import subprocess

record_template = [
    'sym',
    'pnl',
    'pos',
    'vol',
    'pnl2',
    'pos2',
    'vol2'
#    'bid',
#    'ask',
#    'bsz',
#    'asz',
#    'mp',
#    'time'
]

class Window(QtGui.QWidget):

    def __init__(self, file_name='status_test', search = 'TEST', strategy='NONE', max_symbols=25):
        self.file_name = file_name
        self.f = open(self.file_name)
        self.f.seek(0,2)
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
        self.table.horizontalHeader().setDefaultSectionSize(80)
        self.table.verticalHeader().setDefaultSectionSize(16)

        self.resize(600,200) 

        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.table)
        self.setLayout(layout)

        self.offset_file = tempfile.mktemp()
        
        self.timer = QtCore.QTimer()        
        self.timer.timeout.connect(self.on_timer)
        self.timer.start(100)
        
        self.symbols = {}
        self.symbol_pnls = {}
        self.symbol_pos = {}
        self.symbol_vol = {}
        self.symbol_offsets = {}
        self.symbol_offsets_pos = {}
        self.symbol_offsets_vol = {}
        
        self.setWindowTitle(strategy)

    def keyPressEvent(self, e):

        if QtGui.QApplication.keyboardModifiers() != QtCore.Qt.ControlModifier:
            return
        if e.key() == QtCore.Qt.Key_Q:
            for sym in self.symbols:
                if self.symbol_pnls.has_key(sym): 
                    self.symbol_offsets[sym] = self.symbol_pnls[sym]
                    self.symbol_offsets_pos[sym] = self.symbol_pos[sym]
                    self.symbol_offsets_vol[sym] = self.symbol_vol[sym]

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
        toks = line.split()
        
        for tok in toks:
            t = tok.split('=')
            if len(t) != 2:
                continue
            record[t[0]] = t[1]
        record['time'] = toks[0] 
        
    
        if self.symbol_offsets.has_key(record['sym']):
            record['pnl2'] = str(float(record['pnl']) - self.symbol_offsets[record['sym']])
            record['pos2'] = str(int(float(record['pos'])) - self.symbol_offsets_pos[record['sym']])
            record['vol2'] = str(int(float(record['vol'])) - int(self.symbol_offsets_vol[record['sym']]))
        else:
            record['pnl2'] = record['pnl']
            record['pos2'] = record['pos']
            record['vol2'] = record['vol']
           
        self.symbol_pnls[record['sym']] = float(record['pnl'])
        self.symbol_pos[record['sym']] = float(record['pos'])
        self.symbol_vol[record['sym']] = float(record['vol'])
        
        self.update_table(record)    

    def get_symbol_index(self, s):
        if not  self.symbols.has_key(s):
            self.symbols[s] = self.table.rowCount()
            self.table.setRowCount(self.table.rowCount() + 1)
        return self.symbols[s]

    def update_table(self, record):
        if not record.has_key('sym'):
            return 
        j =  self.get_symbol_index(record['sym'])
        i = 0
        for e in record_template:
            if record.has_key(e):
                if e == 'sym':
                    item = QtGui.QTableWidgetItem(record[e]) 
                    self.table.setItem(j,i,item)
                    item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                elif e =='pos' or e == 'pos2':
                    if float(record[e]) >= 0:
                        item = QtGui.QTableWidgetItem('%d' % int(float(record[e]))) 
                        self.table.setItem(j,i,item )
                        item.setForeground(QtGui.QColor('Blue'))
                    else:
                        item = QtGui.QTableWidgetItem('(%d)' % -int(float(record[e]))) 
                        self.table.setItem(j,i,item)
                        item.setForeground(QtGui.QColor('Red'))
                        item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                    item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                elif e =='pnl' or e =='pnl2':
                    item = QtGui.QTableWidgetItem(record[e]) 
                    self.table.setItem(j,i,item)
                    if float(record[e]) >= 0:
                        item.setForeground(QtGui.QColor('Blue'))
                    else:
                        item.setForeground(QtGui.QColor('Red'))
                    item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                else:
                    item = QtGui.QTableWidgetItem(record[e]) 
                    self.table.setItem(j,i,item)
                    item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
            i += 1


if __name__ == "__main__":

    app = QtGui.QApplication(sys.argv)
    window = Window( file_name=sys.argv[1], search=sys.argv[2], strategy=sys.argv[3], max_symbols=10000)
    window.show()
    sys.exit(app.exec_())
