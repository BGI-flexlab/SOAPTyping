#ifndef GHTHREADOBJECT_H
#define GHTHREADOBJECT_H

#include <QObject>
#include "ghthreadtaskitem.h"

class GHThreadObject : public QObject
{
    Q_OBJECT
public:
    GHThreadObject(QObject *parent,GHThreadTaskItem * pThreadTaskItem);

    ~GHThreadObject();
    void SetThreadTaskItem(GHThreadTaskItem * pThreadTaskItem);
signals:

    void sig_process_finished(int data_source);
public slots:

    void process(int data_source,FileTable &fileTableList,QObject * target);
    void process(int data_source,GsspFileTable &fileTableList,QObject * target);
    void process(int data_source,QByteArray &fileTableList,QObject * target);
private:
    GHThreadTaskItem * m_pThreadTaskItem;

};

#endif // GHTHREADOBJECT_H
