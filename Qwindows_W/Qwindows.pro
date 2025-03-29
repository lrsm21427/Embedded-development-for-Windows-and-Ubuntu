QT       += core gui qmqtt network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    page1.cpp \
    page2.cpp \
    page3.cpp \
    page4.cpp \
    win_main.cpp

HEADERS += \
    page1.h \
    page2.h \
    page3.h \
    page4.h \
    win_main.h

FORMS += \
    page1.ui \
    page2.ui \
    page3.ui \
    page4.ui \
    win_main.ui

# 添加头文件路径
INCLUDEPATH += "D:\vosk\vosk-win64-0.3.45"

# 添加库文件路径（根据个人系统调整）
LIBS += -L"D:\vosk\vosk-win64-0.3.45" -lvosk
LIBS += -L"D:\vosk\OpenBLAS-0.3.29_x64\lib" -lopenblas

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    css.qrc
