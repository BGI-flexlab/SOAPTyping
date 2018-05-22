#include "typethreadpool.h"
#include "QApplication"
#include "config.h"
TypeThreadPool::TypeThreadPool(){
    count=0;
    count= getPoolThreadNum();
    if(count<1)
        count=1;
    pool=new GHThreadEngine(0,this,count,true);
    current=0;
    total=0;
    connect(pool,SIGNAL(finishedone()),
            this,SLOT(finishedbyOne()));

}
void TypeThreadPool::mapped(QList<FileTable>  fileTableList,bool (* task_fun)(FileTable ))
{
    total=total+fileTableList.count();
    task_source_File=task_fun;

    for(int i=0;i<fileTableList.count();i++)
    {
        pool->append_new(current,fileTableList[i]);
        current=((++current)%count);
    }

}
void TypeThreadPool::mapped(QList<GsspFileTable>  fileTableList,bool (* task_fun)(GsspFileTable ))
{
    total=total+fileTableList.count();
    task_source_Gssp=task_fun;

    for(int i=0;i<fileTableList.count();i++)
    {
        pool->append_new(current,fileTableList[i]);
        current=((++current)%count);
    }

}
void TypeThreadPool::mapped(QList<QByteArray>  fileTableList,bool (* task_fun)(const QByteArray & ))
{
    total=total+fileTableList.count();
    task_source_Byte=task_fun;
    for(int i=0;i<fileTableList.count();i++)
    {
        pool->append_new(current,fileTableList[i]);
        current=((++current)%count);
    }

}

void TypeThreadPool::run( FileTable &fileTableList)
{
    task_source_File(fileTableList);
}
void TypeThreadPool::run( GsspFileTable &fileTableList)
{
    task_source_Gssp(fileTableList);
}
void TypeThreadPool::run( QByteArray &fileTableList)
{
    task_source_Byte(fileTableList);
}
void TypeThreadPool::finishedbyOne()
{
    emit resultReadyAt(total );
    total--;
    if(total<1)
    {
       emit finished(true);
    }

}
