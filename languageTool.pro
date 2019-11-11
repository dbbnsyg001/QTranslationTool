

QT       += core gui widgets

QT       += xml sql
#不支持Linux，舍弃QT       += axcontainer

unix{
#头文件存放目录
INCLUDEPATH += /usr/local/include
#这里是安装libxls的lib目录下读取需要lxlsreader
LIBS += -L/usr/local/lib -lxlsreader
}


TARGET = translate
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mxmlparser.cpp

HEADERS  += mainwindow.h \
    mxmlparser.h

FORMS    += mainwindow.ui

