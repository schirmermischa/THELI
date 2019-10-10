#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog();
    QString code;
    QString title;
    QString explanation;
    QString suggestion;
    QString logname;
    QString line;
    void translate();
    void getErrorLine(QString code);
    void update();

private slots:
    void on_errorShowPushButton_clicked();

    void on_errorLogfileLineEdit_textChanged(const QString &arg1);

private:
    Ui::ErrorDialog *ui;
    QString editorPreference;
    int readSettings();
};

#endif // ERRORDIALOG_H
