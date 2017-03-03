TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -std=c99

SOURCES += main.c \
    genetic.c

HEADERS += \
    genetic.h

