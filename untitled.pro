QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addguestdialog.cpp \
    booking.cpp \
    deletebookingdialog.cpp \
    deleteguestdialog.cpp \
    econom.cpp \
    feature.cpp \
    filemanager.cpp \
    guest.cpp \
    hotel.cpp \
    initializer.cpp \
    lux.cpp \
    main.cpp \
    mainwindow.cpp \
    person.cpp \
    preference.cpp \
    room.cpp \
    roomfactory.cpp \
    selectionstrategy.cpp \
    selectroomdialog.cpp \
    standard.cpp

HEADERS += \
    addguestdialog.h \
    booking.h \
    deletebookingdialog.h \
    deleteguestdialog.h \
    econom.h \
    feature.h \
    filemanager.h \
    guest.h \
    hotel.h \
    initializer.h \
    lux.h \
    mainwindow.h \
    person.h \
    preference.h \
    room.h \
    roomfactory.h \
    selectionstrategy.h \
    selectroomdialog.h \
    standard.h

FORMS += \
    addguestdialog.ui \
    deletebookingdialog.ui \
    deleteguestdialog.ui \
    mainwindow.ui \
    selectroomdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
