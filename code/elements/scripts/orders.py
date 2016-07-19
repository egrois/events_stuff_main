import os
import sys
from PyQt4 import QtCore, QtGui, QtSql
import numpy as np
import tempfile
import subprocess

columns = [
    'sym',
    'type',
    'action',
    'order_id',
    'sec_id',
    'side',
    'price',
    'size',
    'tif',
    'last_qty',
    'leaves_qty',
    'last_px',
    'open'
]

numbers = set([
    'order_id',
    'sec_id',
    'price',
    'size',
    'last_qty',
    'leaves_qty',
    'last_px',
    'open'
])

class Window(QtGui.QWidget):

    def __init__(self, file_name='status_test', search = 'TEST', strategy='NONE', max_symbols=25):
        self.file_name = file_name
        self.f = open(self.file_name)
        self.search_string = search
        self.max_symbols = max_symbols

        QtGui.QWidget.__init__(self)
        
        self.setStyleSheet("border: 0px");

        self.db = QtSql.QSqlDatabase.addDatabase("QSQLITE");
        self.db.setDatabaseName(":memory:");
        self.db.open()       
 
        query = QtSql.QSqlQuery("CREATE TABLE orders ( \
                id integer primary key,\
                time varchar(20), \
                sym varchar(20), \
                type varchar(20), \
                action varchar(20), \
                order_id integer, \
                sec_id integer, \
                side varchar(20), \
                price double, \
                size integer, \
                tif varchar(20), \
                last_qty integer, \
                leaves_qty integer, \
                last_px double, \
                open integer)")

        self.model =  QtSql.QSqlTableModel(None, self.db)
        self.model.setTable('orders')
        self.model.select()
        self.model.setEditStrategy(QtSql.QSqlTableModel.OnRowChange);

        font = QtGui.QFont()
        font.setPointSize(8);
        font.setFamily("Arial")
        font.setBold(True)

        self.table = QtGui.QTableView()
        self.table.setModel(self.model)       
        self.table.setSortingEnabled(True)
        self.table.setAlternatingRowColors(True)
        self.table.resizeColumnsToContents()
        self.table.setColumnHidden(0, True)
        self.table.verticalHeader().setDefaultSectionSize(20)
        self.table.setFont(font)

        self.resize(800,200) 

        self.line_edit = QtGui.QLineEdit("FILTER")
        self.line_edit.returnPressed.connect(self.on_filter)
        layout = QtGui.QGridLayout()
        layout.addWidget(self.line_edit, 0,0)
        layout.addWidget(self.table,     1,0)
        self.setLayout(layout)
        
        self.timer = QtCore.QTimer()        
        self.timer.timeout.connect(self.on_timer)
        self.timer.start(100)
        
        self.setWindowTitle(strategy)
    
    def on_filter(self):
        try:
            self.model.setFilter(self.line_edit.text())
            print self.line_edit.text()
            self.model.select()
        except:
            pass

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
            self.model.select()

    def parse_line(self, line):
        if line.count(self.search_string) == 0:
            return
        record = {}
        toks = line.split()
        for tok in toks:
            t = tok.split('=')
            if len(t) != 2:
                continue
            if t[0] in numbers:
                record[t[0]] = t[1]
            else:
                record[t[0]] = '\'' + t[1] + '\''
        record['time'] = '\'' + toks[0] + '\'' 
        
        order_id = int(record['order_id'])
        qstr = 'SELECT * FROM orders WHERE order_id=%s and open=0' %  order_id
        query = QtSql.QSqlQuery(qstr)
        
        if query.size() > 0:
            record['open'] = '0' 
        else:
            record['open'] = '1'        

        qstr = "INSERT INTO orders (%s) VALUES (%s)" % (','.join(record.keys()), ','.join(record.values()))
        query = QtSql.QSqlQuery(qstr)
 
        if ((record['type'].count('FILL') > 0 and int(record['leaves_qty']) == 0) 
                or (record['type'].count('CANCEL_ACK') > 0) 
                or (record['type'].count('EXPIRED') > 0)):
            order_id = int(record['order_id'])
            if order_id >= 0:
                qstr = 'UPDATE orders SET open=0 WHERE order_id=%s' % order_id
                query = QtSql.QSqlQuery(qstr)  

    def get_symbol_index(self, s):
        if not  self.symbols.has_key(s):
            self.symbols[s] = self.table.rowCount()
            self.table.setRowCount(self.table.rowCount() + 1)
        return self.symbols[s]

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Window( file_name=sys.argv[1], search=sys.argv[2], strategy=sys.argv[3], max_symbols=10000)
    window.show()
    sys.exit(app.exec_())
