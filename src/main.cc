/*
Copyright (C) 2019 Mischa Schirmer

This file is part of THELI.

THELI is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the LICENSE file.
If not, see https://www.gnu.org/licenses/ .
*/

#include "mainwindow.h"
#include "functions.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QCoreApplication>
#include <QStandardPaths>
#include <omp.h>

void dependencyCheck();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::locale::global( std::locale( "en_US.UTF-8" ) );

    // Read the THELIDIR environment variable
    if (!QProcessEnvironment::systemEnvironment().contains("THELIDIR")) {
        // See if we can find it under /usr/share/theli/
        QDir thelidir1("/usr/share/theli/config/");
        QDir thelidir2("/usr/share/theli/python/");
        if (thelidir1.exists() && thelidir2.exists()) {
            // system-wide installation found, we are good!
        }
        else {
            QMessageBox::critical(0,"THELI","THELI configuration files not found under /usr/share/theli/\n\n"
                                            "You can fix this by setting the THELIDIR environment variable. For example, if the THELI source tree was installed under \n"
                                            ""+QDir::homePath()+"/THELI/ and you are using the 'bash' shell, "
                                                                "then you would include the following line in your .bashrc file:\n\n"
                                                                "export THELIDIR="+QDir::homePath()+"/THELI/",
                                  QMessageBox::Ok);
            exit (1);
        }
    }

    // Dependency checks
    dependencyCheck();

    // Required when starting the first time
    QString dotTheliName = QDir::homePath()+"/.theli/";
    QDir dotTheli(dotTheliName);
    dotTheli.mkdir(dotTheliName);
    dotTheli.mkdir(dotTheliName+"/instruments_user");

    // Fetch the process ID of the main program
    QString mainPID = QString::number(QCoreApplication::applicationPid());

    MainWindow w(mainPID);

    w.show();

    return a.exec();
}

void dependencyCheck()
{
    // Dependency checks
    QString scamp = findExecutableName("scamp");    // testing different executable names
    QString swarp = findExecutableName("swarp");
    QString sourceExtractor = findExecutableName("source-extractor");
    QString python = findExecutableName("python");
    QString anet1 = findExecutableName("build-astrometry-index");
    QString anet2 = findExecutableName("solve-field");
    QString anet3 = findExecutableName("astrometry-engine");

    QString scampDep = "";
    QString swarpDep = "";
    QString sourceExtractorDep = "";
    QString anetDep = "";
    QString pythonDep = "";

    int missingDep = 0;

    if (anet1.isEmpty()) {
        anetDep += "astrometry.net 'build-astrometry-index' not found.\n";
    }

    if (anet2.isEmpty()) {
        anetDep = "astrometry.net 'solve-field' not found.\n";
    }

//    if (anet3.isEmpty()) {
//        anetDep += "astrometry.net 'astrometry-engine' not found.\n";
//    }

    if (python.isEmpty()) {
        pythonDep = "python required (working binary names: 'python3' or 'python').\n";
    }

    if (!anetDep.isEmpty()) {
        qDebug() << "Astrometry.net installation was not found. Corresponding solvers are deactivated.";
        qDebug() << "Astrometry.net is available at https://github.com/dstndstn/astrometry.net/releases\n";
    }

    if (scamp.isEmpty()) {
        scampDep = "'Scamp' v2.0.4 or later required (working binary names: 'scamp' or 'Scamp').\nhttps://github.com/astromatic/scamp\n";
        ++missingDep;
    }
    else {
        // Check if scamp has the right version
        QString scampCommand = scamp + " -v";
        QString result = read_system_command(scampCommand);
        QStringList list = result.split(" ");
        if (list.length() < 3) {
            qDebug() << "WARNING: Unexpected output format when checking Scamp version:" << result;
            qDebug() << "WARNING: Required version: 2.0.4 or later";
        }
        else {
            // Minimum scamp version required: 2.0.4
            int version = list[2].remove('.').toInt();
            if (version < 204) {
                scampDep = "'Scamp' v2.0.4 or later required.\nhttps://github.com/astromatic/scamp\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (swarp.isEmpty()) {
        swarpDep = "'Swarp' v2.38.0 or later required (working binary names: 'swarp, Swarp or SWarp').\nhttps://github.com/astromatic/swarp\n";
    }
    else {
        // Check if swarp has the right version
        QString swarpCommand = swarp + " -v";
        QString result = read_system_command(swarpCommand);
        QStringList list = result.split(" ");
        if (list.length() < 3) {
            qDebug() << "WARNING: Unexpected output format when checking swarp version:" << result;
            qDebug() << "WARNING: Required version: 2.38.0 or later";
        }
        else {
            // Minimum swarp version required: 2.38.0
            int version = list[2].remove('.').toInt();
            if (version < 2380) {
                swarpDep = "'swarp' v2.38.0 or later required.\nhttps://github.com/astromatic/swarp\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (sourceExtractor.isEmpty()) {
        sourceExtractorDep = "'Source Extractor' v2.19.5 or later required (working binary names: 'source-extractor, sextractor, SExtractor, or sex').\nhttps://github.com/astromatic/sextractor\n";
    }
    else {
        // Check if Source Extractor has the right version
        // expected formats are:
        // Source Extractor version 2.25.0 (2020-03-19)
        // Source Extractor version 2.25.0 (2018-02-08)
        QString sourceExtractorCommand = sourceExtractor + " -v";
        QString result = read_system_command(sourceExtractorCommand);
        QString resultOrig = result;
        result.remove("SExtractor", Qt::CaseInsensitive);
        result.remove("Source", Qt::CaseInsensitive);
        result.remove("Extractor", Qt::CaseInsensitive);
        result.remove("version", Qt::CaseInsensitive);
        result = result.simplified();

        QStringList list = result.split(" ");
        if (list.length() != 2) {
            qDebug() << "WARNING: Unexpected output format when checking Source Extractor version:" << resultOrig;
            qDebug() << "WARNING: Required version: 2.19.5 or later";
        }
        else {
            // Minimum Source Extractor version required: 2.19.5
            int version = list[0].remove('.').toInt();
            if (version < 2195) {
                sourceExtractorDep = "'Source Extractor' v2.19.5 or later required.\nhttps://github.com/astromatic/sextractor\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (missingDep == 0) return;

    QString title = "Missing dependency:\n\n";
    if (missingDep > 1) title = "Missing dependencies:\n\n";

    QMessageBox::critical(0, "THELI", title
                          +scampDep
                          +swarpDep
                          +pythonDep
                          +sourceExtractorDep,
                          QMessageBox::Ok);
    exit (1);
}
