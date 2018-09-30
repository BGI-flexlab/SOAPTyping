#ifndef ALLELEPAIRDLG_H
#define ALLELEPAIRDLG_H

#include <QDialog>

namespace Ui {
class AllelePairDlg;
}

class AllelePairDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AllelePairDlg(QWidget *parent = nullptr);
    ~AllelePairDlg();
    void getSelectAllele(QString &str_allele1, QString &str_allele2);
    void SetData(const QString &str_genename);
private:
    void ConnectSignalandSlot();
private slots:
    void slotClickOK();
private:
    Ui::AllelePairDlg *ui;
    QString m_str_allele1;
    QString m_str_allele2;
};

#endif // ALLELEPAIRDLG_H
