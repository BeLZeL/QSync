
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = QSync
TEMPLATE = app

SOURCES += \
    form.cpp \
    main.cpp \
    search_del_dir.cpp \
    search_del_files.cpp \
    search_new_dir.cpp \
    search_new_files.cpp \
    apply_changes.cpp \
    scan_entries.cpp

HEADERS += \
    form.h \
    search_del_dir.h \
    search_del_files.h \
    search_new_dir.h \
    search_new_files.h \
    apply_changes.h \
    scan_entries.h

FORMS    += \
    form.ui

DISTFILES += \
    bzl.rc

RC_FILE = bzl.rc
