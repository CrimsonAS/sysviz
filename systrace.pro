TARGET = sysviz

SOURCES = \
    main.cpp \
    traceevent.cpp \
    tracemodel.cpp \
    cpufrequencymodel.cpp

HEADERS = \
    tracemodel.h \
    slice.h \
    cpufrequencymodel.h

CONFIG -= app_bundle
CONFIG += c++11
