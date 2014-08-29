TARGET = sysviz

MOC_DIR = .moc
OBJECTS_DIR = .obj

QT += quick

SOURCES = \
    main.cpp \
    traceevent.cpp \
    tracemodel.cpp \
    cpufrequencymodel.cpp \
    cpucstatemodel.cpp \
    gpufrequencymodel.cpp

HEADERS = \
    tracemodel.h \
    slice.h \
    cpufrequencymodel.h \
    cpucstatemodel.h \
    tracetime.h \
    gpufrequencymodel.h

CONFIG -= app_bundle
CONFIG += c++11
