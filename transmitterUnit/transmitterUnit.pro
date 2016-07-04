TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    accelerometer.cpp \
    anemometer.cpp \
    battery.cpp \
    compass.cpp \
    packet.cpp \
    system.cpp \
    windvane.cpp \
    HMC5883L.cpp

HEADERS += \
    accelerometer.h \
    anemometer.h \
    battery.h \
    compass.h \
    packet.h \
    system.h \
    transmitterUnit.ino \
    winddirection.h \
    windvane.h \
    windvanelut.h \
    HMC5883L.h
