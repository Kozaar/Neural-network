QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Interface/crossvalidationconf.cpp \
    Interface/dialogstarttraining.cpp \
    Interface/newnetsetting.cpp \
    Interface/reportgraph.cpp \
    main.cpp \
    Controller/controller.cpp \
    Interface/interface.cpp \
    Interface/paintscene.cpp \
    Interface/qcustomplot.cpp \
    Model/GNN.cpp \
    Model/fileexecutor.cpp \
    Model/matrixneuralnet.cpp \
    Model/model.cpp

HEADERS += \
    Controller/controller.h \
    Interface/crossvalidationconf.h \
    Interface/dialogstarttraining.h \
    Interface/interface.h \
    Interface/newnetsetting.h \
    Interface/paintscene.h \
    Interface/qcustomplot.h \
    Interface/reportgraph.h \
    Model/GNN.h \
    Model/fileexecutor.h \
    Model/matrixneuralnet.h \
    Model/model.h \
    Model/neuralnet.h

FORMS += \
    Interface/crossvalidationconf.ui \
    Interface/dialogstarttraining.ui \
    Interface/interface.ui \
    Interface/newnetsetting.ui \
    Interface/reportgraph.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
