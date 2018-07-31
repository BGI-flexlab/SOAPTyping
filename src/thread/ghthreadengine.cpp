#include "ghthreadengine.h"
#include <assert.h>

GHThreadEngine::GHThreadEngine(QObject *parent,GHThreadTaskItem * pTaskItem,
                               int nThreads,bool bFIFOKeep)
    : QObject(parent),
      m_nThreads(nThreads),
      m_pThreadTaskItem(pTaskItem),
      m_bFIFOKeep(bFIFOKeep)
{

    Q_ASSERT(nThreads>0 && nThreads<512 && pTaskItem!=NULL);
    qRegisterMetaType<FileTable>("FileTable &");
    qRegisterMetaType<GsspFileTable>("GsspFileTable &");
    qRegisterMetaType<QByteArray>("QByteArray &");
    for (int i=0;i<nThreads;i++)
    {
        QThread * pNewThread = new QThread(this);
        GHThreadObject * pNewObject = new GHThreadObject(0,pTaskItem);
        m_ThreadPool.push_back(pNewThread);
        m_ThreadObjs.push_back(pNewObject);
        m_map_Tasks[pNewObject] = 0;

        pNewThread->start();

        pNewObject->moveToThread(pNewThread);
        connect(pNewObject,SIGNAL(sig_process_finished(int)),this,SLOT(on_sig_process_finished(int)));
        connect(this,SIGNAL(do_task(int,FileTable &, QObject *)),pNewObject,SLOT(process(int,FileTable &, QObject *)));
        connect(this,SIGNAL(do_task(int,GsspFileTable &, QObject *)),pNewObject,SLOT(process(int,GsspFileTable &, QObject *)));
        connect(this,SIGNAL(do_task(int,QByteArray &, QObject *)),pNewObject,SLOT(process(int,QByteArray &, QObject *)));

    }
}

GHThreadEngine::~GHThreadEngine()
{
    foreach(GHThreadObject * obj,m_ThreadObjs)
    {
        disconnect(obj,SIGNAL(sig_process_finished(QObject *)),this,SLOT(on_sig_process_finished(QObject *)));
        obj->deleteLater();
    }
    foreach(QThread * th ,m_ThreadPool)
    {
        disconnect(this,SIGNAL(do_task(int,FileTable &, QObject *)),th,SLOT(process(int,FileTable &, QObject *)));
        disconnect(this,SIGNAL(do_task(int,GsspFileTable &, QObject *)),th,SLOT(process(int,GsspFileTable &, QObject *)));
        disconnect(this,SIGNAL(do_task(int,QByteArray &, QObject *)),th,SLOT(process(int,QByteArray &, QObject *)));
        th->exit(0);
        th->wait();
    }
}

void GHThreadEngine::append_new(int data_source,FileTable & fileTableList )
{
    QObject * pMinObj = 0;
    if (m_map_busy_source_counter.find(data_source)!=m_map_busy_source_counter.end()&& m_bFIFOKeep==true)
    {
        m_map_busy_source_counter[data_source]++;
        pMinObj = m_map_busy_source_task[data_source];
    }
    else
    {
        qint32 nMinCost = 0x7fffffff;
        for (QMap<QObject *,qint32>::iterator p = m_map_Tasks.begin();p!=m_map_Tasks.end();p++)
        {
            if (p.value()< nMinCost)
            {
                nMinCost = p.value();
                pMinObj = p.key();
            }
        }
        if (pMinObj)
        {
            m_map_busy_source_counter[data_source] = 1;
            m_map_busy_source_task[data_source] = pMinObj;
        }
    }
    if (pMinObj)
    {
        m_map_Tasks[pMinObj]++;
        emit do_task(data_source,fileTableList,pMinObj);
    }
}



void GHThreadEngine::append_new(int data_source,GsspFileTable & fileTableList )
{
    QObject * pMinObj = 0;
    if (m_map_busy_source_counter.find(data_source)!=m_map_busy_source_counter.end()&& m_bFIFOKeep==true)
    {
        m_map_busy_source_counter[data_source]++;
        pMinObj = m_map_busy_source_task[data_source];
    }
    else
    {
        qint32 nMinCost = 0x7fffffff;
        for (QMap<QObject *,qint32>::iterator p = m_map_Tasks.begin();p!=m_map_Tasks.end();p++)
        {
            if (p.value()< nMinCost)
            {
                nMinCost = p.value();
                pMinObj = p.key();
            }
        }
        if (pMinObj)
        {
            m_map_busy_source_counter[data_source] = 1;
            m_map_busy_source_task[data_source] = pMinObj;
        }
    }
    if (pMinObj)
    {
        m_map_Tasks[pMinObj]++;
        emit do_task(data_source,fileTableList,pMinObj);
    }
}



void GHThreadEngine::append_new(int data_source,QByteArray fileTableList )
{
    QObject * pMinObj = 0;
    if (m_map_busy_source_counter.find(data_source)!=m_map_busy_source_counter.end()&& m_bFIFOKeep==true)
    {
        m_map_busy_source_counter[data_source]++;
        pMinObj = m_map_busy_source_task[data_source];
    }
    else
    {
        qint32 nMinCost = 0x7fffffff;
        for (QMap<QObject *,qint32>::iterator p = m_map_Tasks.begin();p!=m_map_Tasks.end();p++)
        {
            if (p.value()< nMinCost)
            {
                nMinCost = p.value();
                pMinObj = p.key();
            }
        }
        if (pMinObj)
        {
            m_map_busy_source_counter[data_source] = 1;
            m_map_busy_source_task[data_source] = pMinObj;
        }
    }
    if (pMinObj)
    {
        m_map_Tasks[pMinObj]++;
        emit do_task(data_source,fileTableList,pMinObj);
    }
}

void GHThreadEngine::on_sig_process_finished(int data_source)
{
    if (m_map_Tasks.find(sender())!=m_map_Tasks.end())
    {
        m_map_Tasks[sender()]--;

    }
    if (m_map_busy_source_counter.find(data_source)!=m_map_busy_source_counter.end())
    {

        m_map_busy_source_counter[data_source]--;
        if (m_map_busy_source_counter[data_source]<=0)
        {
            m_map_busy_source_counter.remove(data_source);
            m_map_busy_source_task.remove(data_source);
        }
    }
    emit finishedone();

}
void GHThreadEngine::on_sig_process_finished()
{

}
