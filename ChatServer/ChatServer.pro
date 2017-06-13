TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=c11



include (NetWork/NetWork.pri)
include (DataBase/DataBase.pri)
include (DataStructure/DataStructure.pri)
include (thread/thread.pri)
include (Utility/Utility.pri)

HEADERS += \
    error.h


SOURCES += \
    main.c \
    error.c

LIBS+=\
    -lpthread\
    -lmysqlclient

