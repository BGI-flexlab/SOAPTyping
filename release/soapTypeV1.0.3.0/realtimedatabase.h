#ifndef REALTIMEDATABASE_H
#define REALTIMEDATABASE_H
#include <QtCore>
#include "All_Base_Struct.h"

struct FileTreeInfo
{
    int isGood;
    int analysisType;
    int exonIndex;
    QString rOrF;
    QString fileName;
};

struct GsspTreeInfo
{
    int isGood;
    int analysisType;
    int exonIndex;
    QString rOrF;
    QString gsspName;
    QString fileName;
};

struct SampleTreeInfo
{
    int analysisType;
    int markType;
    QString sampleName;
    QString geneName;
    QVector<FileTreeInfo> fileTreeInfo;
    QVector<GsspTreeInfo> gsspTreeInfo;
};

//struct SignalInfo
//{
//    bool isUndefined;   //若是没有定义, 则外显子没有错配, 型别没有列表,
//    bool isCombined;  //只有型别表有信息, 比对表还不清楚
//    bool isGsspFilter; //型别表显示不一样. 比对表也是
//    bool isGssp; //型别表..
//    int exonIndex;
//    QString sampleName; //样品名, 若换, 其他模块肯定会换
//    QString rOrF;
//    QString geneName;
//    QString fileName;
//    SignalInfo()
//    {
//        isUndefined = false;
//        isCombined = false;
//        isGsspFilter = false;
//        isGssp = false;
//    }
//};
enum MarkType
{
    OWNED = 0,
    PENDING,
    REVIEWED,
    APPROVED
};
enum AnalysisType
{
    MATCHTOTAL = 0,
    MATCHRARE,
    MATCHBAD,
    MISMATCH,
    UNMATCH
};
enum FileQual
{
    GOOD = 0,
    BAD
};

struct SampleTable
{
    QByteArray sampleName;
    QByteArray geneName;
    int fileType;
    int markType;
    int analysisType;
    int minExonIndex;
    int maxExonIndex;
    int exonStartPos;
    int exonEndPos;
    QByteArray consensusSequence;
    QByteArray forwardSequence;
    QByteArray reverseSequence;
    QByteArray patternSequence;
    QString mismatchBetweenPC;
    QString mismatchBetweenFR;
    QString mmismatchBetweenFR;
    QString editPostion;
    QString typeResult;
    QString gsspInfo;
    QString shieldAllele;
    QString setResult;
    QString setNote;
    QString setGSSP;
    QString combinedResult;
    SampleTable()
    {
        fileType = 0;
        markType = OWNED;
    }
};

struct SampleInfo
{
    QByteArray sampleName;
    QByteArray geneName;
    int analysisType;
    int minExonIndex;
    int maxExonIndex;
    int exonStartPos;
    int exonEndPos;
    QByteArray consensusSequence;
    QByteArray forwardSequence;
    QByteArray reverseSequence;
    QByteArray patternSequence;
    QString mismatchBetweenPC;
    QString mismatchBetweenFR;
    QString mmismatchBetweenFR;
    QString editPostion;
    QString typeResult;
    QString combinedResult;
};


struct ExonInfo
{
    int minExonIndex;
    int maxExonIndex;
    int exonStartPosition;
    int exonEndPostion;
    ExonInfo()
    {
        minExonIndex = 100;
        maxExonIndex = 0;
        exonStartPosition = 2000;
        exonEndPostion = -1;
    }
};

struct FileInfo
{
    int exonStartPos;
    int exonEndPos;
    int isGood;
    int isUnMatch;
    QByteArray fileName;
    QByteArray geneName;
    QByteArray rOrF;
    QByteArray gsspName;
    QByteArray sequence;
    QSet<int> editPostion;
};

struct BaseAlignGsspInfo
{
    int gsspFileAlignStartPos;
    QString gsspName;
    int gsspPostion;
    QString gsspSeq;
    QString gsspFileSeq;
};

struct BaseAlignSampleInfo
{
    int alignStartPos;
    int alignEndPos;
    QString consensusSeq;
    QString forwardSeq;
    QString reverseSeq;
    QString patternSeq;
    QSet<int> pcMisMatchPostion;
    QSet<int> frMisMatchPostion;
    QSet<int> editPostion;
    QMap<QString, BaseAlignGsspInfo> gsspInfoMap;
    void clear()
    {
        alignStartPos = 0;
        alignEndPos = 0;
        consensusSeq.resize(0);
        forwardSeq.resize(0);
        reverseSeq.resize(0);
        patternSeq.resize(0);
        pcMisMatchPostion.clear();
        frMisMatchPostion.clear();
        editPostion.clear();
        gsspInfoMap.clear();
    }
};
struct ExonNavigatorInfo
{
    int minExonIndex;
    int maxExonIndex;
    QVector<int> pcMis;
    QVector<int> frMis;
    QVector<int> frUnEqual;
    QVector<int> editPos;
    void clear()
    {
        minExonIndex = 0;
        maxExonIndex = 0;
        pcMis.clear();
        frMis.clear();
        frUnEqual.clear();
        editPos.clear();
    }
};

void createRealTimeDatabase();
void createSampleTable();
void createFileTable();
void createGsspFileTable();
void loadRealTimeDatabase();
void removeRealTiemDatabase();
void newStartRealTimeDatabase();
void showSampleTable();
void showFileTable();
void showGsspFileTable();

void insertSampleToRealTimeDatabase(const SampleTable &sampleTable);
void insertFileToRealTimeDatabase(const FileTable &fileTable);
void insertGsspFileToRealTimeDatabase(const GsspFileTable &gsspFileTable);
void insertOneSampleTable(const SampleTable &sampleTable);
void insertOneFileTable(const FileTable &fileTable);
void insertOneGsspFileTable(const GsspFileTable &gsspFileTable);
void updateOneSampleTable(const SampleTable &sampleTable);
void updateOneFileTable(const FileTable &fileTable);
void updateOneGsspFileTable(const GsspFileTable &gsspFileTable);

int getFileInfoFromRealTimeDatabase(const QByteArray &sampleName, QVector<FileInfo> &fileInfos, ExonInfo &exonInfo);
int getGsspFileInfoFromRealTimeDatabase(const QByteArray &sampleName, QVector<FileInfo> &gsspFileInfos, ExonInfo &exonInfo);

void compareWithExonInfo(ExonInfo &exonInfo, int exonIndex, int exonStartPos, int exonEndPos);
void modifySequence(QByteArray &sequence, QSet<int> &editPostion, int exonStartPos,int excludeLeft, int excludeRight, const QString &editString);
void getShieldAllelesFromDatabase(const QByteArray &sampleName, QSet<QString> &shieldAlleles);

void updateGsspFileResultToRealTimeDatabase(const QByteArray &fileName, int alignResult, const QString &typeResult, const QString &filterResult);
void insertSampleInfoToRealTimeDatabase(const SampleInfo &sampleInfo);
void updateSampleInfoToRealTimeDatabaseT(const SampleInfo &sampleInfo);
void insertSampleInfoToRealTimeDatabaseT(const SampleInfo &sampleInfo);
void getBaseAlignSampleInfo(const QString &sampleName, BaseAlignSampleInfo &baseAlignSampleInfo);
void getSampleTreeDataFromRealTimeDatabase(QVector<SampleTreeInfo> &sampleTreeInfoList);
void getFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo> &fileTreeInfos);
void getGsspFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<GsspTreeInfo> &gsspTreeInfos);

void getExonNavigatorInfo(const QString &sampleName, ExonNavigatorInfo &exonNavigatorInfo);
void insertShieldAllelesToSampleTable(const QByteArray &sampleName, const QString &alleles);

void getCombinedFromRealTimeDatabase(const QByteArray &sampleName, QStringList &typeResult);
void getTypeResultFromRealTimeDatabse(const QByteArray &sampleName, QStringList &typeResult);
void getGsspFilterFromRealTimeDatabase(const QByteArray &fileName, QStringList &typeResult);
void getGsspResultFromRealTimeDatabase(const QByteArray &fileName, QStringList &typeResult);

void markAllSampleApproved();
int markAllSampleReviewed();
int getMarkTypeBySampleName(const QByteArray &sampleName);
int getAnalysisTypeBySampleName(const QByteArray &sampleName);
void setMarkTypeBySampleName(const QByteArray &sampleName, int type);
void resetGsspFileByFileName(const QByteArray &fileName);
void resetFileByFileName(const QByteArray &fileName);
void getSampleStartEndBySampleName(const QByteArray &sampleName, int &start, int &end);


QList<FileInfo_ly> OgetAllData(QString sampleName ,int exonIndex);
QList<FileInfo_ly> OgetOneData(QString fileName ,bool isgssp);
bool upDatabyChangebp(QString filename, QString editinfo,bool isgssp);
bool updatabyLeftExclude(QString filename ,int pos,bool isgssp);
bool updatabyRightExclude(QString filename ,int pos,bool isgssp);
int getcountof_file();


#define SAMPLE_TABLE_FIELD 24
#define FILE_TABLE_FIELD 30
#define GSSP_FILE_TABLE_FIELD 32

void saveSample(const QByteArray &,const QString&,const QString&);
void saveFile(const QByteArray&,const QString&,const QString&,const QString&);
void saveGssp(const QByteArray&,const QString&,const QString&,const QString&);
void loadSample(const QString&);
void loadFile(const QString&);
void loadGssp(const QString&);


//delete
void deleteSample(const QByteArray&);
void deleteFile(const QByteArray&);
void getSampleNamesFromRealTimeDatabase(QStringList &sampleNames);
int getResultFromRealTimeDatabaseBySampleName(const QByteArray &sampleName, QString &result);

QString getSetResultBySampleName(const QByteArray &sampleName);
void updateSetResultBySampleName(const QByteArray &sampleName, const QString &result);
QString getSetNoteBySampleName(const QByteArray &sampleName);
void updateSetNoteBySampleName(const QByteArray &sampleName, const QString &info);
void updateSetGsspBySampleName(const QByteArray sampleName, const QString &gsspInfo);
QString getSetGsspBySampleName(const QByteArray &sampleName);

void getExonIndexAndGeneBySampleName(const QByteArray &sampleName, int &exonStart, int &exonEnd, QByteArray &geneName);
#endif // REALTIMEDATABASE_H
