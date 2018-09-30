#ifndef FINALTYPEDLG_H
#define FINALTYPEDLG_H
#include <QtCore>
#include <QtGui>
class FinalTypeDlg:public QDialog
{
    Q_OBJECT
public:
    FinalTypeDlg(QWidget *parent=0);
    void setSampleName(const QString &sampleName);
    void setAlleleResult(const QVector<QStringList> &result);
private:
    void setFinalTypeDlgDefault();
private slots:
    void slotClickSetButton();
    void slotRowChanged();
    void slotClickSaveButton();
private:
    QLineEdit *sampleNameLine_;
    QLineEdit *allele1Line_;
    QLineEdit *allele2Line_;
    QTableWidget *table_;
    QPushButton *saveButton_;
    QPushButton *exitButton_;
    QPushButton *setButton_;
    QString alleleName1_;
    QString alleleName2_;
    QString sampleName_;
};

#endif // FINALTYPEDLG_H
