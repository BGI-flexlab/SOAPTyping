#ifndef REPORTDLG_H
#define REPORTDLG_H
#include <QtGui>
#include <QtCore>
class ReportDlg : public QDialog
{
    Q_OBJECT
public:
    ReportDlg(QWidget *parent=0);
    void setVersionName(QString version);
private:
    void setDefault();

private slots:
    void slotNumLineChanged(QString);
    void slotClickCheckBox(bool);
    void slotClickScanBt();
    void slotClickSaveBt();
    void slotFileLineChanged(QString);
private:
    QLineEdit *fileLine;
    QLineEdit *numLine;
    QCheckBox *ignoreIndelBox;
    QPushButton *saveBt;
    QPushButton *exitBt;
    QPushButton *scanBt;

    QString fileName;
    int keepNum;
    bool ignoreIndel;
    QString versionName;
};

void printReport(const QString &outFile, int keepNum, bool ignoreIndel, QString versionName);
#endif // REPORTDLG_H
