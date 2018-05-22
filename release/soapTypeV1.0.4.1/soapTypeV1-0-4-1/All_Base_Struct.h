#ifndef ALL_BASE_STRUCT_H
#define ALL_BASE_STRUCT_H
#include"ab1.h"
#include"QString"
#include<qvector.h>
#include <QMetaType>
#include"QMessageBox"
#include"QFile"
#include"QTextStream"

typedef struct
{
    char        basetype;    	// A T G C (sequences or stored in trace data)
    int        	pos;        	// position
    //int         gradient;      	// signal strength value
    int         qual;       	// quality value if sequence or else 0
    int         Matchpos;		// 比对到一致性序列的位置，-1表示没匹配上
    // 加一下与consensus比对的位置
    QString         signalA;
    QString         signalT;
    QString         signalC;
    QString         signalG;
} BaseInfor;

typedef struct
{
    QString    basetype;
    int     pos;
} ModifyData;

typedef struct
{
    int maxSgnal;
    int maxQuality;
} MaxValue;

typedef struct
{
    int pos;
    int signal;
    double relative;

}Each_Base_Signal;
typedef struct
{
    int Consen_Pos;
    char base_type;
}Base_EditInfo;

struct FileInfomation
{
    bool isgssp;
    QString rOrF;
    QString samplename;
    QString genegname;
    int exonnum;
    bool isUndefined;
    //QString genename;
};

typedef struct
{
    int minrelat;
    int maxrelat;
    double maxsiganl;
    int startindex;
    int endindex;
    int maxsignalpos;
    int startpos;
    int endpos;
    short scroe;
    char base;
}Peak;
typedef struct
{
    Peak onepeak;
    int startindex;
}hypeak;



typedef struct
{
    QVector< Peak> PeaksVector;
    int Pos;
    int  matchPos;
    double percent;
    short score;
    char base;
    short maxone;

}Peakinfo;


typedef struct
{
    char type;
    int pos;
}analyse;
typedef struct                                                           //	画图基本信息结构体
{
    QVector <int> leftLimits;						                     //	保存每个文件左屏蔽位点
    QVector <int> rightLimits;                                           //	保存每个文件右屏蔽位点
    QVector <QString> fileNames;                                         //	用来画图的文件名vector
    QVector <MaxValue> filemax;
    QVector <QVector <BaseInfor> >base;                                  //	Baseo结构的base信息vector

    QVector <QVector <Each_Base_Signal> >baseG;                                     //	Baseo结构的baseG信息vector
    QVector <QVector <Each_Base_Signal> >baseA;                                     //	Baseo结构的baseA信息vector
    QVector <QVector <Each_Base_Signal> >baseT;                                     //	Baseo结构的baseT信息vector
    QVector <QVector <Each_Base_Signal> >baseC;                                     //	Baseo结构的baseC信息vector
    QVector <QVector <Base_EditInfo>  >edit_info;

    QVector <int > excludeleft;
    QVector <int > excludeRight;
    QVector <FileInfomation> infor;
    QVector <QVector <Peak> >  PeaksA;
    QVector <QVector <Peak> >  PeaksT;
    QVector <QVector <Peak> >  PeaksC;
    QVector <QVector <Peak> >  PeaksG;
    QVector <QVector <Peakinfo> > Peaksinform  ;
    QVector <int > moves;
    QVector <double> avgsignal;
    int currentfile;

    //QVector <int > movePos;
} RegionShowData;




typedef struct                                                           // 修改碱基信息结构体
{
    QString 	filename;                                                // 修改的文件名
    int 	    pos;							                         // 修改的位置信息
    QString 	previousBaseType;                                        // 修改之前的碱基型
    QString	    currentBaseType;                                         // 修改之后的碱基型
} ChangeInfor;




typedef struct                                                           // 屏蔽信息结构体
{
    int         leftBasePos;                                             // 左侧屏蔽位点
    int         rightBasePos;                                            // 右侧屏蔽位点
    QString     filename;                                                // 屏蔽文件名
} ExcludeData;

typedef struct
{
    double filesxExpand;
    double  filesyExpand;
    bool  addordecreaseOne;
    int curedHigth;
    int currentfile;
    int    rightMenuX;

} SaveData;
/*
struct SignalInfo
{
    bool isUndefined;   //若是没有定义, 则外显子没有错配, 型别没有列表,
    bool isCombined;  //只有型别表有信息, 比对表还不清楚
    bool isGsspFilter; //型别表显示不一样. 比对表也是
    bool isGssp; //型别表..
    int exonIndex;
    QString sampleName; //样品名, 若换, 其他模块肯定会换
    QString rOrF;
    QString geneName;
    QString fileName;
    SignalInfo()
    {
        isUndefined = false;
        isCombined = false;
        isGsspFilter = false;
        isGssp = false;
    }
};*/

struct SignalInfo
{
    bool isUndefined;   //若是没有定义, 则外显子没有错配, 型别没有列表,
    bool isCombined;  //只有型别表有信息, 比对表还不清楚
    bool isGsspFilter; //型别表显示不一样. 比对表也是
    bool isGssp; //型别表..
    short exonIndex;
    QString sampleName; //样品名, 若换, 其他模块肯定会换
    QString rOrF;
    QString geneName;
    QString fileName;
    SignalInfo()
    {
        isUndefined = false;
        isCombined = false;
        isGsspFilter = false;
        isGssp = false;
        exonIndex=-1;
        sampleName="";
        rOrF="";
        geneName="";
        fileName="";
    }

} ;


typedef struct
{
    QString fileName;
    QString sampleName;
    QString filePath;
    bool isExtraFile;
    QString geneName;
    int exonIndex;
    QString rOrF;
    int exonStartPos;
    int exonEndPos;
    QString usefulSequence;
    QString baseSequence;
    QString basePostion;
    QString baseQuality;
    int baseNumber;
    QString baseASignal;
    QString baseTSignal;
    QString baseGSignal;
    QString baseCSignal;
    int signalNumber;
    int maxSgnal;
    int maxQuality;
    double averageBaseWidth;
    bool isGood;
    int alignResult;
    int alignStartPos;
    int alignEndPos;
    QString alignInfo;
    int excludeleft;
    int excludeRight ;
    QString editInfo;
    double avgsignal;
    bool isgssp;


}  FileInfo_ly;




struct FileTable
{
    QByteArray fileName;
    QByteArray sampleName;
    QString filePath;
    int isExtraFile;
    QByteArray geneName;
    int exonIndex;
    QByteArray rOrF;
    int exonStartPos;
    int exonEndPos;
    QByteArray usefulSequence;
    QByteArray baseSequence;
    QString basePostion;
    QString baseQuality;
    int baseNumber;
    QString baseASignal;
    QString baseTSignal;
    QString baseGSignal;
    QString baseCSignal;
    int signalNumber;
    int maxSignal;
    int maxQuality;
    float averageBaseWidth;
    int isGood;
    int alignResult;
    int alignStartPos;
    int alignEndPos;
    QString alignInfo;
    int excludeLeft;
    int excludeRight;
    QString editInfo;
    float avgsignal;
    FileTable()
    {
        isExtraFile = 0;
        averageBaseWidth = 0.0;
        excludeLeft = -1;
        excludeRight = -1;
    }
};
Q_DECLARE_METATYPE (FileTable);

struct GsspFileTable
{
    QByteArray fileName;
    QByteArray sampleName;
    QString filePath;
    QByteArray gsspName;
    QByteArray geneName;
    int exonIndex;
    QByteArray rOrF;
    int exonStartPos;
    int exonEndPos;
    QByteArray usefulSequence;
    QByteArray baseSequence;
    QString basePostion;
    QString baseQuality;
    int baseNumber;
    QString baseASignal;
    QString baseTSignal;
    QString baseGSignal;
    QString baseCSignal;
    int signalNumber;
    int maxSignal;
    int maxQuality;
    float averageBaseWidth;
    int isGood;
    int alignResult;
    int alignStartPos;
    int alignEndPos;
    QByteArray alignInfo;
    int excludeLeft;
    int excludeRight;
    QString editInfo;
    QString typeResult;
    QString filterResult;
    GsspFileTable()
    {
        averageBaseWidth = 0.0;
        excludeLeft = -1;
        excludeRight = -1;
    }
};
Q_DECLARE_METATYPE(GsspFileTable);

class ErrorWrite{
public:
static void  errorocur(QString error)
 {
     QFile data("usebyme.aa");
     if (!data.open(QFile::WriteOnly | QFile::Text|QFile::Append))
     {
         QMessageBox :: warning(0 , " ERROR" , "software broken" );
         return;
     }
     QTextStream out(&data);//QDataStream
     out << error<<"\r\n";
     data.flush();
     data.close();
 }
};

#endif // ALL_BASE_STRUCT_H
