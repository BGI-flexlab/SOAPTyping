#ifndef SOAPTYPINGDB_H
#define SOAPTYPINGDB_H

#include <QSqlDatabase>
#include "all_base_struct.h"
#include "Core/fileTablebase.h"

class SoapTypingDB
{
public:
    static SoapTypingDB *GetInstance()
    {
        static SoapTypingDB instance;
        return &instance;
    }
    ~SoapTypingDB();
    void GetGsspNames(QStringList &gsspNames);      //获取gssp名称列表
    bool GetCommonGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR);
    void GetGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR);
    void GetExonInfo(const QString &gene_name, int exon_index, ExonInfoS& exonInfo);
    int  GetExcludePosition(const QString &key, int &lefpos, int &rightpos);
    void InsertOneFileTable(Ab1FileTableBase &fileTable);
    void insertOneGsspFileTable(Ab1FileTableBase &fileTable);
    int getFileInfoFromRealTimeDatabase(const QString &sampleName, QVector<FileInfo> &fileInfos,
                                        ExonInfo &exonInfo);
    int getGsspFileInfoFromRealTimeDatabase(const QString &sampleName, QVector<FileInfo> &gsspFileInfos,
                                                          ExonInfo &exonInfo);
    void compareWithExonInfo(ExonInfo &exonInfo, int exonIndex, int exonStartPos, int exonEndPos);
    void modifySequence(QByteArray &sequence, QSet<int> &editPostion, int exonStartPos, int excludeLeft,
                                      int excludeRight, const QString &editString);
    void insertSampleInfoToRealTimeDatabase(const SampleInfo &sampleInfo);
    void getConsensusSequenceFromStaticDatabase(const QString &geneName, QByteArray &geneSequence,
                                                int exonStartPos, int exonLength);
    void getShieldAllelesFromDatabase(const QString &sampleName, QSet<QString> &shieldAlleles);
    void getAlleleInfosFromStaticDatabase(const QString &geneName, int exonStartPos, int alleleLength,
                                          int minExonIndex, int maxExonIndex, QVector<AlleleInfo> &alleleInfos,
                                          QSet<QString> &shieldAlleles);
    void getGsspAlleleInfosFromStaticDatabase(const QString &geneName, int exonStartPos, int gsspLength,
                                              QVector<GsspAlleleInfo> &gsspAlleleInfos, const QString &gsspName);
    void getGsspPosAndSeqFromGsspDatabase(const QString &gsspName, int &gsspPos, QString &gsspSeq);
    void updateGsspFileResultToRealTimeDatabase(const QString &fileName, int alignResult,
                                                const QString &typeResult, const QString &filterResult);

    void getSampleTreeDataFromSampleTable(QMap<QString,SampleTreeInfo_t> &map_sampleTreeInfo);
    void getFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo_t> &fileTreeInfos);
    void getGsspFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo_t> &gsspTreeInfos);
    void getResultDataFromsampleTable(const QString &sampleName, bool isCombined, QStringList &typeResult);
    void getResultDataFromGsspTable(const QString &fileName, bool isGsspFilter,
                                                  bool isGssp, QStringList &typeResult);

    void getAlldataFormRealTime(const QString &sampleName, int exonIndex,
                                 QVector<Ab1FileTableBase> &vec_filetable);

    void getExonPositionIndex(const QString &geneName, QVector<int> &position);
    void getExonNavigatorInfo(const QString &sampleName, ExonNavigatorInfo &exonNavigatorInfo);

    void getBaseAlignSampleInfo(const QString &sampleName, BaseAlignSampleInfo &baseAlignSampleInfo);

    void getMarkTypeAndAnalysisFromSampleTable(const QString &sampleName, int &marktype, int &analysis);
    void setMarkTypeBySampleName(const QString &sampleName, int type);

    void getSetNoteFromSampleTable(const QString &sampleName, QString &noteinfo);
    void updateSetNoteBySampleName(const QString &sampleName, const QString &info);

    void deleteSample(const QString &sampleName);
    void deleteFile(bool isgssp, const QString &str_filename);

    void saveSample(const QString &sampleName,const QString &samplePath,const QString &date);
    void saveFile(bool isGssp, const QString &fileName, const QString &filePath,const QString &dir,
                  const QString &date);

    void getIndelInfoFromalleleTable(const QString &alleleName, IndelInfo &indelInfo);
    void updateShieldAllelesToSampleTable(const QString &sampleName, const QString &alleles);

    void updateSetGsspBySampleName(const QString& sampleName, const QString &gsspInfo);
    void updateSetResultBySampleName(const QString &sampleName, const QString &result);
    void getSetResultBySampleName(const QString &sampleName, QString & result);
    void getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq,int alignStartPos = 0, int alignLength = 0);
    void getExonIndexAndGeneBySampleName(const QString &sampleName, int &exonStart, int &exonEnd, QByteArray &geneName);
    void getGsspTablesFromGsspDatabase(const QString &geneName, int exon, QVector<GsspTable> &gsspTables);
    void getExonPositionIndexFromStaticDatabase(const QString &geneName, QVector<int> &position);
    void insertOneSampleTable(SampleTable &sampleTable);
    void getSampleNamesFromRealTimeDatabase(QStringList &sampleNames);
    void getSampleStartEndBySampleName(const QString &sampleName, int &start, int &end);
    int getResultFromRealTimeDatabaseBySampleName(const QString &sampleName, QString &result);
    bool isIndelInRange(const QString &alleleName, int start, int end);

    int getMarkTypeBySampleName(const QString &sampleName);
    void resetFileByFileName(const QString &fileName, bool isGssp);
    void markAllSampleApproved();
    int markAllSampleReviewed();
    void getAlleleNameListFromStaticDabase(const QString &geneName, QStringList &alleleNames);

    void getGeneNames(QStringList &geneNames);
    void getGeneVersion(QString &strver);
    void getExonTrimListByGeneName(const QString &geneName, QVector<ExonTrimTable> &exonTrimTableList);
    void updateExonTrim(const ExonTrimTable &exonTrimTable);
    void getAlleleNamesAndSeqsByGeneName(const QString &geneName, QStringList &alleleNames,
                                         QStringList &alleleSeqs, QVector< QVector<int> > &misPositions);
    void getAlleleSequenceByAlleleName(const QString &alleleName, QString &alleleSeq);

    bool upDateAlignInfo(const QString &filename, const QString &straligninfo,bool isgssp);
    bool upDateUsefulSeq(const QString &filename, const QString &usefulseq,bool isgssp);
    bool upDatabyChangebp(const QString &filename, const QString &streditinfo,bool isgssp);
    bool upDataExclude(bool isgssp, const QString &filename, int exclude_left, int exclude_right);
    bool upDataAlignPos(bool isgssp, const QString &filename, int start, int end);
    bool deleteTable(const QString &tableName);
    void readGeneTableTxtFile(const QString &geneFile);
    void insertGeneTable(const GeneTable &geneTable);
    void readAlleleTableTxtFile(const QString &alleleFile);
    void insertAlleleTable(const AlleleTable &alleleTable);
    void readGsspTableTxtFile(const QString &gsspFile);
    void insertGsspTable(const GsspTable &gsspTable);
    void readLabAlignTableTxtFile(const QString &labAlignFile);
    void insertLabAlignTable(const LabAlignTable &labAlignTable);
    void readCommonGsspTableTxt(const QString &txtFile);
    void insertCommonGsspTable(const CommonGsspTable &commonGsspTable);

    void StartTransaction();
    void EndTransaction();
    void getTypeResultFromSampleTable(const QString &sampleName, QMap<int, QString> &typeResult);
    bool getSampleanalysisType(const QString &samplename, SampleTreeInfo_t &sampleTreeInfo);
private:
    bool InitDB();

public:
    SoapTypingDB();
    SoapTypingDB(const SoapTypingDB &){}
    SoapTypingDB& operator=(const SoapTypingDB&){}
private:
    static QSqlDatabase m_SqlDB;
    //QSqlQuery *m_pQuery;
};

#endif // SOAPTYPINGDB_H
