#ifndef COMMONGSSPDATABASE_H
#define COMMONGSSPDATABASE_H
#include <QtCore>
struct CommonGsspTable
{
    QByteArray gsspName;
    QByteArray geneName;
    QByteArray exonIndex;
    QByteArray fOrR;
};
void createCommonGsspDatabase();
void createCommonGsspTable();
void readCommonGsspTableTxt(const QString &txtFile);
void insertCommonGsspTable(const CommonGsspTable &commonGsspTable);
void loadCommonGsspDatabase();
void showCommonGsspDatabase();
bool findExonAndRFByCommonGsspName(const QString &name, QString &exonIndex, QString &RF);

#endif // COMMONGSSPDATABASE_H
