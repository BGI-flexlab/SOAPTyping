#include "realtimedatabase.h"
#include "staticdatabase.h"
//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qsqlite)
#include <QtSql>
#include <QtGui>
const QString realTimeDatabaseFilePath = QString("Database%1soapTypingRealTimeDatabase").arg(QDir::separator());
QSqlDatabase realTimeDatabase;

void createRealTimeDatabase()
{
    QDir dir;
    dir.mkpath("Database");
    realTimeDatabase = QSqlDatabase::addDatabase("QSQLITE","third");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    realTimeDatabase.setDatabaseName(realTimeDatabaseFilePath);
    realTimeDatabase.open();
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
        qDebug()<<"create realTimeDatabase error!";
    }
}

void createSampleTable()
{
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("CREATE TABLE sampleTable (sampleName VARCHAR PRIMARY KEY,"
                                "geneName VARCHAR,"
                                "fileType INT,"
                                "markType INT,"
                                "analysisType INT,"
                                "minExonIndex INT,"
                                "maxExonIndex INT,"
                                "exonStartPos INT,"
                                "exonEndPos INT,"
                                "consensusSequence VARCHAR,"
                                "forwardSequence VARCHAR,"
                                "reverseSequence VARCHAR,"
                                "patternSequence VARCHAR,"
                                "mismatchBetweenPC VARCHAR,"
                                "mismatchBetweenFR VARCHAR,"
                                "mmismatchBetweenFR VARCHAR,"
                                "editPostion VARCHAR,"
                                "typeResult VARCHAR,"
                                "gsspInfo VARCHAR,"
                                "shieldAllele VARCHAR,"
                                "setResult VARCHAR,"
                                "setNote VARCHAR,"
                                "setGSSP VARCHAR,"
                                "combinedResult VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create sampleTable error";
    }
}

void createFileTable()
{
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("CREATE TABLE fileTable (fileName VARCHAR PRIMARY KEY,"
                                "sampleName VARCHAR,"
                                "filePath VARCHAR,"
                                "isExtraFile INT,"
                                "geneName VARCHAR,"
                                "exonIndex INT,"
                                "rOrF VARCHAR,"
                                "exonStartPos INT,"
                                "exonEndPos INT,"
                                "usefulSequence VARCHAR,"
                                "baseSequence VARCHAR,"
                                "basePostion VARCHAR,"
                                "baseQuality VARCHAR,"
                                "baseNumber INT,"
                                "baseASignal VARCHAR,"
                                "baseTSignal VARCHAR,"
                                "baseGSignal VARCHAR,"
                                "baseCSignal VARCHAR,"
                                "signalNumber INT,"
                                "maxSignal INT,"
                                "maxQuality INT,"
                                "averageBaseWidth VARCHAR,"
                                "isGood INT,"
                                "alignResult INT,"
                                "alignStartPos INT,"
                                "alignEndPos INT,"
                                "alignInfo VARCHAR,"
                                "excludeLeft INT,"
                                "excludeRight INT,"
                                "editInfo VARCHAR,"
                                "avgSignal VARCHAR)");
//                                "editInfo VARCHAR,"
//                                "FOREIGN KEY (sampleName) REFERENCES sampleTable)");
    if(!isSuccess)
    {
        qDebug()<<"create fileTable error";
    }
}

void createGsspFileTable()
{
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("CREATE TABLE gsspFileTable (fileName VARCHAR PRIMARY KEY,"
                                "sampleName VARCHAR,"
                                "filePath VARCHAR,"
                                "gsspName VARCHAR,"
                                "geneName VARCHAR,"
                                "exonIndex INT,"
                                "rOrF VARCHAR,"
                                "exonStartPos INT,"
                                "exonEndPos INT,"
                                "usefulSequence VARCHAR,"
                                "baseSequence VARCHAR,"
                                "basePostion VARCHAR,"
                                "baseQuality VARCHAR,"
                                "baseNumber INT,"
                                "baseASignal VARCHAR,"
                                "baseTSignal VARCHAR,"
                                "baseGSignal VARCHAR,"
                                "baseCSignal VARCHAR,"
                                "signalNumber INT,"
                                "maxSignal INT,"
                                "maxQuality INT,"
                                "averageBaseWidth VARCHAR,"
                                "isGood INT,"
                                "alignResult INT,"
                                "alignStartPos INT,"
                                "alignEndPos INT,"
                                "alignInfo VARCHAR,"
                                "excludeLeft INT,"
                                "excludeRight INT,"
                                "editInfo VARCHAR,"
                                "typeResult VARCHAR,"
                                "filterResult VARCHAR)");
//                                "filterResult VARCHAR,"
//                                "FOREIGN KEY (sampleName) REFERENCES sampleTable)");
    if(!isSuccess)
    {
        qDebug()<<"create gssPFileTable error";
    }
}

void loadRealTimeDatabase()
{
    realTimeDatabase = QSqlDatabase::addDatabase("QSQLITE","third");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    realTimeDatabase.setDatabaseName(realTimeDatabaseFilePath);
    realTimeDatabase.open();
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
        qDebug()<<"load realTimeDatabase error!";
    }
}

void removeRealTiemDatabase()
{
    QFile::remove(realTimeDatabaseFilePath);
}

void newStartRealTimeDatabase()
{
    removeRealTiemDatabase();
    createRealTimeDatabase();
    createSampleTable();
    createFileTable();
    createGsspFileTable();
    createGeneTable();
}

void showSampleTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM sampleTable",realTimeDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("sampleTable");
    view->show();
}

void showFileTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM fileTable",realTimeDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("fileTable");
    view->show();
}

void showGsspFileTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM gsspFileTable",realTimeDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("gsspFileTable");
    view->show();
}

void insertSampleToRealTimeDatabase(const SampleTable &sampleTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT sampleName FROM sampleTable WHERE sampleName = ?");
    query.bindValue(0, sampleTable.sampleName);
    query.exec();
    if(query.next())
    {
        updateOneSampleTable(sampleTable);
    }
    else
    {
        insertOneSampleTable(sampleTable);
    }
}

void insertFileToRealTimeDatabase(const FileTable &fileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT fileName FROM fileTable WHERE fileName = ?");
    query.bindValue(0, fileTable.fileName);
    query.exec();
    if(query.next())
    {
        updateOneFileTable(fileTable);
    }
    else
    {
        insertOneFileTable(fileTable);
    }
}

void insertGsspFileToRealTimeDatabase(const GsspFileTable &gsspFileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT fileName FROM gsspFileTable WHERE fileName = ?");
    query.bindValue(0, gsspFileTable.fileName);
    query.exec();
    if(query.next())
    {
        updateOneGsspFileTable(gsspFileTable);
    }
    else
    {
        insertOneGsspFileTable(gsspFileTable);
    }
}

void insertOneSampleTable(const SampleTable &sampleTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("INSERT INTO sampleTable (sampleName,"
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
    query.bindValue(0, sampleTable.sampleName);
    query.bindValue(1, sampleTable.geneName);
    query.bindValue(2, sampleTable.fileType);
    query.bindValue(3, sampleTable.markType);
    query.bindValue(4, sampleTable.analysisType);
    query.bindValue(5, sampleTable.minExonIndex);
    query.bindValue(6, sampleTable.maxExonIndex);
    query.bindValue(7, sampleTable.exonStartPos);
    query.bindValue(8, sampleTable.exonEndPos);
    query.bindValue(9, sampleTable.consensusSequence);
    query.bindValue(10, sampleTable.forwardSequence);
    query.bindValue(11, sampleTable.reverseSequence);
    query.bindValue(12, sampleTable.patternSequence);
    query.bindValue(13, sampleTable.mismatchBetweenPC);
    query.bindValue(14, sampleTable.mismatchBetweenFR);
    query.bindValue(15, sampleTable.mmismatchBetweenFR);
    query.bindValue(16, sampleTable.editPostion);
    query.bindValue(17, sampleTable.typeResult);
    query.bindValue(18, sampleTable.gsspInfo);
    query.bindValue(19, sampleTable.shieldAllele);
    query.bindValue(20, sampleTable.setResult);
    query.bindValue(21, sampleTable.setNote);
    query.bindValue(22, sampleTable.setGSSP);
    query.bindValue(23, sampleTable.combinedResult);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Insert sampleTable error";
    }

}

void insertOneFileTable(const FileTable &fileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("INSERT INTO fileTable (fileName,"
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
    query.bindValue(0, fileTable.fileName);
    query.bindValue(1, fileTable.sampleName);
    query.bindValue(2, fileTable.filePath);
    query.bindValue(3, fileTable.isExtraFile);
    query.bindValue(4, fileTable.geneName);
    query.bindValue(5, fileTable.exonIndex);
    query.bindValue(6, fileTable.rOrF);
    query.bindValue(7, fileTable.exonStartPos);
    query.bindValue(8, fileTable.exonEndPos);
    query.bindValue(9, fileTable.usefulSequence);
    query.bindValue(10, fileTable.baseSequence);
    query.bindValue(11, fileTable.basePostion);
    query.bindValue(12, fileTable.baseQuality);
    query.bindValue(13, fileTable.baseNumber);
    query.bindValue(14, fileTable.baseASignal);
    query.bindValue(15, fileTable.baseTSignal);
    query.bindValue(16, fileTable.baseGSignal);
    query.bindValue(17, fileTable.baseCSignal);
    query.bindValue(18, fileTable.signalNumber);
    query.bindValue(19, fileTable.maxSignal);
    query.bindValue(20, fileTable.maxQuality);
    query.bindValue(21, fileTable.averageBaseWidth);
    query.bindValue(22, fileTable.isGood);
    query.bindValue(23, fileTable.alignResult);
    query.bindValue(24, fileTable.alignStartPos);
    query.bindValue(25, fileTable.alignEndPos);
    query.bindValue(26, fileTable.alignInfo);
    query.bindValue(27, fileTable.excludeLeft);
    query.bindValue(28, fileTable.excludeRight);
    query.bindValue(29, fileTable.editInfo);
    query.bindValue(30, fileTable.avgsignal);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"insert fileTable error";
    }
}

void insertOneGsspFileTable(const GsspFileTable &gsspFileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("INSERT INTO gsspFileTable (fileName,"
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
    query.bindValue(0, gsspFileTable.fileName);
    query.bindValue(1, gsspFileTable.sampleName);
    query.bindValue(2, gsspFileTable.filePath);
    query.bindValue(3, gsspFileTable.gsspName);
    query.bindValue(4, gsspFileTable.geneName);
    query.bindValue(5, gsspFileTable.exonIndex);
    query.bindValue(6, gsspFileTable.rOrF);
    query.bindValue(7, gsspFileTable.exonStartPos);
    query.bindValue(8, gsspFileTable.exonEndPos);
    query.bindValue(9, gsspFileTable.usefulSequence);
    query.bindValue(10, gsspFileTable.baseSequence);
    query.bindValue(11, gsspFileTable.basePostion);
    query.bindValue(12, gsspFileTable.baseQuality);
    query.bindValue(13, gsspFileTable.baseNumber);
    query.bindValue(14, gsspFileTable.baseASignal);
    query.bindValue(15, gsspFileTable.baseTSignal);
    query.bindValue(16, gsspFileTable.baseGSignal);
    query.bindValue(17, gsspFileTable.baseCSignal);
    query.bindValue(18, gsspFileTable.signalNumber);
    query.bindValue(19, gsspFileTable.maxSignal);
    query.bindValue(20, gsspFileTable.maxQuality);
    query.bindValue(21, gsspFileTable.averageBaseWidth);
    query.bindValue(22, gsspFileTable.isGood);
    query.bindValue(23, gsspFileTable.alignResult);
    query.bindValue(24, gsspFileTable.alignStartPos);
    query.bindValue(25, gsspFileTable.alignEndPos);
    query.bindValue(26, gsspFileTable.alignInfo);
    query.bindValue(27, gsspFileTable.excludeLeft);
    query.bindValue(28, gsspFileTable.excludeRight);
    query.bindValue(29, gsspFileTable.editInfo);
    query.bindValue(30, gsspFileTable.typeResult);
    query.bindValue(31, gsspFileTable.filterResult);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"insert gsspFileTable error";
    }
}

void updateOneSampleTable(const SampleTable &sampleTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable set geneName=?,"
                  "fileType=?,"
                  "markType=?,"
                  "analysisType=?,"
                  "minExonIndex=?,"
                  "maxExonIndex=?,"
                  "exonStartPos=?,"
                  "exonEndPos=?,"
                  "consensusSequence=?,"
                  "forwardSequence=?,"
                  "reverseSequence=?,"
                  "patternSequence=?,"
                  "mismatchBetweenPC=?,"
                  "mismatchBetweenFR=?,"
                  "mmismatchBetweenFR=?,"
                  "editPostion=?,"
                  "typeResult=?,"
                  "gsspInfo=?,"
                  "shieldAllele=?,"
                  "setResult=?,"
                  "setNote=?,"
                  "setGSSP=?,"
                  "combinedResult=? where sampleName=?");
    query.bindValue(0, sampleTable.geneName);
    query.bindValue(1, sampleTable.fileType);
    query.bindValue(2, sampleTable.markType);
    query.bindValue(3, sampleTable.analysisType);
    query.bindValue(4, sampleTable.minExonIndex);
    query.bindValue(5, sampleTable.maxExonIndex);
    query.bindValue(6, sampleTable.exonStartPos);
    query.bindValue(7, sampleTable.exonEndPos);
    query.bindValue(8, sampleTable.consensusSequence);
    query.bindValue(9, sampleTable.forwardSequence);
    query.bindValue(10, sampleTable.reverseSequence);
    query.bindValue(11, sampleTable.patternSequence);
    query.bindValue(12, sampleTable.mismatchBetweenPC);
    query.bindValue(13, sampleTable.mismatchBetweenFR);
    query.bindValue(14, sampleTable.mmismatchBetweenFR);
    query.bindValue(15, sampleTable.editPostion);
    query.bindValue(16, sampleTable.typeResult);
    query.bindValue(17, sampleTable.gsspInfo);
    query.bindValue(18, sampleTable.shieldAllele);
    query.bindValue(19, sampleTable.setResult);
    query.bindValue(20, sampleTable.setNote);
    query.bindValue(21, sampleTable.setGSSP);
    query.bindValue(22, sampleTable.combinedResult);
    query.bindValue(23, sampleTable.sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"update sampleTable error";
    }
}

void updateOneFileTable(const FileTable &fileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE fileTable set "
                  "sampleName=?,"
                  "filePath=?,"
                  "isExtraFile=?,"
                  "geneName=?,"
                  "exonIndex=?,"
                  "rOrF=?,"
                  "exonStartPos=?,"
                  "exonEndPos=?,"
                  "usefulSequence=?,"
                  "baseSequence=?,"
                  "basePostion=?,"
                  "baseQuality=?,"
                  "baseNumber=?,"
                  "baseASignal=?,"
                  "baseTSignal=?,"
                  "baseGSignal=?,"
                  "baseCSignal=?,"
                  "signalNumber=?,"
                  "maxSignal=?,"
                  "maxQuality=?,"
                  "averageBaseWidth=?,"
                  "isGood=?,"
                  "alignResult=?,"
                  "alignStartPos=?,"
                  "alignEndPos=?,"
                  "alignInfo=?,"
                  "excludeLeft=?,"
                  "excludeRight=?,"
                  "editInfo=? "
                  "where fileName=?");
    query.bindValue(0, fileTable.sampleName);
    query.bindValue(1, fileTable.filePath);
    query.bindValue(2, fileTable.isExtraFile);
    query.bindValue(3, fileTable.geneName);
    query.bindValue(4, fileTable.exonIndex);
    query.bindValue(5, fileTable.rOrF);
    query.bindValue(6, fileTable.exonStartPos);
    query.bindValue(7, fileTable.exonEndPos);
    query.bindValue(8, fileTable.usefulSequence);
    query.bindValue(9, fileTable.baseSequence);
    query.bindValue(10, fileTable.basePostion);
    query.bindValue(11, fileTable.baseQuality);
    query.bindValue(12, fileTable.baseNumber);
    query.bindValue(13, fileTable.baseASignal);
    query.bindValue(14, fileTable.baseTSignal);
    query.bindValue(15, fileTable.baseGSignal);
    query.bindValue(16, fileTable.baseCSignal);
    query.bindValue(17, fileTable.signalNumber);
    query.bindValue(18, fileTable.maxSignal);
    query.bindValue(19, fileTable.maxQuality);
    query.bindValue(20, fileTable.averageBaseWidth);
    query.bindValue(21, fileTable.isGood);
    query.bindValue(22, fileTable.alignResult);
    query.bindValue(23, fileTable.alignStartPos);
    query.bindValue(24, fileTable.alignEndPos);
    query.bindValue(25, fileTable.alignInfo);
    query.bindValue(26, fileTable.excludeLeft);
    query.bindValue(27, fileTable.excludeRight);
    query.bindValue(28, fileTable.editInfo);
    query.bindValue(29, fileTable.fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"update fileTable error";
    }
}

void updateOneGsspFileTable(const GsspFileTable &gsspFileTable)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE gsspFileTable set "
                  "sampleName=?,"
                  "filePath=?,"
                  "gsspName=?,"
                  "geneName=?,"
                  "exonIndex=?,"
                  "rOrF=?,"
                  "exonStartPos=?,"
                  "exonEndPos=?,"
                  "usefulSequence=?,"
                  "baseSequence=?,"
                  "basePostion=?,"
                  "baseQuality=?,"
                  "baseNumber=?,"
                  "baseASignal=?,"
                  "baseTSignal=?,"
                  "baseGSignal=?,"
                  "baseCSignal=?,"
                  "signalNumber=?,"
                  "maxSignal=?,"
                  "maxQuality=?,"
                  "averageBaseWidth=?,"
                  "isGood=?,"
                  "alignResult=?,"
                  "alignStartPos=?,"
                  "alignEndPos=?,"
                  "alignInfo=?,"
                  "excludeLeft=?,"
                  "excludeRight=?,"
                  "editInfo=?,"
                  "typeResult=?,"
                  "filterResult=? where fileName=?");
    query.bindValue(0, gsspFileTable.sampleName);
    query.bindValue(1, gsspFileTable.filePath);
    query.bindValue(2, gsspFileTable.gsspName);
    query.bindValue(3, gsspFileTable.geneName);
    query.bindValue(4, gsspFileTable.exonIndex);
    query.bindValue(5, gsspFileTable.rOrF);
    query.bindValue(6, gsspFileTable.exonStartPos);
    query.bindValue(7, gsspFileTable.exonEndPos);
    query.bindValue(8, gsspFileTable.usefulSequence);
    query.bindValue(9, gsspFileTable.baseSequence);
    query.bindValue(10, gsspFileTable.basePostion);
    query.bindValue(11, gsspFileTable.baseQuality);
    query.bindValue(12, gsspFileTable.baseNumber);
    query.bindValue(13, gsspFileTable.baseASignal);
    query.bindValue(14, gsspFileTable.baseTSignal);
    query.bindValue(15, gsspFileTable.baseGSignal);
    query.bindValue(16, gsspFileTable.baseCSignal);
    query.bindValue(17, gsspFileTable.signalNumber);
    query.bindValue(18, gsspFileTable.maxSignal);
    query.bindValue(19, gsspFileTable.maxQuality);
    query.bindValue(20, gsspFileTable.averageBaseWidth);
    query.bindValue(21, gsspFileTable.isGood);
    query.bindValue(22, gsspFileTable.alignResult);
    query.bindValue(23, gsspFileTable.alignStartPos);
    query.bindValue(24, gsspFileTable.alignEndPos);
    query.bindValue(25, gsspFileTable.alignInfo);
    query.bindValue(26, gsspFileTable.excludeLeft);
    query.bindValue(27, gsspFileTable.excludeRight);
    query.bindValue(28, gsspFileTable.editInfo);
    query.bindValue(29, gsspFileTable.typeResult);
    query.bindValue(30, gsspFileTable.filterResult);
    query.bindValue(31, gsspFileTable.fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"update gsspFileTable error";
    }
}

int getFileInfoFromRealTimeDatabase(const QByteArray &sampleName, QVector<FileInfo> &fileInfos, ExonInfo &exonInfo)
{
    QSqlQuery query(realTimeDatabase);
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
    if(!isSuccess)
    {
        qDebug()<<"getFileInfoFromRealTimeDatabase error:"<<sampleName;
        return -1;
    }
    else
    {
        while(query.next())
        {
            if(query.value(7).toInt() != 0)
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
            modifySequence(fileInfo.sequence, fileInfo.editPostion, fileInfo.exonStartPos, query.value(8).toInt(), query.value(9).toInt(), query.value(10).toString());
            compareWithExonInfo(exonInfo, query.value(2).toInt(), fileInfo.exonStartPos, fileInfo.exonEndPos);
            fileInfos.push_back(fileInfo);
        }
    }
    return flag;
}

int getGsspFileInfoFromRealTimeDatabase(const QByteArray &sampleName, QVector<FileInfo> &gsspFileInfos, ExonInfo &exonInfo)
{
    QSqlQuery query(realTimeDatabase);
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
    if(!isSuccess)
    {
        qDebug()<<"getGsspFileInfoFromRealTimeDatabase error:" << sampleName;
        return -1;
    }
    else
    {
        while(query.next())
        {
            if(query.value(7).toInt() == UNMATCH) //UNMATCH
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
            modifySequence(fileInfo.sequence, fileInfo.editPostion, fileInfo.exonStartPos, query.value(8).toInt(), query.value(9).toInt(), query.value(10).toString());
            compareWithExonInfo(exonInfo, query.value(2).toInt(), fileInfo.exonStartPos, fileInfo.exonEndPos);
            gsspFileInfos.push_back(fileInfo);
        }
    }
    return flag;
}

void compareWithExonInfo(ExonInfo &exonInfo, int exonIndex, int exonStartPos, int exonEndPos)
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

void modifySequence(QByteArray &sequence, QSet<int> &editPostion, int exonStartPos, int excludeLeft, int excludeRight, const QString &editString)
{
    if(editString.size()>0)
    {
        QStringList edits = editString.split(";", QString::SkipEmptyParts);
        for(int i=0; i<edits.size(); i++)
        {
            QStringList posBase = edits.at(i).split(":", QString::SkipEmptyParts);
            int pos = posBase.at(0).toInt(); //外显子位点
            sequence[pos-exonStartPos] = posBase.at(1).at(0).toAscii();
            editPostion.insert(pos);
        }
    }
    if(excludeLeft >= 0)
    {
		int i = excludeLeft-exonStartPos+1;
        while(sequence[i]=='.')
        {
            sequence[i]= '-';
            i++;
        }
        for(int i=excludeLeft-exonStartPos; i>=0; i--)
        {
            sequence[i]='-';
        }
    }
    if(excludeRight >= 0)
    {
		int i = excludeRight - exonStartPos -1;
        while(sequence[i]=='.')
        {
            sequence[i] = '-';
            i--;
        }
        for(int i=excludeRight-exonStartPos; i<sequence.size(); i++)
        {
            sequence[i]='-';
        }
    }
}

void getShieldAllelesFromDatabase(const QByteArray &sampleName, QSet<QString> &shieldAlleles)
{
    QSqlQuery query(realTimeDatabase);
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

void updateGsspFileResultToRealTimeDatabase(const QByteArray &fileName, int alignResult, const QString &typeResult, const QString &filterResult)
{
    QSqlQuery query(realTimeDatabase);
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

void insertSampleInfoToRealTimeDatabase(const SampleInfo &sampleInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT sampleName FROM sampleTable WHERE sampleName = ?");
    query.bindValue(0, sampleInfo.sampleName);
    query.exec();
    if(query.next())
    {
        updateSampleInfoToRealTimeDatabaseT(sampleInfo);
    }
    else
    {
        insertSampleInfoToRealTimeDatabaseT(sampleInfo);
    }
}

void insertSampleInfoToRealTimeDatabaseT(const SampleInfo &sampleInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("INSERT INTO sampleTable (sampleName,"
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

void updateSampleInfoToRealTimeDatabaseT(const SampleInfo &sampleInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable set "
                  "geneName=?,"
                  "analysisType=?,"
                  "minExonIndex=?,"
                  "maxExonIndex=?,"
                  "exonStartPos=?,"
                  "exonEndPos=?,"
                  "consensusSequence=?,"
                  "forwardSequence=?,"
                  "reverseSequence=?,"
                  "patternSequence=?,"
                  "mismatchBetweenPC=?,"
                  "mismatchBetweenFR=?,"
                  "mmismatchBetweenFR=?,"
                  "editPostion=?,"
                  "typeResult=?,"
                  "combinedResult=? WHERE sampleName=?");
    query.bindValue(16, sampleInfo.sampleName);
    query.bindValue(0, sampleInfo.geneName);
    query.bindValue(1, sampleInfo.analysisType);
    query.bindValue(2, sampleInfo.minExonIndex);
    query.bindValue(3, sampleInfo.maxExonIndex);
    query.bindValue(4, sampleInfo.exonStartPos);
    query.bindValue(5, sampleInfo.exonEndPos);
    query.bindValue(6, sampleInfo.consensusSequence);
    query.bindValue(7, sampleInfo.forwardSequence);
    query.bindValue(8, sampleInfo.reverseSequence);
    query.bindValue(9, sampleInfo.patternSequence);
    query.bindValue(10, sampleInfo.mismatchBetweenPC);
    query.bindValue(11, sampleInfo.mismatchBetweenFR);
    query.bindValue(12, sampleInfo.mmismatchBetweenFR);
    query.bindValue(13, sampleInfo.editPostion);
    query.bindValue(14, sampleInfo.typeResult);
    query.bindValue(15, sampleInfo.combinedResult);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"updateSampleInfoToRealTimeDatabaseT error:"<<sampleInfo.sampleName;
    }
}

void getBaseAlignSampleInfo(const QString &sampleName, BaseAlignSampleInfo &baseAlignSampleInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT exonStartPos,exonEndPos,consensusSequence,forwardSequence,reverseSequence,"
                  "patternSequence,mismatchBetweenPC,mismatchBetweenFR,editPostion "
                  "FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"find from sampleTable "<<sampleName<<" error";
    }
    else
    {
        while(query.next())
        {
            baseAlignSampleInfo.alignStartPos = query.value(0).toInt();
            baseAlignSampleInfo.alignEndPos = query.value(1).toInt();
            baseAlignSampleInfo.consensusSeq = query.value(2).toString();
            baseAlignSampleInfo.forwardSeq = query.value(3).toString();
            baseAlignSampleInfo.reverseSeq = query.value(4).toString();
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

    query.prepare("SELECT fileName,gsspName,exonStartPos,usefulSequence, excludeLeft, excludeRight, editInfo FROM gsspFileTable WHERE sampleName=? AND alignResult != 4");
    query.bindValue(0, sampleName.toAscii());
    isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"find gsspFileTable"<<sampleName<<"error";
    }
    else
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

void getSampleTreeDataFromRealTimeDatabase(QVector<SampleTreeInfo> &sampleTreeInfoList)
{
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("SELECT sampleName,geneName,analysisType,markType FROM sampleTable ORDER BY sampleName DESC");
    if(!isSuccess)
    {
        qDebug()<<"getSampleTreeDataFromRealTimeDatabase error";
    }
    else
    {
        while(query.next())
        {
            SampleTreeInfo sampleTreeInfo;
            sampleTreeInfo.sampleName = query.value(0).toString();
            sampleTreeInfo.geneName = query.value(1).toString();
            sampleTreeInfo.analysisType = query.value(2).toInt();
            sampleTreeInfo.markType = query.value(3).toInt();
            getFileTreeInfosFromRealTimeDatabase(sampleTreeInfo.sampleName, sampleTreeInfo.fileTreeInfo);
            getGsspFileTreeInfosFromRealTimeDatabase(sampleTreeInfo.sampleName, sampleTreeInfo.gsspTreeInfo);
            sampleTreeInfoList.push_front(sampleTreeInfo);

        }
    }
}
void getFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<FileTreeInfo> &fileTreeInfos)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT fileName,exonIndex,rOrF,isGood,alignResult FROM fileTable WHERE sampleName=? ORDER BY fileName DESC");
    query.bindValue(0, sampleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getFileInfoFromRealTimeDatabase error";
    }
    else
    {
        while(query.next())
        {
            FileTreeInfo fileTreeInfo;
            fileTreeInfo.fileName = query.value(0).toString();
            fileTreeInfo.exonIndex= query.value(1).toInt();
            fileTreeInfo.rOrF = query.value(2).toString();
            fileTreeInfo.isGood = query.value(3).toInt();
            fileTreeInfo.analysisType = query.value(4).toInt();
            fileTreeInfos.push_front(fileTreeInfo);
        }
    }
}

void getGsspFileTreeInfosFromRealTimeDatabase(const QString &sampleName, QVector<GsspTreeInfo> &gsspTreeInfos)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT fileName,gsspName,exonIndex,rOrF,isGood,alignResult FROM gsspFileTable WHERE sampleName=? ORDER BY fileName DESC");
    query.bindValue(0, sampleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getGsspFileTreeInfosFromRealTimeDatabase error";
    }
    else
    {
        while(query.next())
        {
            GsspTreeInfo gsspTreeInfo;
            gsspTreeInfo.fileName = query.value(0).toString();
            gsspTreeInfo.gsspName = query.value(1).toString();
            gsspTreeInfo.exonIndex= query.value(2).toInt();
            gsspTreeInfo.rOrF = query.value(3).toString();
            gsspTreeInfo.isGood = query.value(4).toInt();
            gsspTreeInfo.analysisType = query.value(5).toInt();
            gsspTreeInfos.push_front(gsspTreeInfo);
        }
    }
}

void getExonNavigatorInfo(const QString &sampleName, ExonNavigatorInfo &exonNavigatorInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT minExonIndex,maxExonIndex,mismatchBetweenPC,mismatchBetweenFR,mmismatchBetweenFR,editPostion"
                  " FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExonNavigatorInfo error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            exonNavigatorInfo.minExonIndex = query.value(0).toInt();
            exonNavigatorInfo.maxExonIndex = query.value(1).toInt();
            QStringList pcMis = query.value(2).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<pcMis.size(); i++)
            {
                exonNavigatorInfo.pcMis.push_back(pcMis.at(i).toInt());
            }
            QStringList frMis = query.value(3).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<frMis.size(); i++)
            {
                exonNavigatorInfo.frMis.push_back(frMis.at(i).toInt());
            }
            QStringList frUnEqul = query.value(4).toString().split(":",QString::SkipEmptyParts);
            for(int i=0; i<frUnEqul.size(); i++)
            {
                exonNavigatorInfo.frUnEqual.push_back(frUnEqul.at(i).toInt());
            }
            QStringList edit = query.value(5).toString().split(":", QString::SkipEmptyParts);
            for(int i=0; i<edit.size(); i++)
            {
                exonNavigatorInfo.editPos.push_back(edit.at(i).toInt());
            }
        }
    }
}

void insertShieldAllelesToSampleTable(const QByteArray &sampleName, const QString &alleles)
{
    QSqlQuery query(realTimeDatabase);
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

void getCombinedFromRealTimeDatabase(const QByteArray &sampleName, QStringList &typeResult)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT combinedResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getCombinedFromRealTimeDatabase error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
            break;
        }
    }
}

void getTypeResultFromRealTimeDatabse(const QByteArray &sampleName, QStringList &typeResult)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT typeResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getTypeResultFromRealTimeDatabse error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
            break;
        }
    }
}

void getGsspFilterFromRealTimeDatabase(const QByteArray &fileName, QStringList &typeResult)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT filterResult FROM gsspFileTable WHERE fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getGsspFilterFromRealTimeDatabase error"<<fileName;
    }
    else
    {
        while(query.next())
        {
            typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
            break;
        }
    }
}

void getGsspResultFromRealTimeDatabase(const QByteArray &fileName, QStringList &typeResult)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT typeResult FROM gsspFileTable WHERE fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getGsspResultFromRealTimeDatabase error"<<fileName;
    }
    else
    {
        while(query.next())
        {
            typeResult = query.value(0).toString().split(";", QString::SkipEmptyParts);
            break;
        }
    }
}

void markAllSampleApproved()
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable SET markType =?");
    query.bindValue(0, APPROVED);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"markAllSampleApproved error";
    }
}

int markAllSampleReviewed()
{
    QSqlQuery query(realTimeDatabase);
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

int getMarkTypeBySampleName(const QByteArray &sampleName)
{
    int is = -1;
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT markType FROM sampleTable WHERE sampleName =?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getMarkTypeBySampleName error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            is = query.value(0).toInt();
            break;
        }
    }
    return is;
}
int getAnalysisTypeBySampleName(const QByteArray &sampleName)
{
    int is = -1;
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT analysisType FROM sampleTable WHERE sampleName =?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAnalysisTypeBySampleName error"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            is = query.value(0).toInt();
            break;
        }
    }
    return is;
}

void setMarkTypeBySampleName(const QByteArray &sampleName, int type)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable SET markType=? WHERE sampleName=?");
    query.bindValue(0, type);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"setMarkTypeBySampleName error"<<sampleName;
    }
}



void resetFileByFileName(const QByteArray &fileName)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT geneName,exonIndex,rOrF FROM fileTable WHERE fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"resetFileByFileName error 1:"<<fileName;
        return;
    }
    if(query.next())
    {
        QString etKey = query.value(0).toString() + "_" + query.value(1).toString() + "_" + query.value(2).toString();
        query.prepare("UPDATE fileTable SET excludeLeft=?,excludeRight=?,editInfo=? WHERE fileName=?");
        query.bindValue(0, getExcludeLeftPosition(etKey));
        query.bindValue(1, getExcludeRightPostion(etKey));
        query.bindValue(2, "");
        query.bindValue(3, fileName);
        isSuccess = query.exec();
        if(!isSuccess)
        {
            qDebug()<<"resetFileByFileName error"<<fileName;
        }
    }
}

void resetGsspFileByFileName(const QByteArray &fileName)
{
    //qDebug()<<"GSSP";
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT geneName,exonIndex,rOrF FROM gsspFileTable WHERE fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"resetGsspFileByFileName error 1:"<<fileName;
        return;
    }
    if(query.next())
    {
        QString etKey = query.value(0).toString() + "_" + query.value(1).toString() + "_" + query.value(2).toString();
        query.prepare("UPDATE gsspFileTable SET excludeLeft=?,excludeRight=?,editInfo=? WHERE fileName=?");
//        query.bindValue(0, getExcludeLeftPosition(etKey));
//        query.bindValue(1, getExcludeRightPostion(etKey));
        query.bindValue(0, -1);
        query.bindValue(1, -1);
        query.bindValue(2, "");
        query.bindValue(3, fileName);
        isSuccess = query.exec();
        if(!isSuccess)
        {
            qDebug()<<"resetGsspFileByFileName error"<<fileName;
        }
    }
}





QList<FileInfo_ly> OgetAllData(QString sampleName ,int exonIndex)
{
    //OPenDatabase();
    QList<FileInfo_ly> fileinfo;
    QSqlQuery query(realTimeDatabase);
//    SELECT shieldAllele FROM sampleTable WHERE sampleName=? and exonIndex=? where sampleName=?
    query.prepare("SELECT * from fileTable WHERE sampleName=? and exonIndex=?  order by fileName ");
    query.bindValue(0, sampleName.toAscii());
    query.bindValue(1, exonIndex);
    int i=0;
    if(query.exec())
    {
        while(query.next())
        {
            ++i;
            if (query.value(23).toInt()!=0)
            {
               continue;
            }
            FileInfo_ly one;
            one.fileName=query.value(0).toString();
            one.sampleName=query.value(1).toString();
            one.filePath=query.value(2).toString();
            one.isExtraFile=query.value(3).toBool();
            one.geneName=query.value(4).toString();
            one.exonIndex=query.value(5).toInt();
            //QChar aa=query.value(6).ByteArray().data()[0];
            one.rOrF=query.value(6).toString();
            //qDebug()<<one.rOrF;
            one.exonStartPos=query.value(7).toInt();
            one.exonEndPos=query.value(8).toInt();
            one.usefulSequence=query.value(9).toString();
            one.baseSequence=query.value(10).toString();
            one.basePostion=query.value(11).toString();
            one.baseQuality=query.value(12).toString();
            one.baseNumber=query.value(13).toInt();
            one.baseASignal=query.value(14).toString();
            one.baseTSignal=query.value(15).toString();
            one.baseGSignal=query.value(16).toString();
            one.baseCSignal=query.value(17).toString();
            one.signalNumber=query.value(18).toInt();
            one.maxSgnal=query.value(19).toInt();
            one.maxQuality=query.value(20).toInt();
            one.averageBaseWidth=query.value(21).toDouble();
            one.isGood=query.value(22).toBool();
            one.alignResult=query.value(23).toInt();
            one.alignStartPos=query.value(24).toInt();
            one.alignEndPos=query.value(25).toInt();
            one.alignInfo=query.value(26).toString();
            one.excludeleft=query.value(27).toInt();
            one.excludeRight=query.value(28).toInt();
            one.editInfo=query.value(29).toString();
            one.avgsignal=query.value(30).toDouble();
            one.isgssp=false;
            fileinfo.push_back(one);



        }
        //qDebug()<<"result ="<<i;
    }
    QSqlQuery query2(realTimeDatabase);
    query2.prepare("SELECT * from gsspFileTable WHERE sampleName=? and exonIndex=? order by fileName");
    query2.bindValue(0, sampleName.toAscii());
    query2.bindValue(1, exonIndex);
    if(query2.exec())
    {
        while(query2.next())
        {
            ++i;
            if (query2.value(23).toInt()==4)
            {
              continue;
            }
            FileInfo_ly one;
            one.fileName=query2.value(0).toString();
            one.sampleName=query2.value(1).toString();
            one.filePath=query2.value(2).toString();
            //one.isExtraFile=query.value(3).toBool();
            one.geneName=query2.value(4).toString();
            one.exonIndex=query2.value(5).toInt();
            one.rOrF=query2.value(6).toString();
            one.exonStartPos=query2.value(7).toInt();
            one.exonEndPos=query2.value(8).toInt();
            one.usefulSequence=query2.value(9).toString();
            one.baseSequence=query2.value(10).toString();
            one.basePostion=query2.value(11).toString();
            one.baseQuality=query2.value(12).toString();
            one.baseNumber=query2.value(13).toInt();
            one.baseASignal=query2.value(14).toString();
            one.baseTSignal=query2.value(15).toString();
            one.baseGSignal=query2.value(16).toString();
            one.baseCSignal=query2.value(17).toString();
            one.signalNumber=query2.value(18).toInt();
            one.maxSgnal=query2.value(19).toInt();
            one.maxQuality=query2.value(20).toInt();
            one.averageBaseWidth=query2.value(21).toDouble();
            one.isGood=query2.value(22).toBool();
            one.alignResult=query2.value(23).toInt();
            one.alignStartPos=query2.value(24).toInt();
            one.alignEndPos=query2.value(25).toInt();
            one.alignInfo=query2.value(26).toString();
            one.excludeleft=query2.value(27).toInt();
            one.excludeRight=query2.value(28).toInt();
            one.editInfo=query2.value(29).toString();
            one.isgssp=true;
            fileinfo.push_back(one);



        }
        //qDebug()<<"result ="<<i;
    }
    //qDebug()<<"opened database";
    return fileinfo;

}



QList<FileInfo_ly> OgetOneData(QString fileName ,bool isgssp)
{
    //OPenDatabase();
    QList<FileInfo_ly> fileinfo;
    QSqlQuery query(realTimeDatabase);
//    SELECT shieldAllele FROM sampleTable WHERE sampleName=? and exonIndex=? where sampleName=?
    if(isgssp)
    {
        query.prepare("SELECT * from gsspFileTable WHERE fileName=? ");
    }
    else
    {
        query.prepare("SELECT * from fileTable WHERE fileName=? ");

    }
    query.bindValue(0, fileName.toAscii());
    //query.bindValue(1, exonIndex);
    int i=0;
    if(query.exec())
    {
        while(query.next())
        {
            ++i;
            FileInfo_ly one;
            one.fileName=query.value(0).toString();
            one.sampleName=query.value(1).toString();
            one.filePath=query.value(2).toString();
            one.isExtraFile=query.value(3).toBool();
            one.geneName=query.value(4).toString();
            one.exonIndex=query.value(5).toInt();
            //QChar aa=query.value(6).ByteArray().data()[0];
            one.rOrF=query.value(6).toString();
            //qDebug()<<one.rOrF;
            one.exonStartPos=query.value(7).toInt();
            one.exonEndPos=query.value(8).toInt();
            one.usefulSequence=query.value(9).toString();
            one.baseSequence=query.value(10).toString();
            one.basePostion=query.value(11).toString();
            one.baseQuality=query.value(12).toString();
            one.baseNumber=query.value(13).toInt();
            one.baseASignal=query.value(14).toString();
            one.baseTSignal=query.value(15).toString();
            one.baseGSignal=query.value(16).toString();
            one.baseCSignal=query.value(17).toString();
            one.signalNumber=query.value(18).toInt();
            one.maxSgnal=query.value(19).toInt();
            one.maxQuality=query.value(20).toInt();
            one.averageBaseWidth=query.value(21).toDouble();
            one.isGood=query.value(22).toBool();
            one.alignResult=query.value(23).toInt();
            one.alignStartPos=query.value(24).toInt();
            one.alignEndPos=query.value(25).toInt();
            one.alignInfo=query.value(26).toString();
            one.excludeleft=query.value(27).toInt();
            one.excludeRight=query.value(28).toInt();
            one.editInfo=query.value(29).toString();
            one.isgssp=false;
            fileinfo.push_back(one);



        }
        //qDebug()<<"result ="<<i;
    }
    /*
    QSqlQuery query2(realTimeDatabase);
    query2.prepare("SELECT * from gsspFileTable WHERE sampleName=? and exonIndex=?");
    query2.bindValue(0, sampleName.toAscii());
    query2.bindValue(1, exonIndex);
    if(query2.exec())
    {
        while(query2.next())
        {
            ++i;
//            if (query2.value(23).toInt()!=0)
//            {
//              continue;
//            }
            FileInfo_ly one;
            one.fileName=query2.value(0).toString();
            one.sampleName=query2.value(1).toString();
            one.filePath=query2.value(2).toString();
            //one.isExtraFile=query.value(3).toBool();
            one.geneName=query2.value(4).toString();
            one.exonIndex=query2.value(5).toInt();
            one.rOrF=query2.value(6).toChar().toLatin1();
            one.exonStartPos=query2.value(7).toInt();
            one.exonEndPos=query2.value(8).toInt();
            one.usefulSequence=query2.value(9).toString();
            one.baseSequence=query2.value(10).toString();
            one.basePostion=query2.value(11).toString();
            one.baseQuality=query2.value(12).toString();
            one.baseNumber=query2.value(13).toInt();
            one.baseASignal=query2.value(14).toString();
            one.baseTSignal=query2.value(15).toString();
            one.baseGSignal=query2.value(16).toString();
            one.baseCSignal=query2.value(17).toString();
            one.signalNumber=query2.value(18).toInt();
            one.maxSgnal=query2.value(19).toInt();
            one.maxQuality=query2.value(20).toInt();
            one.averageBaseWidth=query2.value(21).toDouble();
            one.isGood=query2.value(22).toBool();
            one.alignResult=query2.value(23).toInt();
            one.alignStartPos=query2.value(24).toInt();
            one.alignEndPos=query2.value(25).toInt();
            one.alignInfo=query2.value(26).toString();
            one.excludeleft=query2.value(27).toInt();
            one.excludeRight=query2.value(28).toInt();
            one.editInfo=query2.value(29).toString();
            one.isgssp=true;
            fileinfo.push_back(one);



        }
        //qDebug()<<"result ="<<i;
    }
    */
    //qDebug()<<"opened database";
    return fileinfo;

}
bool upDatabyChangebp(QString filename, QString streditinfo,bool isgssp)
{

    QSqlQuery query(realTimeDatabase);
    if(isgssp)
        query.prepare("UPDATE gsspFileTable SET editInfo=? WHERE fileName=?");
    else
        query.prepare("UPDATE fileTable SET editInfo=? WHERE fileName=?");

    query.bindValue(0, streditinfo);
    query.bindValue(1, filename.toAscii());
    //qDebug()<< query.lastQuery();
    if(!query.exec())
    {
        qDebug()<<query.lastError().databaseText();
        qDebug()<<query.lastError();
        return false;
    }
    return true;


}

bool updatabyLeftExclude(QString filename ,int pos,bool isgssp)
{
    QSqlQuery query(realTimeDatabase);
    if(isgssp)
        query.prepare("UPDATE gsspFileTable SET excludeLeft=? WHERE fileName=?");
    else
        query.prepare("UPDATE fileTable SET excludeLeft=? WHERE fileName=?");
    query.bindValue(0, pos);
    query.bindValue(1, filename.toAscii());
    if(!query.exec())
    {
        qDebug()<<query.lastError().databaseText();
        qDebug()<<query.lastError();
        return false;
    }
    return true;
}
bool updatabyRightExclude(QString filename ,int pos,bool isgssp)
{
    QSqlQuery query(realTimeDatabase);
    if(isgssp)
        query.prepare("UPDATE gsspFileTable SET excludeRight=? WHERE fileName=?");
    else
        query.prepare("UPDATE fileTable SET excludeRight=? WHERE fileName=?");
    query.bindValue(0, pos);
    query.bindValue(1, filename.toAscii());
    if(!query.exec())
    {
        qDebug()<<query.lastError().databaseText();
        qDebug()<<query.lastError();
        return false;
    }
    return true;
}
int getcountof_file()
{

    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT COUNT(*) from fileTable");
    int i=0;
    if(query.exec())
    {
        while(query.next())
        {
            i=query.value(0).toInt();
        }
    }
    query.prepare("SELECT COUNT(*) from gsspFileTable");
    if(query.exec())
    {
        while(query.next())
        {
            i=query.value(0).toInt();
        }
    }
    return i;
    //qDebug()<<i;
}






void saveSample(const QByteArray &sampleName,const QString &samplePath,const QString &date)
{
    QFile file(samplePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream stream(&file);

    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT * FROM sampleTable where sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();

    if(!isSuccess)
        qDebug()<<"get sample result from realTimeDatabase failed"<<sampleName;
    else
    {
        while(query.next())
        {
            if(!date.isEmpty())
            {
                if(!sampleName.contains("_"))
                    stream << query.value(0).toString().append(QString("_%1").arg(date))<<"\n";
                else
                    stream << query.value(0).toString().split("_")[0].append(QString("_%1").arg(date))<<"\n";
            }
            else
                stream << query.value(0).toString()<<"\n";
            for(int i = 1; i < SAMPLE_TABLE_FIELD; i++)
                stream << query.value(i).toString()<<"\n";
        }
    }

    file.close();

    return;
}

void saveFile(const QByteArray &fileName, const QString &filePath,const QString &dir,const QString &date)
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

    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT * FROM fileTable where fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();

    if(!isSuccess)
        qDebug()<<"get sample result from realTimeDatabase failed"<<fileName;
    else
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

            for(int i = 3; i < FILE_TABLE_FIELD; i++)
                stream << query.value(i).toString()<<"\n";

            orignalFilePath = query.value(2).toString();
        }
    }

    QFileInfo ab1(desPath);
    if(!ab1.exists())
        QFile::copy(orignalFilePath,desPath);

    file.close();

    return;
}

void saveGssp(const QByteArray &fileName, const QString &gsspFilePath,const QString &dir,const QString &date)
{
    QString orignalFilePath;
    //QString desPath = QString("%1%2%3").arg(dir).arg(QDir::separator()).arg(QString(fileName).split(":",QString::SkipEmptyParts).at(0));
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

    QFile file(gsspFilePath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream stream(&file);

    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT * FROM gsspFileTable where fileName=?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();

    if(!isSuccess)
        qDebug()<<"get sample result from realTimeDatabase failed"<<fileName;
    else
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

            for(int i = 3; i < GSSP_FILE_TABLE_FIELD; i++)
                stream << query.value(i).toString()<<"\n";

            orignalFilePath = query.value(2).toString();
        }
    }

    QFileInfo ab1(desPath);
    if(!ab1.exists())
        QFile::copy(orignalFilePath,desPath);

    file.close();

    return;
}

void loadSample(const QString &samplePath)
{
    SampleTable sampletable;

    QFile file(samplePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() <<"can not open the file" << endl;

    QTextStream stream(&file);

   sampletable.sampleName = stream.readLine().toAscii();
   sampletable.geneName = stream.readLine().toAscii();
   sampletable.fileType = stream.readLine().toInt();
   sampletable.markType = stream.readLine().toInt();
   sampletable.analysisType = stream.readLine().toInt();
   sampletable.minExonIndex = stream.readLine().toInt();
   sampletable.maxExonIndex = stream.readLine().toInt();
   sampletable.exonStartPos = stream.readLine().toInt();
   sampletable.exonEndPos = stream.readLine().toInt();
   sampletable.consensusSequence = stream.readLine().toAscii();
   sampletable.forwardSequence = stream.readLine().toAscii();
   sampletable.reverseSequence = stream.readLine().toAscii();
   sampletable.patternSequence = stream.readLine().toAscii();
   sampletable.mismatchBetweenPC = stream.readLine();
   sampletable.mismatchBetweenFR = stream.readLine();
   sampletable.mmismatchBetweenFR = stream.readLine();
   sampletable.editPostion = stream.readLine();
   sampletable.typeResult = stream.readLine();
   sampletable.gsspInfo = stream.readLine();
   sampletable.shieldAllele = stream.readLine();
   sampletable.setResult = stream.readLine();
   sampletable.setNote = stream.readLine();
   sampletable.setGSSP = stream.readLine();
   sampletable.combinedResult = stream.readLine();
   file.close();
   insertSampleToRealTimeDatabase(sampletable);
   return;
}

void loadFile(const QString &filePath)
{
    FileTable filetable;

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"can not open file"<<endl;

    QTextStream stream(&file);

    filetable.fileName = stream.readLine().toAscii();
    filetable.sampleName = stream.readLine().toAscii();
    filetable.filePath = stream.readLine();
    filetable.isExtraFile = stream.readLine().toInt();
    filetable.geneName = stream.readLine().toAscii();
    filetable.exonIndex = stream.readLine().toInt();
    filetable.rOrF = stream.readLine().toAscii();
    filetable.exonStartPos = stream.readLine().toInt();
    filetable.exonEndPos = stream.readLine().toInt();
    filetable.usefulSequence = stream.readLine().toAscii();
    filetable.baseSequence = stream.readLine().toAscii();
    filetable.basePostion = stream.readLine();
    filetable.baseQuality = stream.readLine();
    filetable.baseNumber = stream.readLine().toInt();
    filetable.baseASignal = stream.readLine();
    filetable.baseTSignal = stream.readLine();
    filetable.baseGSignal = stream.readLine();
    filetable.baseCSignal = stream.readLine();
    filetable.signalNumber = stream.readLine().toInt();
    filetable.maxSignal = stream.readLine().toInt();
    filetable.maxQuality = stream.readLine().toInt();
    filetable.averageBaseWidth = stream.readLine().toFloat();
    filetable.isGood = stream.readLine().toInt();
    filetable.alignResult = stream.readLine().toInt();
    filetable.alignStartPos = stream.readLine().toInt();
    filetable.alignEndPos = stream.readLine().toInt();
    filetable.alignInfo = stream.readLine();
    filetable.excludeLeft = stream.readLine().toInt();
    filetable.excludeRight = stream.readLine().toInt();
    filetable.editInfo = stream.readLine();

    insertFileToRealTimeDatabase(filetable);

    file.close();

    return;
}

void loadGssp(const QString &gsspFilePath)
{
    GsspFileTable gsspfiletable;

    QFile file(gsspFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"can not open file" <<endl;

    QTextStream stream(&file);

    gsspfiletable.fileName = stream.readLine().toAscii();
    gsspfiletable.sampleName = stream.readLine().toAscii();
    gsspfiletable.filePath = stream.readLine();
    gsspfiletable.gsspName = stream.readLine().toAscii();
    gsspfiletable.geneName = stream.readLine().toAscii();
    gsspfiletable.exonIndex = stream.readLine().toInt();
    gsspfiletable.rOrF = stream.readLine().toAscii();
    gsspfiletable.exonStartPos = stream.readLine().toInt();
    gsspfiletable.exonEndPos = stream.readLine().toInt();
    gsspfiletable.usefulSequence = stream.readLine().toAscii();
    gsspfiletable.baseSequence = stream.readLine().toAscii();
    gsspfiletable.basePostion = stream.readLine();
    gsspfiletable.baseQuality = stream.readLine();
    gsspfiletable.baseNumber = stream.readLine().toInt();
    gsspfiletable.baseASignal = stream.readLine();
    gsspfiletable.baseTSignal = stream.readLine();
    gsspfiletable.baseGSignal = stream.readLine();
    gsspfiletable.baseCSignal = stream.readLine();
    gsspfiletable.signalNumber = stream.readLine().toInt();
    gsspfiletable.maxSignal = stream.readLine().toInt();
    gsspfiletable.maxQuality = stream.readLine().toInt();
    gsspfiletable.averageBaseWidth = stream.readLine().toFloat();
    gsspfiletable.isGood = stream.readLine().toInt();
    gsspfiletable.alignResult = stream.readLine().toInt();
    gsspfiletable.alignStartPos = stream.readLine().toInt();
    gsspfiletable.alignEndPos = stream.readLine().toInt();
    gsspfiletable.alignInfo = stream.readLine().toAscii();
    gsspfiletable.excludeLeft = stream.readLine().toInt();
    gsspfiletable.excludeRight = stream.readLine().toInt();
    gsspfiletable.editInfo = stream.readLine();
    gsspfiletable.typeResult = stream.readLine();
    gsspfiletable.filterResult = stream.readLine();

    insertGsspFileToRealTimeDatabase(gsspfiletable);

    file.close();

    return;
}


//delete
void deleteSample(const QByteArray &sampleName)
{
    QSqlQuery query(realTimeDatabase);
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

void deleteFile(const QByteArray &fileName)
{
    QByteArray sampleName;
    int flag = -1;
    int fileNumbers = 0;
    QSqlQuery query(realTimeDatabase);

    query.prepare("select sampleName from fileTable where fileName = ?");
    query.bindValue(0, fileName);
    bool isSuccess = query.exec();
    if(!isSuccess)
        qDebug()<<"select sampleName from fileTable error:"<<fileName;
    else
    {
        while(query.next())
        {
            sampleName = query.value(0).toByteArray();
            flag = 1;
        }
    }

    query.prepare("select sampleName from gsspFileTable where fileName = ?");
    query.bindValue(0, fileName);
    isSuccess = query.exec();
    if(!isSuccess)
        qDebug()<<"select sampleName from gsspFileTable error:"<<fileName;
    else
    {
        while(query.next())
        {
            sampleName = query.value(0).toByteArray();
            flag = 0;
        }
    }

    if(flag == -1)
        return;

    query.prepare("SELECT sampleName FROM fileTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"select sampleName from fileTable error:"<<fileName;
    }
    else
    {
        while(query.next())
        {
            fileNumbers++;
            if(fileNumbers>1)
                break;
        }
    }

    query.prepare("SELECT sampleName FROM gsspFileTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"select sampleName from gsspFileTable error:"<<fileName;
    }
    else
    {
        while(query.next())
        {
            fileNumbers++;
            if(fileNumbers>1)
                break;
        }
    }

    if(fileNumbers > 1)
    {
        if(flag == 1)
        {
            query.prepare("delete from fileTable where fileName=?");
            query.bindValue(0, fileName);
            isSuccess = query.exec();
            if(!isSuccess)
                qDebug()<<"delete file from fileTable error:"<<fileName;
        }
        else
        {
            query.prepare("delete from gsspFileTable where fileName=?");
            query.bindValue(0, fileName);
            isSuccess = query.exec();
            if(!isSuccess)
                qDebug()<<"delete file from gsspFileTable error:"<<fileName;
        }
    }
    else
        deleteSample(sampleName);

    return;
}

void getSampleNamesFromRealTimeDatabase(QStringList &sampleNames)
{
    QSqlQuery query(realTimeDatabase);
    bool isSuccess = query.exec("SELECT sampleName FROM sampleTable ORDER BY sampleName");
    if(!isSuccess)
    {
        qDebug()<<"getSampleNamesFromRealTimeDatabase error";
    }
    else
    {
        while(query.next())
        {
            sampleNames.push_back(query.value(0).toString());
        }
    }
}

int getResultFromRealTimeDatabaseBySampleName(const QByteArray &sampleName, QString &result)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT typeResult,setResult,combinedResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getResultFromRealTimeDatabaseBySampleName error:"<<sampleName;
    }
    else
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


QString getSetResultBySampleName(const QByteArray &sampleName)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT setResult FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getSetResultBySampleName error:"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            return query.value(0).toString();
        }
    }
    return "";
}

void updateSetResultBySampleName(const QByteArray &sampleName, const QString &result)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable SET setResult=? WHERE sampleName=?");
    query.bindValue(0, result);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"updateSetResultBySampleName error";
    }
}

QString getSetNoteBySampleName(const QByteArray &sampleName)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT setNote FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getSetNoteBySampleName error:"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            return query.value(0).toString();
        }
    }
    return "";
}

void updateSetNoteBySampleName(const QByteArray &sampleName, const QString &info)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable SET setNote=? WHERE sampleName=?");
    query.bindValue(0, info);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"updateSetNoteBySampleName error"<<sampleName;
    }
}

void getExonIndexAndGeneBySampleName(const QByteArray &sampleName, int &exonStart, int &exonEnd, QByteArray &geneName)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT minExonIndex,maxExonIndex,geneName FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExonIndexAndGeneBySampleName error:"<<sampleName;
    }
    else
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



void updateSetGsspBySampleName(const QByteArray sampleName, const QString &gsspInfo)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("UPDATE sampleTable SET setGSSP=? WHERE sampleName=?");
    query.bindValue(0, gsspInfo);
    query.bindValue(1, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"updateSetGsspBySampleName error!"<<sampleName;
    }
}

QString getSetGsspBySampleName(const QByteArray &sampleName)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT setGSSP from sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
        qDebug()<<"getSetGsspBySampleName error!"<<sampleName;
    else
    {
        while(query.next())
            return query.value(0).toString();
    }
}

void getSampleStartEndBySampleName(const QByteArray &sampleName, int &start, int &end)
{
    QSqlQuery query(realTimeDatabase);
    query.prepare("SELECT exonStartPos,exonEndPos FROM sampleTable WHERE sampleName=?");
    query.bindValue(0, sampleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getSampleStartEndBySampleName error:"<<sampleName;
    }
    else
    {
        while(query.next())
        {
            start = query.value(0).toInt();
            end = query.value(1).toInt();
            return;
        }
    }

}
