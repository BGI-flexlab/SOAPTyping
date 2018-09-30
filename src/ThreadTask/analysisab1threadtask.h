#ifndef ANALYSISAB1THREADTASK_H
#define ANALYSISAB1THREADTASK_H

#include <QObject>
#include <QRunnable>
#include "Core/fileTablebase.h"

class AnalysisAB1ThreadTask:public QObject,public QRunnable
{
    Q_OBJECT
public:
    AnalysisAB1ThreadTask(Ab1FileTableBase *pfiletable);
    ~AnalysisAB1ThreadTask();
    virtual void run();
signals:
    void analysisfinished();
private:
    Ab1FileTableBase *m_pFiletable;
};

#endif // ANALYSISTHREADTASK_H
