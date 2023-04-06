#include "soaptypingdb.h"
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QMutex>
#include <QTime>
#include <QThread>
#include <QFile>
#include <QDir>
#include "all_base_struct.h"
#include "log/log.h"

QMutex g_mutex;
QSqlDatabase SoapTypingDB::m_SqlDB;

const int SAMPLE_TABLE_FIELD = 24;
const int FILE_TABLE_FIELD = 30;
const int GSSP_FILE_TABLE_FIELD =32;

SoapTypingDB::SoapTypingDB()
{
    InitDB();
}

SoapTypingDB::~SoapTypingDB()
{
    m_SqlDB.close();
}

bool SoapTypingDB::InitDB()
{
    //QMutexLocker locker(&g_mutex);
    QString str = QTime::currentTime().toString("hh_mm_ss_zzz");
    QString str_connect = QString("connect-%1").arg(str);
    m_SqlDB = QSqlDatabase::addDatabase("QSQLITE",str_connect);
    m_SqlDB.setDatabaseName("./DataBase/SoapTypingDB.db");

    bool bRet = m_SqlDB.open();
    if(!bRet)
    {
        QSqlError sql_err  = m_SqlDB.lastError();
        qDebug()<<sql_err.text();
    }
    return bRet;
}

void SoapTypingDB::GetGsspNames(QStringList &gsspNames)
{
    QSqlQuery query(m_SqlDB);
    QString str_sql("SELECT gsspName FROM gsspTable");
    bool bRet = query.prepare(str_sql);
    if(bRet)
    {
        bRet = query.exec(str_sql);
        if(bRet)
        {
            while(query.next())
            {
                gsspNames.push_back(query.value(0).toString());
            }
        }
        else
        {
            QSqlError err = query.lastError();
            QString str_err = err.text();
            qDebug()<<str_err;
        }
    }
}

//CommonGssp里的数据虽然是引物，但是当作正常的序列进行处理
bool SoapTypingDB::GetCommonGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT gsspName, exonIndex,fOrR FROM commonGsspTable");
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            ExonAndRF exonAndRF;
            exonAndRF.exonIndex = query.value(1).toString();
            exonAndRF.rOrF = query.value(2).toString();
            mapToExonAndFR.insert(query.value(0).toString(), exonAndRF);
        }
    }
}

void SoapTypingDB::GetGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR)
{
    QSqlQuery query(m_SqlDB);
    QString str_sql("SELECT gsspName,exonIndex,rOrF FROM gsspTable");
    query.prepare(str_sql);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            ExonAndRF exonAndRF;
            exonAndRF.exonIndex = query.value(1).toString();
            exonAndRF.rOrF = query.value(2).toString();
            mapToExonAndFR.insert(query.value(0).toString(), exonAndRF);
        }
    }
}

void SoapTypingDB::GetExonInfo(const QString &gene_name, int exon_index, ExonInfoS& exonInfo)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT geneSequence,exonPositionIndex FROM GeneTable WHERE geneName = ?");
    query.bindValue(0, gene_name);
    bool isSuccess = query.exec();
    if (isSuccess)
    {
        while(query.next())
        {
            exonInfo.exonStartPos = query.value(1).toString().split(":")[exon_index-1].toInt();
            exonInfo.exonEndPos = query.value(1).toString().split(":")[exon_index].toInt();
            exonInfo.consensusSeq = query.value(0).toByteArray().mid(exonInfo.exonStartPos,
                                                                     exonInfo.exonEndPos-exonInfo.exonStartPos);
        }
    }
}

int SoapTypingDB::GetExcludePosition(const QString &key, int &lefpos, int &rightpos)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    //query.prepare("SELECT exonStart,exonEnd,excludeLeft,excludeRight FROM exonTrimTable WHERE etKey=?");
    query.prepare("SELECT excludeLeft,excludeRight FROM exonTrimTable WHERE etKey=?");
    query.bindValue(0, key);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        if(query.next())
        {
            lefpos = query.value(0).toInt();
            rightpos = query.value(1).toInt();
//            int start = query.value(0).toInt();
//            int end = query.value(1).toInt();
//            int l_pos = query.value(2).toInt();
//            int r_pos = query.value(3).toInt();
//            if(l_pos <= 0 || l_pos > end-start)
//            {
//                lefpos = -1;
//            }
//            else
//            {
//                lefpos = start + l_pos - 1;
//            }

//            if(r_pos <= 0 || r_pos > end-start)
//            {
//                rightpos = -1;
//            }
//            else
//            {
//                rightpos = end - r_pos;
//            }
        }
    }
    return -1;
}


void SoapTypingDB::InsertOneFileTable(Ab1FileTableBase &fileTable)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("insert or replace INTO fileTable (fileName,"
                  "sampleName,"
                  "filePath,"
                  "isExtraFile,"
                  "geneName,"
                  "exonIndex,"
                  "rOrF,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "usefulSequence,"
                  "baseSequence,"
                  "basePostion,"
                  "baseQuality,"
                  "baseNumber,"
                  "baseASignal,"
                  "baseTSignal,"
                  "baseGSignal,"
                  "baseCSignal,"
                  "signalNumber,"
                  "maxSignal,"
                  "maxQuality,"
                  "averageBaseWidth,"
                  "isGood,"
                  "alignResult,"
                  "alignStartPos,"
                  "alignEndPos,"
                  "alignInfo,"
                  "excludeLeft,"
                  "excludeRight,"
                  "editInfo,"
                  "avgSignal)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, fileTable.getFileName());
    query.bindValue(1, fileTable.getSampleName());
    query.bindValue(2, fileTable.getFilePath());
    query.bindValue(3, fileTable.getExtraFile());
    query.bindValue(4, fileTable.getGeneName());
    query.bindValue(5, fileTable.getExonIndex());
    query.bindValue(6, fileTable.getROrF());
    query.bindValue(7, fileTable.getExonStartPos());
    query.bindValue(8, fileTable.getExonEndPos());
    query.bindValue(9, fileTable.getUsefulSequence());
    query.bindValue(10, fileTable.getBaseSequence());
    query.bindValue(11, fileTable.getBasePostion());
    query.bindValue(12, fileTable.getBaseQuality());
    query.bindValue(13, fileTable.getBaseNumber());
    query.bindValue(14, fileTable.getBaseASignal());
    query.bindValue(15, fileTable.getBaseTSignal());
    query.bindValue(16, fileTable.getBaseGSignal());
    query.bindValue(17, fileTable.getBaseCSignal());
    query.bindValue(18, fileTable.getSignalNumber());
    query.bindValue(19, fileTable.getMaxSignal());
    query.bindValue(20, fileTable.getMaxQuality());
    query.bindValue(21, fileTable.getAverageBaseWidth());
    query.bindValue(22, fileTable.getIsGood());
    query.bindValue(23, fileTable.getAlignResult());
    query.bindValue(24, fileTable.getAlignStartPos());
    query.bindValue(25, fileTable.getAlignEndPos());
    query.bindValue(26, fileTable.getAlignInfo());
    query.bindValue(27, fileTable.getExcludeLeft());
    query.bindValue(28, fileTable.getExcludeRight());
    query.bindValue(29, fileTable.getEditInfo());
    query.bindValue(30, fileTable.getAvgsignal());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"insert fileTable error";
    }
}

void SoapTypingDB::insertOneGsspFileTable( Ab1FileTableBase &fileTable)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT or replace INTO gsspFileTable (fileName,"
                  "sampleName,"
                  "filePath,"
                  "gsspName,"
                  "geneName,"
                  "exonIndex,"
                  "rOrF,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "usefulSequence,"
                  "baseSequence,"
                  "basePostion,"
                  "baseQuality,"
                  "baseNumber,"
                  "baseASignal,"
                  "baseTSignal,"
                  "baseGSignal,"
                  "baseCSignal,"
                  "signalNumber,"
                  "maxSignal,"
                  "maxQuality,"
                  "averageBaseWidth,"
                  "isGood,"
                  "alignResult,"
                  "alignStartPos,"
                  "alignEndPos,"
                  "alignInfo,"
                  "excludeLeft,"
                  "excludeRight,"
                  "editInfo,"
                  "typeResult,"
                  "filterResult)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, fileTable.getFileName());
    query.bindValue(1, fileTable.getSampleName());
    query.bindValue(2, fileTable.getFilePath());
    query.bindValue(3, fileTable.getGsspName());
    query.bindValue(4, fileTable.getGeneName());
    query.bindValue(5, fileTable.getExonIndex());
    query.bindValue(6, fileTable.getROrF());
    query.bindValue(7, fileTable.getExonStartPos());
    query.bindValue(8, fileTable.getExonEndPos());
    query.bindValue(9, fileTable.getUsefulSequence());
    query.bindValue(10, fileTable.getBaseSequence());
    query.bindValue(11, fileTable.getBasePostion());
    query.bindValue(12, fileTable.getBaseQuality());
    query.bindValue(13, fileTable.getBaseNumber());
    query.bindValue(14, fileTable.getBaseASignal());
    query.bindValue(15, fileTable.getBaseTSignal());
    query.bindValue(16, fileTable.getBaseGSignal());
    query.bindValue(17, fileTable.getBaseCSignal());
    query.bindValue(18, fileTable.getSignalNumber());
    query.bindValue(19, fileTable.getMaxSignal());
    query.bindValue(20, fileTable.getMaxQuality());
    query.bindValue(21, fileTable.getAverageBaseWidth());
    query.bindValue(22, fileTable.getIsGood());
    query.bindValue(23, fileTable.getAlignResult());
    query.bindValue(24, fileTable.getAlignStartPos());
    query.bindValue(25, fileTable.getAlignEndPos());
    query.bindValue(26, fileTable.getAlignInfo());
    query.bindValue(27, fileTable.getExcludeLeft());
    query.bindValue(28, fileTable.getExcludeRight());
    query.bindValue(29, fileTable.getEditInfo());
    query.bindValue(30, fileTable.getTypeResult());
    query.bindValue(31, fileTable.getFilterResult());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"insert gsspFileTable error";
    }
}


int SoapTypingDB::getFileInfoFromRealTimeDatabase(const QString &sampleName, QVector<FileInfo> &fileInfos,
                                                  ExonInfo &exonInfo)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT isExtraFile,"
                  "geneName,"
                  "exonIndex,"
                  "rOrF,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "usefulSequence,"
                  "alignResult,"
                  "excludeLeft,"
                  "excludeRight,"
                  "editInfo,"
                  "isGood "
                  "FROM fileTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    int flag = 0;
    if(isSuccess)
    {

        while(query.next()){
            if(query.value(7).toInt() != 0)//alignResult
            {
                flag = 1;
                continue;
            }
            FileInfo fileInfo;
            fileInfo.geneName = query.value(1).toByteArray();
            fileInfo.rOrF = query.value(3).toByteArray();
            fileInfo.exonStartPos = query.value(4).toInt();
            fileInfo.exonEndPos = query.value(5).toInt();
            fileInfo.sequence = query.value(6).toByteArray();
            fileInfo.isGood = query.value(11).toInt();
            modifySequence(fileInfo.sequence, fileInfo.editPostion, fileInfo.exonStartPos, query.value(8).toInt(),
                           query.value(9).toInt(), query.value(10).toString());
            compareWithExonInfo(exonInfo, query.value(2).toInt(), fileInfo.exonStartPos, fileInfo.exonEndPos);
            fileInfos.push_back(fileInfo);
        }
    }
    return flag;
}

int SoapTypingDB::getGsspFileInfoFromRealTimeDatabase(const QString &sampleName, QVector<FileInfo> &gsspFileInfos,
                                                      ExonInfo &exonInfo)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT gsspName,"
                  "geneName,"
                  "exonIndex,"
                  "rOrF,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "usefulSequence,"
                  "alignResult,"
                  "excludeLeft,"
                  "excludeRight,"
                  "editInfo,"
                  "isGood,"
                  "fileName "
                  "FROM gsspFileTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    int flag = 0;
    if(isSuccess)
    {
        while(query.next())
        {
            if(query.value(7).toInt() == UNMATCH)//alignResult
            {
                flag = 1;
                continue;
            }
            FileInfo fileInfo;
            fileInfo.gsspName = query.value(0).toByteArray();
            fileInfo.geneName = query.value(1).toByteArray();
            fileInfo.rOrF = query.value(3).toByteArray();
            fileInfo.exonStartPos = query.value(4).toInt();
            fileInfo.exonEndPos = query.value(5).toInt();
            fileInfo.sequence = query.value(6).toByteArray();
            fileInfo.isGood = query.value(11).toInt();
            fileInfo.fileName = query.value(12).toByteArray();
            modifySequence(fileInfo.sequence, fileInfo.editPostion, fileInfo.exonStartPos, query.value(8).toInt(),
                           query.value(9).toInt(), query.value(10).toString());
            compareWithExonInfo(exonInfo, query.value(2).toInt(), fileInfo.exonStartPos, fileInfo.exonEndPos);
            gsspFileInfos.push_back(fileInfo);
        }
    }
    return flag;
}

void SoapTypingDB::compareWithExonInfo(ExonInfo &exonInfo, int exonIndex, int exonStartPos, int exonEndPos)
{
    if(exonIndex < exonInfo.minExonIndex)
        exonInfo.minExonIndex = exonIndex;
    if(exonInfo.maxExonIndex < exonIndex)
        exonInfo.maxExonIndex = exonIndex;
    if(exonStartPos < exonInfo.exonStartPosition)
        exonInfo.exonStartPosition = exonStartPos;
    if(exonInfo.exonEndPostion < exonEndPos)
        exonInfo.exonEndPostion = exonEndPos;
}

void SoapTypingDB::modifySequence(QByteArray &sequence, QSet<int> &editPostion, int exonStartPos, int excludeLeft,
                                  int excludeRight, const QString &editString)
{
    if(editString.size()>0)
    {
        QStringList edits = editString.split(";", QString::SkipEmptyParts);
        for(int i=0; i<edits.size(); i++)
        {
            QStringList posBase = edits.at(i).split(":", QString::SkipEmptyParts);
            int pos = posBase.at(1).toInt()-exonStartPos;
            sequence[pos] = posBase.at(2).at(0).toLatin1();
            editPostion.insert(posBase.at(1).toInt());
        }
    }
    if(excludeLeft > 0)
    {
        for(int i=0;i<sequence.size();i++)
        {
            if(excludeLeft)
            {
                if(sequence[i]!='-')
                {
                    sequence[i] = '-';
                    excludeLeft--;
                }
            }
            else
            {
                break;
            }
        }
    }
    if(excludeRight > 0)
    {
        for(int i=sequence.size();i>0;i--)
        {
            if(excludeRight)
            {
                if(sequence[i]!='-')
                {
                    sequence[i] = '-';
                    excludeRight--;
                }
            }
            else
            {
                break;
            }
        }
    }
}

void SoapTypingDB::insertSampleInfoToRealTimeDatabase(const SampleInfo &sampleInfo)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT or replace INTO sampleTable (sampleName,"
                  "geneName,"
                  "analysisType,"
                  "minExonIndex,"
                  "maxExonIndex,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "consensusSequence,"
                  "forwardSequence,"
                  "reverseSequence,"
                  "patternSequence,"
                  "mismatchBetweenPC,"
                  "mismatchBetweenFR,"
                  "mmismatchBetweenFR,"
                  "editPostion,"
                  "typeResult,"
                  "combinedResult,"
                  "markType)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, sampleInfo.sampleName);
    query.bindValue(1, sampleInfo.geneName);
    query.bindValue(2, sampleInfo.analysisType);
    query.bindValue(3, sampleInfo.minExonIndex);
    query.bindValue(4, sampleInfo.maxExonIndex);
    query.bindValue(5, sampleInfo.exonStartPos);
    query.bindValue(6, sampleInfo.exonEndPos);
    query.bindValue(7, sampleInfo.consensusSequence);
    query.bindValue(8, sampleInfo.forwardSequence);
    query.bindValue(9, sampleInfo.reverseSequence);
    query.bindValue(10, sampleInfo.patternSequence);
    query.bindValue(11, sampleInfo.mismatchBetweenPC);
    query.bindValue(12, sampleInfo.mismatchBetweenFR);
    query.bindValue(13, sampleInfo.mmismatchBetweenFR);
    query.bindValue(14, sampleInfo.editPostion);
    query.bindValue(15, sampleInfo.typeResult);
    query.bindValue(16, sampleInfo.combinedResult);
    query.bindValue(17, OWNED);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"insertSampleInfoToRealTimeDatabaseT error:"<<sampleInfo.sampleName;
    }
}

void SoapTypingDB::getConsensusSequenceFromStaticDatabase(const QString &geneName, QByteArray &geneSequence,
                                            int exonStartPos, int exonLength)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT geneSequence FROM geneTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            geneSequence = query.value(0).toByteArray().mid(exonStartPos, exonLength);
        }
    }
}

void SoapTypingDB::getShieldAllelesFromDatabase(const QString &sampleName, QSet<QString> &shieldAlleles)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT shieldAllele FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"getShieldAllelesFromDatabase error:"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            QStringList ss = query.value(0).toString().split(";", QString::SkipEmptyParts);
            for(int i=0; i<ss.size(); i++)
            {
                shieldAlleles.insert(ss.at(i));
            }
        }
    }
}

void SoapTypingDB::getAlleleInfosFromStaticDatabase(const QString &geneName, int exonStartPos, int alleleLength,
                                      int minExonIndex, int maxExonIndex, QVector<AlleleInfo> &alleleInfos,
                                      QSet<QString> &shieldAlleles)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleName,alleleSequence,isRare,isIndel,noStar FROM alleleTable WHERE geneName=? order by alleleName");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            AlleleInfo alleleInfo;
            alleleInfo.alleleName = query.value(0).toString();
            if(shieldAlleles.contains(alleleInfo.alleleName))
                continue;
            alleleInfo.alleleSequence = query.value(1).toByteArray().mid(exonStartPos, alleleLength);
            int isRare = query.value(2).toInt();
            if(isRare)
            {
                alleleInfo.isRare = "r";
            }
            alleleInfo.isIndel = query.value(3).toInt();
            QStringList star = query.value(4).toString().split("", QString::SkipEmptyParts);
            int min = star.at(0).toInt();
            int max = star.at(1).toInt();
            if(min>max)
            {
//                for(int i=minExonIndex; i<=maxExonIndex; i++)
//                {
//                    alleleInfo.starInfo.append(QString("*%1").arg(i));
//                }
                qDebug()<<__func__<<alleleInfo.alleleName<<query.value(4).toString()<<"error";
                continue;
            }
            else
            {
                if(minExonIndex >= min && maxExonIndex<=max)
                {
                    alleleInfo.starInfo.append("-");
                }
                else
                {
                    for(int i=minExonIndex; i<min; i++) //样品外显子范围超过了该型别的外显子范围
                    {
                        alleleInfo.starInfo.append(QString("*%1").arg(i));
                    }
                    for(int i=max; i<maxExonIndex; i++)
                    {
                        alleleInfo.starInfo.append(QString("*%1").arg(i));
                    }
                }
            }
            alleleInfos.push_back(alleleInfo);
        }
    }
}

void SoapTypingDB::getGsspPosAndSeqFromGsspDatabase(const QString &gsspName, int &gsspPos, QString &gsspSeq)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT position,base FROM gsspTable WHERE gsspName =?");
    query.bindValue(0, gsspName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            gsspPos = query.value(0).toInt()-1;
            gsspSeq = query.value(1).toString();
        }
    }
}

void SoapTypingDB::getGsspAlleleInfosFromStaticDatabase(const QString &geneName, int exonStartPos, int gsspLength,
                                          QVector<GsspAlleleInfo> &gsspAlleleInfos, const QString &gsspName)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleName,alleleSequence FROM alleleTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();

    int gssp_pos;
    QString gssp_seq;

    //getGsspPosAndSeqFromGsspDatabase(gsspName, gssp_pos, gssp_seq);

    if(isSuccess)
    {
        while(query.next())
        {
            GsspAlleleInfo gsspAlleleInfo;
            gsspAlleleInfo.alleleName = query.value(0).toString();
            QByteArray alleleSequence = query.value(1).toByteArray();
//            if(alleleSequence[gssp_pos]!=gssp_seq[0].toLatin1())
//                continue;

            gsspAlleleInfo.alleleSequence = alleleSequence.mid(exonStartPos, gsspLength);
            gsspAlleleInfos.push_back(gsspAlleleInfo);//i2++;
        }
    }
}

void SoapTypingDB::updateGsspFileResultToRealTimeDatabase(const QString &fileName, int alignResult,
                                            const QString &typeResult, const QString &filterResult)
{
    QMutexLocker locker(&g_mutex);
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE gsspFileTable set "
                  "alignResult=?,"
                  "typeResult=?,"
                  "filterResult=? "
                  "WHERE fileName=?");
    query.bindValue(0, alignResult);
    query.bindValue(1, typeResult);
    query.bindValue(2, filterResult);
    query.bindValue(3, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"updateGsspFileResultToRealTimeDatabase error"<<fileName;
    }
}

void SoapTypingDB::getSampleTreeDataFromSampleTable(QMap<QString,SampleTreeInfo_t> &map_sampleTreeInfo)
{
    QSqlQuery query(m_SqlDB);
    bool isSuccess = query.exec("SELECT sampleName,geneName,analysisType,markType FROM sampleTable ORDER BY sampleName");
    if(isSuccess)
    {
        while(query.next())
        {
            SampleTreeInfo_t sampleTreeInfo;
            sampleTreeInfo.sampleName = query.value(0).toString();
            sampleTreeInfo.geneName = query.value(1).toString();
            sampleTreeInfo.analysisType = query.value(2).toInt();
            sampleTreeInfo.markType = query.value(3).toInt();
            getFileTreeInfosFromRealTimeDatabase(sampleTreeInfo.sampleName, sampleTreeInfo.treeinfo);
            getGsspFileTreeInfosFromRealTimeDatabase(sampleTreeInfo.sampleName, sampleTreeInfo.treeinfo);
            //sampleTreeInfoList.push_front(sampleTreeInfo);
            map_sampleTreeInfo.insert(sampleTreeInfo.sampleName, sampleTreeInfo);

        }
    }
}

bool SoapTypingDB::getSampleanalysisType(const QString &samplename, SampleTreeInfo_t &sampleTreeInfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT analysisType, markType FROM sampleTable where sampleName=?");
    query.bindValue(0, samplename);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        if(query.next())
        {           
            sampleTreeInfo.analysisType = query.value(0).toInt();
            sampleTreeInfo.markType = query.value(1).toInt();
            getFileTreeInfosFromRealTimeDatabase(samplename, sampleTreeInfo.treeinfo);
            getGsspFileTreeInfosFromRealTimeDatabase(samplename, sampleTreeInfo.treeinfo);
            return true;
        }
    }
    return false;
}

void SoapTypingDB::getFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo_t> &fileTreeInfos)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT fileName,exonIndex,rOrF,isGood,alignResult FROM fileTable WHERE sampleName=? "
                  "ORDER BY fileName,exonIndex,rOrF");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            FileTreeInfo_t fileTreeInfo;
            fileTreeInfo.isGssp = false;
            fileTreeInfo.fileName = query.value(0).toString();
            fileTreeInfo.exonIndex= query.value(1).toInt();
            fileTreeInfo.rOrF = query.value(2).toString();
            fileTreeInfo.isGood = query.value(3).toInt();
            fileTreeInfo.analysisType = query.value(4).toInt();
            fileTreeInfos.push_back(fileTreeInfo);
        }
    }
}

void SoapTypingDB::getGsspFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo_t> &gsspTreeInfos)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT fileName,gsspName,exonIndex,rOrF,isGood,alignResult FROM gsspFileTable WHERE sampleName=? ORDER BY fileName");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            FileTreeInfo_t gsspTreeInfo;
            gsspTreeInfo.isGssp = true;
            gsspTreeInfo.fileName = query.value(0).toString();
            gsspTreeInfo.gsspName = query.value(1).toString();
            gsspTreeInfo.exonIndex= query.value(2).toInt();
            gsspTreeInfo.rOrF = query.value(3).toString();
            gsspTreeInfo.isGood = query.value(4).toInt();
            gsspTreeInfo.analysisType = query.value(5).toInt();
            gsspTreeInfos.push_back(gsspTreeInfo);
        }
    }
}

void SoapTypingDB::getResultDataFromsampleTable(const QString &sampleName, bool isCombined, QStringList &typeResult)
{
    QSqlQuery query(m_SqlDB);
    if(isCombined)
    {
        query.prepare("SELECT combinedResult FROM sampleTable WHERE sampleName=?");
    }
    else
    {
        query.prepare("SELECT typeResult FROM sampleTable WHERE sampleName=?");
    }

    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
            break;
        }
    }
}

void SoapTypingDB::getResultDataFromGsspTable(const QString &fileName, bool isGsspFilter,
                                              bool isGssp, QStringList &typeResult)
{
    if(isGsspFilter || isGssp)
    {
        QSqlQuery query(m_SqlDB);
        if(isGsspFilter)
        {
            query.prepare("SELECT filterResult FROM gsspFileTable WHERE fileName=?");
        }
        else if (isGssp)
        {
            query.prepare("SELECT typeResult FROM gsspFileTable WHERE fileName=?");
        }

        query.bindValue(0, fileName);
        bool isSuccess = query.exec();
        if(isSuccess)
        {
            while(query.next())
            {
                typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
                break;
            }
        }
    }
}


void SoapTypingDB::getAlldataFormRealTime(const QString &sampleName, int exonIndex,
                                           QVector<Ab1FileTableBase> &vec_filetable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT * FROM fileTable WHERE sampleName=? and exonIndex=?  order by fileName");
    query.bindValue(0, sampleName);
    query.bindValue(1, exonIndex);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            Ab1FileTableBase table;
            table.setIsGssp(false);
            table.setFileName(query.value(0).toString());
            table.setExonStartPos(query.value(7).toInt());
            table.setExonEndPos(query.value(8).toInt());
            table.setUsefulSequence(query.value(9).toByteArray());
            table.setBaseSequence(query.value(10).toByteArray());
            table.setBasePostion(query.value(11).toString());
            table.setBaseQuality(query.value(12).toString());
            table.setBaseNumber(query.value(13).toInt());
            table.setBaseASignal(query.value(14).toString());
            table.setBaseTSignal(query.value(15).toString());
            table.setBaseGSignal(query.value(16).toString());
            table.setBaseCSignal(query.value(17).toString());
            table.setSignalNumber(query.value(18).toInt());
            table.setMaxSignal(query.value(19).toInt());
            table.setAverageBaseWidth(query.value(21).toFloat());
            table.setAlignStartPos(query.value(24).toInt());
            table.setAlignEndPos(query.value(25).toInt());
            table.setAlignInfo(query.value(26).toString());
            table.setExcludeLeft(query.value(27).toInt());
            table.setExcludeRight(query.value(28).toInt());
            table.setEditInfo(query.value(29).toString());
            table.setAvgsignal(query.value(30).toInt());

            vec_filetable.push_back(table);
        }
    }

    QSqlQuery query_gssp(m_SqlDB);
    query_gssp.prepare("SELECT * from gsspFileTable WHERE sampleName=? and exonIndex=? order by fileName");
    query_gssp.bindValue(0, sampleName);
    query_gssp.bindValue(1, exonIndex);
    isSuccess = query_gssp.exec();
    if(isSuccess)
    {
        while(query_gssp.next())
        {
            Ab1FileTableBase table;
            table.setIsGssp(true);
            table.setFileName(query_gssp.value(0).toString());
            table.setExonStartPos(query_gssp.value(7).toInt());
            table.setExonEndPos(query_gssp.value(8).toInt());
            table.setUsefulSequence(query_gssp.value(9).toByteArray());
            table.setBaseSequence(query_gssp.value(10).toByteArray());
            table.setBasePostion(query_gssp.value(11).toString());
            table.setBaseQuality(query_gssp.value(12).toString());
            table.setBaseNumber(query_gssp.value(13).toInt());
            table.setBaseASignal(query_gssp.value(14).toString());
            table.setBaseTSignal(query_gssp.value(15).toString());
            table.setBaseGSignal(query_gssp.value(16).toString());
            table.setBaseCSignal(query_gssp.value(17).toString());
            table.setSignalNumber(query_gssp.value(18).toInt());
            table.setMaxSignal(query_gssp.value(19).toInt());
            table.setAverageBaseWidth(query_gssp.value(21).toFloat());
            table.setAlignStartPos(query_gssp.value(24).toInt());
            table.setAlignEndPos(query_gssp.value(25).toInt());
            table.setAlignInfo(query_gssp.value(26).toString());
            table.setExcludeLeft(query_gssp.value(27).toInt());
            table.setExcludeRight(query_gssp.value(28).toInt());
            table.setEditInfo(query_gssp.value(29).toString());

            vec_filetable.push_back(table);
        }
    }

}

void SoapTypingDB::getExonPositionIndex(const QString &geneName, QVector<int> &vec_index)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT exonPositionIndex FROM geneTable WHERE geneName = ?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            QStringList str_list = query.value(0).toString().split(":", QString::SkipEmptyParts);
            foreach(const QString &str, str_list)
            {
                vec_index.push_back(str.toInt());
            }
        }
    }
}

void SoapTypingDB::getExonNavigatorInfo(const QString &sampleName, ExonNavigatorInfo &exonNavigatorInfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT minExonIndex,maxExonIndex,mismatchBetweenPC,mismatchBetweenFR,mmismatchBetweenFR,editPostion"
                  " FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            exonNavigatorInfo.minExonIndex = query.value(0).toInt();
            exonNavigatorInfo.maxExonIndex = query.value(1).toInt();
            QStringList sl_pcMis = query.value(2).toString().split(":", QString::SkipEmptyParts);
            foreach(const QString &str, sl_pcMis)
            {
                exonNavigatorInfo.vec_pcMis.push_back(str.toInt());
            }
            QStringList sl_frMis = query.value(3).toString().split(":", QString::SkipEmptyParts);
            foreach(const QString &str, sl_frMis)
            {
                exonNavigatorInfo.vec_frMis.push_back(str.toInt());
            }
            QStringList sl_frUnEqual = query.value(4).toString().split(":",QString::SkipEmptyParts);
            foreach(const QString &str, sl_frUnEqual)
            {
                exonNavigatorInfo.vec_frUnEqual.push_back(str.toInt());
            }
            QStringList sl_editPos = query.value(5).toString().split(":", QString::SkipEmptyParts);
            foreach(const QString &str, sl_editPos)
            {
                exonNavigatorInfo.vec_editPos.push_back(str.toInt());
            }
        }
    }
}


void SoapTypingDB::getBaseAlignSampleInfo(const QString &sampleName, BaseAlignSampleInfo &baseAlignSampleInfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT exonStartPos,exonEndPos,consensusSequence,forwardSequence,reverseSequence,"
                  "patternSequence,mismatchBetweenPC,mismatchBetweenFR,editPostion "
                  "FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);

    bool isSuccess = query.exec();

    if(isSuccess)
    {
        while(query.next())
        {
            baseAlignSampleInfo.alignStartPos = query.value(0).toInt();
            baseAlignSampleInfo.alignEndPos = query.value(1).toInt();
            baseAlignSampleInfo.consensusSeq = query.value(2).toString();
            baseAlignSampleInfo.forwardSeq = query.value(3).toString();
           // qDebug()<<baseAlignSampleInfo.forwardSeq;
            baseAlignSampleInfo.reverseSeq = query.value(4).toString();
           // qDebug()<<baseAlignSampleInfo.reverseSeq;
           // qDebug()<<"====";
            baseAlignSampleInfo.patternSeq = query.value(5).toString();

            QStringList pcMis = query.value(6).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<pcMis.size(); i++)
            {
                baseAlignSampleInfo.pcMisMatchPostion.insert(pcMis.at(i).toInt());
            }
            QStringList frMis = query.value(7).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<frMis.size(); i++)
            {
                baseAlignSampleInfo.frMisMatchPostion.insert(frMis.at(i).toInt());
            }
            QStringList edit = query.value(8).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<edit.size(); i++)
            {
                baseAlignSampleInfo.editPostion.insert(edit.at(i).toInt());
            }
        }
    }

    query.prepare("SELECT fileName,gsspName,exonStartPos,usefulSequence, excludeLeft, excludeRight, editInfo "
                  "FROM gsspFileTable WHERE sampleName=? AND alignResult != 4");
    query.bindValue(0, sampleName);
    isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            QSet<int> pos;
            BaseAlignGsspInfo gsspInfo;
            gsspInfo.gsspName = query.value(1).toString();
            gsspInfo.gsspFileAlignStartPos = query.value(2).toInt();
            QByteArray seq = query.value(3).toByteArray();
            modifySequence(seq, pos, gsspInfo.gsspFileAlignStartPos, query.value(4).toInt(), query.value(5).toInt(), query.value(6).toString());
            gsspInfo.gsspFileSeq = seq;
            getGsspPosAndSeqFromGsspDatabase(gsspInfo.gsspName, gsspInfo.gsspPostion, gsspInfo.gsspSeq);
            baseAlignSampleInfo.gsspInfoMap.insert(query.value(0).toString(), gsspInfo);
        }
    }
}


void SoapTypingDB::getMarkTypeAndAnalysisFromSampleTable(const QString &sampleName, int &marktype, int &analysis)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT markType, analysisType FROM sampleTable WHERE sampleName =?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if (isSuccess)
    {
        while(query.next())
        {
            marktype = query.value(0).toInt();
            analysis = query.value(1).toInt();
            break;
        }
    }
}

void SoapTypingDB::setMarkTypeBySampleName(const QString &sampleName, int type)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET markType=? WHERE sampleName=?");
    query.bindValue(0, type);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"setMarkTypeBySampleName error"<<sampleName;
    }
}

void SoapTypingDB::getSetNoteFromSampleTable(const QString &sampleName, QString &noteinfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT setNote FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            noteinfo =  query.value(0).toString();
        }
    }
}

void SoapTypingDB::updateSetNoteBySampleName(const QString &sampleName, const QString &info)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET setNote=? WHERE sampleName=?");
    query.bindValue(0, info);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"updateSetNoteBySampleName error"<<sampleName;
    }
}

void SoapTypingDB::deleteSample(const QString &sampleName)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("DELETE FROM sampleTable where sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
         qDebug()<<"delete sample from sampleTable error:"<<sampleName;

    query.prepare("delete from fileTable where sampleName=?");
    query.bindValue(0, sampleName);
    isSuccess = query.exec();
    if(!isSuccess)
         qDebug()<<"delete sample from fileTable error:"<<sampleName;

    query.prepare("delete from gsspFileTable where sampleName=?");
    query.bindValue(0, sampleName);
    isSuccess = query.exec();
    if(!isSuccess)
         qDebug()<<"delete from gsspFileTable error:"<<sampleName;

    return;
}

void SoapTypingDB::deleteFile(bool isgssp, const QString &str_filename)
{
    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        query.prepare("delete from gsspFileTable where fileName=?");
    }
    else
    {
        query.prepare("delete from fileTable where fileName=?");
    }

    query.bindValue(0, str_filename);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        QSqlError sql_err  = m_SqlDB.lastError();
        QString str = QString("delete file %1 error:%2").arg(str_filename).arg(sql_err.text());
        LOG_DEBUG("%s",str.toStdString().c_str());
    }
    else
    {
        QString str = QString("delete file %1 success").arg(str_filename);
        LOG_DEBUG("%s",str.toStdString().c_str());
    }
}


void SoapTypingDB::saveSample(const QString &sampleName,const QString &samplePath,const QString &date)
{
    QFile file(samplePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
         qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream stream(&file);

    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT * FROM sampleTable where sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();

    if(isSuccess)
    {
        while(query.next())
        {
            if(!date.isEmpty())
            {
                if(!sampleName.contains("_"))
                {
                    stream << query.value(0).toString().append(QString("_%1").arg(date))<<"\n";
                }
                else
                {
                    stream << query.value(0).toString().split("_")[0].append(QString("_%1").arg(date))<<"\n";
                }
            }
            else
            {
                stream << query.value(0).toString()<<"\n";
            }

            for(int i = 1; i < SAMPLE_TABLE_FIELD; i++)
            {
                stream << query.value(i).toString()<<"\n";
            }
        }
    }

    file.close();
}


void SoapTypingDB::saveFile(bool isGssp, const QString &fileName, const QString &filePath,
                            const QString &dir,const QString &date)
{
    QString orignalFilePath;
    QStringList newFileName = QString(fileName).split("_");
    QString desPath;
    if(newFileName.size()==4)
    {
        desPath = QString("%1%2%3").arg(dir).arg(QDir::separator()).arg(QString(fileName));
    }
    else
    {
        newFileName.removeAt(1);
        desPath = QString("%1%2%3").arg(dir).arg(QDir::separator()).arg(newFileName.join("_"));
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
         qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream stream(&file);

    int i_count = 0;
    QSqlQuery query(m_SqlDB);
    if (isGssp)
    {
        query.prepare("SELECT * FROM gsspFileTable where fileName=?");
        i_count = GSSP_FILE_TABLE_FIELD;
    }
    else
    {
        query.prepare("SELECT * FROM fileTable where fileName=?");
        i_count = FILE_TABLE_FIELD;
    }

    query.bindValue(0, fileName);
    bool isSuccess = query.exec();

    if(isSuccess)
    {
        while(query.next())
        {
            if(!date.isEmpty())
            {
                newFileName[0].append(QString("_%1").arg(date));
                stream<<newFileName.join("_")<<"\n";
                stream<<query.value(1).toString().split("_")[0].append(QString("_%1").arg(date))<<"\n";
            }
            else
            {
                stream << query.value(0).toString()<<"\n";
                stream << query.value(1).toString()<<"\n";
            }

            stream << desPath<<"\n";

            for(int i = 3; i < i_count; i++)
            {
                stream << query.value(i).toString()<<"\n";
            }

            orignalFilePath = query.value(2).toString();
        }
    }

    QFileInfo ab1(desPath);
    if(!ab1.exists())
    {
        QFile::copy(orignalFilePath,desPath);
    }

    file.close();
}


void SoapTypingDB::getIndelInfoFromalleleTable(const QString &alleleName, IndelInfo &indelInfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT isIndel,indelPostion,indelInfo FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            indelInfo.isIndel = query.value(0).toInt();
            indelInfo.indelPostion = query.value(1).toInt();
            indelInfo.indelInfo = query.value(2).toString();
            break;
        }
    }
}

void SoapTypingDB::updateShieldAllelesToSampleTable(const QString &sampleName, const QString &alleles)
{
    QSqlQuery query(m_SqlDB);
    QString  line;
    query.prepare("SELECT shieldAllele FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"insertShieldAllelesToSampleTable select error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            line = query.value(0).toString();
            break;
        }
    }

    if(line.size()>0)
        line.append(";");
    line.append(alleles);
    query.prepare("UPDATE sampleTable SET shieldAllele=? WHERE sampleName=?");
    query.bindValue(0, line);
    query.bindValue(1, sampleName);
    isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"insertShieldAllelesToSampleTable update error"<<sampleName;
    }
}

void SoapTypingDB::updateSetGsspBySampleName(const QString& sampleName, const QString &gsspInfo)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET setGSSP=? WHERE sampleName=?");
    query.bindValue(0, gsspInfo);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"updateSetGsspBySampleName error!"<<sampleName;
    }
}

void SoapTypingDB::updateSetResultBySampleName(const QString &sampleName, const QString &result)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET setResult=? WHERE sampleName=?");
    query.bindValue(0, result);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"updateSetResultBySampleName error";
    }
}

void SoapTypingDB::getSetResultBySampleName(const QString &sampleName, QString & result)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT setResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            result =  query.value(0).toString();
        }
    }
}

void SoapTypingDB::getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq, int alignStartPos, int alignLength)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleSequence FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            if(alignStartPos != 0 && alignLength != 0)
            {
                alleleSeq = query.value(0).toByteArray().mid(alignStartPos, alignLength);
            }
            else
            {
                alleleSeq = query.value(0).toByteArray();
            }
        }
    }
}

void SoapTypingDB::getExonIndexAndGeneBySampleName(const QString &sampleName, int &exonStart,
                                                   int &exonEnd, QByteArray &geneName)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT minExonIndex,maxExonIndex,geneName FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            exonStart = query.value(0).toInt();
            exonEnd = query.value(1).toInt();
            geneName = query.value(2).toByteArray();
            return;
        }
    }
}

void SoapTypingDB::getGsspTablesFromGsspDatabase(const QString &geneName, int exon, QVector<GsspTable> &gsspTables)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT gsspName,rOrF,position,base FROM gsspTable WHERE geneName =?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            GsspTable t;
            t.geneName = geneName;
            t.exonIndex = exon;
            t.gsspName = query.value(0).toByteArray();
            t.rOrF = query.value(1).toByteArray();
            t.position = query.value(2).toInt();
            t.base = query.value(3).toByteArray();
            gsspTables.push_back(t);
        }
    }
}

void SoapTypingDB::getExonPositionIndexFromStaticDatabase(const QString &geneName, QVector<int> &position)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT exonPositionIndex FROM geneTable WHERE geneName = ?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            QStringList line = query.value(0).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<line.size();i++)
            {
                position.push_back(line.at(i).toInt());
            }
            return;
        }
    }
}


void SoapTypingDB::insertOneSampleTable(SampleTable &sampleTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT or replace INTO sampleTable (sampleName,"
                  "geneName,"
                  "fileType,"
                  "markType,"
                  "analysisType,"
                  "minExonIndex,"
                  "maxExonIndex,"
                  "exonStartPos,"
                  "exonEndPos,"
                  "consensusSequence,"
                  "forwardSequence,"
                  "reverseSequence,"
                  "patternSequence,"
                  "mismatchBetweenPC,"
                  "mismatchBetweenFR,"
                  "mmismatchBetweenFR,"
                  "editPostion,"
                  "typeResult,"
                  "gsspInfo,"
                  "shieldAllele,"
                  "setResult,"
                  "setNote,"
                  "setGSSP,"
                  "combinedResult)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, sampleTable.getSampleName());
    query.bindValue(1, sampleTable.getGeneName());
    query.bindValue(2, sampleTable.getFileType());
    query.bindValue(3, sampleTable.getMarkType());
    query.bindValue(4, sampleTable.getAnalysisType());
    query.bindValue(5, sampleTable.getMinExonIndex());
    query.bindValue(6, sampleTable.getMaxExonIndex());
    query.bindValue(7, sampleTable.getExonStartPos());
    query.bindValue(8, sampleTable.getExonEndPos());
    query.bindValue(9, sampleTable.getConsensusSequence());
    query.bindValue(10, sampleTable.getForwardSequence());
    query.bindValue(11, sampleTable.getReverseSequence());
    query.bindValue(12, sampleTable.getPatternSequence());
    query.bindValue(13, sampleTable.getMismatchBetweenPC());
    query.bindValue(14, sampleTable.getMismatchBetweenFR());
    query.bindValue(15, sampleTable.getMmismatchBetweenFR());
    query.bindValue(16, sampleTable.getEditPostion());
    query.bindValue(17, sampleTable.getTypeResult());
    query.bindValue(18, sampleTable.getGsspInfo());
    query.bindValue(19, sampleTable.getShieldAllele());
    query.bindValue(20, sampleTable.getSetResult());
    query.bindValue(21, sampleTable.getSetNote());
    query.bindValue(22, sampleTable.getSetGSSP());
    query.bindValue(23, sampleTable.getCombinedResult());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"Insert sampleTable error";
    }

}

void SoapTypingDB::getSampleNamesFromRealTimeDatabase(QStringList &sampleNames)
{
    QSqlQuery query(m_SqlDB);
    bool isSuccess = query.exec("SELECT sampleName FROM sampleTable ORDER BY sampleName");
    if(isSuccess)
    {
        while(query.next())
        {
            sampleNames.push_back(query.value(0).toString());
        }
    }
}

void SoapTypingDB::getSampleStartEndBySampleName(const QString &sampleName, int &start, int &end)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT exonStartPos,exonEndPos FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            start = query.value(0).toInt();
            end = query.value(1).toInt();
            return;
        }
    }
}

int SoapTypingDB::getResultFromRealTimeDatabaseBySampleName(const QString &sampleName, QString &result)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT typeResult,setResult,combinedResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            QString type = query.value(0).toString();
            QString set = query.value(1).toString();
            QString combined = query.value(2).toString();

            if(!set.isEmpty())
            {
                result = set;
                return 2;
            }

            if(!combined.isEmpty())
            {
                result = combined;
                return 1;
            }

            if(!type.isEmpty())
            {
                result = type;
                return 0;
            }
            return 3;
        }
    }
    return 3;
}

bool SoapTypingDB::isIndelInRange(const QString &alleleName, int start, int end)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT isIndel,indelPostion FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            if(query.value(0).toInt()> 0 && query.value(1).toInt()>=start && query.value(1).toInt()<end)
                return true;
        }
    }
    return false;
}


int SoapTypingDB::getMarkTypeBySampleName(const QString &sampleName)
{
    int is = -1;
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT markType FROM sampleTable WHERE sampleName =?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            is = query.value(0).toInt();
            break;
        }
    }
    return is;
}

void SoapTypingDB::resetFileByFileName(const QString &fileName, bool isGssp)
{
    QSqlQuery query(m_SqlDB);
    if(isGssp)
    {
        query.prepare("UPDATE gsspFileTable SET excludeLeft=?,excludeRight=?,editInfo=? WHERE fileName=?");
    }
    else
    {
        query.prepare("UPDATE fileTable SET excludeLeft=?,excludeRight=?,editInfo=? WHERE fileName=?");
    }

    query.bindValue(0, -1);
    query.bindValue(1, -1);
    query.bindValue(2, "");
    query.bindValue(3, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"resetGsspFileByFileName error"<<fileName;
    }
}

void SoapTypingDB::markAllSampleApproved()
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET markType =?");
    query.bindValue(0, APPROVED);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"markAllSampleApproved error";
    }
}

int SoapTypingDB::markAllSampleReviewed()
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE sampleTable SET markType =? WHERE (markType <> ?)");
    query.bindValue(0, REVIEWED);
    query.bindValue(1, APPROVED);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
         qDebug()<<"markAllSampleApproved error";
        return -1;
    }

    query.prepare("SELECT sampleName FROM sampleTable WHERE markType=?");
    query.bindValue(0, APPROVED);
    query.exec();
    if(query.next())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void SoapTypingDB::getAlleleNameListFromStaticDabase(const QString &geneName, QStringList &alleleNames)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleName FROM alleleTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            alleleNames.push_back(query.value(0).toString());
        }
    }
}

void SoapTypingDB::getGeneVersion(QString &strver)
{
    QSqlQuery query(m_SqlDB);
    bool isSuccess = query.exec("SELECT version FROM geneTable");
    if(isSuccess)
    {
        while(query.next())
        {
            strver = query.value(0).toString();
            break;
        }
    }
}

void SoapTypingDB::getGeneNames(QStringList &geneNames)
{
    QSqlQuery query(m_SqlDB);
    bool isSuccess = query.exec("SELECT geneName FROM geneTable");
    if(isSuccess)
    {
        while(query.next())
        {
            geneNames.push_back(query.value(0).toString());
        }
    }
}

void SoapTypingDB::getExonTrimListByGeneName(const QString &geneName, QVector<ExonTrimTable> &exonTrimTableList)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT * FROM exonTrimTable WHERE geneName=? ORDER BY etKey DESC");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            ExonTrimTable exonTrimTable;
            exonTrimTable.etKey = query.value(0).toString();
            exonTrimTable.geneName = query.value(1).toString();
            exonTrimTable.exonIndex = query.value(2).toString();
            exonTrimTable.fOrR = query.value(3).toString();
            exonTrimTable.exonStart = query.value(4).toString();
            exonTrimTable.exonEnd = query.value(5).toString();
            exonTrimTable.excludeLeft = query.value(6).toString();
            exonTrimTable.excludeRight = query.value(7).toString();
            exonTrimTableList.push_front(exonTrimTable);
        }
    }
}

void SoapTypingDB::updateExonTrim(const ExonTrimTable &exonTrimTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("UPDATE exonTrimTable set geneName=?,"
                  "exonIndex=?,"
                  "fOrR=?,"
                  "exonStart=?,"
                  "exonEnd=?,"
                  "excludeLeft=?,"
                  "excludeRight=? WHERE etKey=?");
    query.bindValue(7, exonTrimTable.etKey);
    query.bindValue(0, exonTrimTable.geneName);
    query.bindValue(1, exonTrimTable.exonIndex);
    query.bindValue(2, exonTrimTable.fOrR);
    query.bindValue(3, exonTrimTable.exonStart);
    query.bindValue(4, exonTrimTable.exonEnd);
    query.bindValue(5, exonTrimTable.excludeLeft);
    query.bindValue(6, exonTrimTable.excludeRight);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"updateExonTrim Error exonTrimTable";
    }
}

void SoapTypingDB::getAlleleNamesAndSeqsByGeneName(const QString &geneName, QStringList &alleleNames,
                                     QStringList &alleleSeqs, QVector< QVector<int> > &misPositions)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleName,alleleSequence,misPosition FROM labAlignTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            alleleNames.push_back(query.value(0).toString());
            alleleSeqs.push_back(query.value(1).toString());
            QStringList line = query.value(2).toString().split(":",QString::SkipEmptyParts);
            QVector<int> mis;
            for (int i=0; i<line.size(); i++)
            {
                mis.push_back(line.at(i).toInt());
            }
            misPositions.push_back(mis);
        }
    }
}

void SoapTypingDB::getAlleleSequenceByAlleleName(const QString &alleleName, QString &alleleSeq)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT alleleSequence FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            alleleSeq = query.value(0).toString();
        }
    }

}


bool SoapTypingDB::upDateAlignInfo(const QString &filename, const QString &straligninfo,bool isgssp)
{
   // table.setAlignInfo(query_gssp.value(26).toString());

    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        query.prepare("UPDATE gsspFileTable SET alignInfo=? WHERE fileName=?");
    }
    else
    {
        query.prepare("UPDATE fileTable SET alignInfo=? WHERE fileName=?");
    }
    query.bindValue(0, straligninfo);
    query.bindValue(1, filename);
    if(!query.exec())
    {
        return false;
    }
    return true;
}

bool SoapTypingDB::upDateUsefulSeq(const QString &filename, const QString &usefulseq,bool isgssp)
{
   // table.setAlignInfo(query_gssp.value(26).toString());

    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        query.prepare("UPDATE gsspFileTable SET usefulSequence=? WHERE fileName=?");
    }
    else
    {
        query.prepare("UPDATE fileTable SET usefulSequence=? WHERE fileName=?");
    }
    query.bindValue(0, usefulseq);
    query.bindValue(1, filename);
    if(!query.exec())
    {
        return false;
    }
    return true;
}



bool SoapTypingDB::upDatabyChangebp(const QString &filename, const QString &streditinfo,bool isgssp)
{
    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        query.prepare("UPDATE gsspFileTable SET editInfo=? WHERE fileName=?");
    }
    else
    {
        query.prepare("UPDATE fileTable SET editInfo=? WHERE fileName=?");
    }
    query.bindValue(0, streditinfo);
    query.bindValue(1, filename);
    if(!query.exec())
    {
        return false;
    }
    return true;
}

bool SoapTypingDB::upDataExclude(bool isgssp, const QString &filename, int exclude_left, int exclude_right)
{
    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        if(exclude_left == -1)
        {
            query.prepare("UPDATE gsspFileTable SET excludeRight=?  WHERE fileName=?");
            query.bindValue(0, exclude_right);
            query.bindValue(1, filename);
        }
        else if(exclude_right == -1)
        {
            query.prepare("UPDATE gsspFileTable SET excludeLeft=? WHERE fileName=?");
            query.bindValue(0, exclude_left);
            query.bindValue(1, filename);
        }
        else
        {
            query.prepare("UPDATE gsspFileTable SET excludeLeft=?,excludeRight=?  WHERE fileName=?");
            query.bindValue(0, exclude_left);
            query.bindValue(1, exclude_right);
        }
    }
    else
    {
        if(exclude_left == -1)
        {
            query.prepare("UPDATE fileTable SET excludeRight=?  WHERE fileName=?");
            query.bindValue(0, exclude_right);
            query.bindValue(1, filename);
        }
        else if(exclude_right == -1)
        {
            query.prepare("UPDATE fileTable SET excludeLeft=? WHERE fileName=?");
            query.bindValue(0, exclude_left);
            query.bindValue(1, filename);
        }
        else
        {
            query.prepare("UPDATE fileTable SET excludeLeft=?,excludeRight=?  WHERE fileName=?");
            query.bindValue(0, exclude_left);
            query.bindValue(1, exclude_right);
            query.bindValue(2, filename);
        }
    }

    if(!query.exec())
    {
        return false;
    }
    return true;
}


bool SoapTypingDB::upDataAlignPos(bool isgssp, const QString &filename, int start, int end)
{
    QSqlQuery query(m_SqlDB);
    if(isgssp)
    {
        if(start == -1)
        {
            query.prepare("UPDATE gsspFileTable SET alignEndPos=?  WHERE fileName=?");
            query.bindValue(0, end);
            query.bindValue(1, filename);
        }
        else if(end == -1)
        {
            query.prepare("UPDATE gsspFileTable SET alignStartPos=? WHERE fileName=?");
            query.bindValue(0, start);
            query.bindValue(1, filename);
        }
        else
        {
            query.prepare("UPDATE gsspFileTable SET alignStartPos=?,alignEndPos=?  WHERE fileName=?");
            query.bindValue(0, start);
            query.bindValue(1, end);
        }
    }
    else
    {
        if(start == -1)
        {
            query.prepare("UPDATE fileTable SET alignEndPos=?  WHERE fileName=?");
            query.bindValue(0, start);
            query.bindValue(1, filename);
        }
        else if(end == -1)
        {
            query.prepare("UPDATE fileTable SET alignStartPos=? WHERE fileName=?");
            query.bindValue(0, start);
            query.bindValue(1, filename);
        }
        else
        {
            query.prepare("UPDATE fileTable SET alignStartPos=?,alignEndPos=?  WHERE fileName=?");
            query.bindValue(0, start);
            query.bindValue(1, end);
            query.bindValue(2, filename);
        }
    }

    if(!query.exec())
    {
        return false;
    }
    return true;
}





bool SoapTypingDB::deleteTable(const QString &tableName)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("DELETE FROM "+tableName);
    return query.exec();
}

void SoapTypingDB::insertGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT INTO geneTable (geneName,"
                  "geneSequence,exonCount,exonPositionIndex,"
                  "geneClasses,availableExon,version)"
                  "VALUES (?,?,?,?,?,?,?)");
    query.bindValue(0, geneTable.geneName);
    query.bindValue(1, geneTable.geneSequence);
    query.bindValue(2, geneTable.exonCount);
    query.bindValue(3, geneTable.exonPostionIndex);
    query.bindValue(4, geneTable.geneClasses);
    query.bindValue(5, geneTable.availableExon);
    query.bindValue(6, geneTable.version);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        LOG_DEBUG("%s insert fail",geneTable.geneName.toStdString().c_str());
    }
}

void SoapTypingDB::readGeneTableTxtFile(const QString &geneFile)
{
    QFile file(geneFile);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        deleteTable("geneTable");//更新版本，清除旧版本数据
        m_SqlDB.transaction();
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList part = line.split("\t");
            if(part.size() == 7)
            {
                GeneTable geneTable;
                geneTable.geneName = part.at(0);
                geneTable.geneSequence = part.at(1);
                geneTable.exonCount = part.at(2).toInt();
                geneTable.exonPostionIndex = part.at(3);
                geneTable.geneClasses = part.at(4);
                geneTable.availableExon = part.at(5);
                geneTable.version = part.at(6);
                insertGeneTable(geneTable);
            }

        }
        file.close();
        m_SqlDB.commit();
    }
}

void SoapTypingDB::insertAlleleTable(const AlleleTable &alleleTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT INTO alleleTable (alleleName,"
                  "alleleSequence,geneName,isRare,"
                  "isIndel,classesNumber,indelPostion,"
                  "indelInfo,noStar)"
                  "VALUES (?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, alleleTable.alleleName);
    query.bindValue(1, alleleTable.alleleSequence);
    query.bindValue(2, alleleTable.geneName);
    query.bindValue(3, alleleTable.isRare);
    query.bindValue(4, alleleTable.isIndel);
    query.bindValue(5, alleleTable.classesNumber);
    query.bindValue(6, alleleTable.indelPosition);
    query.bindValue(7, alleleTable.indelInfo);
    query.bindValue(8, alleleTable.noStar);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        LOG_DEBUG("%s insert fail",alleleTable.alleleName.toStdString().c_str());
    }
}

void SoapTypingDB::readAlleleTableTxtFile(const QString &alleleFile)
{
    QFile file(alleleFile);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        deleteTable("alleleTable");
        m_SqlDB.transaction();
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList part = line.split("\t");
            if(part.size() == 9)
            {
                AlleleTable alleleTable;
                alleleTable.alleleName = part.at(0);
                alleleTable.alleleSequence = part.at(1);
                alleleTable.geneName = part.at(2);
                alleleTable.isRare = part.at(3).toInt();
                alleleTable.isIndel = part.at(4).toInt();
                alleleTable.classesNumber = part.at(5).toInt();
                alleleTable.indelPosition = part.at(6).toInt();
                alleleTable.indelInfo = part.at(7);
                alleleTable.noStar = part.at(8);
                insertAlleleTable(alleleTable);
            }
        }
        file.close();
        m_SqlDB.commit();
    }
}

void SoapTypingDB::insertGsspTable(const GsspTable &gsspTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT or REPLACE INTO gsspTable (gsspKey,"
                  "gsspName,geneName,exonIndex,rOrF,"
                  "position,base)"
                  "VALUES (?,?,?,?,?,?,?)");
    query.bindValue(0, gsspTable.gsspKey);
    query.bindValue(1, gsspTable.gsspName);
    query.bindValue(2, gsspTable.geneName);
    query.bindValue(3, gsspTable.exonIndex);
    query.bindValue(4, gsspTable.rOrF);
    query.bindValue(5, gsspTable.position);
    query.bindValue(6, gsspTable.base);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        LOG_DEBUG("%s insert fail",gsspTable.gsspName.toStdString().c_str());
    }
}

void SoapTypingDB::readGsspTableTxtFile(const QString &gsspFile)
{
    QFile file(gsspFile);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        deleteTable("gsspTable");
        m_SqlDB.transaction();
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList part = line.split("\t");
            if(part.size() == 6)
            {
                GsspTable gsspTable;
                gsspTable.gsspName = part.at(0);
                gsspTable.exonIndex = part.at(2).toInt();
                gsspTable.rOrF = part.at(3);
                gsspTable.position = part.at(4).toInt();
                gsspTable.base = part.at(5).toLatin1();


                QString str_tmp = part.at(1);
                if(str_tmp.contains(':'))
                {
                    qDebug()<<str_tmp;
                    QStringList str_gene_list = str_tmp.split(':');
                    foreach(const QString &name, str_gene_list)
                    {
                        gsspTable.geneName = name;
                        gsspTable.gsspKey = gsspTable.gsspName+"_"+gsspTable.geneName;
                        qDebug()<<name;
                        insertGsspTable(gsspTable);
                    }
                }
                else
                {
                    gsspTable.geneName = str_tmp;
                    gsspTable.gsspKey = gsspTable.gsspName+"_"+gsspTable.geneName;
                    insertGsspTable(gsspTable);
                }
            }
        }
        file.close();
        m_SqlDB.commit();
    }
}

void SoapTypingDB::insertLabAlignTable(const LabAlignTable &labAlignTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT INTO labAlignTable (alleleName,"
                  "alleleSequence,geneName,misPosition)"
                  "VALUES (?,?,?,?)");
    query.bindValue(0, labAlignTable.alleleName);
    query.bindValue(1, labAlignTable.alleleSequence);
    query.bindValue(2, labAlignTable.geneName);
    query.bindValue(3, labAlignTable.misPosition);

    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        LOG_DEBUG("%s insert fail",labAlignTable.alleleName.toStdString().c_str());
    }
}

void SoapTypingDB::readLabAlignTableTxtFile(const QString &labAlignFile)
{
    QFile file(labAlignFile);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        deleteTable("labAlignTable");
        m_SqlDB.transaction();
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList part = line.split("\t");
            if(part.size() == 4)
            {
                LabAlignTable labAlignTable;
                labAlignTable.alleleName = part.at(0);
                labAlignTable.alleleSequence = part.at(1);
                labAlignTable.geneName = part.at(2);
                labAlignTable.misPosition = part.at(3);
                insertLabAlignTable(labAlignTable);
            }

        }
        file.close();
        m_SqlDB.commit();
    }
}

void SoapTypingDB::insertCommonGsspTable(const CommonGsspTable &commonGsspTable)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("INSERT or REPLACE INTO commonGsspTable (gsspName, geneName, exonIndex, fOrR)"
                  "VALUES (?,?,?,?)");
    query.bindValue(0, commonGsspTable.gsspName);
    query.bindValue(1, commonGsspTable.geneName);
    query.bindValue(2, commonGsspTable.exonIndex);
    query.bindValue(3, commonGsspTable.fOrR);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        LOG_DEBUG("%s insert fail",commonGsspTable.gsspName.toStdString().c_str());
    }
}

void SoapTypingDB::readCommonGsspTableTxt(const QString &txtFile)
{
    QFile file(txtFile);
    if(file.open(QFile::ReadOnly))
    {
        deleteTable("commonGsspTable");
        QTextStream stream(&file);
        m_SqlDB.transaction();
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList part = line.split("\t");
            if(part.size() == 4)
            {
                CommonGsspTable commonGsspTable;
                commonGsspTable.gsspName = part.at(0);
                commonGsspTable.exonIndex = part.at(2);
                commonGsspTable.fOrR = part.at(3);
                QString str_tmp = part.at(1);
                if(str_tmp.contains(':'))
                {
                    QStringList str_gene_list = str_tmp.split(':');
                    foreach(const QString &name, str_gene_list)
                    {
                        commonGsspTable.geneName = name;
                        insertCommonGsspTable(commonGsspTable);
                    }
                }
                else
                {
                    commonGsspTable.geneName = str_tmp;
                    insertCommonGsspTable(commonGsspTable);
                }
            }
        }
        file.close();
        m_SqlDB.commit();
    }
}


void SoapTypingDB::StartTransaction()
{
    m_SqlDB.transaction();
}

void SoapTypingDB::EndTransaction()
{
    m_SqlDB.commit();
}

void SoapTypingDB::getTypeResultFromSampleTable(const QString &sampleName, QMap<int, QString> &zeroResult)
{
    QSqlQuery query(m_SqlDB);
    query.prepare("SELECT typeResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(isSuccess)
    {
        while(query.next())
        {
            QVector<QStringRef> vec_res = query.value(0).toString().splitRef(";", QString::SkipEmptyParts);
            foreach (QStringRef str_ref, vec_res) {
                QVector<QStringRef> vec_item = str_ref.split(',');
                if(vec_item.at(0) == '0')
                {
                    zeroResult.insertMulti(vec_item.at(0).toInt(), vec_item.at(1).toString());
                }
            }
            break;
        }
    }
}
