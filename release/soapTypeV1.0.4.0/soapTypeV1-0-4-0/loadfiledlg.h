#ifndef LOADFILEDLG_H
#define LOADFILEDLG_H
#include <QtGui>
#include <QtCore>
#include "realtimedatabase.h"

void readListFile(const QString &listFilePath, QVector<QString> &samplePaths, QVector<QString> &filePaths, QVector<QString> &gsspFilePaths);

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
    LoadFileDlg(bool *isDone, QWidget *parent=0);
private slots:
    void slotClickCheckAllBox(bool);
    void slotClickLoadButton();
    void slotdateComboBoxChagned(int);
    void slotLineEditChanged(QString);
private:
    void getLoadInfo();
    void getOkIndex();
    void setLoadFileDlgDefault();
    void setTableDate();
private:
    int daysNum;
    QString name;
    QTableWidget *table;
    QCheckBox *checkAllBox;
    QPushButton *loadButton;
    QPushButton *exitButton;
    QComboBox *dateComboBox;
    QLineEdit *nameLineEdit;
    QPushButton *labelButton;
    QProgressBar *progressBar;
    QVector<LoadInfo> loadInfos;
    QVector<int> okFileIndex;
    bool *isDone_;
};
#endif // LOADFILEDLG_H
