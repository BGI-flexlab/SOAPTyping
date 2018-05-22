#ifndef OPENFILEDLG_H
#define OPENFILEDLG_H
#include <QtCore>
#include <QtGui>
#include "staticdatabase.h"
#include "realtimedatabase.h"
#include "analysis.h"
//#include "ghthreadtaskitem.h"
#include "typethreadpool.h"

struct OpenFileTable
{
    bool right;
    QString sampleName;
    QString geneName;
    QString gsspName;
    QString fileName;
    QString exonIndex;
    QString rOrF;
    QString filePath;
    OpenFileTable()
    {
        right=true;
    }
};

class QComboBoxNew : public QComboBox
{
    Q_OBJECT
public:
    QComboBoxNew(QWidget *parent=0)
        :QComboBox(parent)
    {
//        setStyleSheet("QComboBox::drop-down{border-style: none;}");
    }
protected:
    bool event(QEvent *event)
    {
        if(event->type() == QEvent::Wheel)
        {
            event->ignore();
            return true;
        }
        else
        {
            return QComboBox::event(event);
        }
    }
};

class OpenFileDlg : public QDialog
{
    Q_OBJECT
public:
    OpenFileDlg(QWidget *parent=0);
    void setData(bool *done, SignalInfo *signalInfo);
signals:
    void signalGetNumbers();
private:
    void filePathListProcess(const QStringList &filePathList);
    void filePathProcess(const QString &filePath);
    bool analysisFileName(const QString &filePath, OpenFileTable &openFileTable);
    bool analysisExonInfo(const QString &exonString, OpenFileTable &openFileTable);
    void addRowToTableWidget(const OpenFileTable &openFileTable);
    void setComboBoxData(QComboBoxNew *box, const QString &text);

    void analysisFileTable(QList<FileTable> &fileTableList);
    void analysisGsspFileTable(QList<GsspFileTable> &gsspFileTableList);
    void analysisSampleTable(QList<QByteArray> &sampleList);
private slots:
    void slotCheck(int state);
    void slotRemove();
    void slotOpenFile();
    void slotAnalysis();
    void slotProgressBarSetValue(int);
    void slotFileTableWatcherFinished(bool);
    void slotGsspFileTableWatcherFinished(bool);
    void slotSampleTableWathcerFinished(bool);
    void slotGetNumbers();
private:
    int file_num;
    int sample_num;
    QTableWidget *tableWidget_;
    QSet <QString> fileExists_;
    QCheckBox *checkBox_;
    QPushButton *removeCheckedButton_;
    QPushButton *openFileButton_;
    QPushButton *analysisButton_;
    QPushButton *cancelButton_;
    QStringList geneNames_;
    QStringList gsspNames_;
    QStringList exonNames_;
    QStringList rOrFNames_;
    QMap <QString, ExonAndRF> mapToExonAndRF_;
    QPushButton *button_;
    QProgressBar *progressBar_;
    QFutureWatcher<bool> *fileTableWatcher_;
    QFutureWatcher<bool> *gsspFileTableWatcher_;
    QFutureWatcher<bool> *sampleTableWatcher_;
    QList<QByteArray> sampleNameList_;
    QList<FileTable> fileTableList_;
    QList<GsspFileTable> gsspFileTableList_;
    QSet<QByteArray> sampleSet_;
    bool *done_;
    SignalInfo *signalInfo_;
    int range_;
    int value_;
    TypeThreadPool  * threadpool;

};

#endif // OPENFILEDLG_H
