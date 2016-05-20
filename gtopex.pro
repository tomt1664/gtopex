RESOURCES += images.qrc

HEADERS += mainwindow.h atom.h \
    bond.h \
    cdialog.h \
    sdialog.h \
    tdialog.h \
    rdialog.h \
    pdialog.h \
    hlight.h
SOURCES += main.cpp \
    bond.cpp \
    cdialog.cpp \
    sdialog.cpp \
    tdialog.cpp \
    rdialog.cpp \
    pdialog.cpp \
    hlight.cpp
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

