RESOURCES += images.qrc

HEADERS += mainwindow.h atom.h \
    bond.h \
    cdialog.h \
    sdialog.h
SOURCES += main.cpp \
    bond.cpp \
    cdialog.cpp \
    sdialog.cpp
SOURCES += mainwindow.cpp atom.cpp

QT += widgets
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = /home/tt200/gtpex/gtopex/
INSTALLS += target
