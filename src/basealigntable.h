#ifndef BASEALIGNTABLE_H
#define BASEALIGNTABLE_H
#include <QtCore>
#include <QtGui>
#include "analysis.h"
namespace Ui {
class BaseAlignTableWidget;
}
//struct BaseAlignGsspInfo
//{
//    int gsspFileAlignStartPos;
//    QString gsspName;
//    int gsspPostion;
//    QString gsspSeq;
//    QString gsspFileSeq;
//};

//struct BaseAlignSampleInfo
//{
//    int alignStartPos;
//    int alignEndPos;
//    QString consensusSeq;
//    QString forwardSeq;
//    QString reverseSeq;
//    QString patternSeq;
//    QSet<int> pcMisMatchPostion;
//    QSet<int> frMisMatchPostion;
//    QSet<int> editPostion;
//    QMap<QString, BaseAlignGsspInfo> gsspInfoMap;
//    void clear()
//    {
//        alignStartPos = 0;
//        alignEndPos = 0;
//        consensusSeq.resize(0);
//        forwardSeq.resize(0);
//        reverseSeq.resize(0);
//        patternSeq.resize(0);
//        pcMisMatchPostion.clear();
//        frMisMatchPostion.clear();
//        editPostion.clear();
//        gsspInfoMap.clear();
//    }
//};


//数据库
void getTypeAlignResult(char *result, char*pattern, char *alleleSeq1, char *alleleSeq2, QSet<int> &misMatch, int alignStart1, int alignStart2);

class BaseAlignTable : public QTableWidget
{
    Q_OBJECT
public:
    BaseAlignTable(QWidget * parent=0);
    ~BaseAlignTable();
signals:
    void signalFocusPosition(int basePosition, int exonIndex, int leftDistance);
    void signalTypeMisMatchPosition(QSet<int> &typeMisMatchPositions, int type);
    //void signalSelectType(QString &allele1, QString &allele2);用于part2;
public slots:
    void slotFocusPosition(int basePosition, int exonIndex, int leftDistance);
    void slotFileChanged(SignalInfo &signalInfo, int type); //type 1强制刷新, 0不强制
    void slotAllelePairChanged(QString &allele1, QString &allele2);
private slots:
    void slotSentBasePos(QTableWidgetItem*);
    void slotSliderPressed();
    void slotSliderReleased();
    void onSliderMoved(int pos);
private:
    void setBaseAlignTableDefault();
    void setBaseAlignTableSampleData();
    void setBaseAlignTableTypeData(int type);
    void clearBaseAlignTableSampleItem();
    void clearBaseAlignTableTypeItem();
    void getTableHead(QStringList &head, int length, int start);
    int getExonNum(int basePos);
private:
    SignalInfo signalInfo_;
    QVector<int> exonPosition_;//0:73:343....
    int rowNum_;
    int colNum_;
    int startColumn_;
    int endColumn_;
    QStringList defaultHead_;
    QString allele1Name_;
    QString allele2Name_;
    BaseAlignSampleInfo baseAlignSampleInfo_;
private:
    Ui::BaseAlignTableWidget *ui;
};
#endif // BASEALIGNTABLE_H
