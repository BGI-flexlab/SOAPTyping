/*
 * =====================================================================================
 *
 *       Filename:  interface.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2013-1-8 16:59:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "equal.h"
#include "QtCore"

#define MIN_EDGE 20
#define MIN_MIS 15
#define MIN_LENGTH 50
#define MIN_CUT_MIS 10
#define MIN_MATCH_LENGTH 50

/*void regionShowAlign(FileAlign *fileAlign, FileAlignResult *result, bool is_gssp)
{

    align nw;
    nw.r1=NULL;
    nw.r2=NULL;
    int i,k = 0;
    int index;
    int left = 0, right = 0;
    char *consensus = fileAlign->consensusSequence;
    char *sample = fileAlign->rawSequence;
    int start = fileAlign->consensusStart;
    result->baseMatchConsensusPos = (int*) malloc(sizeof(int) * (strlen(sample)));
    int length = strlen(sample);

    semigloblalalign(consensus, sample, &nw);
    if (nw.start2 <= MIN_EDGE && !is_gssp)
        left += MIN_EDGE;

    if (abs(nw.stop2 - length) <= MIN_EDGE && !is_gssp)
    {
        right = length - MIN_EDGE;
        sample[right] = '\0';
    }

    if(left != 0 || right != 0)
    {
        semigloblalalign(consensus, sample+left, &nw);
    }

    while(nw.errors > MIN_MIS && length - left >= MIN_LENGTH)
    {
        left += 1;
        semigloblalalign(consensus, sample+left, &nw);
    }

    int strSize = strlen(nw.r1);

    result->sampleAlign = (char*) malloc(sizeof(char) * (strlen(consensus)+1));

    result->leftLimit = nw.start2 + left;
    result->rightLimit = nw.stop2 + left;

    index = nw.start2 + left;

    if(nw.errors > MIN_MIS || (length-left<=MIN_LENGTH && nw.errors > 10))
        result->isUnDefined = 1;
    else
        result->isUnDefined = 0;

    //qDebug()<<result->isUnDefined;

    //    for(i = 0; i < strSize; i++)
    //        if(nw.r1[i] != '-')
    //            result->sampleAlign[k++] = nw.r2[i];
    //    result->sampleAlign[k] = '\0';

    int rightP;
    for( rightP = strSize-1; rightP >= 0; rightP--)
    {
        if(nw.r2[rightP] != '.')
            break;
    }


    for(i = 0; i < nw.start1; i++)
        result->sampleAlign[k++] = '-';
    for(i = nw.start1; i <= rightP; i++)
        if(nw.r1[i] != '.')
            result->sampleAlign[k++] = nw.r2[i];
    for(i = k; i < strlen(consensus); i++)
        result->sampleAlign[k++] = '-';
    result->sampleAlign[k] = '\0';

    // start += nw.start1;


    for(i = 0; i < nw.start2 + left; i++)
        result->baseMatchConsensusPos[i] = -1;
    for(i = nw.start2; index <= nw.stop2 + left; i++)
    {
        if(nw.r1[i] != '.' && nw.r2[i] != '.')
            result->baseMatchConsensusPos[index++] = start++;
        else if(nw.r1[i] == '.')
            result->baseMatchConsensusPos[index++] = -1;
        else if(nw.r2[i] == '.')
            start++;
    }
    for(i = index; i < length; i++)
        result->baseMatchConsensusPos[i] = -1;

    nw.clear();
    return;
}*/

void new_align_function(FileAlignNew &file_align_new, FileAlignResult &result, bool auto_cut)
{
    if(strlen(file_align_new.consensus)==0 || file_align_new.exclude_left_num+file_align_new.exclude_right_num >= strlen(file_align_new.raw_seq)){
        result.isUnDefined =1;
        return;
    }
    align al;
    al.r1 = NULL;
    al.r2 = NULL;
    FileAlignResultNew tmp_result;
    tmp_result.left_cut = file_align_new.exclude_left_num;
    tmp_result.right_cut = file_align_new.exclude_right_num;
    int orange_length = strlen(file_align_new.raw_seq);
    int align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;

    semigloblalalign_new(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
    while(!optimize_boundary(&al, &tmp_result, auto_cut)){
        align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;
        semigloblalalign_new(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
    }

    result.sampleAlign = (char*) malloc(sizeof(char) * (strlen(file_align_new.consensus)+1));
    result.baseMatchConsensusPos = (int*) malloc(sizeof(int) * (strlen(file_align_new.raw_seq)));
    if(tmp_result.is_match)
    {
        result.isUnDefined = 0;
        result.leftLimit = tmp_result.left_cut;
        result.rightLimit = strlen(file_align_new.raw_seq) -tmp_result.right_cut;
        int start=0, end=tmp_result.sample_alignment.size();
        for(int i=0; i<end; i++){
            if(tmp_result.sample_alignment[i]=='.'){
                result.sampleAlign[i]='-';
                start++;
            }else{
                break;
            }
        }

        int c_index = strlen(file_align_new.consensus);
        for(int i=end-1; i>=start; i--)
        {
            if(tmp_result.sample_alignment[i]=='.'){
                result.sampleAlign[--c_index]='-';
                end--;
            }else{
                break;
            }
        }
        int index=0;
        for(int i=0; i<tmp_result.left_cut; i++){
            result.baseMatchConsensusPos[index]=-1;
            index++;
        }
        c_index = start;
        for(int i=start; i<end; i++){
            if(tmp_result.consensus_alignment[i] != '.'){
                result.sampleAlign[c_index]=tmp_result.sample_alignment[i];
                if(tmp_result.sample_alignment[i]!='.'){
                    result.baseMatchConsensusPos[index]=c_index+file_align_new.consensus_start;
                    index++;
                }
                c_index++;
            }else{
                result.baseMatchConsensusPos[index]=-1;
                index++;
            }
        }
        for(int i=0; i<tmp_result.right_cut;i++){
            result.baseMatchConsensusPos[index]=-1;
            index++;
        }
        result.sampleAlign[strlen(file_align_new.consensus)]='\0';
    }
    else
    {
        result.isUnDefined = 1;
    }
    al.clear();
}


bool optimize_boundary(align *nw, FileAlignResultNew *result, bool auto_cut)
{
    if(nw->r1==NULL || nw->r2==NULL){
        result->is_match = false;
        return true;
    }

    char *ref = nw->r1;
    char *seq = nw->r2;
    int start = nw->start2, end=nw->stop2;
    int length = strlen(seq);
    if(nw->stop1<length)
    {
        result->right_cut+=length-nw->stop1;
        nw->r1[nw->stop1]='\0';
        nw->r2[nw->stop1]='\0';
        end = nw->stop1;
    }

    if(nw->start1>0)
    {
        result->left_cut+=nw->start1;
        ref = ref+nw->start1;
        seq = seq+nw->start1;
        end -= nw->start1;
    }

    //    length = strlen(seq);
    length = end - start;
    //改，去掉了
    //    if(length<MIN_MATCH_LENGTH)
    //    {
    //        result->is_match = false;
    //        return true;
    //    }

    int *mis= new int[length];
    mis[0]=0;
    int mis_index=0,max_leng=0;

    int left_mis=0, right_mis=0, half_pos = (start+end)/2;
    for(int i=start; i<end; i++)
    {
        if(seq[i]==ref[i] || can_merge(ref[i], seq[i]))
        {
            mis[mis_index]++;
        }
        else
        {
            if(max_leng<mis[mis_index])
                max_leng = mis[mis_index];
            mis_index++;
            mis[mis_index]=1;

            if(i<=half_pos)
                left_mis++;
            else
                right_mis++;
        }
    }

    int total_mis = left_mis+right_mis;
    if(max_leng<mis[mis_index]){
        max_leng = mis[mis_index];
    }

    //    if(max_leng <= MIN_MATCH_LENGTH)
    //    {
    //        result->is_match = false;
    //        delete []mis;
    //        return true;
    //    }

    if(total_mis > MIN_MIS)
    {
        if(left_mis<=right_mis)
            result->right_cut++;
        else
            result->left_cut++;
        delete []mis;
        return false;
    }


    // >= 50
    if(!auto_cut)
    {
        result->consensus_alignment = QByteArray(ref);
        result->sample_alignment = QByteArray(seq);
        result->is_match = true;
        delete[] mis;
        return true;
    }

    int left_cut=0,right_cut=0;
    for(int i=0; i<=mis_index; i++)
    {
        if(mis[i]>=MIN_CUT_MIS)
        {
            if(!can_merge(ref[start+left_cut], seq[start+left_cut]))
                left_cut++;
            break;
        }
        left_cut+=mis[i];
    }

    for(int i=mis_index; i>=0; i--)
    {
        if(mis[i]>=MIN_CUT_MIS)
            break;
        right_cut+=mis[i];
    }

    delete[] mis;
    if(left_cut==0 && right_cut==0)
    {
        result->consensus_alignment = QByteArray(ref);
        result->sample_alignment = QByteArray(seq);
        result->is_match = true;
        return true;
    }


    result->left_cut += left_cut;
    for(int i=start; i<start+left_cut; i++)
    {
        if(seq[i]=='.')
            result->left_cut--;
    }

    result->right_cut += right_cut;
    for(int i=end-right_cut; i<end; i++)
    {
        if(seq[i]=='.')
            result->right_cut--;
    }
    return false;
}
