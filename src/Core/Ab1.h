#ifndef AB1_H
#define AB1_H

#include <QFile>
#include <QDataStream>
#include <QMultiHash>

typedef struct _tagAB1Tag
{
    qint32 name;              /* tag name */
    qint32 number;            /* tag number */
    qint16 elementtype;      /* element type code */
    qint16 elementsize;      /* element size */
    qint32 numelements;       /* number of elements */
    qint32 datasize;          /* data size */
    qint32 dataoffset;        /* data offset */
    qint32 datahandle;        /*reserve,unuse*/
} AB1Tag;

typedef struct _tagBaseo
{
    char basetype;   /* A T G C (sequences or stored in trace data) */
    int  pos;        /* position */
    int  signal;     /* signal strength value */
    int  qual;       /* quality value if sequence or else 0 */
} Baseo;

class Ab1
{
public:
    Ab1(const QString &str_path);
    ~Ab1();
    bool AnalysisFile();
    bool IsAb1File();
    int getNumBase();
    int getBaseOffset();
    int getBasePosOffset();
    int getQualOffset();
    int getNumTraceData();
    bool getFWO();
    float getBaseSpace();
    bool getTraceDataOffset(long *Offset);
    bool isAb1Good();
    bool getBaseSeq(long offset, long num, char* pseq);
    bool getBaseQual(long offset, long num, int *qualArry, int &maxQual);
    bool getBasePosition(long offset, long num, int *posArry);
    bool getTraceData(Baseo *baseG, Baseo *baseA, Baseo *baseT, Baseo *baseC,
                           long *offset, long num,  int &maxSignal);
    bool getBaseData(Baseo* base, long num, int *basqQual, char *baseSeq, int *basePos,
                          Baseo* baseG, Baseo* baseA, Baseo* baseT, Baseo* baseC);
    bool hasPeak_test(Baseo* base, int low, int high, int &pos);
    bool hasPeak(Baseo* base, int low, int high, int &pos);
    bool IsValidPeak(Baseo* base, int low, int high,int basePos,int signal);
    bool getPeak(Baseo* baseG, Baseo* baseA, Baseo* baseT, Baseo* baseC, Baseo* base,
                      int *basePos, long num);
    float signalRatio(Baseo *baseG, Baseo *baseA, Baseo *baseT, Baseo *baseC, Baseo *base,
                            int *basePos, long num);
    Baseo* reverseComplement(Baseo* base, int numBase, int numTrace, int flag);
private:
    QFile m_file;
    char m_cFWO[4];
    QMultiHash<qint32,AB1Tag> m_multiHash;
};

#endif // AB1_H
