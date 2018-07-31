#ifndef SAVEFILEDLG_H
#define SAVEFILEDLG_H
#include <QtGui>
#include <QtCore>
#include "database/realtimedatabase.h"

QString getAnalysisType(int type);
QString getMarkType(int type);
QIcon getIcon(int analysisType, int markType);

void saveSampleT(const SampleTreeInfo &sampleInfo);
void saveSampleTByDate(const SampleTreeInfo &sampleInfo, QString date);


class SaveFileDlg : public QDialog
{
    Q_OBJECT
public:
    SaveFileDlg(QWidget *parent=0);
    void autoSaveAllFile();//新增，自动保存
private:
    void setSaveFileDlgDefault();
    void setTableDefaultSample();
    void saveFileT(QVector<int> &index, QVector<SampleTreeInfo> &sampleInfos);

private slots:
    void slotClickCheckAllBox(bool);
    void slotClickCheckSaveByDateBox(bool);
    void slotClickSaveButton();
private:
    bool saveByDate_;
    QCheckBox *checkAllBox_;
    QCheckBox *saveByDateBox_;
    QPushButton *saveButton_;
    QPushButton *exitButton_;
    QTableWidget *table_;
    QPushButton *labelButton_;
    QProgressBar *progressBar_;
    QVector<SampleTreeInfo> sampleTreeInfoList_;
};


#endif // SAVEFILEDLG_H
