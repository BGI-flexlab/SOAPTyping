#ifndef EXONTIMDLG_H
#define EXONTIMDLG_H

#include <QDialog>
#include "all_base_struct.h"

namespace Ui {
class ExonTimDlg;
}

class ExonTimDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ExonTimDlg(QWidget *parent = nullptr);
    ~ExonTimDlg();

private:
    void InitUI();
    void ConnectSignalandSlot();
    void setGeneBoxData();
    void setTableData(QVector<ExonTrimTable> &etList);
private slots:
    void slotClickResetButton();
    void slotClickApplyButton();
    void slotGeneBoxIndexChanged(const QString &);
    void slotClickTableItem();
private:
    Ui::ExonTimDlg *ui;
};

#endif // EXONTIMDLG_H
