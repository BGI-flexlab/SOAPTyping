#ifndef DELETEFILEDLG_H
#define DELETEFILEDLG_H

#include <QDialog>
#include "all_base_struct.h"

namespace Ui {
class DeleteFileDlg;
}

class DeleteFileDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteFileDlg(QWidget *parent = nullptr);
    ~DeleteFileDlg();
private:
    void InitUI();
    void ConnectSignalandSlot();
    void setTableDefaultSample();
    void deleteFile(QVector<SampleTreeInfo_t> &sampleInfos);
private slots:
    void slotClickCheckAllBox(bool);
    void slotClickDeleteButton();
private:
    Ui::DeleteFileDlg *ui;
    QMap<QString,SampleTreeInfo_t> m_map_SampleTreeInfo;
};

#endif // DELETEFILEDLG_H
