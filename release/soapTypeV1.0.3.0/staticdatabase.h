#ifndef GETLAB_H
#define GETLAB_H
#include <QtCore>
//使用QByteArray相比Qstring时间相差为
//geneTable 2168 2387
//alleleTable 1127124 1226372 能够快点
//在插入之前做一下检查, 时间不会太久
struct  GeneTable
{
    QByteArray geneName;
    QByteArray geneSequence;
    int exonCount;
    QByteArray exonPostionIndex;
    QByteArray geneClasses;
    QByteArray availableExon;
    QByteArray version;
};

struct AlleleTable
{
    QByteArray alleleName;
    QByteArray alleleSequence;
    QByteArray geneName;
    int isRare;
    int isIndel;
    int classesNumber;
    int indelPosition;
    QByteArray indelInfo;
    QByteArray noStar;
};


struct GsspTable
{
    QString gsspKey;
    QByteArray gsspName;
    QByteArray geneName;
    int exonIndex;
    QByteArray rOrF;
    int position;
    QByteArray base;
};

struct LabAlignTable
{
    QByteArray alleleName;
    QByteArray alleleSequence;
    QByteArray geneName;
    QByteArray misPosition;
};

struct ExonAndRF
{
    QString exonIndex;
    QString rOrF;
};

struct AlleleInfo
{
    QString alleleName;
    QByteArray alleleSequence;
    QString isRare;
    int isIndel;
    QString starInfo;
};

struct GsspAlleleInfo
{
    QString alleleName;
    QByteArray alleleSequence;
};

struct ExonInfoS
{
    int exonStartPos;
    int exonEndPos;
    QByteArray consensusSeq;
};
struct IndelInfo
{
    int isIndel;
    int indelPostion;
    QString indelInfo;
};

struct ExonTrimTable
{
    QString etKey;
    QString geneName;
    QString exonIndex;
    QString fOrR;
    QString exonStart;
    QString exonEnd;
    QString excludeLeft;
    QString excludeRight;
};

void createStaticDatabase();
void createGeneTable();
void createOldGeneTable();
void createAlleleTable();
void createLabAlignTable();
int loadStaticDatabase();
void removeStaticDatabase();
void updateStaticDatabase(const QString &geneFile, const QString &oldGeneFile, const QString &alleleFile, const QString &labAlignFile, const QString &exonTrimFile);

void insertGeneToGeneTable(const GeneTable &geneTable);
void insertGeneTable(const GeneTable &geneTable);
void updateGeneTable(const GeneTable &geneTable);

void insertGeneToOldGeneTable(const GeneTable &geneTable);
void insertOldGeneTable(const GeneTable &geneTable);
void updateOldGeneTable(const GeneTable &geneTable);

void insertAlleleToAlleleTable(const AlleleTable &alleleTable);
void insertAlleleTable(const AlleleTable &alleleTable);
void updateAlleleTable(const AlleleTable &alleleTable);


void insertLabAlignToLabAlignTable(const LabAlignTable &labAlignTable);
void insertLabAlignTable(const LabAlignTable &labAlignTable);
void updateLabAlignTable(const LabAlignTable &labAlignTable);

void readGeneTableTxtFile(const QString &geneFile);
void readOldGeneTableTxtFile(const QString &geneFile);
void readAlleleTableTxtFile(const QString &alleleFile);
void readLabAlignTableTxtFile(const QString &labAlignFile);
void showGeneTable();
void showAlleleTable();
void showLabAlignTable();
void showExonTrimTable();

void createExonTrimTable();
void insertExonTrimToExonTrimTable(const ExonTrimTable &exonTrimTable);
void readExonTrimTableTxtFile(const QString &exonTrimFile);
void insertExonTrim(const ExonTrimTable &exonTrimTable);
void updateExonTrim(const ExonTrimTable &exonTrimTable);
void getExonTrimListByGeneName(const QString &geneName, QVector<ExonTrimTable> &exonTrimTableList);
int getExcludeLeftPosition(const QString &key);
int getExcludeRightPostion(const QString &key);

void createGsspDatabase();
void createGsspTable();
int loadGsspDatabase();
void removeGsspDatabase();
void updateGsspDatabase(const QString &gsspFile);
void insertGsspToGsspTable(const GsspTable &gsspTable);
void insertGsspTable(const GsspTable &gsspTable);
void updateGsspTable(const GsspTable &gsspTable);

void readGsspTableTxtFile(const QString &gsspFile);
void showGsspTable();


//fucntion
void getExonPositionIndexFromStaticDatabase(const QByteArray &geneName, QVector<int> &position);
void getGeneNames(QStringList &geneNames);
void getGsspNames(QStringList &gsspNames);
void getGsspMapToExonAndFR(QMap<QString, ExonAndRF> &mapToExonAndFR);
void getConsensusSequenceFromStaticDatabase(const QByteArray &geneName, QByteArray &geneSequence, int exonStartPos, int exonLength);

void getAlleleInfosFromStaticDatabase(const QByteArray &geneName, int exonStartPos, int alleleLength, int minExonIndex, int maxExonIndex, QVector<AlleleInfo> &alleleInfos,  QSet<QString> &shieldAlleles);
void getGsspAlleleInfosFromStaticDatabase(const QByteArray &geneName, int exonStartPos, int gsspLength, QVector<GsspAlleleInfo> &gsspAlleleInfos, const QString &gsspName);
void extract_exon_info(const QByteArray &gene_name, int exon_index, ExonInfoS& exonInfo);
void getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq, int alignStartPos, int alignLength);
void getAlleleSequence(const QString &alleleName, QByteArray &alleleSeq);
void getGsspPosAndSeqFromGsspDatabase(const QString &gsspName, int &gsspPos, QString &gsspSeq);
void getIndelInfoFromStaticDatabase(const QByteArray &alleleName, IndelInfo &indelInfo);

void getAlleleNameListFromStaticDabase(const QByteArray &geneName, QStringList &alleleNames);
void getAlleleNamesAndSeqsByGeneName(const QByteArray &geneName, QStringList &alleleNames, QStringList &alleleSeqs, QVector< QVector<int> > &misPositions);
void getAlleleSequenceByAlleleName(const QByteArray &alleleName, QString &alleleSeq);

void getGsspTablesFromGsspDatabase(const QByteArray &geneName, int exon, QVector<GsspTable> &gsspTables);

bool isIndelInRange(const QByteArray &alleleName, int start, int end);
#endif // GETLAB_H
