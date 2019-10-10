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

    // Read the THELIDIR environment variable
    if (!QProcessEnvironment::systemEnvironment().contains("THELIDIR")) {
        QMessageBox::critical(0,"THELI","The THELIDIR environment variable was not set!\n\n"
                                        "For example, if THELI was installed under"
                                        ""+QDir::homePath()+"/THELI/ and you are using the 'bash' shell,"
                                                            "then you would include the following line in your .bashrc file:\n\n"
                                                            "export THELIDIR="+QDir::homePath()+"/THELI/",
                              QMessageBox::Ok);
        exit (1);
    }

    // Dependency checks
    dependencyCheck();

    // Required when starting the first time
    QString dotTheliName = QDir::homePath()+"/.theli/";
    QDir dotTheli(dotTheliName);
    dotTheli.mkdir(dotTheliName);
    dotTheli.mkdir(dotTheliName+"/scripts");
    dotTheli.mkdir(dotTheliName+"/tmp");
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
    QString vizquery = QStandardPaths::findExecutable("vizquery.py");
    QString sesame = QStandardPaths::findExecutable("sesame");
    QString scamp = QStandardPaths::findExecutable("scamp");
    QString swarp = QStandardPaths::findExecutable("swarp");
    QString sex = QStandardPaths::findExecutable("sex");

    QString vizqueryDep = "";
    QString sesameDep = "";
    QString scampDep = "";
    QString swarpDep = "";
    QString sexDep = "";

    int missingDep = 0;

    if (vizquery.isEmpty()) {
        vizqueryDep = "'vizquery.py' required.\nhttp://cds.u-strasbg.fr/resources/doku.php?id=cdsclient\n\n";
        ++missingDep;
    }

    if (sesame.isEmpty()) {
        sesameDep = "'sesame' required.\nhttp://cdsarc.u-strasbg.fr/ftp/pub/sw/cdsclient.tar.gz\n\n";
        ++missingDep;
    }

    if (scamp.isEmpty()) {
        scampDep = "'Scamp' v2.7.7 or later required (binary name must be: 'scamp').\nhttps://github.com/astromatic/scamp\n";
        ++missingDep;
    }
    else {
        // Check if scamp has the right version
        QString scampCommand = "scamp -v";
        QString result = read_system_command(scampCommand);
        QStringList list = result.split(" ");
        if (list.length() < 3) {
            qDebug() << "WARNING: Unexpected output format when checking Scamp version:" << result;
            qDebug() << "WARNING: Required version: 2.7.7 or later";
        }
        else {
            // Minimum scamp version required: 2.7.7
            int version = list[2].remove('.').toInt();
            if (version < 277) {
                scampDep = "'Scamp' v2.7.7 or later required.\nhttps://github.com/astromatic/scamp\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (swarp.isEmpty()) {
        swarpDep = "'Swarp' v2.38.0 or later required (binary name must be: 'swarp').\nhttps://www.astromatic.net/software/swarp\n";
    }
    else {
        // Check if swarp has the right version
        QString swarpCommand = "swarp -v";
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
                swarpDep = "'swarp' v2.38.0 or later required.\nhttps://www.astromatic.net/software/swarp\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (sex.isEmpty()) {
        sexDep = "'SExtractor' v2.19.5 or later required (binary name must be: 'sex').\nhttps://www.astromatic.net/software/sextractor\n";
    }
    else {
        // Check if sextractor has the right version
        QString sexCommand = "sex -v";
        QString result = read_system_command(sexCommand);
        QStringList list = result.split(" ");
        if (list.length() < 3) {
            qDebug() << "WARNING: Unexpected output format when checking sex version:" << result;
            qDebug() << "WARNING: Required version: 2.19.5 or later";
        }
        else {
            // Minimum sex version required: 2.19.5
            int version = list[2].remove('.').toInt();
            if (version < 2195) {
                sexDep = "'SExtractor' v2.19.5 or later required.\nhttps://www.astromatic.net/software/sextractor\nInstalled:  " + result + "\n";
                ++missingDep;
            }
        }
    }

    if (missingDep == 0) return;

    QString title = "Missing dependency:\n\n";
    if (missingDep > 1) title = "Missing dependencies:\n\n";

    QMessageBox::critical(0, "THELI", title
                          +vizqueryDep
                          +scampDep
                          +swarpDep
                          +sexDep,
                          QMessageBox::Ok);
    exit (1);
}
