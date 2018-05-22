#ifndef UPDATEDATABASEDLG_H
#define UPDATEDATABASEDLG_H
#include <QtCore>
#include <QtGui>
#include "staticdatabase.h"
class UpdateDlg : public QDialog
{
    Q_OBJECT
public:
    UpdateDlg(QWidget *parent=0);
private slots:
    void slotClickT1ApplyBt();
    void slotClickT2ApplyBt();
    void slotClickT3InsertBt();
    void slotClickT3ClearBt();
    void slotClickT1BrowseBt1();
    void slotClickT2BrowseBt1();
    void slotClickT2BrowseBt2();
    void slotClickT2BrowseBt3();
    void slotClickT2BrowseBt4();
    void slotClickT2BrowseBt5();
    void slotClickT2BrowseBt6();

private:
    void getOpenFile(QLineEdit *line);
    void showWarningBox(const QString &info);
    void showInfomationBox(const QString &info);
    void setDefaultData();
    void setw1();
    void setw2();
    void setw3();
    void SetDisableAll(bool);
    int isFileExists(QString file);
private:
    QTabWidget *tab;
    QWidget *w1;
    QWidget *w2;
    QWidget *w3;
    QLineEdit *t1Line1;
    QLineEdit *t2Line1;
    QLineEdit *t2Line2;
    QLineEdit *t2Line3;
    QLineEdit *t2Line4;
    QLineEdit *t2Line5;
    QLineEdit *t2Line6;
    QLineEdit *t3Line1;
    QLineEdit *t3Line2;
    QLineEdit *t3Line3;
    QLineEdit *t3Line4;
    QLineEdit *t3Line5;
    QLineEdit *t3Line6;
    QPushButton *t1BrowseBt1;
    QPushButton *t2BrowseBt1;
    QPushButton *t2BrowseBt2;
    QPushButton *t2BrowseBt3;
    QPushButton *t2BrowseBt4;
    QPushButton *t2BrowseBt5;
    QPushButton *t2BrowseBt6;
    QPushButton *t1ApplyBt;
    QPushButton *t1ExitBt;
    QPushButton *t2ApplyBt;
    QPushButton *t2ExitBt;
    QPushButton *t3ClearBt;
    QPushButton *t3InsertBt;
//    QPushButton *t3DeleteBt;
    QPushButton *t3ExitBt;
    QPushButton *waitBt1;
    QPushButton *waitBt2;
    QString dir;
    QString dir2;
};

#endif // UPDATEDATABASEDLG_H
