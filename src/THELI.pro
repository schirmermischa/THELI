#-------------------------------------------------
#
# Project created by QtCreator 2018-02-18T10:31:22
#
#-------------------------------------------------

QT       += core gui printsupport testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = theli
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cc \
    abszp/absphot.cc \
    abszp/abszeropoint.cc \
    colorpicture/colorpicture.cc \
    colorpicture/refcatdata.cc \
    colorpicture/subtaskColorcalib.cc \
    colorpicture/subtaskCropcoadd.cc \
    colorpicture/subtaskbbnb.cc \
    colorpicture/subtaskfits2tiff.cc \
    consistencychecks.cc \
    datadir.cc \
    datamodel/datamodel.cc \
    deepmessages.cc \
    dockwidgets/confdockwidget.cc \
    dockwidgets/defaults.cc \
    dockwidgets/memoryviewer.cc \
    dockwidgets/monitor.cc \
    dockwidgets/validate_confdock.cc \
    functions.cc \
    imagestatistics/imagestatistics.cc \
    imagestatistics/imagestatistics_events.cc \
    imagestatistics/imagestatistics_plotting.cc \
    instrumentdefinition.cc \
    iview/actions.cc \
    iview/constructors.cc \
    iview/dockwidgets/ivcolordockwidget.cc \
    iview/dockwidgets/ivconfdockwidget.cc \
    iview/dockwidgets/ivscampdockwidget.cc \
    iview/events.cc \
    iview/iview.cc \
    iview/mygraphicsellipseitem.cc \
    iview/mygraphicsscene.cc \
    iview/mygraphicsview.cc \
    iview/wavelet/filtercoeffs.cc \
    iview/wavelet/wavelet.cc \
    mainwindow.cc \
    myfits/myfits.cc \
    myimage/background.cc \
    myimage/myimage.cc \
    myimage/segmentation.cc \
    myimage/sextractor.cc \
    myimage/skysub.cc \
    myimage/tifftools.cc \
    myimage/weighting.cc \
    mywcs.cc \
    preferences.cc \
    processingExternal/errordialog.cc \
    processingInternal/controller.cc \
    processingInternal/data.cc \
    processingInternal/displayconfig.cc \
    processingInternal/dictionaries.cc \
    processingInternal/mask.cc \
    processingInternal/photinst.cc \
    processingInternal/processingAncillary.cc \
    processingInternal/processingAstrometry.cc \
    processingInternal/processingBackground.cc \
    processingInternal/processingCalibration.cc \
    processingInternal/processingCoadd.cc \
    processingInternal/processingCollapse.cc \
    processingInternal/processingSkysub.cc \
    processingInternal/processingSplitter.cc \
    processingInternal/processingWeight.cc \
    processingStatus/processingStatus.cc \
    qcustomplot.cpp \
    query/query.cc \
    readmes/imstatsreadme.cc \
    readmes/multidirreadme.cc \
    readmes/scampreadme.cc \
    readmes/swarpreadme.cc \
    settings.cc \
    status.cc \
    taskinfrastructure.cc \
    tasks.cc \
    threading/abszpworker.cc \
    threading/colorpictureworker.cc \
    threading/mainguiworker.cc \
    threading/memoryworker.cc \
    threading/scampworker.cc \
    threading/sexworker.cc \
    threading/swarpworker.cc \
    threading/worker.cc \
    tools/cfitsioerrorcodes.cc \
    tools/correlator.cc \
    tools/cpu.cc \
    tools/debayer.cc \
    tools/detectedobject.cc \
    tools/fileprogresscounter.cc \
    tools/fitgauss1d.cc \
    tools/fitting.cc \
    tools/imagequality.cc \
    tools/polygon.cc \
    tools/splitter.cc \
    tools/splitter_RAW.cc \
    tools/splitter_buildHeader.cc \
    tools/splitter_processingGeneric.cc \
    tools/splitter_processingSpecific.cc \
    tools/splitter_queryHeaderLists.cc \
    tools/swarpfilter.cc \
    tools/tools.cc \
    tools/xcorr.cc \
    validators.cc \
    myimage/memoryoperations.cc \
    tools/splitter_multiport.cc



HEADERS += \
        mainwindow.h \
    abszp/absphot.h \
    abszp/abszeropoint.h \
    colorpicture/colorpicture.h \
    colorpicture/refcatdata.h \
    datadir.h \
    datamodel/datamodel.h \
    dockwidgets/confdockwidget.h \
    dockwidgets/memoryviewer.h \
    dockwidgets/monitor.h \
    functions.h \
    imagestatistics/imagestatistics.h \
    instrumentdata.h \
    instrumentdefinition.h \
    iview/dockwidgets/ivcolordockwidget.h \
    iview/dockwidgets/ivconfdockwidget.h \
    iview/dockwidgets/ivscampdockwidget.h \
    iview/iview.h \
    iview/mygraphicsellipseitem.h \
    iview/mygraphicsscene.h \
    iview/mygraphicsview.h \
    iview/wavelet/wavelet.h \
    myfits/myfits.h \
    myimage/myimage.h \
    mywcs.h \
    preferences.h \
    processingExternal/errordialog.h \
    processingInternal/controller.h \
    processingInternal/data.h \
    processingInternal/mask.h \
    processingInternal/photinst.h \
    processingStatus/processingStatus.h \
    qcustomplot.h \
    query/query.h \
    readmes/imstatsreadme.h \
    readmes/multidirreadme.h \
    readmes/scampreadme.h \
    readmes/swarpreadme.h \
    status.h \
    threading/abszpworker.h \
    threading/colorpictureworker.h \
    threading/mainguiworker.h \
    threading/memoryworker.h \
    threading/scampworker.h \
    threading/sexworker.h \
    threading/swarpworker.h \
    threading/worker.h \
    tools/cfitsioerrorcodes.h \
    tools/correlator.h \
    tools/cpu.h \
    tools/detectedobject.h \
    tools/fileprogresscounter.h \
    tools/fitgauss1d.h \
    tools/fitting.h \
    tools/imagequality.h \
    tools/polygon.h \
    tools/splitter.h \
    tools/swarpfilter.h \
    tools/tools.h

FORMS += \
        mainwindow.ui \
    abszp/abszeropoint.ui \
    colorpicture/colorpicture.ui \
    dockwidgets/confdockwidget.ui \
    dockwidgets/monitor.ui \
    dockwidgets/memoryviewer.ui \
    imagestatistics/imagestatistics.ui \
    instrumentdefinition.ui \
    iview/iview.ui \
    iview/dockwidgets/ivconfdockwidget.ui \
    iview/dockwidgets/ivscampdockwidget.ui \
    iview/dockwidgets/ivcolordockwidget.ui \
    preferences.ui \
    processingExternal/errordialog.ui \
    readmes/imstatsreadme.ui \
    readmes/multidirreadme.ui \
    readmes/scampreadme.ui \
    readmes/swarpreadme.ui

RESOURCES = resources.qrc

CONFIG += c++11

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -Wno-unused
QMAKE_CXXFLAGS += -O3

QMAKE_LFLAGS  +=  -fopenmp

# INCLUDEPATH += /usr/local/include/SPLINTER/
# LIBS += -lcfitsio -lcurl -lpthread -lfftw3_omp -lfftw3 -lsplinter-4-0 -lm -lgsl -lgslcblas

INCLUDEPATH += /usr/local/include/wcslib-6.4/

LIBS += -lcfitsio -lcurl -lfftw3_omp -lfftw3 -lm -lgsl -lgslcblas -lwcs -ltiff -lraw
