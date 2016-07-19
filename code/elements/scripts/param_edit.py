import sys
from PyQt4 import QtCore, QtGui
from PyQt4.Qt import *
from google.protobuf import text_format
import params_pb2
import os
from datetime import *


def load_params(file_name):
    f = open(file_name, 'r')
    lines = f.readlines()
    msg = params_pb2.Params()
    msg = text_format.Merge(" ".join(lines),msg )
    f.close()
    return msg

def save_params(file_name,msg):
    f = open(file_name, 'w')
    s = text_format.MessageToString(msg)
    f.write(s)
    f.close()

def load_field_mask():
    ignore = set([])
    f = open('field_mask')
    lines = f.readlines()
    for line in lines:
        name = line.strip().split(' ')[0]
        if name is not None:
            ignore.add(name)
    return ignore

def lastModifiedTime(file_name):
    lmt = os.path.getmtime(file_name)
    return datetime.fromtimestamp(lmt)

class dialog(QtGui.QDialog):
    
    def __init__(self, parent=None):
        QtGui.QDialog.__init__(self)
        label =  QtGui.QLabel('RELOAD FIRST!')
        font = label.font();
        font.setPointSize(20);
        font.setBold(True);
        label.setFont(font);      
        layout = QtGui.QGridLayout()
        layout.addWidget(label, 0, 0)   
        self.setLayout(layout)

class Window(QtGui.QWidget):

    def __init__(self, file_name = 'click_trader.param'):

        QtGui.QWidget.__init__(self)

        self.file_name = file_name
        self.exists = False
        self.last_modified = None
        self.item_opened = {}       
 
        try:
            self.ignores = load_field_mask()
        except:
            self.ignores = set([])

        self.treeWidget = QtGui.QTreeWidget()
        self.treeWidget.setHeaderHidden(False)
        self.treeWidget.setColumnCount(2)

 
        self.item_map = {}
        self.load()
        self.save_item_opened() 

        self.treeWidget.itemChanged.connect (self.handleChanged) 
        
        self.apply_button = QtGui.QPushButton("APPLY")
        self.apply_button.clicked.connect(self.handleApplyChanges)
        
        self.undo_button = QtGui.QPushButton("RELOAD")
        self.undo_button.clicked.connect(self.handleUndoChanges)
       
        self.mode_box = QtGui.QCheckBox('SINGLE CLICK')

       # layout = QtGui.QVBoxLayout()
        layout = QtGui.QGridLayout()
        layout.addWidget(self.apply_button, 0,0)
        layout.addWidget(self.undo_button, 0, 1)
        layout.addWidget(self.mode_box, 0, 2)
        layout.addWidget(self.treeWidget, 1, 0,3, 3)
        self.setLayout(layout)

        self.edited_items = []
        self.edited_brush = QtGui.QBrush(QtGui.QColor(174,198,207)) 
        self.unedited_brush = QtGui.QBrush(QtGui.QColor(254,254,254))

        self.resize(450,800)
        self.treeWidget.setColumnWidth(0, 200)     
        self.treeWidget.setColumnWidth(1, 200)     


    def is_object(self, a):
        if str(a).count('object') > 0:
            return True
        else:
            return False

    def add_item2(self, parent, source, source_msg):
        for field in source.ListFields():
            if self.is_object(field[1]):
                for obj in field[1]:
                    
                    if field[0].name == 'instrument':
                        root_s = QTreeWidgetItem(parent, [obj.symbol])
                        root_s.idx = self.index
                        if self.item_opened.has_key(root_s.idx) and self.item_opened[root_s.idx] == True: 
                            root_s.setExpanded(True)
                        else:
                            root_s.setExpanded(False)
                        self.index += 1        
                    else:
                        root_s = QTreeWidgetItem(parent, [field[0].name])
                        root_s.idx = self.index
                        if self.item_opened.has_key(root_s.idx) and self.item_opened[root_s.idx] == True: 
                            root_s.setExpanded(True)
                        else:
                            root_s.setExpanded(False)
                        self.index += 1        
                    root_s.setChildIndicatorPolicy(QtGui.QTreeWidgetItem.ShowIndicator)
                    root_s.setFont(0, QtGui.QFont("", 9, QtGui.QFont.Bold))
                    self.add_item2(root_s, obj, source)
            elif field[0].name == 'bid_config' or  field[0].name == 'ask_config' or  field[0].name == "user_strategy" or field[0].name == "user_instrument" or field[0].name == "auto_stacker_config":
                    root_s = QTreeWidgetItem(parent, [field[0].name])
                    root_s.idx = self.index
                    if self.item_opened.has_key(root_s.idx) and self.item_opened[root_s.idx] == True: 
                        root_s.setExpanded(True)
                    else:
                        root_s.setExpanded(False)
                    self.index += 1        
                    root_s.setChildIndicatorPolicy(QtGui.QTreeWidgetItem.ShowIndicator)
                    root_s.setFont(0, QtGui.QFont("", 9, QtGui.QFont.Bold))
                    self.add_item2(root_s, field[1], source)
            else:
                self.add_item(parent, field, source)  

    def load(self):
        self.treeWidget.clear()

        self.index = 0

        self.edited = load_params(self.file_name)
        self.last_modified = lastModifiedTime(self.file_name)
        
        root_s = QTreeWidgetItem(self.treeWidget.invisibleRootItem(), [self.edited.strategy.name])
        root_s.idx = self.index
        if self.item_opened.has_key(root_s.idx) and self.item_opened[root_s.idx] == True: 
            root_s.setExpanded(True)
        else:
            root_s.setExpanded(False)
        self.index += 1        

        root_s.setChildIndicatorPolicy(QtGui.QTreeWidgetItem.ShowIndicator)
        root_s.setFont(0, QtGui.QFont("", 9, QtGui.QFont.Bold))
       
        self.root = root_s
 
        self.add_item2(root_s, self.edited.strategy, None)
        self.setWindowTitle(self.edited.strategy.name)
        self.exists = True

    def add_item(self, parent, source, source_msg):
        if source[0].name in self.ignores:
            return

        item = QtGui.QTreeWidgetItem(parent)
        item.idx = self.index
        if self.item_opened.has_key(item.idx) and self.item_opened[item.idx] == True: 
            item.setExpanded(True)
        else:
            item.setExpanded(False)
        self.index += 1        
        item.setData(0, QtCore.Qt.DisplayRole, source[0].name)
        if source[0].type == source[0].TYPE_BOOL:
            item.setCheckState (1, (QtCore.Qt.Checked if source[1] else QtCore.Qt.Unchecked) ) 
        elif source[0].type == source[0].TYPE_DOUBLE:
            item.setData(1, QtCore.Qt.EditRole, str(source[1]))
        else:
            item.setData(1, QtCore.Qt.EditRole, source[1])
        item.setFlags(item.flags() | QtCore.Qt.ItemIsEditable) 
        self.item_map[item] = (source, source_msg)
 
    def handleChanged(self, item, column):
        entry = self.item_map[item][0] 
        owner =  self.item_map[item][1] 
        if entry[0].type == entry[0].TYPE_BOOL:
            if item.checkState(column) == QtCore.Qt.Checked:
                owner.__setattr__(entry[0].name, True)
            else:
                owner.__setattr__(entry[0].name, False)
        elif entry[0].type == entry[0].TYPE_INT32:
            owner.__setattr__(entry[0].name, int(item.text(column))) 
        elif entry[0].type == entry[0].TYPE_DOUBLE:
            owner.__setattr__(entry[0].name, float(item.text(column))) 
        else:
            owner.__setattr__(entry[0].name, str(item.text(column))) 

        self.treeWidget.itemChanged.disconnect(self.handleChanged) 
        item.setBackground(0, self.edited_brush)   
        self.treeWidget.itemChanged.connect(self.handleChanged) 
        
        self.edited_items.append(item)

        if self.mode_box.checkState() == QtCore.Qt.Checked:
            self.handleApplyChanges()

    def handleApplyChanges(self):
        if lastModifiedTime(self.file_name) != self.last_modified: 
            self.w = dialog()
            self.w.exec_()
            return

        self.treeWidget.itemChanged.disconnect(self.handleChanged) 
        save_params(self.file_name, self.edited) 
        self.last_modified = lastModifiedTime(self.file_name)
        for ii in self.edited_items:
            ii.setBackground(0, self.unedited_brush)   
        self.edited_items = []
        self.treeWidget.itemChanged.connect(self.handleChanged) 
 
    def handleUndoChanges(self):
        self.save_item_opened() 
        self.treeWidget.itemChanged.disconnect (self.handleChanged) 
        for ii in self.edited_items:
            ii.setBackground(0, self.unedited_brush)   
        self.edited_items = []
        self.load()
        self.treeWidget.itemChanged.connect(self.handleChanged) 
       
    def lastModifiedTime(file_name):
        lmt = os.path.getmtime(file_name)
        return datetime.fromtimestamp(lmt)

    def save_item_opened(self):
        self.item_opened = {}

        if self.root  is None:
            return

        self.enum_item_opened(self.root)

    def enum_item_opened(self, item):
        self.item_opened[item.idx] = item.isExpanded()
        for i in xrange(item.childCount()):
            self.enum_item_opened(item.child(i))

if __name__ == "__main__":

    app = QtGui.QApplication(sys.argv)
    window = Window( sys.argv[1] )
    window.show()
    sys.exit(app.exec_())
