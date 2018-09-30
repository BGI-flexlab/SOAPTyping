#ifndef GSSPINFODLG_H
#define GSSPINFODLG_H

#include <QDialog>

namespace Ui {
class GsspInfoDlg;
}

class GsspInfoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GsspInfoDlg(QWidget *parent = nullptr);
    ~GsspInfoDlg();
    void setSampleName(const QString &sampleName);
    void setGsspTable1(QVector<QStringList> &gsspInfos);

private:
    void InitUI();
    void ConnectSignalandSlot();

private slots:
    void slotSetGsspInfoTable2();
    void slotClickSaveDefaultBt();
    void slotClickSaveUserBt();


private:
    Ui::GsspInfoDlg *ui;
    QString m_str_SampleName;
    QVector<QStringList> m_vec_infos;
};

#endif // GSSPINFODLG_H
