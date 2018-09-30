#ifndef ALL_BASE_STRUCT_H
#define ALL_BASE_STRUCT_H

#include <QString>
#include <QSet>
#include <QVector>
#include <QMap>

enum MarkType
{
    OWNED = 0,
    PENDING,
    REVIEWED,
    APPROVED
};
enum AnalysisType
{
    MATCHTOTAL = 0,
    MATCHRARE,
    MATCHBAD,
    MISMATCH,
    UNMATCH
};

struct ExonAndRF
{
    QString exonIndex;      //外显子序号
    QString rOrF;           //正序or反序
};

struct ExonInfoS
{
    int exonStartPos;       //外显子开始位置
    int exonEndPos;         //外显子结束位置
    QByteArray consensusSeq;   //一致性序列
};

struct ExonInfo
{
    int minExonIndex;
    int maxExonIndex;
    int exonStartPosition;
    int exonEndPostion;
    ExonInfo()
    {
        minExonIndex = 100;
        maxExonIndex = 0;
        exonStartPosition = 2000;
        exonEndPostion = -1;
    }
};

typedef struct
{
    int leftLimit; //原始序列匹配的起始位置（等同于left_cut）
    int rightLimit; //原始序列匹配的结束位置（等同于原始序列总长度-right_cut）
    int *baseMatchConsensusPos; //原始序列长度的数组，从leftLimit到rightLimit保存匹配一致序列的位置（从数据库查询的裁剪起始位置）
    char *sampleAlign;//对数组sample_alignment进行处理，如果是.替换成-，然后依次复制到sampleAlign数组
    int  isUnDefined;
} FileAlignResult;

typedef struct
{
    int exclude_left_num; //左边剔除的个数
    int exclude_right_num; //右边剔除的个数
    int consensus_start; //从数据库查询的，裁剪起始位置
    int consensus_end; //从数据库查询的，裁剪结束位置
    char *consensus; //从数据库查询的，经过裁剪的一致性序列
    char *raw_seq; //从ab文件获取的原始序列
} FileAlignNew;

typedef struct
{
    bool is_match; //是否匹配
    int left_cut; //左边剔除的个数
    int right_cut; //右边剔除的个数
    QByteArray consensus_alignment; //从数据库查询的，经过裁剪的一致性序列
    QByteArray sample_alignment; //从ab文件获取的原始序列，经过最长公共字符序列匹配，以及边界优化得到的序列
}FileAlignResultNew;

typedef struct
{
    char *r1;
    char *r2;
    int errors;
    int start1;
    int start2;
    int stop1;
    int stop2;

    void clear()
    {
        if(r1!=NULL)
        {
            free(r1);
            r1=NULL;
        }
        if(r2!=NULL)
        {
            free(r2);
            r2=NULL;
        }
        errors = 0;
        start1 = 0;
        start2 = 0;
        stop1 = 0;
        stop2 = 0;
    }
} align;

struct FileInfo
{
    int exonStartPos;
    int exonEndPos;
    int isGood;
    int isUnMatch;
    QByteArray fileName;
    QByteArray geneName;
    QByteArray rOrF;
    QByteArray gsspName;
    QByteArray sequence;
    QSet<int> editPostion;
};

struct SampleInfo
{
    QString sampleName;
    QString geneName;
    int analysisType;
    int minExonIndex;
    int maxExonIndex;
    int exonStartPos;
    int exonEndPos;
    QByteArray consensusSequence;
    QByteArray forwardSequence;
    QByteArray reverseSequence;
    QByteArray patternSequence;
    QString mismatchBetweenPC;
    QString mismatchBetweenFR;
    QString mmismatchBetweenFR;
    QString editPostion;
    QString typeResult;
    QString combinedResult;
};

struct AlleleInfo
{
    QString alleleName;
    QByteArray alleleSequence;
    QString isRare;
    int isIndel;
    QString starInfo;
};

struct PairStartEnd
{
    int startIndex;
    int endIndex;
};

struct GsspAlleleInfo
{
    QString alleleName;
    QByteArray alleleSequence;
};

typedef struct FileTreeInfo
{
    bool isGssp;
    int isGood;
    int analysisType;
    int exonIndex;
    QString rOrF;
    QString gsspName;
    QString fileName;
    FileTreeInfo()
    {
        isGssp = false;
        isGood = 0;
        analysisType = 0;
        exonIndex = 0;
        rOrF = "";
        gsspName = "";
        fileName = "";
    }
}FileTreeInfo_t;

struct SampleTreeInfo_t
{
    int analysisType;
    int markType;
    QString sampleName;
    QString geneName;
    QVector<FileTreeInfo_t> treeinfo;
};

struct ExonNavigatorInfo
{
    int minExonIndex;
    int maxExonIndex;
    QVector<int> vec_pcMis;//样品简并序列和一致性序列不兼容
    QVector<int> vec_frMis; //正反序列不兼容
    QVector<int> vec_frUnEqual; //正反序列兼容但不同
    QVector<int> vec_editPos;//编辑过的碱基
};

struct BaseAlignGsspInfo
{
    int gsspFileAlignStartPos;
    QString gsspName;
    int gsspPostion;
    QString gsspSeq;
    QString gsspFileSeq;
};

struct BaseAlignSampleInfo
{
    int alignStartPos;
    int alignEndPos;
    QString consensusSeq;
    QString forwardSeq;
    QString reverseSeq;
    QString patternSeq;
    QSet<int> pcMisMatchPostion;
    QSet<int> frMisMatchPostion;
    QSet<int> editPostion;
    QMap<QString, BaseAlignGsspInfo> gsspInfoMap;
    void clear()
    {
        alignStartPos = 0;
        alignEndPos = 0;
        consensusSeq.resize(0);
        forwardSeq.resize(0);
        reverseSeq.resize(0);
        patternSeq.resize(0);
        pcMisMatchPostion.clear();
        frMisMatchPostion.clear();
        editPostion.clear();
        gsspInfoMap.clear();
    }
};

struct IndelInfo
{
    int isIndel;
    int indelPostion;
    QString indelInfo;
};

struct AllelePair
{
    QString allele1;
    QString allele2;
};

struct GsspTable
{
    QString gsspKey;
    QString gsspName;
    QString geneName;
    int exonIndex;
    QString rOrF;
    int position;
    QByteArray base;
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

#endif // ALL_BASE_STRUCT_H

