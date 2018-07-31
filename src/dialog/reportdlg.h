#ifndef REPORTDLG_H
#define REPORTDLG_H
#include <QtGui>
#include <QtCore>
class ReportDlg : public QDialog
{
    Q_OBJECT
public:
    ReportDlg(QWidget *parent=0);
    void setVersionName(QString version); //新增
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
    QString versionName; //新增
};

void printReport(const QString &outFile, int keepNum, bool ignoreIndel, QString versionName); //改
#endif // REPORTDLG_H
