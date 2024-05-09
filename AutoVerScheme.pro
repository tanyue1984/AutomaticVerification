QT       += core gui
QT       += sql
QT       += widgets
QT       += axcontainer
QT       += serialport
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#INCLUDEPATH += $$quote(D:/work/OpenSSL-Win64/include/)
#LIBS += -L"D:/work/OpenSSL-Win64/lib"
#LIBS += -L"D:/work/OpenSSL-Win64/lib/VC/x64/MD" -llibcrypto


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
    DataProcessingBLL.cpp \
    PageControl.cpp \
    TaskManager.cpp \
    basecommonapi.cpp \
    checkdevice.cpp \
    beexamined_deviceitem.cpp \
    checkmethodmanage.cpp \
    checkwindow.cpp \
    create_task.cpp \
    createtask_scandevice.cpp \
    databaseconnection.cpp \
    databaseconnectionmainsys.cpp \
    datasyncview.cpp \
    depar_task.cpp \
    depar_task_server.cpp \
    dialogcheckresult.cpp \
    dialogdcstabinput.cpp \
    dialogverifyresult.cpp \
    formaddrsetting.cpp \
    formbizdeviceselect.cpp \
    formcheckpoint.cpp \
    formcommsetting.cpp \
    formgageresult.cpp \
    formverypoint.cpp \
    global.cpp \
    indexwindow.cpp \
    infomanageview.cpp \
    inspectdataitem.cpp \
    loginwindow.cpp \
    main.cpp \
    mainsysdatabaseconnection.cpp \
    mainwindow.cpp \
    multiinputdialog.cpp \
    my_task.cpp \
    newcheckmethod.cpp \
    newverifymethod.cpp \
    rulefileitem.cpp \
    standard_deviceitem.cpp \
    syncdialog.cpp \
    synctask.cpp \
    syncupload.cpp \
    testdataitem.cpp \
    testview.cpp \
    verifydevice.cpp \
    verifymethodmanage.cpp \
    verifywindow.cpp \
    visacommonengine.cpp \
    wordengine.cpp

HEADERS += \
    CheckBoxTableHeader.h \
    DataProcessingBLL.h \
    PageControl.h \
    SequentialMap.h \
    createtask_scandevice.h \
    databaseconnectionmainsys.h \
    dialogdcstabinput.h \
    dialogverifyresult.h \
    formaddrsetting.h \
    formbizdeviceselect.h \
    formcommsetting.h \
    formgageresult.h \
    global.h \
    id/IdWorker.h \
    id/Noncopyable.h \
    id/Singleton.h \
    id/TaskManager.h \
    id/Timer.h \
    Visa/Include/visa.h \
    Visa/Include/visatype.h \
    Visa/Include/vpptype.h \
    basecommonapi.h \
    checkdevice.h \
    InfoBase.h \
    beexamined_deviceitem.h \
    checkmethodmanage.h \
    checkwindow.h \
    create_task.h \
    databaseconnection.h \
    datasyncview.h \
    depar_task.h \
    depar_task_server.h \
    dialogcheckresult.h \
    formcheckpoint.h \
    formverypoint.h \
    indexwindow.h \
    infomanageview.h \
    inspectdataitem.h \
    loginwindow.h \
    mainsysdatabaseconnection.h \
    mainwindow.h \
    multiinputdialog.h \
    my_task.h \
    newcheckmethod.h \
    newverifymethod.h \
    rulefileitem.h \
    standard_deviceitem.h \
    syncdialog.h \
    synctask.h \
    syncupload.h \
    testdataitem.h \
    testview.h \
    verifydevice.h \
    verifymethodmanage.h \
    verifywindow.h\
    biz_device_manage_serve.h \
    visacommonengine.h \
    wordengine.h

FORMS += \
    beexamined_deviceitem.ui \
    checkdevice.ui \
    checkmethodmanage.ui \
    checkwindow.ui \
    create_task.ui \
    createtask_scandevice.ui \
    datasyncview.ui \
    depar_task.ui \
    dialogcheckresult.ui \
    dialogdcstabinput.ui \
    dialogverifyresult.ui \
    formaddrsetting.ui \
    formbizdeviceselect.ui \
    formcheckpoint.ui \
    formcommsetting.ui \
    formgageresult.ui \
    formverypoint.ui \
    indexwindow.ui \
    infomanageview.ui \
    inspectdataitem.ui \
    loginwindow.ui \
    mainwindow.ui \
    my_task.ui \
    newcheckmethod.ui \
    newverifymethod.ui \
    rulefileitem.ui \
    standard_deviceitem.ui \
    syncdialog.ui \
    testdataitem.ui \
    testview.ui \
    verifydevice.ui \
    verifymethodmanage.ui \
    verifywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/qss.qrc \
    res/res.qrc

INCLUDEPATH += $$PWD/Visa/Include
LIBS += -L$$PWD/Visa/Lib_x64/msc/ -lvisa64
LIBS += -L$$PWD/Visa/Lib_x64/msc/ -lnivisa64

DISTFILES += \
    Visa/Bin/NiVi488.dll \
    Visa/Bin/NiViAsrl.dll \
    Visa/Bin/NiViEnet.dll \
    Visa/Bin/NiViEnetAsrl.dll \
    Visa/Bin/NiViPxi.dll \
    Visa/Bin/NiViRpc.dll \
    Visa/Bin/NiViUsb.dll \
    Visa/Bin/NiVisaTulip.dll \
    Visa/Include/visa32.bas \
    Visa/Include/vpptype.bas \
    Visa/Lib_x64/msc/nivisa64.lib \
    Visa/Lib_x64/msc/visa64.lib \
    Visa/NIvisa/DIFxAPI.dll \
    Visa/NIvisa/DefaultVisaConf.ini \
    Visa/NIvisa/DriverWizardInstallInf64.exe \
    Visa/NIvisa/NI-VISA Driver Wizard.exe \
    Visa/NIvisa/NIVisaServer.exe \
    Visa/NIvisa/NIvisaic.exe \
    Visa/NIvisa/NIvisaic.ini \
    Visa/NIvisa/NiVisaCom.dll \
    Visa/NIvisa/PxiPlugins/NiViPpiD.dll \
    Visa/NIvisa/PxiPlugins/NiViPpiP.dll \
    Visa/NIvisa/ni-visa.chm \
    Visa/NIvisa/nivisa_usbverifieru.dll \
    Visa/NIvisa/nivisagsg.chm \
    Visa/NIvisa/viic_ext.dll \
    Visa/lib/bc/visa32.lib \
    Visa/lib/msc/visa32.lib \
    res/image/u2.png

