#ifndef GHTHREADENGINE_H
#define GHTHREADENGINE_H
#include <QMap>
#include "ghthreadobject.h"
#include <QObject>
#include <QThread>
#include <QVector>
#include <QList>
#include <QMutex>

class GHThreadEngine : public QObject
{
    Q_OBJECT
public:
    GHThreadEngine(QObject *parent,
                   GHThreadTaskItem * pTaskItem,int nThreads = 4,bool bFIFOKeep = true);
    ~GHThreadEngine();
    void SetThreadTaskItem(GHThreadTaskItem * pTaskItem);
    QList<qint32> CurrentLoad()
    {
        return m_map_Tasks.values();
    }
signals:
public slots:
    void append_new(int data_source, FileTable & fileTableList );
    void append_new(int data_source, GsspFileTable & fileTableList );
    void append_new(int data_source, QByteArray fileTableList );

    void on_sig_process_finished();
    void on_sig_process_finished(int );
signals:

    void do_task(int,FileTable &fileTableList ,QObject *);
    void do_task(int,GsspFileTable &fileTableList ,QObject *);
    void do_task(int,QByteArray &fileTableList ,QObject *);
    void finishedone();
protected:
    QVector< QThread * > m_ThreadPool;
    QVector<GHThreadObject *> m_ThreadObjs;
    GHThreadTaskItem * m_pThreadTaskItem;
    int m_nThreads;
    bool m_bFIFOKeep;
private:
    QMap<QObject *,qint32> m_map_Tasks;
    QMap<int,QObject *> m_map_busy_source_task;
    QMap<int,int> m_map_busy_source_counter;

};

#endif // GHTHREADENGINE_H
