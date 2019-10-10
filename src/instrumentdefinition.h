#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QDialog>
#include <QLineEdit>
#include <QButtonGroup>
#include <QTextStream>
#include <QStatusBar>

#include <QMainWindow>

namespace Ui {
class Instrument;
}

class Instrument : public QMainWindow
{
    Q_OBJECT

public:
    explicit Instrument(QWidget *parent = nullptr);
    ~Instrument();

private slots:
    void paintNumLineEdits(QString geometry);
    void on_numchipSpinBox_valueChanged(int arg1);
    void validate(QString arg1);
    void on_saveConfigPushButton_clicked();
    void on_instrumentTypeComboBox_currentIndexChanged(int index);
    void on_loadConfigPushButton_clicked();
    void toggle_bayer_ToolButtons();
    void timerConfigDone();
    void on_clearPushButton_clicked();
    void on_bayerCheckBox_clicked(bool checked);

    void on_actionClose_triggered();

private:
    Ui::Instrument *ui;

    QString thelidir;
    QString userdir;
    QString tmpdir;

    QList<QLineEdit*> geometryList;
    QList<QLineEdit*> geometryNumList;
    int numChips = 1;
    QString geometryToConfig(QString geometry);
    QString configToGeometry(QString config);
    void altStream(QTextStream &stream, QString keyword, QString altValue);
    QButtonGroup *bayerButtonGroup = new QButtonGroup(this);
    bool compareChipNumbers();
    void getKey(QTextStream &stream, QString key, QString bashKey, QString mode = "");
    void truncateFITSkey(QString &key);
    void applyStyleSheets();
};

#endif // INSTRUMENT_H
