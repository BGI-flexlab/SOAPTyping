#ifndef FINALTYPEDLG_H
#define FINALTYPEDLG_H

#include <QDialog>

namespace Ui {
class FinalTypeDlg;
}

class FinalTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FinalTypeDlg(QWidget *parent = nullptr);
    ~FinalTypeDlg();
    void setSampleName(const QString &sampleName);
    void setAlleleResult(const QVector<QStringList> &result);

private:
    void InitUI();
    void ConnectSignalandSlot();
private slots:
    void slotClickSetButton();
    void slotRowChanged();
    void slotClickSaveButton();

private:
    Ui::FinalTypeDlg *ui;
    QString m_str_SampleName;
    QString m_str_allele1;
    QString m_str_allele2;
};

#endif // FINALTYPEDLG_H
