#ifndef GHTHREADTASKITEM_H
#define GHTHREADTASKITEM_H

#include <QObject>
#include "All_Base_Struct.h"

class GHThreadTaskItem : public QObject
{
    Q_OBJECT
public:
    explicit GHThreadTaskItem(QObject *parent = 0);
    ~GHThreadTaskItem();
    virtual void run( FileTable &fileTableList)=0;
    virtual void run( GsspFileTable &fileTableList)=0;
    virtual void run( QByteArray &fileTableList)=0;
signals:
    void resultReadyAt(int );
    void finished(bool);
    void sigappendfile(int ,FileTable &);
    void sigappendgssp(int ,GsspFileTable &);
    void sigappendbyte(int ,QByteArray );

public slots:
    virtual void finishedbyOne()=0;
};

#endif // GHTHREADTASKITEM_H
