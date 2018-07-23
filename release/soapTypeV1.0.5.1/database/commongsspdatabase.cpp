#include "commongsspdatabase.h"
//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qsqlite)
#include <QtSql>
#include <QtGui>

const QString commonFilePath = "Database/commonGsspDatabase";
QSqlDatabase commonGsspDatabase;

void createCommonGsspDatabase()
{
    commonGsspDatabase = QSqlDatabase::addDatabase("QSQLITE","forth");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    commonGsspDatabase.setDatabaseName(commonFilePath);
    commonGsspDatabase.open();
    QSqlQuery query(commonGsspDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
//20180608close qDebug()<<"create commonGsspDatabase error!";
    }
}

void loadCommonGsspDatabase()
{
    commonGsspDatabase = QSqlDatabase::addDatabase("QSQLITE","forth");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    commonGsspDatabase.setDatabaseName(commonFilePath);
    commonGsspDatabase.open();
    QSqlQuery query(commonGsspDatabase);
    bool isSuccess = query.exec("PRAGMA foreign_keys = ON");
    if(!isSuccess)
    {
//20180608close qDebug()<<"load commonGsspDatabase error!";
    }
}

void createCommonGsspTable()
{
    QSqlQuery query(commonGsspDatabase);
    bool isSuccess = query.exec("CREATE TABLE commonGsspTable (gsspName VARCHAR PRIMARY KEY,"
                                "geneName VARCHAR,"
                                "exonIndex VARCHAR,"
                                "fOrR VARCHAR)");
    if(!isSuccess)
    {
//20180608close qDebug()<<"create commonGsspTable error";
    }
}

void insertCommonGssp(const CommonGsspTable &commonGsspTable){
    QSqlQuery query(commonGsspDatabase);
    query.prepare("INSERT INTO commonGsspTable (gsspName, geneName, exonIndex, fOrR)"
                  "VALUES (?,?,?,?)");
    query.bindValue(0, commonGsspTable.gsspName);
    query.bindValue(1, commonGsspTable.geneName);
    query.bindValue(2, commonGsspTable.exonIndex);
    query.bindValue(3, commonGsspTable.fOrR);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
//20180608close qDebug()<<"Insert error commonGsspDatabase";
    }
}

void updateCommonGssp(const CommonGsspTable &commonGsspTable){
    QSqlQuery query(commonGsspDatabase);
    query.prepare("UPDATE commonGsspTable SET geneName=?, exonIndex=?, fOrR=? WHERE gsspName=?");
    query.bindValue(0, commonGsspTable.geneName);
    query.bindValue(1, commonGsspTable.exonIndex);
    query.bindValue(2, commonGsspTable.fOrR);
    query.bindValue(3, commonGsspTable.gsspName);
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
//20180608close qDebug()<<"Insert error commonGsspDatabase";
    }
}

void insertCommonGsspTable(const CommonGsspTable &commonGsspTable)
{
    QSqlQuery query(commonGsspDatabase);
    query.prepare("SELECT gsspName FROM commonGsspTable WHERE gsspName=?");
    query.bindValue(0, commonGsspTable.gsspName);
    query.exec();
    if(query.next())
    {
        updateCommonGssp(commonGsspTable);
    }
    else
    {
        insertCommonGssp(commonGsspTable);
    }
}


void readCommonGsspTableTxt(const QString &txtFile)
{
    QFile file(txtFile);
    if(!file.exists())
    {
//20180608close qDebug()<<txtFile<<" is not exists!";
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
//20180608close qDebug()<<"read commonGsspTableFile line Error "<<line;
        }
        CommonGsspTable commonGsspTable;
        commonGsspTable.gsspName = part.at(0).toAscii();
        commonGsspTable.geneName = part.at(1).toAscii();
        commonGsspTable.exonIndex = part.at(2).toAscii();
        commonGsspTable.fOrR = part.at(3).toAscii();
        insertCommonGsspTable(commonGsspTable);
    }
    file.close();
}

void showCommonGsspDatabase()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM commonGsspTable",commonGsspDatabase);
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("commonGsspTable");
    view->show();
}

bool findExonAndRFByCommonGsspName(const QString &name, QString &exonIndex, QString &RF)
{
    QSqlQuery query(commonGsspDatabase);
    /**
      * 只是根据gssp名称查询F/R和外显子序号，所以geneName一列没啥影响
      */
    query.prepare("SELECT exonIndex,fOrR FROM commonGsspTable WHERE gsspName=?");
    query.bindValue(0, name.toAscii());
    bool isSuccess = query.exec();
    if(!isSuccess)
    {
//20180608close qDebug()<<"findExonAndRFByCommonGsspName error:"<<name;
        return false;
    }
    else
    {
        if(query.next())
        {
            exonIndex = query.value(0).toString();
            RF = query.value(1).toString();
            return true;
        }
    }
    return false;
}
