#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QFont>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

signals:
    int diskspacewarnChanged(int);
    int fontSizeChanged(int);
    int fontChanged(QFont);
    int instPreferenceChanged(int);
    int numcpuChanged(int);
    int serverChanged(QString);
    int memoryUsageChanged(bool);
    int intermediateDataChanged(QString);
    void verbosityLevelChanged(int index);
    void preferencesUpdated();
    int switchProcessMonitorChanged(bool);

public:
    explicit Preferences(bool running, QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void on_prefFontdialogPushButton_clicked();
    void on_prefFontsizeSpinBox_valueChanged(int arg1);
    void on_prefDefaultFontPushButton_clicked();
    void on_prefDiskspacewarnSpinBox_valueChanged(int arg1);
    void on_prefServerComboBox_activated(const QString &arg1);
    void on_prefCPUSpinBox_valueChanged(int arg1);
    void on_prefIOthreadsSpinBox_valueChanged(int arg1);
    void on_prefMemoryCheckBox_clicked();
    void on_prefIntermediateDataComboBox_currentTextChanged(const QString &arg1);
    void on_prefVerbosityComboBox_currentIndexChanged(int index);
    void on_prefSwitchProcessMonitorCheckBox_clicked();

public slots:
    void receiveDefaultFont(QFont);
    void updateParallelization(bool running);

private:
    Ui::Preferences *ui;
    QFont defaultFont;
    float totalMemory;
    int maxMemoryUsed;
    void closeEvent(QCloseEvent *event);
    void applyStyleSheets();
    void configureMemory();
    int readSettings();
    int writeSettings();
};

#endif // PREFERENCES_H
