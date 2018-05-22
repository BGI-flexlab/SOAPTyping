#ifndef _INTERFACE_H
#define _INTERFACE_H
#include <QtCore>
#include "semigloble.h"

typedef struct
{
    char *rawSequence;
    int  consensusStart;
    int  consensusEnd;
    char *consensusSequence;
} FileAlign;

typedef struct
{
    int leftLimit;
    int rightLimit;
    int *baseMatchConsensusPos;
    char *sampleAlign;
    int  isUnDefined;
} FileAlignResult;

typedef struct
{
    int exclude_left_num;
    int exclude_right_num;
    int consensus_start;
    int consensus_end;
    char *consensus;
    char *raw_seq;
} FileAlignNew;

typedef struct
{
    bool is_match;
    int left_cut;
    int right_cut;
    QByteArray consensus_alignment;
    QByteArray sample_alignment;
}FileAlignResultNew;



void regionShowAlign(FileAlign *fileAlign, FileAlignResult *result, bool is_gssp);
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
