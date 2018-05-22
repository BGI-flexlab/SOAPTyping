#include "staticdatabase.h"

//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qsqlite)
#include <QtSql>
#include <QtGui>
const QString staticDatabaseFilePath = QString("Database%1soapTypingStaticDatabase").arg(QDir::separator());
QSqlDatabase staticDatabase;

const QString gsspDatabaseFilePath = QString("Database%1soapTypingGsspDatabase").arg(QDir::separator());
QSqlDatabase gsspDatabase;

void createStaticDatabase()
{
    QDir dir;
    dir.mkpath("Database");
    staticDatabase = QSqlDatabase::addDatabase("QSQLITE","first");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    staticDatabase.setDatabaseName(staticDatabaseFilePath);
    staticDatabase.open();
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
        qDebug()<<"create staticDatabase error!";
    }
}

void createGeneTable()
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("CREATE TABLE geneTable (geneName VARCHAR PRIMARY KEY,"
                                "geneSequence VARCHAR,"
                                "exonCount INT,"
                                "exonPositionIndex VARCHAR,"
                                "geneClasses VARCHAR,"
                                "availableExon VARCHAR,"
                                "version VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create geneTable error!";
    }
}

void createOldGeneTable()
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("CREATE TABLE oldGeneTable (geneName VARCHAR PRIMARY KEY,"
                                "geneSequence VARCHAR,"
                                "exonCount INT,"
                                "exonPositionIndex VARCHAR,"
                                "geneClasses VARCHAR,"
                                "availableExon VARCHAR,"
                                "version VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create oldGeneTable error!";
    }
}

void createAlleleTable()
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("CREATE TABLE alleleTable (alleleName VARCHAR PRIMARY KEY,"
                                "alleleSequence VARCHAR,"
                                "geneName VARCHAR,"
                                "isRare INT,"
                                "isIndel INT,"
                                "classesNumber INT,"
                                "indelPostion INT,"
                                "indelInfo VARCHAR,"
                                "noStar VARCHAR,"
                                "FOREIGN KEY (geneName) REFERENCES geneTable)");
    if(!isSuccess)
    {
        qDebug()<<"create alleleTable error";
    }
}

void createLabAlignTable()
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("CREATE TABLE labAlignTable (alleleName VARCHAR PRIMARY KEY,"
                                "alleleSequence VARCHAR,"
                                "geneName VARCHAR,"
                                "misPosition VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create labAlignTable error";
    }
}

int loadStaticDatabase()
{
    QFile file(staticDatabaseFilePath);
    if(!file.exists())
    {
        return 1;
    }
    staticDatabase = QSqlDatabase::addDatabase("QSQLITE","first");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    staticDatabase.setDatabaseName(staticDatabaseFilePath);
    staticDatabase.open();
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {

        qDebug()<<"load staticDatabase error!";
        return 1;
    }
    return 0;
}

void removeStaticDatabase()
{
    QSqlDatabase::database("first").close();
    QSqlDatabase::removeDatabase("first");
    QFile::remove(staticDatabaseFilePath);
}

void updateStaticDatabase(const QString &geneFile, const QString &oldGeneFile, const QString &alleleFile, const QString &labAlignFile, const QString &exonTrimFile)
{
    removeStaticDatabase();
    createStaticDatabase();
    createGeneTable();
    createOldGeneTable();
    createAlleleTable();
    createLabAlignTable();
    createExonTrimTable();
    readGeneTableTxtFile(geneFile);
    readOldGeneTableTxtFile(oldGeneFile);
    readAlleleTableTxtFile(alleleFile);
    readLabAlignTableTxtFile(labAlignFile);
    readExonTrimTableTxtFile(exonTrimFile);
}

void insertGeneToGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT geneName FROM geneTable WHERE geneName=?");
    query.bindValue(0, geneTable.geneName);
    query.exec();
    if(query.next())
    {
        updateGeneTable(geneTable);
    }
    else
    {
        insertGeneTable(geneTable);
    }

}

void insertGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
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
        qDebug()<<"Insert Error geneTable";
    }
}

void updateGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("UPDATE geneTable set geneSequence=?,"
                  "exonCount=?,"
                  "exonPositionIndex=?,"
                  "geneClasses=?,"
                  "availableExon=?,"
                  "version=? WHERE geneName=?");
    query.bindValue(6, geneTable.geneName);
    query.bindValue(0, geneTable.geneSequence);
    query.bindValue(1, geneTable.exonCount);
    query.bindValue(2, geneTable.exonPostionIndex);
    query.bindValue(3, geneTable.geneClasses);
    query.bindValue(4, geneTable.availableExon);
    query.bindValue(5, geneTable.version);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Update Error geneTable";
    }
}

void insertGeneToOldGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT geneName FROM oldGeneTable WHERE geneName=?");
    query.bindValue(0, geneTable.geneName);
    query.exec();
    if(query.next())
    {
        updateOldGeneTable(geneTable);
    }
    else
    {
        insertOldGeneTable(geneTable);
    }
}
void insertOldGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("INSERT INTO oldGeneTable (geneName,"
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
        qDebug()<<"Insert Error oldGeneTable";
    }
}
void updateOldGeneTable(const GeneTable &geneTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("UPDATE oldGeneTable set geneSequence=?,"
                  "exonCount=?,"
                  "exonPositionIndex=?,"
                  "geneClasses=?,"
                  "availableExon=?,"
                  "version=? WHERE geneName=?");
    query.bindValue(6, geneTable.geneName);
    query.bindValue(0, geneTable.geneSequence);
    query.bindValue(1, geneTable.exonCount);
    query.bindValue(2, geneTable.exonPostionIndex);
    query.bindValue(3, geneTable.geneClasses);
    query.bindValue(4, geneTable.availableExon);
    query.bindValue(5, geneTable.version);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Update Error oldGeneTable";
    }
}


void insertAlleleToAlleleTable(const AlleleTable &alleleTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleTable.alleleName);
    query.exec();
    if(query.next())
    {
        updateAlleleTable(alleleTable);
    }
    else
    {
        insertAlleleTable(alleleTable);
    }

}
void insertAlleleTable(const AlleleTable &alleleTable)
{
    QSqlQuery query(staticDatabase);
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
        qDebug()<<"Insert error allaleTable";
    }
}
void updateAlleleTable(const AlleleTable &alleleTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("UPDATE alleleTable set "
                  "alleleSequence=?,geneName=?,isRare=?,"
                  "isIndel=?,classesNumber=?,indelPostion=?,"
                  "indelInfo=?,noStar=? WHERE alleleName=?");
    query.bindValue(8, alleleTable.alleleName);
    query.bindValue(0, alleleTable.alleleSequence);
    query.bindValue(1, alleleTable.geneName);
    query.bindValue(2, alleleTable.isRare);
    query.bindValue(3, alleleTable.isIndel);
    query.bindValue(4, alleleTable.classesNumber);
    query.bindValue(5, alleleTable.indelPosition);
    query.bindValue(6, alleleTable.indelInfo);
    query.bindValue(7, alleleTable.noStar);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Update error allaleTable";
    }
}


void insertLabAlignToLabAlignTable(const LabAlignTable &labAlignTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName FROM labAlignTable WHERE alleleName=?");
    query.bindValue(0, labAlignTable.alleleName);
    query.exec();
    if(query.next())
    {
        updateLabAlignTable(labAlignTable);
    }
    else
    {
        insertLabAlignTable(labAlignTable);
    }

}
void insertLabAlignTable(const LabAlignTable &labAlignTable)
{
    QSqlQuery query(staticDatabase);
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
        qDebug()<<"Insert error labAlignTable";
    }
}
void updateLabAlignTable(const LabAlignTable &labAlignTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("UPDATE labAlignTable set "
                  "alleleSequence=?,geneName=?,misPosition=? "
                  "WHERE alleleName=?");
    query.bindValue(3, labAlignTable.alleleName);
    query.bindValue(0, labAlignTable.alleleSequence);
    query.bindValue(1, labAlignTable.geneName);
    query.bindValue(2, labAlignTable.misPosition);

    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Update error labAlignTable";
    }
}

void readGeneTableTxtFile(const QString &geneFile)
{
    QFile file(geneFile);
    if(!file.exists())
    {
        qDebug()<<geneFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 7)
        {
            qDebug()<<"read geneFile line Error "<<line;
        }
        GeneTable geneTable;
        geneTable.geneName = part.at(0).toAscii();
        geneTable.geneSequence = part.at(1).toAscii();
        geneTable.exonCount = part.at(2).toInt();
        geneTable.exonPostionIndex = part.at(3).toAscii();
        geneTable.geneClasses = part.at(4).toAscii();
        geneTable.availableExon = part.at(5).toAscii();
        geneTable.version = part.at(6).toAscii();
        insertGeneToGeneTable(geneTable);
    }
    file.close();
}

void readOldGeneTableTxtFile(const QString &geneFile)
{
    QFile file(geneFile);
    if(!file.exists())
    {
        qDebug()<<geneFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 7)
        {
            qDebug()<<"read geneFile line Error "<<line;
        }
        GeneTable geneTable;
        geneTable.geneName = part.at(0).toAscii();
        geneTable.geneSequence = part.at(1).toAscii();
        geneTable.exonCount = part.at(2).toInt();
        geneTable.exonPostionIndex = part.at(3).toAscii();
        geneTable.geneClasses = part.at(4).toAscii();
        geneTable.availableExon = part.at(5).toAscii();
        geneTable.version = part.at(6).toAscii();
        insertGeneToOldGeneTable(geneTable);
    }
    file.close();
}

void readAlleleTableTxtFile(const QString &alleleFile)
{
    QFile file(alleleFile);
    if(!file.exists())
    {
        qDebug()<<alleleFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 9)
        {
            qDebug()<<"read alleleFile line Error "<<line;
        }
        AlleleTable alleleTable;
        alleleTable.alleleName = part.at(0).toAscii();
        alleleTable.alleleSequence = part.at(1).toAscii();
        alleleTable.geneName = part.at(2).toAscii();
        alleleTable.isRare = part.at(3).toInt();
        alleleTable.isIndel = part.at(4).toInt();
        alleleTable.classesNumber = part.at(5).toInt();
        alleleTable.indelPosition = part.at(6).toInt();
        alleleTable.indelInfo = part.at(7).toAscii();
        alleleTable.noStar = part.at(8).toAscii();
        insertAlleleToAlleleTable(alleleTable);
    }
    file.close();
}

void readLabAlignTableTxtFile(const QString &labAlignFile)
{
    QFile file(labAlignFile);
    if(!file.exists())
    {
        qDebug()<<labAlignFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 4)
        {
            qDebug()<<"read labAlignFile line Error "<<line;
        }
        LabAlignTable labAlignTable;
        labAlignTable.alleleName = part.at(0).toAscii();
        labAlignTable.alleleSequence = part.at(1).toAscii();
        labAlignTable.geneName = part.at(2).toAscii();
        labAlignTable.misPosition = part.at(3).toAscii();
        insertLabAlignToLabAlignTable(labAlignTable);
    }
    file.close();
}


void showAlleleTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM alleleTable",staticDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("alleleTable");
    view->show();
}

void showGeneTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM geneTable",staticDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("geneTable");
    view->show();
}

void showLabAlignTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM labAlignTable",staticDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("labAlignTable");
    view->show();
}
void showExonTrimTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM exonTrimTable",staticDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("exonTrimTable");
    view->show();
}

void createGsspDatabase()
{
    QDir dir;
    dir.mkpath("Database");
    gsspDatabase = QSqlDatabase::addDatabase("QSQLITE","second");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    gsspDatabase.setDatabaseName(gsspDatabaseFilePath);
    gsspDatabase.open();
    QSqlQuery query(gsspDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
        qDebug()<<"create gsspDatabase error!";
    }
}

void createGsspTable()
{
    QSqlQuery query(gsspDatabase);
    bool isSuccess = query.exec("CREATE TABLE gsspTable (gsspKey VARCHAR PRIMARY KEY,"
                                "gsspName VARCHAR,"
                                "geneName VARCHAR,"
                                "exonIndex INT,"
                                "rOrF VARCHAR,"
                                "position INT,"
                                "base VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create gsspTable error";
    }
}

int loadGsspDatabase()
{
    QFile file(gsspDatabaseFilePath);
    if(!file.exists())
    {
        return 1;
    }
    gsspDatabase = QSqlDatabase::addDatabase("QSQLITE","second");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    gsspDatabase.setDatabaseName(gsspDatabaseFilePath);
    gsspDatabase.open();
    QSqlQuery query(gsspDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
        qDebug()<<"load gsspDatabase error!";
        return 1;
    }
    return 0;
}

void removeGsspDatabase()
{
    QSqlDatabase::database("second").close();
    QSqlDatabase::removeDatabase("second");
    QFile::remove(gsspDatabaseFilePath);
}

void updateGsspDatabase(const QString &gsspFile)
{
    removeGsspDatabase();
    createGsspDatabase();
    createGsspTable();
    readGsspTableTxtFile(gsspFile);
}

void insertGsspToGsspTable(const GsspTable &gsspTable)
{
    QSqlQuery query(gsspDatabase);
    query.prepare("SELECT gsspKey FROM gsspTable WHERE gsspKey=?");
    query.bindValue(0, gsspTable.gsspKey);
    query.exec();
    if(query.next())
    {
        updateGsspTable(gsspTable);
    }
    else
    {
        insertGsspTable(gsspTable);
    }
}
void insertGsspTable(const GsspTable &gsspTable)
{
    QSqlQuery query(gsspDatabase);
    query.prepare("INSERT INTO gsspTable (gsspKey,"
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
        qDebug()<<"Insert error gsspTable";
    }
}
void updateGsspTable(const GsspTable &gsspTable)
{
    QSqlQuery query(gsspDatabase);
    query.prepare("UPDATE gsspTable set "
                  "gsspName=?,geneName=?,exonIndex=?,rOrF=?,"
                  "position=?,base=? WHERE gsspKey=?");
    query.bindValue(6, gsspTable.gsspKey);
    query.bindValue(0, gsspTable.gsspName);
    query.bindValue(1, gsspTable.geneName);
    query.bindValue(2, gsspTable.exonIndex);
    query.bindValue(3, gsspTable.rOrF);
    query.bindValue(4, gsspTable.position);
    query.bindValue(5, gsspTable.base);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Update error gsspTable";
    }
}

void readGsspTableTxtFile(const QString &gsspFile)
{
    QFile file(gsspFile);
    if(!file.exists())
    {
        qDebug()<<gsspFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 6)
        {
            qDebug()<<"read gsspFile line Error "<<line;
        }
        GsspTable gsspTable;
        gsspTable.gsspName = part.at(0).toAscii();
        gsspTable.geneName = part.at(1).toAscii();
        gsspTable.exonIndex = part.at(2).toInt();
        gsspTable.rOrF = part.at(3).toAscii();
        gsspTable.position = part.at(4).toInt();
        gsspTable.base = part.at(5).toAscii();
        gsspTable.gsspKey = QString(gsspTable.gsspName)+"_"+QString(gsspTable.geneName);
        insertGsspToGsspTable(gsspTable);
    }
    file.close();
}

void showGsspTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM gsspTable",gsspDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("gsspTable");
    view->show();
}


//fucntion
void getGeneNames(QStringList &geneNames)
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("SELECT geneName FROM geneTable");
    if(!isSuccess)
    {
        qDebug()<<"getGeneNames error";
    }
    else
    {
        while(query.next())
        {
            geneNames.push_back(query.value(0).toString());
        }
    }
}

void getGsspNames(QStringList &gsspNames)
{
    QSqlQuery query(gsspDatabase);
//    bool isSuccess = query.exec("SELECT gsspName FROM gsspTable ORDER BY gsspName DESC");
    bool isSuccess = query.exec("SELECT gsspName FROM gsspTable");
    if(!isSuccess)
    {
        qDebug()<<"getGsspNames error";
    }
    else
    {
        while(query.next())
        {
            gsspNames.push_back(query.value(0).toString());
        }
    }
}

void getGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR)
{
    QSqlQuery query(gsspDatabase);
//    bool isSuccess = query.exec("SELECT gsspName,exonIndex,rOrF FROM gsspTable ORDER BY gsspName DESC");
    bool isSuccess = query.exec("SELECT gsspName,exonIndex,rOrF FROM gsspTable");
    if(!isSuccess)
    {
        qDebug()<<"getGsspMapToExonAndFR error";
    }
    else
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

void getConsensusSequenceFromStaticDatabase(const QByteArray &geneName, QByteArray &geneSequence, int exonStartPos, int exonLength)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT geneSequence FROM geneTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getConsensusSequenceFromStaticDatabase error:"<<geneName;
    }
    else
    {
        while(query.next())
        {
            geneSequence = query.value(0).toByteArray().mid(exonStartPos, exonLength);
        }
    }
}

void getAlleleInfosFromStaticDatabase(const QByteArray &geneName, int exonStartPos, int alleleLength, int minExonIndex, int maxExonIndex, QVector<AlleleInfo> &alleleInfos, QSet<QString> &shieldAlleles)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName,alleleSequence,isRare,isIndel,noStar FROM alleleTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleInfosFromStaticDatabase error:"<<geneName;
    }
    else
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
                for(int i=minExonIndex; i<=maxExonIndex; i++)
                {
                    alleleInfo.starInfo.append(QString("*%1").arg(i));
                }
            }
            else
            {
                if(minExonIndex >= min && maxExonIndex<=max)
                {
                    alleleInfo.starInfo.append("-");
                }
                else
                {
                    for(int i=minExonIndex; i<min; i++)
                    {
                        alleleInfo.starInfo.append(QString("*%1").arg(i));
                    }
                    for(int i=maxExonIndex; i>max; i--)
                    {
                        alleleInfo.starInfo.append(QString("*%1").arg(i));
                    }
                }
            }
            alleleInfos.push_back(alleleInfo);
        }
    }
}

void getGsspAlleleInfosFromStaticDatabase(const QByteArray &geneName, int exonStartPos, int gsspLength, QVector<GsspAlleleInfo> &gsspAlleleInfos, const QString &gsspName)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName,alleleSequence FROM alleleTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();

    int gssp_pos;
    QString gssp_seq;
//    int i1=0,i2=0;
    getGsspPosAndSeqFromGsspDatabase(gsspName, gssp_pos, gssp_seq);

    if(!isSuccess)
    {
        qDebug()<<"getGsspInfosFromStaticDatabase error:"<<geneName;
    }
    else
    {
        while(query.next())
        {//i1++;
            GsspAlleleInfo gsspAlleleInfo;
            gsspAlleleInfo.alleleName = query.value(0).toString();
//            gsspAlleleInfo.alleleSequence = query.value(1).toByteArray().mid(exonStartPos, gsspLength);
            QByteArray alleleSequence = query.value(1).toByteArray();
            for(int i=0; i<gssp_seq.size(); i++)
            {
                if(alleleSequence[i+gssp_pos]!=gssp_seq[i].toAscii())
                    goto WHILE_LABLE;
            }
            gsspAlleleInfo.alleleSequence = alleleSequence.mid(exonStartPos, gsspLength);
            gsspAlleleInfos.push_back(gsspAlleleInfo);//i2++;
WHILE_LABLE:;
        }
    }
}
void extract_exon_info(const QByteArray &gene_name, int exon_index, ExonInfoS& exonInfo)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT geneSequence,exonPositionIndex FROM oldGeneTable WHERE geneName = ?");
    query.bindValue(0, gene_name);
    bool isSuccess = query.exec();
    if (!isSuccess)
        qDebug() << "extractExonInfo error";
    else
    {
        while(query.next())
        {
            exonInfo.exonStartPos = query.value(1).toString().split(":")[exon_index-1].toInt();
            exonInfo.exonEndPos = query.value(1).toString().split(":")[exon_index].toInt();
            exonInfo.consensusSeq = query.value(0).toByteArray().mid(exonInfo.exonStartPos,exonInfo.exonEndPos-exonInfo.exonStartPos);
        }
    }
}

void getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq, int alignStartPos, int alignLength)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleSequence FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleSequence error"<<alleleName;
    }
    else
    {
        while(query.next())
        {
            alleleSeq = query.value(0).toByteArray().mid(alignStartPos, alignLength);
        }
    }
}

void getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleSequence FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleSequence error"<<alleleName;
    }
    else
    {
        while(query.next())
        {
            alleleSeq = query.value(0).toByteArray();
        }
    }
}

void getGsspPosAndSeqFromGsspDatabase(const QString &gsspName, int &gsspPos, QString &gsspSeq)
{
    QSqlQuery query(gsspDatabase);
    query.prepare("SELECT position,base FROM gsspTable WHERE gsspName =?");
    query.bindValue(0, gsspName.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getGsspPosAndSeqFromGsspDatabase erorr"<<gsspName;
    }
    else
    {
        while(query.next())
        {
            gsspPos = query.value(0).toInt();
            gsspSeq = query.value(1).toString();
        }
    }
}

void getExonPositionIndexFromStaticDatabase(const QByteArray &geneName, QVector<int> &position)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT exonPositionIndex FROM geneTable WHERE geneName = ?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExonPositionIndexFromStaticDatabase error"<<geneName;
    }
    else
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

void getIndelInfoFromStaticDatabase(const QByteArray &alleleName, IndelInfo &indelInfo)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT isIndel,indelPostion,indelInfo FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getIndelInfoFromStaticDatabase error"<<alleleName;
    }
    else
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

void getAlleleNameListFromStaticDabase(const QByteArray &geneName, QStringList &alleleNames)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName FROM alleleTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleNameListFromStaticDabase error"<<geneName;
    }
    else
    {
        while(query.next())
        {
            alleleNames.push_back(query.value(0).toString());
        }
    }
}

void getAlleleNamesAndSeqsByGeneName(const QByteArray &geneName, QStringList &alleleNames, QStringList &alleleSeqs, QVector< QVector<int> > &misPositions)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleName,alleleSequence,misPosition FROM labAlignTable WHERE geneName=?");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleNameListFromStaticDabase error"<<geneName;
    }
    else
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

void getAlleleSequenceByAlleleName(const QByteArray &alleleName, QString &alleleSeq)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT alleleSequence FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getAlleleSequenceByAlleleName error"<<alleleName;
    }
    else
    {
        while(query.next())
        {
            alleleSeq = query.value(0).toString();
        }
    }

}

void getGsspTablesFromGsspDatabase(const QByteArray &geneName, int exon, QVector<GsspTable> &gsspTables)
{
    QSqlQuery query(gsspDatabase);
    query.prepare("SELECT gsspName,rOrF,position,base FROM gsspTable WHERE geneName =? AND exonIndex=?");
    query.bindValue(0, geneName);
    query.bindValue(1, exon);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getGsspTablesFromGsspDatabase error"<<geneName<<exon;
    }
    else
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


//exontrim
void createExonTrimTable()
{
    QSqlQuery query(staticDatabase);
    bool isSuccess = query.exec("CREATE TABLE exonTrimTable (etKey VARCHAR PRIMARY KEY,"
                                "geneName VARCHAR,"
                                "exonIndex VARCHAR,"
                                "fOrR VARCHAR,"
                                "exonStart VARCHAR,"
                                "exonEnd VARCHAR,"
                                "excludeLeft VARCHAR,"
                                "excludeRight VARCHAR)");
    if(!isSuccess)
    {
        qDebug()<<"create exonTrimTable error!";
    }
}
void insertExonTrimToExonTrimTable(const ExonTrimTable &exonTrimTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT etKey FROM exonTrimTable WHERE etKey=?");
    query.bindValue(0, exonTrimTable.etKey);
    query.exec();
    if(query.next())
    {
        updateExonTrim(exonTrimTable);
    }
    else
    {
        insertExonTrim(exonTrimTable);
    }
}

void readExonTrimTableTxtFile(const QString &exonTrimFile)
{
    QFile file(exonTrimFile);
    if(!file.exists())
    {
        qDebug()<<exonTrimFile<<" is not exists!";
        return;
    }
    file.open(QFile::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList part = line.split("\t");
        if(part.size() != 8)
        {
            qDebug()<<"read exonTrimFile line Error "<<line;
        }
        ExonTrimTable exonTrimTable;
        exonTrimTable.etKey=part.at(0);
        exonTrimTable.geneName =part.at(1);
        exonTrimTable.exonIndex =part.at(2);
        exonTrimTable.fOrR =part.at(3);
        exonTrimTable.exonStart =part.at(4);
        exonTrimTable.exonEnd =part.at(5);
        exonTrimTable.excludeLeft =part.at(6);
        exonTrimTable.excludeRight =part.at(7);
        insertExonTrimToExonTrimTable(exonTrimTable);
    }
    file.close();
}

void insertExonTrim(const ExonTrimTable &exonTrimTable)
{
    QSqlQuery query(staticDatabase);
    query.prepare("INSERT INTO exonTrimTable (etKey, geneName, "
                  "exonIndex, fOrR, exonStart, exonEnd, excludeLeft, excludeRight)"
                  "VALUES (?,?,?,?,?,?,?,?)");
    query.bindValue(0, exonTrimTable.etKey);
    query.bindValue(1, exonTrimTable.geneName);
    query.bindValue(2, exonTrimTable.exonIndex);
    query.bindValue(3, exonTrimTable.fOrR);
    query.bindValue(4, exonTrimTable.exonStart);
    query.bindValue(5, exonTrimTable.exonEnd);
    query.bindValue(6, exonTrimTable.excludeLeft);
    query.bindValue(7, exonTrimTable.excludeRight);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"Insert Error exonTrimTable";
    }
}

void updateExonTrim(const ExonTrimTable &exonTrimTable)
{
    QSqlQuery query(staticDatabase);
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

void getExonTrimListByGeneName(const QString &geneName, QVector<ExonTrimTable> &exonTrimTableList)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT * FROM exonTrimTable WHERE geneName=? ORDER BY etKey DESC");
    query.bindValue(0, geneName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExonTrimListByGeneName error:"<<geneName;
    }
    else
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

int getExcludeLeftPosition(const QString &key)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT exonStart,exonEnd,excludeLeft FROM exonTrimTable WHERE etKey=?");
    query.bindValue(0, key);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExcludeLeftPosition error:"<<key;
        return -1;
    }
    else
    {
        if(query.next())
        {
            int start=query.value(0).toInt();
            int end=query.value(1).toInt();
            int num=query.value(2).toInt();
            if(num<=0)
                return -1;
            if(num>end-start)
            {
                qDebug()<<"exclude Left Error"<<num<<key;
                return -1;
            }
            return start+num-1;
        }
    }
    return -1;
}

int getExcludeRightPostion(const QString &key)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT exonStart,exonEnd,excludeRight FROM exonTrimTable WHERE etKey=?");
    query.bindValue(0, key);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"getExcludeRightPostion error:"<<key;
        return -1;
    }
    else
    {
        if(query.next())
        {
            int start=query.value(0).toInt();
            int end=query.value(1).toInt();
            int num=query.value(2).toInt();
            if(num<=0)
                return -1;
            if(num>end-start)
            {
                qDebug()<<"exclude Right Error"<<num<<key;
                return -1;
            }
            return end-num;
        }
    }
    return -1;
}

bool isIndelInRange(const QByteArray &alleleName, int start, int end)
{
    QSqlQuery query(staticDatabase);
    query.prepare("SELECT isIndel,indelPostion FROM alleleTable WHERE alleleName=?");
    query.bindValue(0, alleleName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
        qDebug()<<"isIndelInRange error:"<<alleleName;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toInt()> 0 && query.value(1).toInt()>=start && query.value(1).toInt()<end)
                return true;
        }
    }
    return false;
}
