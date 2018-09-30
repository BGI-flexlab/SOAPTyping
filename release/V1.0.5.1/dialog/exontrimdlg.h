#ifndef EXONTRIMDLG_H
#define EXONTRIMDLG_H
#include <QtCore>
#include <QtGui>
#include "database/staticdatabase.h"
class ExonTimDlg : public QDialog
{
    Q_OBJECT
public:
    ExonTimDlg(QWidget *parent=0);
private slots:
    void slotGeneBoxIndexChanged(QString gene);
    void slotClickResetBt();
    void slotClickSureBt();
    void slotClickTableItem();
private:
    void setDefault();
    void setGeneBoxData();
    void setTableData(QVector<ExonTrimTable> &etList);
private:
    QTableWidget *table;
    QPushButton *resetBt;
    QPushButton *sureBt;
    QPushButton *cancelBt;
    QComboBox *geneBox;
};

#endif // EXONTRIMDLG_H
