QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    HKIPcamera/hkipcamera.cpp

HEADERS += \
    mainwindow.h \
    HKIPcamera/hkipcamera.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/HKIPcamera/lib/ \
    -lAudioRender \
    -lHCCore \
    -lhcnetsdk \
    -lhpr \
    -lNPQos \
    -lPlayCtrl \
    -lSuperRender \
    -lz

LIBS += -L$$PWD/HKIPcamera/lib/HCNetSDKCom/ \
    -lanalyzedata \
    -lAudioIntercom \
    -lHCAlarm \
    -lHCCoreDevCfg \
    -lHCDisplay \
    -lHCGeneralCfgMgr \
    -lHCIndustry \
    -lHCPlayBack \
    -lHCPreview \
    -lHCVoiceTalk \
    -liconv2 \
    -lStreamTransClient \
    -lSystemTransform

LIBS += -L/usr/local/lib/ \
    -lopencv_calib3d \
    -lopencv_core \
    -lopencv_dnn \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gapi \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_imgproc \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_photo \
    -lopencv_stitching \
    -lopencv_video \
    -lopencv_videoio

INCLUDEPATH += $$PWD/HKIPcamera/inc
INCLUDEPATH += /usr/local/include/opencv4
