#ifndef SETDLG_H
#define SETDLG_H
#include <QtCore>
#include <QtGui>

class SetDlg : public QDialog
{
    Q_OBJECT
public:
    SetDlg(QWidget *parent=0);
private slots:
    void slotClickResetBt();
    void slotClickSaveBt();
private:
    void setDefault();
    void setDefaultInfo(QMap<QString, QString> &config);
private:
    QCheckBox *ignoreIndelBox;
    QLineEdit *openFileThreadLine;
    QLineEdit *analysisSampleThreadLine;
    QPushButton *saveBt;
    QPushButton *exitBt;
    QPushButton *resetBt;
};

#endif // SETDLG_H
