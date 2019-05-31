#ifndef ANALYSISSAMPLETHREADTASK_H
#define ANALYSISSAMPLETHREADTASK_H

#include <QObject>
#include <QRunnable>
#include "all_base_struct.h"

class AnalysisSampleThreadTask:public QObject,public QRunnable
{
    Q_OBJECT
public:
    AnalysisSampleThreadTask(const QString &str_sample);
    ~AnalysisSampleThreadTask();
    virtual void run();
//    void analysisSample(SampleInfo &sampleInfo, ExonInfo &exonInfo,
//                        QVector<FileInfo>& fileInfos, QVector<FileInfo>& gsspFileInfos);
signals:
    void analysisfinished();
private:
    int comparePatternWithAllele(char *patternSeq,  char *consensusSeq, const QString &geneName,
                                 int exonStartPos, int minExonIndex, int maxExonIndex,
                                 QVector<QString> &typeResult, QSet<QString> &sheildAlleles);
    int compareGsspWithAlleles(const QByteArray &gsspName, const char *gsspSequence, const QString &geneName,
                               int exonStartPos, QString &typeResult, QSet<QString> &zeroResult);
    void analysisSample(SampleInfo &sampleInfo, ExonInfo &exonInfo,
                        QVector<FileInfo>& fileInfos, QVector<FileInfo>& gsspFileInfos);
private:
    QString m_sample;
};

#endif // ANALYSISSAMPLETHREADTASK_H
