TARGET = sysviz

MOC_DIR = .moc
OBJECTS_DIR = .obj

QT += quick

SOURCES = \
    src/main.cpp \
    src/traceevent.cpp \
    src/tracemodel.cpp \
    src/cpufrequencymodel.cpp \
    src/cpucstatemodel.cpp \
    src/gpufrequencymodel.cpp \
    src/processmodel.cpp \
    src/threadmodel.cpp

HEADERS = \
    src/tracemodel.h \
    src/slice.h \
    src/cpufrequencymodel.h \
    src/cpucstatemodel.h \
    src/tracetime.h \
    src/gpufrequencymodel.h \
    src/processmodel.h \
    src/threadmodel.h

CONFIG -= app_bundle
CONFIG += c++11