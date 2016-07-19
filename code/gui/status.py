import os
import sys
from PyQt4 import QtCore, QtGui
import numpy as np
import tempfile
import subprocess

record_template = [
    'sym',
    'pnl',
    'bid',
    'ask',
    'bsz',
    'asz',
    'pos',
    'vol',
    'mp',
    'time'
]

class Window(QtGui.QWidget):

    def __init__(self, file_name='status_test', search = 'TEST', strategy='NONE', max_symbols=25):
        self.file_name = file_name
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

        self.resize(800,200) 

        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.table)
        self.setLayout(layout)

        self.offset_file = tempfile.mktemp()
        
        self.timer = QtCore.QTimer()        
        self.timer.timeout.connect(self.on_timer)
        self.timer.start(200)
        
        self.symbols = {}
        
        self.setWindowTitle(strategy)

    def on_timer(self):
        tail_command = 'tail -%s %s' % (self.max_symbols,self.file_name)
        temp = subprocess.Popen(tail_command.split(), stdout = subprocess.PIPE)
        grep_command = 'grep %s' % self.search_string
        self.grep = subprocess.Popen(grep_command.split(), stdin=temp.stdout, stdout=subprocess.PIPE)
       
        lines = self.grep.stdout.readlines()
        for line in lines:
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
                elif e =='pos':
                    if float(record[e]) >= 0:
                        item = QtGui.QTableWidgetItem('%d' % int(record[e])) 
                        self.table.setItem(j,i,item )
                        item.setForeground(QtGui.QColor('Blue'))
                    else:
                        item = QtGui.QTableWidgetItem('(%d)' % -int(record[e])) 
                        self.table.setItem(j,i,item)
                        item.setForeground(QtGui.QColor('Red'))
                        item.setFont(QtGui.QFont("", 9, QtGui.QFont.Bold))
                elif e =='pnl':
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
                
                item.setTextAlignment(QtCore.Qt.AlignCenter)        
            i += 1


if __name__ == "__main__":

    app = QtGui.QApplication(sys.argv)
    window = Window( file_name=sys.argv[1], search=sys.argv[2], strategy=sys.argv[3], max_symbols=1000 )
    window.show()
    sys.exit(app.exec_())
