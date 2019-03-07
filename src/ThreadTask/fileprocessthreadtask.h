#ifndef FILEPROCESSTHREADTASK_H
#define FILEPROCESSTHREADTASK_H

#include <QObject>
#include <QRunnable>
#include "all_base_struct.h"

class fileprocessthreadtask:public QObject,public QRunnable
{
    Q_OBJECT
public:
    fileprocessthreadtask(const QStringList &filePathList,
                          const QMap<QString, ExonAndRF> &map_info);
    ~fileprocessthreadtask();
    virtual void run();
signals:
    void processone(const OpenFileTable &info);
private:
    void FilePathProcess(const QString &filePath, OpenFileTable &info);
private:
    QStringList m_strlist;
    QMap<QString, ExonAndRF> m_map_ExonAndRF;
};

#endif // FILEPROCESSTHREADTASK_H
