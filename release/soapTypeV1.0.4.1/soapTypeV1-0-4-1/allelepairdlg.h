#ifndef ALLELEPAIRDLG_H
#define ALLELEPAIRDLG_H

#include <QtGui>
#include <QtCore>
class AllelePairDlg : public QDialog
{
    Q_OBJECT
public:
    AllelePairDlg(QWidget *parent=0);
    void setData(const QString &geneName, QString *alelle1, QString *allele2);
    void setData(QStringList typeResult, QString *alelle1, QString *allele2);
    void findComboxItem(QString alelle1, QString allele2);
    QStringList removeSameData(QStringList list);
private slots:
    void slotClickOK();
private:
    QLabel *label_;
    QPushButton *buttonOK_;
    QComboBox *box1_;
    QComboBox *box2_;
    QString *allele1_;
    QString *allele2_;
};

#endif // ALLELEPAIRDLG_H
