#ifndef LOADFILEDLG_H
#define LOADFILEDLG_H

#include <QDialog>

namespace Ui {
class LoadFileDlg;
}

struct LoadInfo
{
    int analysisType;
    int markType;
    int dateToNow;
    QString sampleName;
    QString createdTime;
    QString modifiedTime;
    QString listFile;
};

class LoadFileDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFileDlg(QWidget *parent = nullptr);
    ~LoadFileDlg();

private:
    void InitUI();
    void ConnectSignalandSlot();
    void getLoadInfo();
    void getOkIndex();
    void SetTableData();
    void readListFile(const QString &listFilePath, QVector<QString> &samplePaths,
                      QVector<QString> &filePaths, QVector<QString> &gsspFilePaths);
    void loadSample(const QString &strPath);
    void loadFile(const QString &strPath);
    void loadGssp(const QString &strPath);
private slots:
    void slotClickLoadButton();
    void slotClickCheckAllBox(bool status);
    void slotdateComboBoxChagned(int index);
    void slotLineEditChanged(const QString & name);
private:
    Ui::LoadFileDlg *ui;
    int m_idaysNum;
    QString m_str_SearchName;
    QVector<int> m_vec_index;
    QVector<LoadInfo> m_vec_LoadInfo;
};

#endif // LOADFILEDLG_H
