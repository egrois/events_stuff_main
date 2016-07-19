import sys
from PyQt4 import QtCore, QtGui
from google.protobuf import text_format
import params_pb2


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


class Window(QtGui.QWidget):

    def __init__(self, file_name = 'strategy.param'):

        QtGui.QWidget.__init__(self)

        self.file_name = file_name
        
        try:
            self.ignores = load_field_mask()
        except:
            self.ignores = set([])

        self.treeWidget = QtGui.QTreeWidget()
        self.treeWidget.setHeaderHidden(False)
        self.treeWidget.setColumnCount(2)

 
        self.item_map = {}
        self.load()

        self.treeWidget.itemChanged.connect (self.handleChanged) 
        
        self.apply_button = QtGui.QPushButton("APPLY")
        self.apply_button.clicked.connect(self.handleApplyChanges)
        
        self.undo_button = QtGui.QPushButton("UNDO")
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

    def load(self):
        self.treeWidget.clear()
        self.edited = load_params(self.file_name)

        root_s = QtGui.QTreeWidgetItem(self.treeWidget.invisibleRootItem(), [self.edited.strategy.name])
        root_s.setChildIndicatorPolicy(QtGui.QTreeWidgetItem.ShowIndicator)
        root_s.setExpanded(True)
        root_s.setFont(0, QtGui.QFont("", 9, QtGui.QFont.Bold))

        for f in self.edited.strategy.ListFields():
            if f[0].name == 'instrument':
                continue
            self.add_item(root_s, f, self.edited.strategy) 
        
        for f in self.edited.strategy.ListFields():
            if f[0].name != 'instrument':
                continue
            
            for ii in f[1]:
                root_s = QtGui.QTreeWidgetItem(self.treeWidget.invisibleRootItem(), [ii.symbol])
                root_s.setChildIndicatorPolicy(QtGui.QTreeWidgetItem.ShowIndicator)
                root_s.setExpanded(True)
                root_s.setFont(0, QtGui.QFont("", 9, QtGui.QFont.Bold))
                for f in ii.ListFields():
                    self.add_item(root_s, f, ii) 

        self.setWindowTitle(self.edited.strategy.name)

    def add_item(self, parent, source, source_msg):
        if source[0].name in self.ignores:
            return

        item = QtGui.QTreeWidgetItem(parent)
        item.setData(0, QtCore.Qt.DisplayRole, source[0].name)
        if source[0].type == source[0].TYPE_BOOL:
            item.setCheckState (1, (QtCore.Qt.Checked if source[1] else QtCore.Qt.Unchecked) ) 
        else:
            item.setData(1, QtCore.Qt.EditRole, source[1])
        item.setFlags(item.flags() | QtCore.Qt.ItemIsEditable) 
        self.item_map[item] = (source, source_msg)
 
    def handleChanged(self, item, column):
        entry = self.item_map[item][0] 
        owner =  self.item_map[item][1] 
    
        if entry[0].type == entry[0].TYPE_BOOL:
            owner.__setattr__(entry[0].name, bool(item.data(column,QtCore.Qt.DisplayRole).toBool()))

        elif entry[0].type == entry[0].TYPE_INT32:
            owner.__setattr__(entry[0].name, item.data(column,QtCore.Qt.DisplayRole).toInt()[0]) 

        elif entry[0].type == entry[0].TYPE_DOUBLE:
            owner.__setattr__(entry[0].name, item.data(column,QtCore.Qt.DisplayRole).toDouble()[0]) 

        else:
            owner.__setattr__(entry[0].name, str(item.data(column,QtCore.Qt.DisplayRole).toString())) 

        item.setBackground(0, self.edited_brush)   
        self.edited_items.append(item)

        if self.mode_box.checkState() == QtCore.Qt.Checked:
            self.handleApplyChanges()

    def handleApplyChanges(self):
        self.treeWidget.itemChanged.disconnect (self.handleChanged) 
        save_params(self.file_name, self.edited) 
        for ii in self.edited_items:
            ii.setBackground(0, self.unedited_brush)   
        self.edited_items = []
        self.treeWidget.itemChanged.connect(self.handleChanged) 
 
    def handleUndoChanges(self):
        self.treeWidget.itemChanged.disconnect (self.handleChanged) 
        for ii in self.edited_items:
            ii.setBackground(0, self.unedited_brush)   
        self.edited_items = []
        self.load()
        self.treeWidget.itemChanged.connect(self.handleChanged) 
        


if __name__ == "__main__":

    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
