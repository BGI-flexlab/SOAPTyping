#ifndef DELETEFILEDLG_H
#define DELETEFILEDLG_H

#include <QtGui>
#include <QtCore>
#include "savefiledlg.h"
class DeleteFileDlg : public QDialog
{
    Q_OBJECT
public:
    DeleteFileDlg(bool *isDone, QWidget *parent=0);
private:
    void setDeleteFileDlgDefault();
    void setTableDefaultSample();
    void deleteFile(QVector<int> &index, QVector<SampleTreeInfo> &sampleInfos);
private slots:
    void slotClickCheckAllBox(bool);
    void slotClickDeleteButton();
private:
    bool *isDone_;
    QCheckBox *checkAllBox_;
    QPushButton *deleteButton_;
    QPushButton *exitButton_;
    QTableWidget *table_;
    QPushButton *labelButton_;
    QProgressBar *progressBar_;
    QVector<SampleTreeInfo> sampleTreeInfoList_;
};
#endif // DELETEFILEDLG_H
