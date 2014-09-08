TARGET = sysviz

MOC_DIR = .moc
OBJECTS_DIR = .obj

QT += quick

!nodqml:qtHaveModule(dqml):{
    message("Building with DQml support...")
    QT += dqml
}

SOURCES = \
    src/main.cpp \
    src/traceevent.cpp \
    src/tracemodel.cpp \
    src/cpufrequencymodel.cpp \
    src/cpucstatemodel.cpp \
    src/gpufrequencymodel.cpp \
    src/threadmodel.cpp \
    src/iotrafficmodel.cpp

HEADERS = \
    src/tracemodel.h \
    src/slice.h \
    src/cpufrequencymodel.h \
    src/cpucstatemodel.h \
    src/tracetime.h \
    src/gpufrequencymodel.h \
    src/threadmodel.h \
    src/iotrafficmodel.h

CONFIG -= app_bundle
CONFIG += c++11

OTHER_FILES += \
    qml/main.qml \
    qml/theme.js \
    qml/FrequencyView.qml \
    qml/RowGradient.qml \
    qml/LabelBackground.qml \
    qml/ViewLabel.qml \
    qml/Header.qml
