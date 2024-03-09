QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collaboratorsmanagementwindow.cpp \
    databaseconnection.cpp \
    definepasswordwindow.cpp \
    editproductfromsalewindow.cpp \
    inventorymanagementwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    salesmanagementwindow.cpp \
    utilities.cpp

HEADERS += \
    collaboratorsmanagementwindow.h \
    databaseconnection.h \
    definepasswordwindow.h \
    editproductfromsalewindow.h \
    inventorymanagementwindow.h \
    loginwindow.h \
    mainwindow.h \
    salesmanagementwindow.h \
    utilities.h

FORMS += \
    collaboratorsmanagementwindow.ui \
    definepasswordwindow.ui \
    editproductfromsalewindow.ui \
    inventorymanagementwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    salesmanagementwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourcefile.qrc

DISTFILES +=
