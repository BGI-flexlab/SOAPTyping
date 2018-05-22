#ifndef GSSPINFODLG_H
#define GSSPINFODLG_H
#include <QtGui>
#include <QtCore>
#include "staticdatabase.h"
struct AllelePair
{
    QString allele1;
    QString allele2;
};

class GsspInfoDlg : public QDialog
{
    Q_OBJECT
public:
    GsspInfoDlg(QWidget *parent=0);
    void setSampleName(const QString &sampleName);
    void setGsspTable1(QVector<QStringList> &gsspInfos);
private:
    void setDefault();
private slots:
    void slotSetGsspInfoTable2();
    void slotClickSaveDefaultBt();
    void slotClickSaveUserBt();
private:
    QTableWidget *table1_;
    QTableWidget *table2_;
    QLineEdit *sampleNameLine_;
    QLineEdit *gsspDefaultLine_;
    QLineEdit *gsspUserLine_;
    QPushButton *saveDefaultBt_;
    QPushButton *saveUserBt_;
    QPushButton *exitBt_;
    QString sampleName_;
    QVector<QStringList> infos_;
};

bool processGssp(const QVector<AllelePair> &allelePairs, const QString &sampleName, QVector<QStringList> &result);
void findUsefulGssp(const char *seq11, const char *seq12, const char *seq21, const char *seq22, int exonStart, int exonEnd, QVector<GsspTable> &gsspTables, QStringList &gssps, QStringList &infos);
#endif // GSSPINFODLG_H
