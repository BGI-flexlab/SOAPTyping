#ifndef FILEPROCESSTHREADTASK_H
#define FILEPROCESSTHREADTASK_H

#include <QObject>
#include <QRunnable>
#include "all_base_struct.h"

class fileprocessthreadtask:public QObject,public QRunnable
{
    Q_OBJECT
public:
    fileprocessthreadtask(const QStringList &filePathList);
    ~fileprocessthreadtask();
    virtual void run();
    void getFileInfoData();
signals:
    void processone(const OpenFileTable &info);
private:
    void FilePathProcess(const QString &filePath, OpenFileTable &info);
private:
    QStringList m_strlist;
    QMap<QString, ExonAndRF> m_map_ExonAndRF;
    QMap<QString, ExonAndRF> m_map_CommonGssp;
};

#endif // FILEPROCESSTHREADTASK_H
