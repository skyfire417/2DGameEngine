# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GameEditor.ui'
#
# Created by: PyQt5 UI code generator 5.12
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtGui import QPixmap
import socket
import json
import sys
import time

server_address = "/tmp/gamecastle-gamemaker"
#server_address = "../G"
config_address = "../Game/Config/config.json"


class Ui_GameUI(object):
    def setupUi(self, GameUI):
        GameUI.setObjectName("GameUI")
        GameUI.resize(422, 650)
        self.centralWidget = QtWidgets.QWidget(GameUI)
        self.centralWidget.setObjectName("centralWidget")
        self.line = QtWidgets.QFrame(self.centralWidget)
        self.line.setGeometry(QtCore.QRect(0, 610, 420, 20))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.row_spinBox = QtWidgets.QSpinBox(self.centralWidget)
        self.row_spinBox.setGeometry(QtCore.QRect(50, 150, 60, 24))
        self.row_spinBox.setMinimum(10)
        self.row_spinBox.setMaximum(23)
        self.row_spinBox.setProperty("value", 12)
        self.row_spinBox.setObjectName("row_spinBox")
        self.col_spinBox = QtWidgets.QSpinBox(self.centralWidget)
        self.col_spinBox.setGeometry(QtCore.QRect(180, 150, 60, 24))
        self.col_spinBox.setMinimum(40)
        self.col_spinBox.setMaximum(60)
        self.col_spinBox.setProperty("value", 100)
        self.col_spinBox.setObjectName("col_spinBox")
        self.howManyCol_label = QtWidgets.QLabel(self.centralWidget)
        self.howManyCol_label.setGeometry(QtCore.QRect(120, 150, 61, 24))
        self.howManyCol_label.setObjectName("howManyCol_label")
        self.howManyRows_label = QtWidgets.QLabel(self.centralWidget)
        self.howManyRows_label.setGeometry(QtCore.QRect(10, 150, 40, 24))
        self.howManyRows_label.setObjectName("howManyRows_label")
        self.loseCondition_spinBox = QtWidgets.QSpinBox(self.centralWidget)
        self.loseCondition_spinBox.setGeometry(QtCore.QRect(190, 10, 50, 24))
        self.loseCondition_spinBox.setMinimum(1)
        self.loseCondition_spinBox.setMaximum(100)
        self.loseCondition_spinBox.setProperty("value", 3)
        self.loseCondition_spinBox.setObjectName("loseCondition_spinBox")
        self.loseCondition_label = QtWidgets.QLabel(self.centralWidget)
        self.loseCondition_label.setGeometry(QtCore.QRect(140, 10, 40, 24))
        self.loseCondition_label.setObjectName("loseCondition_label")
        self.tile_0 = QtWidgets.QLabel(self.centralWidget)
        self.tile_0.setGeometry(QtCore.QRect(10, 190, 60, 60))
        self.tile_0.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_0.setObjectName("tile_0")
        self.labelTile = QtWidgets.QLabel(self.centralWidget)
        self.labelTile.setGeometry(QtCore.QRect(10, 125, 121, 16))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.labelTile.setFont(font)
        self.labelTile.setTextFormat(QtCore.Qt.AutoText)
        self.labelTile.setObjectName("labelTile")
        self.labelHero = QtWidgets.QLabel(self.centralWidget)
        self.labelHero.setGeometry(QtCore.QRect(10, 0, 40, 16))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.labelHero.setFont(font)
        self.labelHero.setTextFormat(QtCore.Qt.AutoText)
        self.labelHero.setObjectName("labelHero")
        self.hero_00 = QtWidgets.QLabel(self.centralWidget)
        self.hero_00.setGeometry(QtCore.QRect(35, 25, 60, 60))
        self.hero_00.setMinimumSize(QtCore.QSize(50, 50))
        self.hero_00.setObjectName("hero_00")
        self.line_3 = QtWidgets.QFrame(self.centralWidget)
        self.line_3.setGeometry(QtCore.QRect(0, 110, 420, 20))
        self.line_3.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.winLoseButton = QtWidgets.QPushButton(self.centralWidget)
        self.winLoseButton.setGeometry(QtCore.QRect(160, 90, 110, 32))
        self.winLoseButton.setAutoDefault(False)
        self.winLoseButton.setFlat(False)
        self.winLoseButton.setObjectName("winLoseButton")
        self.Resize = QtWidgets.QPushButton(self.centralWidget)
        self.Resize.setGeometry(QtCore.QRect(240, 147, 80, 32))
        self.Resize.setIconSize(QtCore.QSize(24, 24))
        self.Resize.setDefault(False)
        self.Resize.setObjectName("Resize")
        self.pushButton_2 = QtWidgets.QPushButton(self.centralWidget)
        self.pushButton_2.setGeometry(QtCore.QRect(10, 90, 110, 32))
        self.pushButton_2.setDefault(True)
        self.pushButton_2.setObjectName("pushButton_2")
        self.loseCondition_label_2 = QtWidgets.QLabel(self.centralWidget)
        self.loseCondition_label_2.setGeometry(QtCore.QRect(140, 50, 40, 24))
        self.loseCondition_label_2.setObjectName("loseCondition_label_2")
        self.lineEdit = QtWidgets.QLineEdit(self.centralWidget)
        self.lineEdit.setGeometry(QtCore.QRect(190, 50, 40, 24))
        self.lineEdit.setInputMask("")
        self.lineEdit.setText("1")
        self.lineEdit.setObjectName("lineEdit")
        self.lineEdit_2 = QtWidgets.QLineEdit(self.centralWidget)
        self.lineEdit_2.setGeometry(QtCore.QRect(240, 50, 40, 24))
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.lineEdit_2.setText("1")
        self.pushButton_3 = QtWidgets.QPushButton(self.centralWidget)
        self.pushButton_3.setGeometry(QtCore.QRect(320, 147, 80, 32))
        self.pushButton_3.setDefault(True)
        self.pushButton_3.setObjectName("pushButton_3")
        self.pushButton_3.setVisible(False)
        self.line_4 = QtWidgets.QFrame(self.centralWidget)
        self.line_4.setGeometry(QtCore.QRect(120, 0, 20, 120))
        self.line_4.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.tile_1 = QtWidgets.QLabel(self.centralWidget)
        self.tile_1.setGeometry(QtCore.QRect(80, 190, 60, 60))
        self.tile_1.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_1.setObjectName("tile_1")
        self.tile_1.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_2 = QtWidgets.QLabel(self.centralWidget)
        self.tile_2.setGeometry(QtCore.QRect(150, 190, 60, 60))
        self.tile_2.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_2.setObjectName("tile_2")
        self.tile_2.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_3 = QtWidgets.QLabel(self.centralWidget)
        self.tile_3.setGeometry(QtCore.QRect(220, 190, 60, 60))
        self.tile_3.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_3.setObjectName("tile_3")
        self.tile_3.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_4 = QtWidgets.QLabel(self.centralWidget)
        self.tile_4.setGeometry(QtCore.QRect(290, 190, 60, 60))
        self.tile_4.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_4.setObjectName("tile_4")
        self.tile_4.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_5 = QtWidgets.QLabel(self.centralWidget)
        self.tile_5.setGeometry(QtCore.QRect(360, 190, 60, 60))
        self.tile_5.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_5.setObjectName("tile_5")
        self.tile_5.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_6 = QtWidgets.QLabel(self.centralWidget)
        self.tile_6.setGeometry(QtCore.QRect(10, 260, 60, 60))
        self.tile_6.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_6.setObjectName("tile_6")
        self.tile_6.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_7 = QtWidgets.QLabel(self.centralWidget)
        self.tile_7.setGeometry(QtCore.QRect(80, 260, 60, 60))
        self.tile_7.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_7.setObjectName("tile_7")
        self.tile_7.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_8 = QtWidgets.QLabel(self.centralWidget)
        self.tile_8.setGeometry(QtCore.QRect(150, 260, 60, 60))
        self.tile_8.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_8.setObjectName("tile_8")
        self.tile_8.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_9 = QtWidgets.QLabel(self.centralWidget)
        self.tile_9.setGeometry(QtCore.QRect(360, 260, 60, 60))
        self.tile_9.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_9.setObjectName("tile_9")
        self.tile_9.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_10 = QtWidgets.QLabel(self.centralWidget)
        self.tile_10.setGeometry(QtCore.QRect(290, 260, 60, 60))
        self.tile_10.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_10.setObjectName("tile_10")
        self.tile_10.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_11 = QtWidgets.QLabel(self.centralWidget)
        self.tile_11.setGeometry(QtCore.QRect(220, 260, 60, 60))
        self.tile_11.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_11.setObjectName("tile_11")
        self.tile_11.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_12 = QtWidgets.QLabel(self.centralWidget)
        self.tile_12.setGeometry(QtCore.QRect(10, 330, 60, 60))
        self.tile_12.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_12.setObjectName("tile_12")
        self.tile_12.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_13 = QtWidgets.QLabel(self.centralWidget)
        self.tile_13.setGeometry(QtCore.QRect(80, 330, 60, 60))
        self.tile_13.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_13.setObjectName("tile_13")
        self.tile_13.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_14 = QtWidgets.QLabel(self.centralWidget)
        self.tile_14.setGeometry(QtCore.QRect(150, 330, 60, 60))
        self.tile_14.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_14.setObjectName("tile_14")
        self.tile_14.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_15 = QtWidgets.QLabel(self.centralWidget)
        self.tile_15.setGeometry(QtCore.QRect(360, 330, 60, 60))
        self.tile_15.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_15.setObjectName("tile_15")
        self.tile_15.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_16 = QtWidgets.QLabel(self.centralWidget)
        self.tile_16.setGeometry(QtCore.QRect(290, 330, 60, 60))
        self.tile_16.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_16.setObjectName("tile_16")
        self.tile_16.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_17 = QtWidgets.QLabel(self.centralWidget)
        self.tile_17.setGeometry(QtCore.QRect(220, 330, 60, 60))
        self.tile_17.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_17.setObjectName("tile_17")
        self.tile_17.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_18 = QtWidgets.QLabel(self.centralWidget)
        self.tile_18.setGeometry(QtCore.QRect(220, 540, 60, 60))
        self.tile_18.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_18.setObjectName("tile_18")
        self.tile_18.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_19 = QtWidgets.QLabel(self.centralWidget)
        self.tile_19.setGeometry(QtCore.QRect(10, 540, 60, 60))
        self.tile_19.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_19.setObjectName("tile_19")
        self.tile_19.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_20 = QtWidgets.QLabel(self.centralWidget)
        self.tile_20.setGeometry(QtCore.QRect(80, 470, 60, 60))
        self.tile_20.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_20.setObjectName("tile_20")
        self.tile_20.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_21 = QtWidgets.QLabel(self.centralWidget)
        self.tile_21.setGeometry(QtCore.QRect(290, 540, 60, 60))
        self.tile_21.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_21.setObjectName("tile_21")
        self.tile_21.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_22 = QtWidgets.QLabel(self.centralWidget)
        self.tile_22.setGeometry(QtCore.QRect(10, 400, 60, 60))
        self.tile_22.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_22.setObjectName("tile_22")
        self.tile_22.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_23 = QtWidgets.QLabel(self.centralWidget)
        self.tile_23.setGeometry(QtCore.QRect(360, 540, 60, 60))
        self.tile_23.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_23.setObjectName("tile_23")
        self.tile_23.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_24 = QtWidgets.QLabel(self.centralWidget)
        self.tile_24.setGeometry(QtCore.QRect(10, 470, 60, 60))
        self.tile_24.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_24.setObjectName("tile_24")
        self.tile_24.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_25 = QtWidgets.QLabel(self.centralWidget)
        self.tile_25.setGeometry(QtCore.QRect(150, 400, 60, 60))
        self.tile_25.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_25.setObjectName("tile_25")
        self.tile_25.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_26 = QtWidgets.QLabel(self.centralWidget)
        self.tile_26.setGeometry(QtCore.QRect(360, 470, 60, 60))
        self.tile_26.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_26.setObjectName("tile_26")
        self.tile_26.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_27 = QtWidgets.QLabel(self.centralWidget)
        self.tile_27.setGeometry(QtCore.QRect(360, 400, 60, 60))
        self.tile_27.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_27.setObjectName("tile_27")
        self.tile_27.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_28 = QtWidgets.QLabel(self.centralWidget)
        self.tile_28.setGeometry(QtCore.QRect(220, 470, 60, 60))
        self.tile_28.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_28.setObjectName("tile_28")
        self.tile_28.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_29 = QtWidgets.QLabel(self.centralWidget)
        self.tile_29.setGeometry(QtCore.QRect(80, 540, 60, 60))
        self.tile_29.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_29.setObjectName("tile_29")
        self.tile_29.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_30 = QtWidgets.QLabel(self.centralWidget)
        self.tile_30.setGeometry(QtCore.QRect(150, 540, 60, 60))
        self.tile_30.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_30.setObjectName("tile_30")
        self.tile_30.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_31 = QtWidgets.QLabel(self.centralWidget)
        self.tile_31.setGeometry(QtCore.QRect(150, 470, 60, 60))
        self.tile_31.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_31.setObjectName("tile_31")
        self.tile_31.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_32 = QtWidgets.QLabel(self.centralWidget)
        self.tile_32.setGeometry(QtCore.QRect(220, 400, 60, 60))
        self.tile_32.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_32.setObjectName("tile_32")
        self.tile_32.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_33 = QtWidgets.QLabel(self.centralWidget)
        self.tile_33.setGeometry(QtCore.QRect(290, 470, 60, 60))
        self.tile_33.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_33.setObjectName("tile_33")
        self.tile_33.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_34 = QtWidgets.QLabel(self.centralWidget)
        self.tile_34.setGeometry(QtCore.QRect(80, 400, 60, 60))
        self.tile_34.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_34.setObjectName("tile_34")
        self.tile_34.setAlignment(QtCore.Qt.AlignCenter)

        self.tile_35 = QtWidgets.QLabel(self.centralWidget)
        self.tile_35.setGeometry(QtCore.QRect(290, 400, 60, 60))
        self.tile_35.setMinimumSize(QtCore.QSize(50, 50))
        self.tile_35.setObjectName("tile_35")
        self.tile_35.setAlignment(QtCore.Qt.AlignCenter)
        self.line_5 = QtWidgets.QFrame(self.centralWidget)
        self.line_5.setGeometry(QtCore.QRect(290, 0, 20, 120))
        self.line_5.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_5.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_5.setObjectName("line_5")
        self.pushButton = QtWidgets.QPushButton(self.centralWidget)
        self.pushButton.setGeometry(QtCore.QRect(310, 20, 91, 32))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_4 = QtWidgets.QPushButton(self.centralWidget)
        self.pushButton_4.setGeometry(QtCore.QRect(310, 60, 91, 32))
        self.pushButton_4.setDefault(True)
        self.pushButton_4.setObjectName("pushButton_4")
        GameUI.setCentralWidget(self.centralWidget)
        # button
        self.fileName = None
        self.heromap = None
        self.pushButton_2.clicked.connect(self.setHero)
        self.winLoseButton.clicked.connect(self.restartGame)
        self.pushButton.clicked.connect(self.saveToJson)
        self.pushButton_4.clicked.connect(self.loadFromJson)
        self.Resize.clicked.connect(self.resizeTilemap)

        self.retranslateUi(GameUI)
        QtCore.QMetaObject.connectSlotsByName(GameUI)
        # load images
        self.tile_0.mousePressEvent = self.setTile0
        self.tile_1.mousePressEvent = self.setTile1
        self.tile_2.mousePressEvent = self.setTile2
        self.tile_3.mousePressEvent = self.setTile3
        self.tile_6.mousePressEvent = self.setTile6
        self.tile_7.mousePressEvent = self.setTile7

        self.image01 = QPixmap("../Game/Assets/bg.png")
        self.image01 = self.image01.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_0.setPixmap(self.image01)
        self.tile_0.setStyleSheet("background: white;")

        self.image02 = QPixmap("../Game/Assets/boxAlt.png")
        self.image02 = self.image02.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_1.setPixmap(self.image02)
        self.tile_1.setStyleSheet("background: white;")

        self.image03 = QPixmap("../Game/Assets/coinGold.png")
        self.image03 = self.image03.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_2.setPixmap(self.image03)
        self.tile_2.setStyleSheet("background: white;")

        self.image04 = QPixmap("../Game/Assets/grass.png")
        self.image04 = self.image04.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_3.setPixmap(self.image04)
        self.tile_3.setStyleSheet("background: white;")

        self.image05 = QPixmap("../Game/Assets/slimeWalk1.png")
        self.image05 = self.image05.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_6.setPixmap(self.image05)
        self.tile_6.setStyleSheet("background: white;")

        self.image06 = QPixmap("../Game/Assets/flyFly1.png")
        self.image06 = self.image06.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.tile_7.setPixmap(self.image06)
        self.tile_7.setStyleSheet("background: white;")
    
    def setTile0(self, event):
        self.tile_0.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(0)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "none"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")
    
    def setTile1(self, event):
        self.tile_1.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(1)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "box"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")        
        
    def setTile2(self, event):
        self.tile_2.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(2)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "coin_gold"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")

    def setTile3(self, event):
        self.tile_3.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(3)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "grass"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")
    
    def setTile6(self, event):
        self.tile_6.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(6)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "enemy_slime"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")
    
    def setTile7(self, event):
        self.tile_7.setStyleSheet("background: white; border-width: 2px; border-style: solid; border-color: rgb(0, 0 ,255)")
        self.clearborder(7)
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "change_tile_map"}
            message["args"] = {"choice": "enemy_fly"}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")

    def clearborder(self, num):
        tiles = [0,1,2,3,6,7]
        for tile in tiles:
            if tile == num:
                continue
            else:
                eval("self.tile_%i.setStyleSheet('background: white')" % tile)

    def setHero(self):
        self.fileName, _ = QtWidgets.QFileDialog.getOpenFileName(None, "Select Hreo", "", "Image Files (*.png *.jpg *.jpeg *.bmp)")
        if self.fileName:
            self.heromap = QPixmap(self.fileName)
            self.heromap = self.heromap.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
            self.hero_00.setPixmap(self.heromap)
            self.hero_00.setStyleSheet("background: white")
            
            try:
                sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
                sock.connect(server_address)
                message = {"action_name": "change_player_image"}

                if self.fileName.endswith("player1.png"):
                    message["args"] = {"choice": "green"}
                elif self.fileName.endswith("player2.png"):
                    message["args"] = {"choice": "blue"}
                elif self.fileName.endswith("player3.png"):
                    message["args"] = {"choice": "pink"}
                else:
                    message["args"] = {"choice": "green"}

                message_json = json.dumps(message).encode()
                sock.sendall(message_json)
                time.sleep(0.2)
                sock.close()
            except:
                print("connection fail")
    
    def restartGame(self):
        try:
            lives = int(self.loseCondition_spinBox.value())
            startX = int(self.lineEdit.text())
            startY = int(self.lineEdit_2.text())
        except:
            lives = 3
            startX = 1
            startY = 1

        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "restart_game_setting"}
            message["args"] = {"lives": lives, "start_position": [startX, startY]}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")
    
    def saveToJson(self):
        try:
            # send message: save json
            outputPath = str(QtWidgets.QFileDialog.getExistingDirectory(None, "Select Directory"))
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "save_to_json"}
            message["args"] = {"save_json_path": outputPath}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("save fail")
    
    def loadFromJson(self):
        jsonFile, _ = QtWidgets.QFileDialog.getOpenFileName(None, "Select JSON", "", "JSON Files (*.json)")
        if not jsonFile:
            return
        
        # send message: load from json
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.connect(server_address)
        message = {"action_name": "load_from_json"}
        message["args"] = {"load_json_path": str(jsonFile)}
        message_json = json.dumps(message).encode()
        sock.sendall(message_json)
        time.sleep(0.2)
        sock.close()

        # update palette tool information
        with open(str(jsonFile), "rb") as f:
            message2 = json.load(f)
        
        player_color = message2["player"]["init_image_selected"]
        if player_color == "green":
            self.fileName = "./Player/player1.png"
        elif player_color == "blue":
            self.fileName = "./Player/player2.png"
        elif player_color == "pink":
            self.fileName = "./Player/player3.png"
        else:
            return
        self.heromap = QPixmap(self.fileName)
        self.heromap = self.heromap.scaled(60, 60, QtCore.Qt.KeepAspectRatio)
        self.hero_00.setPixmap(self.heromap)
        self.hero_00.setStyleSheet("background: white")
        self.loseCondition_spinBox.setValue(int(message2["player"]["max_lives"]))
        tileMap = message2["levels"]["map"][0]["tiles"]
        playerPosition = None
        for i in range(len(tileMap)):
            if tileMap[i] == "0@":
                playerPosition = i
        if playerPosition == None:
            return
        tileSize = len(tileMap)
        num_col = message2["levels"]["map"][0]["num_col"]
        num_row = tileSize / num_col
        start_x = int(playerPosition % num_col)
        start_y = int(num_row - int(playerPosition / num_col) - 1)
        self.lineEdit.setText(str(start_x))
        self.lineEdit_2.setText(str(start_y))
        
    def resizeTilemap(self):
        try:
            num_row = int(self.row_spinBox.value())
            num_col = int(self.col_spinBox.value())
        except:
            num_row = 20
            num_col = 50
        try:
            sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            sock.connect(server_address)
            message = {"action_name": "resize_tile_map"}
            message["args"] = {"size": [num_row, num_col]}
            message_json = json.dumps(message).encode()
            sock.sendall(message_json)
            time.sleep(0.2)
            sock.close()
        except:
            print("connection fail")

    def retranslateUi(self, GameUI):
        _translate = QtCore.QCoreApplication.translate
        GameUI.setWindowTitle(_translate("GameUI", "PaletteTool"))
        self.howManyCol_label.setText(_translate("GameUI", "Columns:"))
        self.howManyRows_label.setText(_translate("GameUI", "Rows:"))
        self.loseCondition_label.setText(_translate("GameUI", "Lives:"))
        self.tile_0.setText(_translate("GameUI", "T00"))
        self.labelTile.setText(_translate("GameUI", "Tiles and Enemies:"))
        self.labelHero.setText(_translate("GameUI", "Heros:"))
        self.hero_00.setText(_translate("GameUI", "H01"))
        self.winLoseButton.setText(_translate("GameUI", "Restart Game"))
        self.Resize.setText(_translate("GameUI", "Resize"))
        self.pushButton_2.setText(_translate("GameUI", "Select"))
        self.loseCondition_label_2.setText(_translate("GameUI", "Start:"))
        self.lineEdit.setPlaceholderText(_translate("GameUI", "X"))
        self.lineEdit_2.setPlaceholderText(_translate("GameUI", "Y"))
        self.pushButton_3.setText(_translate("GameUI", "Import..."))
        self.pushButton_3.setVisible(False)
        self.tile_1.setText(_translate("GameUI", "."))
        self.tile_2.setText(_translate("GameUI", "."))
        self.tile_3.setText(_translate("GameUI", "."))
        self.tile_4.setText(_translate("GameUI", "."))
        self.tile_5.setText(_translate("GameUI", "."))
        self.tile_6.setText(_translate("GameUI", "."))
        self.tile_7.setText(_translate("GameUI", "."))
        self.tile_8.setText(_translate("GameUI", "."))
        self.tile_9.setText(_translate("GameUI", "."))
        self.tile_10.setText(_translate("GameUI", "."))
        self.tile_11.setText(_translate("GameUI", "."))
        self.tile_12.setText(_translate("GameUI", "."))
        self.tile_13.setText(_translate("GameUI", "."))
        self.tile_14.setText(_translate("GameUI", "."))
        self.tile_15.setText(_translate("GameUI", "."))
        self.tile_16.setText(_translate("GameUI", "."))
        self.tile_17.setText(_translate("GameUI", "."))
        self.tile_18.setText(_translate("GameUI", "."))
        self.tile_19.setText(_translate("GameUI", "."))
        self.tile_20.setText(_translate("GameUI", "."))
        self.tile_21.setText(_translate("GameUI", "."))
        self.tile_22.setText(_translate("GameUI", "."))
        self.tile_23.setText(_translate("GameUI", "."))
        self.tile_24.setText(_translate("GameUI", "."))
        self.tile_25.setText(_translate("GameUI", "."))
        self.tile_26.setText(_translate("GameUI", "."))
        self.tile_27.setText(_translate("GameUI", "."))
        self.tile_28.setText(_translate("GameUI", "."))
        self.tile_29.setText(_translate("GameUI", "."))
        self.tile_30.setText(_translate("GameUI", "."))
        self.tile_31.setText(_translate("GameUI", "."))
        self.tile_32.setText(_translate("GameUI", "."))
        self.tile_33.setText(_translate("GameUI", "."))
        self.tile_34.setText(_translate("GameUI", "."))
        self.tile_35.setText(_translate("GameUI", "."))
        self.pushButton.setText(_translate("GameUI", "Save"))
        self.pushButton_4.setText(_translate("GameUI", "Load"))
        

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_GameUI()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
