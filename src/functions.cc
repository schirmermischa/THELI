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

#include "functions.h"
#include "preferences.h"
#include "instrumentdata.h"
#include "tools/fitgauss1d.h"

#include <wcs.h>
#include <unordered_map>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QSysInfo>
#include <QVector>
#include <QSettings>
#include <QStandardItemModel>
#include <QStorageInfo>
#include <QProgressBar>
#include <QStandardPaths>

QString boolToString(bool test)
{
    if (test) return "Y";
    else return "N";
}

QStringList datadir2StringList(QLineEdit *lineEdit)
{
    QStringList list;
    QString entrystring = lineEdit->text();
    if (entrystring.isEmpty()) {
        return list;
    }
    entrystring.replace(",", " ");
    entrystring = entrystring.simplified();
    return entrystring.split(" ");
}

QString get_fileparameter(QFile *file, QString parametername, QString warn)
{
    if (file->fileName().isEmpty()) return "";  // Suppressing an error when the user launches THELI for the very first time

    if(!file->open(QIODevice::ReadOnly)) {
        qDebug() << __func__ << file->fileName() << file->errorString();
        return "";
    }

    QTextStream in(file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        if (!line.contains(parametername+"=")) continue;
        else {
            QString value = line.split("=")[1];
            file->close();
            return value;
        }
    }
    file->close();
    if (!warn.isEmpty()) {
        QMessageBox::warning(0, "Error reading parameter from config",
                             "Did not find variable "+parametername+" in "+file->fileName()+" !\nReturning empty string.");
    }
    return "";
}

void appendToFile(QString path, QString string)
{
    if (path.isEmpty()) return;

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << __func__ << file.fileName() << file.errorString();
        return;
    }

    QTextStream in(&file);
    in << string << "\n";
    file.close();
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void replaceLineInFile(const QString& filePath, const QString& searchString, const QString& replacementLine)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << __func__ << "Failed to open file " << file.errorString();
        return;
    }

    QTextStream in(&file);
    QStringList lines;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.contains(searchString)) {
            lines << replacementLine;
        } else {
            lines << line;
        }
    }

    file.resize(0);  // Clear the file content.

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << '\n';
    }

    file.close();
}

double extractFitsKeywordValue(const QString& filePath, const QString& keyword)
{
    fitsfile *fitsFile;
    int status = 0;

    if (fits_open_file(&fitsFile, filePath.toLocal8Bit().constData(), READONLY, &status)) {
        fits_report_error(stderr, status);
        return -1.0;
    }

    double extractedValue = 0.0;

    fits_read_key(fitsFile, TDOUBLE, keyword.toLocal8Bit().constData(), &extractedValue, NULL, &status);

    fits_close_file(fitsFile, &status);

    if (status) {
        fits_report_error(stderr, status);
        return -1.0;
    }

    return extractedValue;
}

/*
// same as above, but for .head files with additional (undesired) comment strings, and a card length of 8 chars
QString get_fileHeaderParameter(QFile *file, QString parametername)
{
    if(!file->open(QIODevice::ReadOnly)) {
        qDebug() << "get_fileparameter: "+file->fileName()+" "+file->errorString();
        return "";
    }

    QTextStream in(file);

    QString filename = file->fileName();

    while(!in.atEnd()) {
        QString line = in.readLine();
        QString keyName = parametername;
        keyName.resize(8, ' ');
        if (!line.contains(keyName+"=")) continue;
        else {
            QString value = line.split("=")[1];
            value.truncate(value.lastIndexOf('/'));
            value = value.simplified();
            file->close();
            return value;
        }
    }
    file->close();

    QMessageBox::warning(0, "Error reading parameter from config",
                         "Did not find variable "+parametername+" in "+filename+" !\nReturning empty string.");

    return "";
}
*/

QString sanityCheckWCS(const wcsprm *wcs)
{
    double det = wcs->cd[0]*wcs->cd[3] - wcs->cd[1]*wcs->cd[2];
    if (det == 0.) return "WCS matrix is singular: "+ QString::number(wcs->cd[0],'g',5) + QString::number(wcs->cd[1],'g',5) + QString::number(wcs->cd[2],'g',5) + QString::number(wcs->cd[3],'g',5);
    else {
        // Test for significant shear
        double pscale1 = sqrt(wcs->cd[0]*wcs->cd[0] + wcs->cd[2]*wcs->cd[2]);
        double pscale2 = sqrt(wcs->cd[1]*wcs->cd[1] + wcs->cd[3]*wcs->cd[3]);
        double meanPlateScale = 0.5*(pscale1+pscale2);

        // take out the pixel scale
        double cd11 = wcs->cd[0] / meanPlateScale;
        double cd12 = wcs->cd[1] / meanPlateScale;
        double cd21 = wcs->cd[2] / meanPlateScale;
        double cd22 = wcs->cd[3] / meanPlateScale;

        // CD matrix should be nearly orthogonal, i.e. det = +/- 1

        det = cd11*cd22 - cd12*cd21;

        if (fabs(det)-1. > 0.05) {
            return "WCS matrix is significantly sheared";
        }
    }
    return "";
}

/*
void listSwapLastPairs(QStringList &stringlist, int n)
{
    // At some point during creation of the commandList, we need to swap pairs
    // or triplets of execution commands and log scans (meesage, run, scan).
    // E.g. Before: ... L3 L2 L1 L0    then:  L1 L0 L3 L2
    // E.g. Before: ... L5 L4 L3 L2 L1 L0    then:  L2 L1 L0 L5 L4 L3

    if (n<2 || n>4) {
        qDebug() << "listSwapLastPairs: n must be 2, 3 or 4, you have used " << n;
        return;
    }

    int l0 = stringlist.length()-1;
    int l1 = l0-1;
    int l2 = l0-2;
    int l3 = l0-3;
    int l4 = l0-4;
    int l5 = l0-5;
    int l6 = l0-6;
    int l7 = l0-7;
    if (n==2) {
        stringlist.swap(l2,l0);
        stringlist.swap(l3,l1);
    }
    if (n==3) {
        stringlist.swap(l3,l0);
        stringlist.swap(l4,l1);
        stringlist.swap(l5,l2);
    }
    if (n==4) {
        stringlist.swap(l4,l0);
        stringlist.swap(l5,l1);
        stringlist.swap(l6,l2);
        stringlist.swap(l7,l3);
    }
}
*/

/*
QString translateServer(QString downloadServer)
{
    QString server;
    if ( downloadServer == "France" ) server = "vizier.unistra.fr";
    else if ( downloadServer == "USA" ) server = "vizier.cfa.harvard.edu";
    else if ( downloadServer == "Canada" ) server = "vizier.hia.nrc.ca";
    else if ( downloadServer == "Japan" ) server = "vizier.nao.ac.jp";
    else if ( downloadServer == "India" ) server = "vizier.iucaa.in";
    else if ( downloadServer == "China" ) server = "vizier.china-vo.org";
    else if ( downloadServer == "UK" ) server = "vizier.ast.cam.ac.uk";
    else server = "vizieridia.saao.ac.za";
    return server;
}
*/

// UNUSED
void selectFirstActiveItem(QComboBox *cb)
{
    // This function selects the first enabled item in a combobox
    // (if the enabled status has changed from the outside)
    const QStandardItemModel* model = dynamic_cast< QStandardItemModel * >( cb->model() );
    for (int i=0; i<cb->count(); ++i) {
        if (model->item(i,0)->isEnabled()) cb->setCurrentIndex(i);
        break;
    }
}

long numFilesDir(QString path, QString filter)
{
    QDir dir(path);
    long numFiles;
    if (dir.exists()) {
        QStringList filterList;
        filterList << filter;
        dir.setNameFilters(filterList);
        QStringList fileList = dir.entryList();
        numFiles = fileList.length();
    }
    else numFiles = 0;

    return numFiles;
}

/*
// Get the first vector entry from a parameter in the camera.ini files
QString get_fileparameter_vector(QFile *file, QString parametername, QString warn)
{
    if(!file->open(QIODevice::ReadOnly)) {
        qDebug() << "QDEBUG:: get_fileparameter_vector: "+file->fileName()+" "+file->errorString();
        return "";
    }

    QTextStream in(file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        if (!line.contains(parametername+"=")) continue;
        else {
            QString value = line.split("=").at(2).split(" ").at(0);
            value.replace(")","");
            value = value.simplified();
            file->close();
            return value;
        }
    }
    file->close();
    if (!warn.isEmpty()) {
        QMessageBox::warning(0, "Error reading parameter from config",
                             "Did not find variable "+parametername+" in "+file->fileName()+" !\nReturning empty string.");
    }
    return "";
}
*/

/*
// Get the full vector entry from a parameter in the camera.ini files
// THE FILE IS OPENED / CLOSED EXTERNALLY to avoid repeated file handle operations
// NOTE: already subtracting -1 to make it conform with C++ indexing
QVector<int> get_fileparameter_FullVector(QFile *file, QString parametername)
{
    QVector<int> result;
    // Reset file to beginning
    file->seek(0);
    QTextStream in(file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (!line.contains(parametername+"=")) continue;
        line = line.replace('=',' ').replace(')',' ').replace(")","");
        line = line.simplified();
        QStringList values = line.split(" ");
        for (int i=2; i<values.length(); i=i+2) {
            result.push_back(values.at(i).toInt() - 1);
        }
        return result;
    }
    // Return empty vector if nothing found.
    return result;
}
*/

void showLogfile(QString logname, QString line)
{
    QSettings settings("THELI", "PREFERENCES");
    QString editorPreference = settings.value("prefEditorComboBox").toString();
    // in case we want to go directly to a specific error line:
    if (line != "") {
        if (editorPreference == "emacs") editorPreference = editorPreference + " +"+line;
        else if (editorPreference == "kate") editorPreference = editorPreference + " -l "+line;
        else if (editorPreference == "gedit") editorPreference = editorPreference + " +"+line;
    }
    QString execstr = editorPreference;
    execstr.append(" ");
    execstr.append(logname);

    if (!QFile(logname).exists()) {
        QMessageBox msgBox;
        msgBox.setText("The logfile "+logname+" does not yet exist.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QProcess process;
    bool started = process.startDetached(execstr);

    if (!started) {
        QMessageBox msgBox;
        msgBox.setText("The editor could not be launched. Please specify a suitable program in the preferences.\n\n"
                       "The executable must be found in your PATH variable.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        // Let the user pick a working editor right away
        Preferences *preferences = new Preferences(false);
        preferences->show();
    }
}

void message(QPlainTextEdit *pte, QString text, QString type)
{
    if (type == "ignore") {
        pte->appendHtml(text);
        return;
    }

    if (type == "append") {
        pte->moveCursor(QTextCursor::End);
        pte->appendHtml(" "+text);
        pte->moveCursor(QTextCursor::End);
        return;
    }

    QString color;
    if (type == "") {
        if (text.contains("STOP")) color = "#ee0000";
        else if (text.contains("NOTE")) color = "#00aa00";
        else if (text.contains("INFO")) color = "#0000dd";
        else color = "#000000";
    }
    else if (type == "stop") color = "#ee0000";    // red
    else if (type == "error") color = "#ee0000";   // red
    else if (type == "warning") color = "#ee5500";   // orange
    else if (type == "note") color = "#009955";    // green
    else if (type == "image") color = "#000000";   // black
    else if (type == "info") color = "#0033cc";    // blue
    else if (type == "data") color = "#0033cc";    // blue
    else if (type == "output") color = "#8800aa";  // purple
    else if (type == "controller") color = "#8800aa";  // purple
    else if (type == "config") color = "#006666";  // turquois
    else color = "#000000";                        // black

    //    if (type == "image") text.prepend("IMAG: ");
    //    if (type == "data") text.prepend("DATA: ");
    //    else if (type == "controller") text.prepend("CTRL: ");
    if (type == "error") text.prepend("ERROR: ");

    pte->appendHtml("<font color=\""+color+"\">"+text+"</font>");
}

void fill_combobox(QComboBox *combobox, QString string)
{
    QStringList list = string.split(" ");
    combobox->clear();
    combobox->addItems(list);
}

void paintPathLineEdit(QLineEdit *linedit, QString name, QString check)
{
    QPalette palette_exists, palette_notexists, palette_empty;
    palette_exists.setColor(QPalette::Base,QColor("#a9ffe6"));
    palette_notexists.setColor(QPalette::Base,QColor("#ff99aa"));
    palette_empty.setColor(QPalette::Base,QColor(255,255,255));

    // White background if field is empty. Return if there is nothing else to do.
    if (name.isEmpty()) {
        linedit->setPalette(palette_empty);
        return;
    }

    // Green background if dir/file exists, orange otherwise
    if (check == "dir") {
        QDir dir = QDir(name);
        if (dir.exists()) linedit->setPalette(palette_exists);
        else linedit->setPalette(palette_notexists);
    }
    else {
        QFile file(name);
        QFileInfo fileinfo(file);
        if (file.exists() && fileinfo.isFile()) linedit->setPalette(palette_exists);
        else linedit->setPalette(palette_notexists);
    }
}

long remainingDataDriveSpace(QString maindir)
{
    // Remaining space in MB on the data drive
    double GBfree_data = 0.;

    QStorageInfo storage_data(maindir);
    if (storage_data.isValid() && storage_data.isReady()) {
        GBfree_data = storage_data.bytesAvailable()/1024./1024.;
    }

    return long(GBfree_data);
}

/*
// Execute a shell command, ignore output;
// If you want to send it to the background and return immediately, append "&" to shell_command
void exec_system_command(QString shell_command)
{
    QProcess process;
    process.start("/bin/sh -c \""+shell_command+"\"");
    process.waitForFinished(-1);
}
*/

// Execute a shell command, and retrieve single line output
QString read_system_command(QString shell_command)
{
    QProcess process;
    process.start("/bin/sh -c \""+shell_command+"\"");
    process.waitForFinished(-1);
    QString result(process.readLine());
    return result;
}

/*
// Execute a shell command, and retrieve multiple line output
QString read_MultipleLines_system_command(QString shell_command)
{
    QProcess process;
    process.start("/bin/sh -c \""+shell_command+"\"");
    process.waitForFinished(-1);
    QByteArray ba = process.readAllStandardOutput();
    QString result = QString::fromStdString(ba.toStdString());
    return result;
}
*/

void initEnvironment(QString &thelidir, QString &userdir)
{
    // If the THELIDIR variable is NOT set, try finding THELI in the system path
    if (!QProcessEnvironment::systemEnvironment().contains("THELIDIR")) {
        QDir thelidir1("/usr/share/theli/config/");
        QDir thelidir2("/usr/share/theli/python/");
        if (thelidir1.exists() && thelidir2.exists()) {
            thelidir = "/usr/share/theli/";
        }
    }
    else {
        thelidir = QProcessEnvironment::systemEnvironment().value("THELIDIR","");
    }

    QSysInfo *sysInfo = new QSysInfo;

    // Are we running "linux" or "darwin"?
    QString kernelType = sysInfo->kernelType(); // returns "linux" or "darwin"

    // Capitalize first letter
    kernelType.replace(0, 1, kernelType[0].toUpper());

    // Are we 64-bit Linux?
    QString arch = sysInfo->currentCpuArchitecture();
    if (kernelType == "Linux" && arch.contains("64")) kernelType = "Linux_64";

    userdir = QDir::homePath()+"/.theli/";

    // Simplify strings
    thelidir.replace("//","/");
    userdir.replace("//","/");

    delete sysInfo;
    sysInfo = nullptr;
}

QString findExecutableName(QString program)
{
    QStringList sourceextractorlist = {"source-extractor", "sex", "sextractor", "SExtractor"};
    QStringList scamplist = {"scamp", "Scamp"};
    QStringList swarplist = {"SWarp", "Swarp", "swarp"};
    QStringList anetlist1 = {"solve-field"};
    QStringList anetlist2 = {"build-astrometry-index"};
    QStringList pythonlist = {"python3", "python"};

    QString commandname = "";
    if (program == "source-extractor") {
        for (auto &it : sourceextractorlist) {
            commandname = QStandardPaths::findExecutable(it);
            if (!commandname.isEmpty()) break;
        }
    }
    else if (program == "scamp") {
        for (auto &it : scamplist) {
            commandname = QStandardPaths::findExecutable(it);
            if (!commandname.isEmpty()) break;
        }
    }
    else if (program == "swarp") {
        for (auto &it : swarplist) {
            commandname = QStandardPaths::findExecutable(it);
            if (!commandname.isEmpty()) break;
        }
    }
    else if (program == "solve-field") {
        for (auto &it : anetlist1) {
            commandname = QStandardPaths::findExecutable(it);
            if (commandname.isEmpty()) {
                QStringList paths;
                paths << "/usr/local/Astrometry/bin/";
                paths << "/usr/local/astrometry/bin/";
                commandname = QStandardPaths::findExecutable(it, paths);
            }
            if (!commandname.isEmpty()) break;
        }
    }
    else if (program == "build-astrometry-index") {
        for (auto &it : anetlist2) {
            commandname = QStandardPaths::findExecutable(it);
            if (commandname.isEmpty()) {
                QStringList paths;
                paths << "/usr/local/Astrometry/bin/";
                paths << "/usr/local/astrometry/bin/";
                commandname = QStandardPaths::findExecutable(it, paths);
            }
            if (!commandname.isEmpty()) break;
        }
    }
    else if (program == "python") {
        for (auto &it : pythonlist) {
            commandname = QStandardPaths::findExecutable(it);
            if (!commandname.isEmpty()) {
                break;
            }
        }
    }

    return commandname;
}

void killProcessChildren(qint64 parentProcessId) {

    QProcess get_children;
    QStringList get_children_cmd;
    get_children_cmd << "--ppid" << QString::number(parentProcessId) << "-o" << "pid" << "--no-heading";
    get_children.start("ps", get_children_cmd);
    get_children.waitForFinished();
    QString childIds(get_children.readAllStandardOutput());
    childIds.replace('\n', ' ');

    if (childIds.isEmpty()) return;

    QProcess::execute("kill -9 " + childIds);
}

// Returns the memory in kB
long get_memory()
{
    QSysInfo *sysInfo = new QSysInfo;
    QString kernelType = sysInfo->kernelType();
    QString memory;

    // TODO: use sysctl interface instead
    if (kernelType == "linux") {
        QProcess p;
        p.start("awk", QStringList() << "/MemTotal/ { print $2 }" << "/proc/meminfo");
        p.waitForFinished(-1);
        memory = p.readAllStandardOutput();
        memory = memory.simplified();
        p.close();
    }

    if (kernelType == "darwin") {
        QProcess p;
        p.start("sysctl", QStringList() << "hw.memsize");     // alternative: read output from /usr/bin/vm_stat
        // or 'hostinfo | grep memory | awk '{print $4}'    // returns RAM in GB
        p.waitForFinished(-1);
        QString system_info = p.readAllStandardOutput();
        QStringList list = system_info.split(' ');
        QString memory = list[1];
        p.close();
        delete sysInfo;
        sysInfo = nullptr;
        return memory.toFloat()/1024;  // CHECK normalization!
    }

    delete sysInfo;
    sysInfo = nullptr;
    return memory.toLong();
}

/*
bool listContains(QStringList stringList, QString string)
{
    bool test = false;
    for (auto &it : stringList) {
        if (it.contains(string)) test = true;
    }
    return test;
}
*/

QString removeLastWords(QString string, int n, const QChar sep)
{
    int i = 0;
    while (i<n) {
        string.truncate(string.lastIndexOf(sep));
        ++i;
    }
    return string;
}

QString getNthWord(QString string, int n, const QChar sep)
{
    QStringList list = string.split(sep);
    if (n<1 || n>list.length()) return "";
    else return list.at(n-1);
}

QString getLastWord(QString string, const QChar sep)
{
    QStringList list = string.split(sep);
    return list.last();
}

QString get2ndLastWord(QString string, const QChar sep)
{
    QStringList list = string.split(sep);
    long index = list.count(); // if using 'int', the compiler throws a 'signed overflow' warning
    if (index < 2) return "";
    else {
        return list.at(index-2);
    }
}

void removeLastCharIf(QString &string, const QChar character)
{
    int n = string.length();
    if (string.at(n-1) == character) string.truncate(n-1);
}

bool hasMatchingPartnerFiles(QString dirname1, QString suffix1, QString dirname2, QString suffix2, QString infomessage)
{
    // This function checks whether e.g. all science exposures have a weight, or astrometric header
    QDir dir1(dirname1);
    QDir dir2(dirname2);
    QStringList notMatched;
    QStringList filter1("*"+suffix1);
    QStringList filter2("*"+suffix2);
    dir1.setNameFilters(filter1);
    dir2.setNameFilters(filter2);
    dir1.setSorting(QDir::Name);
    dir2.setSorting(QDir::Name);
    QStringList list1 = dir1.entryList();
    QStringList list2 = dir2.entryList();
    list1.replaceInStrings(suffix1,"");
    list2.replaceInStrings(suffix2,"");
    // If entries are equal, or list1 is fully contained in list2, then we can leave

    // Equal?
    if (list1.operator ==(list2)) {
        return true;
    }
    else {
        // Fully contained?
        for (auto & it : list1) {
            // If not contained in list2, add it to the list of missing items
            if (!list2.contains(it)) notMatched << it;
        }
    }

    if (notMatched.length() == 0) return true;
    else {
        QString missingItems;
        int i = 0;
        for (auto &it: notMatched) {
            if (i>19) break;
            missingItems.append(it);
            missingItems.append(suffix1);
            missingItems.append("\n");
            ++i;
        }
        QMessageBox msgBox;
        msgBox.setText(infomessage);
        long nbad = notMatched.length();
        if (nbad <= 20) {
            msgBox.setInformativeText("The following "+QString::number(nbad)+" files do not have a match:\n"+missingItems);
        }
        else {
            msgBox.setInformativeText("The following is a list of the first 20 (out of "+QString::number(nbad)+") files that did not have a match:\n"+missingItems);
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }
}

void get_array_subsample(const QVector<float> &data, QVector<double> &datasub, int stepSize)
{
    // Select the small array
    for (long i=0; i<data.length(); i+=stepSize) {
        datasub.append(data[i]);
    }
}

// Truncate a list of images for nicer display in error message boxes
QString truncateImageList(QStringList list, int dim)
{
    QStringList listCopy = list;
    int numbad = listCopy.length();
    if (numbad > dim) listCopy.erase(listCopy.begin()+dim, listCopy.end());
    QString summary = listCopy.join(" ").replace(" ", "\n");
    if (numbad > dim) {
        summary.append("\n[...]\n\n");
        summary.append("Total number of affected images: "+QString::number(numbad)+"\n");
    }
    return summary;
}

// Masked mad
double madMask(const QVector<double> &vector_in, const QVector<bool> &mask, QString ignoreZeroes)
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    QVector<double> vector;
    vector.reserve(maxDim);

    long i = 0;
    if (!mask.isEmpty()) {
        for (auto &it: vector_in) {
            if (!mask[i] && ignoreZeroes == "") vector.append(it);
            if (!mask[i] && ignoreZeroes != "" && it != 0.) vector.append(it);
            ++i;
        }
    }
    else {
        for (auto &it: vector_in) {
            if (ignoreZeroes == "") vector.append(it);
            if (ignoreZeroes != "" && it != 0.) vector.append(it);
            ++i;
        }
    }

    QVector<double> diff;
    diff.reserve(maxDim);
    if (vector.size() == 0) return 0.;
    else {
        double med = straightMedian_T(vector);
        for (int i=0; i<vector.size(); ++i) {
            diff.append(fabs(vector.at(i)-med));
        }
        return straightMedian_T(diff);
    }
}

// Masked median_err
double medianerrMask(const QVector<double> &vector_in, const QVector<bool> &mask)
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    QVector<double> vector;
    vector.reserve(maxDim);
    if (!mask.isEmpty()) {
        long i = 0;
        for (auto &it: vector_in) {
            if (!mask[i]) vector.append(it);
            ++i;
        }
    }
    else {
        for (auto &it: vector_in) {
            vector.append(it);
        }
    }

    if (vector.size() == 0) return 0.;
    else {
        // https://en.wikipedia.org/wiki/Median_absolute_deviation#Relation_to_standard_deviation
        return madMask(vector)*1.4826 / std::sqrt(vector.size());
    }
}

bool moveFiles(QString filter, QString sourceDirName, QString targetDirName)
{
    QDir dir1(sourceDirName);
    QDir dir2(targetDirName);
    if (!dir1.exists() || !dir2.exists()) {
        qDebug() << __func__ << "ERROR: One or both of these directories does not exist:" << dir1 << dir2;
        return false;
    }
    QStringList filterList(filter);
    QStringList fileList = dir1.entryList(filterList);
    for (auto &it: fileList) {
        // In Qt5, an existing file must be removed first before a new one with the same name can be moved there
        QFile testFile(dir2.absolutePath()+"/"+it);
        if (testFile.exists()) {
            if (!testFile.remove()) return false;
        }
        QFile file(dir1.absolutePath()+"/"+it);
        if (!file.rename(dir1.absolutePath()+"/"+it, dir2.absolutePath()+"/"+it)) return false;
    }
    return true;
}

bool deleteFile(QString fileName, QString path)
{
    QFile testFile(path+"/"+fileName);
    if (testFile.exists()) {
        if (!testFile.remove()) return false;
    }
    return true;
}

bool moveFile(QString filename, QString sourceDirPath, QString targetDirPath, bool skipNonExistingFile)
{
    QDir targetDir(targetDirPath);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetDirPath)) {
            qDebug() << __func__ << "ERROR: Could not create directory " << targetDirPath;
            // mkpath() returns true if the directory already exists,
            // hence if we are here then something really went wrong
            return false;
        }
    }

    // In Qt5, an existing file must be removed first before a new one with the same name can be moved there
    QFile testFile(targetDirPath+"/"+filename);
    if (testFile.exists()) {
        if (!testFile.remove()) return false;
    }

    QFile file(sourceDirPath + "/" + filename);
    if (skipNonExistingFile && !file.exists()) return true;

    if (!file.rename(targetDirPath + "/" + filename)) {
        QString operation = "mv " + sourceDirPath+"/"+filename + " " + targetDirPath+"/"+filename;
        qDebug() << __func__ << "ERROR: Could not execute this operation:\n" << operation;
        return false;
    }

    return true;
}

void mkAbsDir(QString absDirName)
{
    QDir targetDir(absDirName);
    if (!targetDir.exists()) targetDir.mkpath(absDirName);
}

QString hmsToDecimal(QString hms)
{
    hms = hms.simplified();
    hms = hms.replace(' ',':');
    QStringList list = hms.split(':');
    double hh = list.at(0).toDouble();
    double mm = list.at(1).toDouble();
    double ss = list.at(2).toDouble();
    if (list.length() != 3) {
        qDebug() << "Invalid format for the RA string.";
        return "0.0";
    }
    mm /= 60.;
    ss /= 3600.;
    double decimal = 15.*(hh+mm+ss);
    return QString::number(decimal,'f',12);
}

QString dmsToDecimal(QString dms)
{
    dms = dms.simplified();
    dms = dms.replace(' ',':');
    QStringList list = dms.split(':');
    if (list.length() != 3) {
        qDebug() << "Invalid format for the Dec string.";
        return "0.0";
    }
    double dd = list.at(0).toDouble();
    double mm = list.at(1).toDouble();
    double ss = list.at(2).toDouble();
    // Cannot use sgn() function here, as it returns 0 when argument is (-0)
    int sign = 1;
    if (list.at(0).contains("-")) sign = -1;
    mm /= 60.;
    ss /= 3600.;
    double decimal = (fabs(dd)+mm+ss)*sign;
    return QString::number(decimal,'f',12);
}

QString decimalSecondsToHms(float value)
{
    double h;
    double m;
    double s;
    value /= 3600.;
    value = 60.0 * modf(value, &h);
    s = 60.0 * modf(value, &m);
    QString hh;
    QString mm;
    QString ss;
    if (h<10) hh = '0'+QString::number(int(h));
    else hh = QString::number(int(h));
    if (m<10) mm = '0'+QString::number(int(m));
    else mm = QString::number(int(m));
    ss = QString::number(s, 'f', 3);
    return hh+":"+mm+":"+ss;
}

QString decimalToHms(float value)
{
    double h;
    double m;
    double s;
    value /= 15.;
    value = 60.0 * modf(value, &h);
    s = 60.0 * modf(value, &m);
    QString hh;
    QString mm;
    QString ss;
    if (h<10) hh = '0'+QString::number(int(h));
    else hh = QString::number(int(h));
    if (m<10) mm = '0'+QString::number(int(m));
    else mm = QString::number(int(m));
    ss = QString::number(s, 'f', 3);
    if (s<10) ss = '0'+ss;
    return hh+":"+mm+":"+ss;
}

QString decimalToDms(float value)
{
    QString sign = "";
    if (value < 0.) {
        value *= -1.;
        sign = "-";
    }
    double d;
    double m;
    double s;
    value = 60.0 * modf(value, &d);
    s = 60.0 * modf(value, &m);
    QString dd;
    QString mm;
    QString ss;
    if (d<10) dd = '0'+QString::number(int(d));
    else dd = QString::number(int(d));
    if (m<10) mm = '0'+QString::number(int(m));
    else mm = QString::number(int(m));
    ss = QString::number(s, 'f', 3);
    if (s<10) ss = '0'+ss;
    return sign+dd+":"+mm+":"+ss;
}

double dateobsToDecimal(QString dateobs)
{
    // dateobs format: YYYY-MM-DDTHH:MM:SS.sss
    if (!dateobs.contains("T")
            || !dateobs.contains(":")
            || !dateobs.contains("-")) {
        qDebug() << __func__ << "ERROR: DATE-OBS string " << dateobs << "does not have YYYY-MM-DDTHH:MM:SS.sss format!";
        return 0.;
    }
    QStringList list = dateobs.split("T");
    QString date = list[0];
    QString time = list[1];
    QStringList datelist = date.split("-");
    QStringList timelist = time.split(":");
    if (datelist.length() != 3
            || timelist.length() != 3) {
        qDebug() << __func__ << "ERROR: DATE-OBS string " << dateobs << "does not have YYYY-MM-DDTHH:MM:SS.sss format!";
        return 0.;
    }
    double year = datelist[0].toDouble();
    double month = datelist[1].toDouble();
    double day = datelist[2].toDouble();
    double hh = timelist[0].toDouble();
    double mm = timelist[1].toDouble();
    double ss = timelist[2].toDouble();

    return year + month/12. + day/365.25 + hh/8766. + mm/525960. + ss/3.15576e7;
}

QVector<float> getSmallSample(const QVector<float> &data, const QVector<bool> &mask)
{
    long n = data.length();

    // A small sample for quick first estimates
    QVector<float> sample;
    long step = 1;
    // A prime number whose integer multiples do not get close to pow(2^n)
    // and therefore it samples a typical detector in a random-like fashion
    if (n>10000) {
        step = 379;
        sample.reserve(n/379);
    }
    else sample.reserve(n);

    if (!mask.isEmpty()) {
        for (long i=0; i<n; i+=step) {
            if (!std::isnan(data[i]) && !std::isinf(data[i]) && !mask[i])
                sample.append(data[i]);
        }
    }
    else {
        for (long i=0; i<n; i+=step) {
            if (!std::isnan(data[i]) && !std::isinf(data[i]))
                sample.append(data[i]);
        }
    }
    return sample;
}

// A fast mode calculator
// Optionally, it also provides an rms estimate based on the truncated histogram
QVector<float> modeMask(const QVector<float> &data, QString mode, const QVector<bool> &mask, bool smooth)
{
    QVector<float> sky;

    long n = data.length();
    if (n == 0) return sky << 0. << -1.;
    if (!mask.isEmpty() && n != mask.length()) {
        qDebug() << __func__ << "ERROR: Mask is not empty but has different size than data vector." << n << mask.length();
        return sky << 0. << -1.;
    }

    // Work with values within -3 MAD to 3 MAD (about +/- 2 sigma). Estimated from a small sample (at least 10000 data points)
    QVector<float> sample = getSmallSample(data, mask);
    float medianVal = straightMedian_T(sample);
    float madVal = madMask_T(sample);
    float minVal = medianVal - 3.*madVal;
    float maxVal = medianVal + 3.*madVal;
    float skySigma = 1.3*madVal;  // estimate of sigma based on MAD value (normally 1.48, but we want the clipped range without astrophysical objects)

    // Optimal step size for 100 bins, yielding an average S/N per bin of of 10
    int numBins = 100;
    int sampleDensity = modeMask_sampleDensity(n, numBins, 10);

    // Clip data, use every sampleDensity data point only
    QVector<float> dataClipped = modeMask_clipData(data, mask, sampleDensity, minVal, maxVal);
    if (dataClipped.isEmpty()) return sky << 0. << -1.;

    // Too few data points for mode:
    if (dataClipped.length() < 1000) return sky << medianVal << skySigma;

    // Build histogram (smoothed by default with a kernel 0.5*madVal wide)
    float rescale = 1.0;    // the projection factor from the original data to normalized integer range corresponding to [minVal, maxVal]
    QVector<long> histogram = modeMask_buildHistogram(dataClipped, rescale, numBins, minVal, maxVal, madVal, smooth);

    // Find the histogram peak with various methods (data dependent)
    float skyValue = 0.;
    if (mode == "classic") modeMask_classic(histogram, skyValue);
    else if (mode == "gaussian") modeMask_gaussian(histogram, skyValue);
    else if (mode == "stable") modeMask_stable(histogram, skyValue);
    else {
        // to be implemented
    }

    // Project back onto original scale
    skyValue = skyValue / rescale + minVal;
    // skySigma = skySigma / rescale;        // if skySigma was estimated from the histogram

    return sky << skyValue << skySigma;
}

// MODE: Determine optimal sample density
int modeMask_sampleDensity(long numDataPoints, int numBins, float SNdesired)
{
    // We use only every n-th data point (n = sampleDensity) to speed up calculations.
    // The sample density is chosen such that it yields, on average, the desired S/N per bin (assuming Poisson distribution)
    long sampleDensity = numDataPoints / (numBins * SNdesired * SNdesired);
    if (sampleDensity == 0) sampleDensity = 1;  // max density for few data points
    if (sampleDensity > 10) sampleDensity = 10; // min density; we don't mind good S/N in the histogram

    return sampleDensity;
}

// MODE: Clip data, use every sampleDensity data point only
QVector<float> modeMask_clipData(const QVector<float> &data, const QVector<bool> &mask, int sampleDensity, float minVal, float maxVal)
{
    QVector<float> dataThresholded;
    long n = data.length();
    dataThresholded.reserve(n/sampleDensity);
    if (!mask.isEmpty()) {
        for (long i=0; i<n; i+=sampleDensity) {
            float it = data[i];
            if (it > minVal && it < maxVal && !mask[i]) {
                dataThresholded.append(it);
            }
        }
    }
    else {
        for (long i=0; i<n; i+=sampleDensity) {
            float it = data[i];
            if (it > minVal && it < maxVal) {
                dataThresholded.append(it);
            }
        }
    }

    return dataThresholded;
}

// MODE: Build histogram
QVector<long> modeMask_buildHistogram(QVector<float> &data, float &rescale, const int numBins, const float minVal,
                                      const float maxVal, const float madVal, const bool smooth)
{
    // Project data onto integer values, spread numBins over normalized [minVal, maxVal] range
    rescale = float(numBins) / (maxVal - minVal);
    for (auto &it: data) {
        it = int (rescale * (it-minVal));
        // Stay within bounds
        // It can rarely happen that it == numBins for it=max; floating point round-off issue, causing histogram[it] to fail;
        if (it < 0) it = 0;
        if (it >= numBins) it = numBins-1;
    }

    // Now accumulate the histogram (bin 0 corresponds to minVal, bin numBin to maxVal)
    QVector<long> histogram(numBins);
    for (auto &it : data) {
        histogram[it]++;
    }

    // Optionally, smooth the histogram (true by default)
    // The smoothing is done with a Gaussian that is 'width' bins wide, where width is half the MAD value, but at least 3 bins wide
    int width = 0.5 * madVal * rescale;
    if (width < 3) width = 3;

    //    if (histogram[20] == 0 && histogram[21] == 0 && histogram[22] == 0 && histogram[23] == 0) {
    //        qDebug() << data;
    //        qDebug() << histogram;
    //    }
    if (smooth) smooth_array_T(histogram, width);
    //    if (histogram[20] == 0 && histogram[21] == 0 && histogram[22] == 0 && histogram[23] == 0) {
    //        qDebug() << " ";
    //        qDebug() << data;
    //        qDebug() << histogram;
    //    }

    return histogram;
}

// Classical way to find peak in the histogram: look for the highest bin
void modeMask_classic(const QVector<long> histogram, float &skyValue)
{
    float hist_max = 0.;
    int hist_x = 0;
    long i = 0;
    for (auto &it : histogram) {
        if (it > hist_max) {
            hist_x = i;
            hist_max = it;
        }
        ++i;
    }
    skyValue = hist_x;
}

// Find the peak in the histogram using a Gaussian fit
// WARNING: Does not work if image contains large number of padded or constant pixels,
// creating (multiple) huge histogram spikes, failing the GSL fit.
void modeMask_gaussian(QVector<long> histogram, float &skyValue)
{
    int numBins = histogram.length();

    // Starting values
    double amplitude = maxVec_T(histogram);
    double meanVal = numBins/2;
    double sigmaVal = rmsMask_T(histogram);
    // Arguments 2, 3 and 4 are starting values for amplitude, mean and sigma
    QVector<float> fitResult = fitGauss1D(histogram, amplitude, meanVal, sigmaVal);
    skyValue = fitResult[1];
    //    skySigma = fitResult[2];
}

// Robust way: determine 50% cut-on and cut-off values, and return the mean of data points in between
// This will work if the data is not unimodular (e.g. if gradients are present in an image)
void modeMask_stable(const QVector<long> histogram, float &skyValue)
{
    int numBins = histogram.length();

    double amplitude = maxVec_T(histogram);
    // Locate the classic peak
    modeMask_classic(histogram, skyValue);

    // Extreme limits
    int cutOn = 0;
    int cutOff = numBins - 1;
    // Update limits
    // Take the peak and go left until the 50% value is reached
    for (int i=skyValue; i>=0; --i) {
        if (histogram[i] <= 0.5*amplitude) {
            cutOn = i;
            break;
        }
    }
    // Take the peak and go right until the 50% value is reached
    for (int i=skyValue; i<numBins; ++i) {
        if (histogram[i] <= 0.5*amplitude) {
            cutOff = i;
            break;
        }
    }

    // Image might have a strong tail towards high values, and the upper 50% limit isn't found.
    // In this case, use a symmetric cut off:
    if (cutOff == numBins-1) {
        cutOff = skyValue + skyValue - cutOn;
        if (cutOff >= numBins) cutOff = numBins-1;
    }

    /*
    if (cutOn == 0 || cutOff == numBins) {
        qDebug() << "QDEBUG: functions:modeMask_stable():  Could not locate 50% cut-on and cut-off values for image statistics. Using full range.";
    }
    */
    if (cutOn == cutOff) {
        qDebug() << __func__ << "Error determining 50% cut-on and cut-off values for image statistics. Using middle quantile.";
        cutOn = 0.25*numBins;
        cutOff = 0.75*numBins;
    }

    //    qDebug() << cutOn << cutOff;
    // Calculate mean value within cutOn and cutOff
    skyValue = 0.;
    float weights = 0.;
    for (int i=cutOn; i<=cutOff; ++i) {
        skyValue += i*histogram[i];
        weights += histogram[i];
    }
    skyValue /= weights;
}


//***************************************************************************
// DUPLICATED from templates because I need them in connection with functors.
// These functions may call template functions themselves, though
//***************************************************************************

// Masked median
float medianMask(const QVector<float> &vector_in, const QVector<bool> &mask, long maxLength)
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    // fast-track
    if (mask.isEmpty()) {
        return straightMedian_T(vector_in, maxLength);
    }

    // Usually, we take all elements in a vector.
    // However, in some cases it may be that elements are masked and the vector is not entirely filled.
    // In this case, an optional maxLength argument is provided
    if (maxLength > 0 && maxLength < maxDim) maxDim = maxLength;

    QVector<float> vector;
    vector.reserve(maxDim);

    for (int i=0; i<maxDim; ++i) {
        if (!mask[i]) vector.append(vector_in[i]);
    }

    if (vector.size() == 0) return 0.;
    else {
        return straightMedian_T(vector, maxLength);
    }
}

// Changes data!
float straightMedian_MinMax(QVector<float> &data, const int nlow, const int nhigh)
{
    long maxDim = data.length();
    if (maxDim == 0) return 0.;
    if (nlow+nhigh >= maxDim) return 0;

    std::sort(data.begin(), data.end());
    float med;
    if (nhigh > 0) data.remove(maxDim-nhigh, nhigh);
    if (nlow > 0) data.remove(0, nlow);

    // Calculate average of central two elements if number is even
    long dsize = data.size();
    med = (dsize % 2)
            ? data[dsize / 2]
            : ((float)data[dsize / 2 - 1] + data[dsize / 2]) * .5;

    return med;
}

// Changes data!
float straightMedian_MinMax(QList<float> &data, const int nlow, const int nhigh)
{
    long maxDim = data.length();
    if (maxDim == 0) return 0.;
    if (nlow+nhigh >= maxDim) return 0;

    std::sort(data.begin(), data.end());
    float med;
    if (nhigh > 0) {
        for (int i=0; i<nhigh; ++i) data.pop_back();
    }
    if (nlow > 0) {
        for (int i=0; i<nhigh; ++i) data.pop_front();
    }

    // Calculate average of central two elements if number is even
    long dsize = data.size();
    med = (dsize % 2)
            ? data[dsize / 2.]
            : ((float)data[dsize / 2. - 1] + data[dsize / 2.]) * .5;

    return med;
}

// Masked mean
float meanMask(const QVector<float> &vector_in, const QVector<bool> &mask, long maxLength)
{
    long maxDim = vector_in.length();
    // Usually, we take all elements in a vector.
    // However, in some cases it may be that elements get masked and the vector is not entirely filled.
    // In this case, an optional maxLength argument is provided
    if (maxLength > 0 && maxLength < maxDim) maxDim = maxLength;

    // fast-track
    if (vector_in.size() == 0) return 0.;
    if (mask.isEmpty()) {
        float meanVal = 0.;
        for (long i=0; i<maxDim; ++i) meanVal += vector_in[i];
        return meanVal / maxDim;
    }
    else {
        QVector<float> vector;
        vector.reserve(maxDim);

        for (int i=0; i<maxDim; ++i) {
            if (!mask[i]) vector.append(vector_in[i]);
        }

        if (vector.size() == 0) return 0.;
        else
            return std::accumulate(vector.begin(), vector.end(), .0) / vector.size();
    }
}

// Calculate an iterative mean using sigma outlier rejection.
// The algorithm terminates after iterMax iterations, or if converged.
// Data points can re-enter the process.
float meanIterative(const QVector<float> data, float kappa, int iterMax)
{
    long dim = data.length();
    if (dim == 0) return 0.;

    QVector<bool> mask(dim,false);

    // Calculate first estimate of mean and rms; all masks are false
    float meanVal = meanMask_T(data, mask);
    float rmsVal = rmsMask_T(data, mask);

    if (kappa == 0. || iterMax == 0) return meanVal;

    int iter = 0;
    while (iter < iterMax) {
        // Calculate mean and rms
        meanVal = meanMask_T(data, mask);
        rmsVal = rmsMask_T(data, mask);

        // Recalculate mask
        bool masksChanged = false;
        long i = 0;
        for (auto & it : data) {
            bool currentMask = mask[i];
            // reject an outlier
            if (!currentMask && fabs(it - meanVal) >= kappa*rmsVal) {
                mask[i] = true;
                masksChanged = true;
            }
            // include a previous outlier again
            if (currentMask && fabs(it - meanVal) < kappa*rmsVal) {
                mask[i] = false;
                masksChanged = true;
            }
            ++i;
        }

        // Leave if masks haven't changed
        if (masksChanged == false) break;

        ++iter;
    }
    return meanVal;
}

bool readData3D(QString path, QVector<double> &x, QVector<double> &y, QVector<double> &z)
{
    QFile file(path);
    if (!file.exists()) {
        return false;
    }

    if ( !file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&(file));
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty() || line.contains("#")) continue;
        QStringList values = line.split(" ");
        if (values.length() != 3) continue;
        x.append(values[0].toDouble());
        y.append(values[1].toDouble());
        z.append(values[2].toDouble());
    }
    file.close();

    return true;
}

// Angles are always passed / returned in degrees
double getPosAnglefromCD(double cd11, double cd12, double cd21, double cd22, bool lock)
{
    // In case CD matrix is undefined (e.g. binned images)
    if (cd11 == 0. && cd12 == 0. && cd21 == 0. && cd22 == 0.) return 0.;

    // the pixel scale
    double pscale1 = sqrt(cd11 * cd11 + cd21 * cd21);
    double pscale2 = sqrt(cd12 * cd12 + cd22 * cd22);

    // take out the pixel scale
    cd11 /= pscale1;
    cd12 /= pscale2;
    cd21 /= pscale1;
    cd22 /= pscale2;

    // sqrt(CD elements) is very close to one, but not perfectly
    // as coordinate system is not necessarily orthogonal (shear + contraction)
    double nfac1 = sqrt(cd11 * cd11 + cd21 * cd21);
    double nfac2 = sqrt(cd12 * cd12 + cd22 * cd22);

    // make sure sqrt(CD elements) = 1,
    // so that we can do the inverse trig functions
    cd11 /= nfac1;
    cd21 /= nfac1;
    cd12 /= nfac2;
    cd22 /= nfac2;

    // due to flipping the rotation angle is ambiguous.

    // possibly, the following could be simplified with det(CD),
    // but at the moment i don't see how to identify the additional 2*PI easily

    double pa = 0.;
    double PI = 3.1415926535;

    // normal
    if      (cd11 <  0 && cd12 <= 0 && cd21 <= 0 && cd22 >  0) pa = acos(-cd11);       //   0 <= phi <  90
    else if (cd11 >= 0 && cd12 <  0 && cd21 <  0 && cd22 <= 0) pa = acos(-cd11);       //  90 <= phi < 180
    else if (cd11 >  0 && cd12 >= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(-cd11); // 180 <= phi < 270
    else if (cd11 <= 0 && cd12 >  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(-cd11); // 270 <= phi < 360

    // flipped
    else if (cd11 >= 0 && cd12 >= 0 && cd21 <= 0 && cd22 >= 0) pa = acos(cd11);       //   0 <= phi <  90
    else if (cd11 <  0 && cd12 >  0 && cd21 <  0 && cd22 <  0) pa = acos(cd11);       //  90 <= phi < 180
    else if (cd11 <  0 && cd12 <= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(cd11); // 180 <= phi < 270
    else if (cd11 >= 0 && cd12 <  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(cd11); // 270 <= phi < 360

    else {
        if (lock) {
            // we are very likely close to 0, 90, 180 or 270 degrees, and the CD matrix is slightly non-orthogonal.
            // In this case, lock onto 0, 90, 180 or 270 degrees. Otherwise, exit with an error.
            double cd11cd12 = fabs(cd11/cd12);
            double cd22cd21 = fabs(cd22/cd21);

            if (cd11cd12 > 20. && cd22cd21 > 20.) {
                if (cd11 > 0. && cd22 > 0.) pa = 0.*PI/2.;
                if (cd11 < 0. && cd22 > 0.) pa = 0.*PI/2.;
                if (cd11 > 0. && cd22 < 0.) pa = 2.*PI/2.;
                if (cd11 < 0. && cd22 < 0.) pa = 2.*PI/2.;
            }

            else if (cd11cd12 < 0.05 && cd22cd21 < 0.05) {
                if (cd12 > 0. && cd21 < 0.) pa = 1.*PI/2.;
                if (cd12 < 0. && cd21 < 0.) pa = 1.*PI/2.;
                if (cd12 > 0. && cd21 > 0.) pa = 3.*PI/2.;
                if (cd12 < 0. && cd21 > 0.) pa = 3.*PI/2.;
            }

            else {
                // should print to command line only if in debug mode
                //                qDebug() << __func__ << "ERROR: Could not determine position angle from CD matrix!";
            }
        }
    }

    double rad = PI / 180.;
    return (pa / rad);      // return in degrees
}

// Angles are always passed / returned in degrees
void rotateCDmatrix(double &cd11, double &cd12, double &cd21, double &cd22, double PAnew)
{
    double rad = 3.1415926535 / 180.;

    // the current position angle of the CD matrix
    double PAold = getPosAnglefromCD(cd11, cd12, cd21, cd22);

    // is the matrix flipped (det(CD) > 0 if flipped)
    double det = cd11*cd22 - cd12*cd21;
    double f11 = 0.;
    if (det < 0) f11 = 1.;
    else f11 = -1.;

    double f12 = 0.;
    double f21 = 0.;
    double f22 = 1.;

    // unflip the matrix
    matrixMult_T(f11, f12, f21, f22, cd11, cd12, cd21, cd22);

    // rotate the matrix to the new position angle
    double dPA = rad * (PAnew - PAold);

    matrixMult_T(cos(dPA), -sin(dPA), sin(dPA), cos(dPA), cd11, cd12, cd21, cd22);

    // flip the matrix
    matrixMult_T(f11, f12, f21, f22, cd11, cd12, cd21, cd22);
}

// Angles are always passed / returned in degrees
void get_rotimsize(long naxis1, long naxis2, double PAold, double PAnew, long &Nnew, long &Mnew)
{
    double n = naxis1;
    double m = naxis2;
    double r = 0.5 * sqrt(n*n+m*m) + 1.;

    double rad = 3.1415926535 / 180.;
    double phi = atan(m/n) / rad;

    QVector<double> xarr(4);
    QVector<double> yarr(4);

    xarr[0] = r * cos(rad * (PAnew - PAold + phi));
    xarr[1] = r * cos(rad * (PAnew - PAold - phi + 180.));
    xarr[2] = r * cos(rad * (PAnew - PAold + phi + 180.));
    xarr[3] = r * cos(rad * (PAnew - PAold - phi + 360.));

    yarr[0] = r * sin(rad * (PAnew - PAold + phi));
    yarr[1] = r * sin(rad * (PAnew - PAold - phi + 180.));
    yarr[2] = r * sin(rad * (PAnew - PAold + phi + 180.));
    yarr[3] = r * sin(rad * (PAnew - PAold - phi + 360.));

    long xmin = (long) minVec_T(xarr);
    long xmax = (long) maxVec_T(xarr);
    long ymin = (long) minVec_T(yarr);
    long ymax = (long) maxVec_T(yarr);

    Nnew = xmax - xmin;
    Mnew = ymax - ymin;
}

int gauss_f(const gsl_vector* x, void* params, gsl_vector* f)
{
    size_t nx = ((struct dataGSL *) params)->nx;
    size_t ny = ((struct dataGSL *) params)->ny;
    double *z = ((struct dataGSL *) params)->z;
    double *sigma = ((struct dataGSL *) params)->sigma;

    // extract parameters from x
    double b  = gsl_vector_get(x,0);
    double x0 = gsl_vector_get(x,1);
    double y0 = gsl_vector_get(x,2);
    double s  = gsl_vector_get(x,3);

    for (size_t yi=0; yi<ny; ++yi) {
        for (size_t xi=0; xi<nx; ++xi) {
            int indx = xi + nx*yi;
            // calculate Fxy based on parameters
            double Fxy = b * exp(-0.5*( (xi-x0)*(xi-x0) + (yi-y0)*(yi-y0)) / (s*s));
            gsl_vector_set(f, indx, (Fxy - z[indx])/sigma[indx]);
        }
    }
    return GSL_SUCCESS;
}

int gauss_df(const gsl_vector *x, void *params, gsl_matrix *J)
{
    size_t nx = ((struct dataGSL *)params)->nx;
    size_t ny = ((struct dataGSL *)params)->ny;
    double *sigma = ((struct dataGSL *)params)->sigma;

    double x0 = gsl_vector_get(x,1);
    double y0 = gsl_vector_get(x,2);
    double s  = gsl_vector_get(x,3);

    for (size_t yi=0; yi<ny; ++yi) {
        for (size_t xi=0; xi<nx; ++xi) {
            int indx = xi + nx*yi;
            double f0 = exp(-0.5*( (xi-x0)*(xi-x0) + (yi-y0)*(yi-y0)) / (s*s));
            // The Jacobian matrix
            gsl_matrix_set (J, indx, 0, 1./sigma[indx] * f0);
            gsl_matrix_set (J, indx, 1, 1./(s*s*sigma[indx]) * (xi - x0) * f0);
            gsl_matrix_set (J, indx, 2, 1./(s*s*sigma[indx]) * (yi - y0) * f0);
            gsl_matrix_set (J, indx, 3, 1./(s*s*s*sigma[indx]) * (pow(xi-x0,2) + pow(yi-y0,2)) * f0);
        }
    }
    return GSL_SUCCESS;
}

int gauss_fdf(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *J)
{
    gauss_f(x, params, f);
    gauss_df(x, params, J);

    return GSL_SUCCESS;
}

void flipData(QVector<float> &data, const QString dir, const int naxis1, const int naxis2)
{
    QVector<float> copy;
    copy.resize(naxis1*naxis2);

    long k = 0;
    if (dir == "x") {
        for (int j=0; j<naxis2; ++j) {
            for (int i=0; i<naxis1; ++i) {
                copy[k++] = data[naxis1-1-i + naxis1*j];
            }
        }
    }
    else if (dir == "y") {
        for (int j=0; j<naxis2; ++j) {
            for (int i=0; i<naxis1; ++i) {
                copy[k++] = data[i + naxis1*(naxis2-1-j)];
            }
        }
    }
    else if (dir == "xy") {
        for (int j=0; j<naxis2; ++j) {
            for (int i=0; i<naxis1; ++i) {
                copy[k++] = data[naxis1-i-1+naxis1*(naxis2-j-1)];
            }
        }
    }

    else {
        // nothing yet
    }

    data.swap(copy);
}

void flipSections(QVector<long> &sections, const QString dir, const int naxis1, const int naxis2)
{
    QVector<long> copy = sections;

    if (dir == "x") {
        copy[1] = naxis1-1-sections[0];
        copy[0] = naxis1-1-sections[1];
    }
    else if (dir == "y") {
        copy[3] = naxis2-1-sections[2];
        copy[2] = naxis2-1-sections[3];
    }
    else if (dir == "xy") {
        copy[1] = naxis1-1-sections[0];
        copy[0] = naxis1-1-sections[1];
        copy[3] = naxis2-1-sections[2];
        copy[2] = naxis2-1-sections[3];
    }

    sections = copy;
}

// In case the user has deactivated some chips, we need to find one that is valid
int getValidChip(const instrumentDataType *instData)
{
    /*
    int testChip = -1;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (!instData->badChips.contains(chip)) {
            testChip = chip;
            break;
        }
    }
    if (testChip == -1) {
        qDebug() << __func__ << "error: no data left after filtering";
    }
    return testChip;
    */
    if (instData->goodChips.isEmpty()) return 0;       // error message is printed by MainWindow::updateExcludedDetectors(QString badDetectors)
    else return instData->goodChips[0];                // return the first valid chip
}
