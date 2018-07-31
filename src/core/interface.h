#ifndef _INTERFACE_H
#define _INTERFACE_H
#include <QtCore>
#include "semigloble.h"

//typedef struct
//{
//    char *rawSequence;
//    int  consensusStart;
//    int  consensusEnd;
//    char *consensusSequence;
//} FileAlign;

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



//void regionShowAlign(FileAlign *fileAlign, FileAlignResult *result, bool is_gssp);
void new_align_function(FileAlignNew &file_align_new, FileAlignResult &result, bool auto_cut);
bool optimize_boundary(align *nw, FileAlignResultNew *result, bool auto_cut);

/*typedef struct
{
    char *forwardStrandSeq;
    char *reverseStrandSeq;
    char *consensus;
    char *exonInfo;
    char *geneName;
    int minExonNum;
    int maxExonNum;
    int startPosition;
    int endPosition;
} SequencesInfoOfLocus;

typedef struct
{
    char *pattern;
    char *consensusResult;
    char *patternMismatchInfo;
    int  *consensusMismatchInfo;
} FRSequencesMergeInfo;

typedef struct
{
    ResultLabel resultLabel;
    MatchInfo *matchinfo;
} TypeCompareResult;
*/


#endif
