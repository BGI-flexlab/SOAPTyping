#include "ghthreadobject.h"
#include <assert.h>
#include "QThread"

GHThreadObject::GHThreadObject(QObject *parent,GHThreadTaskItem * pThreadTaskItem)
    : QObject(parent),
    m_pThreadTaskItem(pThreadTaskItem)
{
   Q_ASSERT(pThreadTaskItem!=NULL);

}

GHThreadObject::~GHThreadObject()
{
}

void GHThreadObject::process(int data_source,FileTable &fileTableList ,QObject * target)
{
    if (target==this)
    {
        try{
        m_pThreadTaskItem->run(fileTableList);
        //((QThread* )(QThread::currentThread ()))->setStackSize (0);
        emit sig_process_finished(data_source);
        }
        catch(...)
        {
            ErrorWrite::errorocur("process(int data_source,FileTable &fileTableList ,QObject * target)");
            emit sig_process_finished(data_source);
        }
    }
}
void GHThreadObject::process(int data_source,GsspFileTable &fileTableList ,QObject * target)
{
    if (target==this)
    {
        try{
        m_pThreadTaskItem->run(fileTableList);
        emit sig_process_finished(data_source);
        }
        catch(...)
        {
            ErrorWrite::errorocur("process(int data_source,GsspFileTable &fileTableList ,QObject * target)");
            emit sig_process_finished(data_source);
        }
    }
}
void GHThreadObject::process(int data_source,QByteArray &fileTableList ,QObject * target)
{
    if (target==this)
    {
        try{
        m_pThreadTaskItem->run(fileTableList);
        emit sig_process_finished(data_source);
        }
        catch(...)
        {
            ErrorWrite::errorocur("process(int data_source,QByteArray &fileTableList ,QObject * target)");
            emit sig_process_finished(data_source);
        }
    }
}

void GHThreadObject::SetThreadTaskItem(GHThreadTaskItem * pThreadTaskItem)
{
    assert(pThreadTaskItem!=NULL);
    m_pThreadTaskItem = pThreadTaskItem;
}
