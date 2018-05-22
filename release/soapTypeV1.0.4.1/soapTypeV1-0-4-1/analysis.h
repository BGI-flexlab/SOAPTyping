#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QtCore>
#include "staticdatabase.h"
#include "realtimedatabase.h"
struct PairStartEnd
{
    int startIndex;
    int endIndex;
};
bool runComparePA(const char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector< QVector<int> > &differentPosInfos, QMap<int, QString> *typeResult, PairStartEnd pair);
bool analysisSample(const QByteArray &sampleName);
void mergeFileSequenceToSampleSequence(char *fileSequence, char *sampleSequence);
bool isEqualPC(char consensus, char pattern);
char mergeBases(char A, char B);
void mergeForwardAndReverseToPattern(char *forwardSeq, char *reverseSeq, char *patternSeq, int exonStartPos, QStringList &frDifferenceList, QStringList &frUnequlList);
void comparePatternWithConsensus(char *patternSeq, char *consensusSeq, int exonStartPos, QStringList &pcDifferenceList);
int comparePatternWithAllele(char *patternSeq, const QByteArray &geneName, int exonStartPos, int minExonIndex, int maxExonIndex, QMap<int, QString> &typeResult, QSet<QString> &shieldAlleles);
void removeAlleleInfoByPattern(char *patternSeq, QVector<AlleleInfo> &alleleInfos);
void getDifferentPositionInfo(char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector< QVector<int> > &differentPosInfos);
int comparePatternWithAlleleByThread(char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector< QVector<int> > &differentPosInfos, QMap<int, QString> &typeResult);
int compareByAlleleInfoAndDiffPos(const char *patternSeq, const AlleleInfo &alleleInfoi, const AlleleInfo &alleleInfoj, const QVector<int> &diffInfoi, const QVector<int> &diffInfoj, QString &alignInfo);
unsigned int formatMerge(char A);
char reFormatMerge(unsigned int a);
void changeAlignMapToAlignString(QMap<int, QString> &typeResult, QString &result);
int compareGsspWithAlleles(const QByteArray &gsspName, const char *gsspSequence, const QByteArray &geneName, int exonStartPos, QString &typeResult, QSet<QString> &zeroResult);
void getCombinedAlleles(QSet<QString> &zeroCombinedAlleles, QSet<QString> &zeroGssp, int i);
void getFilterResult(QString &result, QMap<int, QString> &typeResult, QSet<QString> &zeroAlleles, const QString &name);
void shieldAllele(const QString &sampleName, const QString &alleles);
void setOpenThread(bool isOpen);
bool getOpenTrhead();
void removeUnRelationAllelesByGssp(QString gsspName, QVector<GsspAlleleInfo> &gsspAlleleInfos, int start, int length);
void removIndelAlleles(QVector<AlleleInfo> &alleleInfos);
void changeThreadNum(int);
void getCombinedResult(QMap<int, QString> &typeResult, QSet<QString> &zeroAlleles);
void getZeroResultFromTypeResult(QMap<int, QString> &typeResult, QMap<int, QString> &zeroResult);
#endif // ANALYSIS_H
