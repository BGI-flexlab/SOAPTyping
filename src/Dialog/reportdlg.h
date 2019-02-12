#ifndef REPORTDLG_H
#define REPORTDLG_H

#include <QDialog>

namespace Ui {
class ReportDlg;
}

class ReportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDlg(QWidget *parent = nullptr);
    ~ReportDlg();
    void setVersion(const QString &strVer);
private:
    void ConnectSignalandSlot();
    void setDefaultPath();
    void printReport();
private slots:
    void slotClickScanBt();
    void slotClickSaveBt();
private:
    Ui::ReportDlg *ui;
    QString m_str_Version;
};

#endif // REPORTDLG_H
