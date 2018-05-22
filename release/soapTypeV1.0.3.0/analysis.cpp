#include "analysis.h"

int ThreadNum = 2;
bool openThread = true;
bool ignoreIndel = true;
bool analysisSample(const QByteArray &sampleName)
{
    ExonInfo exonInfo;
    QVector<FileInfo> fileInfos;
    QVector<FileInfo> gsspFileInfos;

    int fileFlag = getFileInfoFromRealTimeDatabase(sampleName, fileInfos, exonInfo);
    int gsspFileFlag = getGsspFileInfoFromRealTimeDatabase(sampleName, gsspFileInfos, exonInfo);
    SampleInfo sampleInfo;
    if(fileInfos.size()==0 && gsspFileInfos.size()==0)
    {
        if(gsspFileFlag == 1 || fileFlag==1)
        {
            sampleInfo.sampleName = sampleName;
            sampleInfo.analysisType = UNMATCH;
            sampleInfo.maxExonIndex = 0;
            sampleInfo.minExonIndex = 0;
            sampleInfo.exonStartPos = 0;
            sampleInfo.exonEndPos = 0;
            insertSampleInfoToRealTimeDatabase(sampleInfo);
        }
        return false;
    }
    bool isGood;
    sampleInfo.sampleName = sampleName;
    sampleInfo.minExonIndex = exonInfo.minExonIndex;
    sampleInfo.maxExonIndex = exonInfo.maxExonIndex;
    sampleInfo.exonStartPos = exonInfo.exonStartPosition;
    sampleInfo.exonEndPos = exonInfo.exonEndPostion;
    sampleInfo.forwardSequence.fill('-', sampleInfo.exonEndPos - sampleInfo.exonStartPos);
    sampleInfo.reverseSequence.fill('-', sampleInfo.exonEndPos - sampleInfo.exonStartPos);
    QStringList editPosList;
    QStringList frDifferenceList;
    QStringList pcDifferenceList;
    QStringList frUnequlList;

    for(int i=0; i<fileInfos.size(); i++)
    {
        FileInfo &fileInfo = fileInfos[i];
        sampleInfo.geneName = fileInfo.geneName;
        if(fileInfo.rOrF.contains("F"))
        {
            mergeFileSequenceToSampleSequence(fileInfo.sequence.data(), sampleInfo.forwardSequence.data()+fileInfo.exonStartPos-sampleInfo.exonStartPos);
        }
        else
        {
            mergeFileSequenceToSampleSequence(fileInfo.sequence.data(), sampleInfo.reverseSequence.data()+fileInfo.exonStartPos-sampleInfo.exonStartPos);
        }
        if(fileInfo.isGood)
        {
            isGood = true;
        }
        for(QSet<int>::iterator it=fileInfo.editPostion.begin(); it!=fileInfo.editPostion.end(); it++)
        {
            editPosList.push_back(QString::number(*it));
        }
    }
    QMap<int, QString> typeResult;
    int result = 0;
    if(fileInfos.size()>0)
    {
        sampleInfo.patternSequence.fill('-', sampleInfo.exonEndPos - sampleInfo.exonStartPos);
        mergeForwardAndReverseToPattern(sampleInfo.forwardSequence.data(), sampleInfo.reverseSequence.data(), sampleInfo.patternSequence.data(), sampleInfo.exonStartPos, frDifferenceList, frUnequlList);
        getConsensusSequenceFromStaticDatabase(sampleInfo.geneName, sampleInfo.consensusSequence, sampleInfo.exonStartPos, sampleInfo.exonEndPos-sampleInfo.exonStartPos);
        comparePatternWithConsensus(sampleInfo.patternSequence.data(), sampleInfo.consensusSequence.data(), sampleInfo.exonStartPos, pcDifferenceList);
        QSet<QString> shieldAlleles;
        getShieldAllelesFromDatabase(sampleInfo.sampleName, shieldAlleles);
        result = comparePatternWithAllele(sampleInfo.patternSequence.data(), sampleInfo.geneName, sampleInfo.exonStartPos, sampleInfo.minExonIndex, sampleInfo.maxExonIndex, typeResult, shieldAlleles);
        changeAlignMapToAlignString(typeResult, sampleInfo.typeResult);
        sampleInfo.mismatchBetweenPC = pcDifferenceList.join(":");
        sampleInfo.mismatchBetweenFR = frDifferenceList.join(":");
        sampleInfo.mmismatchBetweenFR = frUnequlList.join(":");
        sampleInfo.editPostion = editPosList.join(":");
    }

    if(gsspFileInfos.size()>0)
    {
        if(sampleInfo.geneName.isEmpty())
        {
            sampleInfo.geneName = gsspFileInfos.at(0).geneName;
            getConsensusSequenceFromStaticDatabase(sampleInfo.geneName, sampleInfo.consensusSequence, sampleInfo.exonStartPos, sampleInfo.exonEndPos-sampleInfo.exonStartPos);
        }
//        QSet<QString> zeroCombinedAlleles;
//        QString zeroString;
        QMap<int, QString> zeroResult;
        getZeroResultFromTypeResult(typeResult, zeroResult);
        for(int i=0; i<gsspFileInfos.size(); i++)
        {
            if(gsspFileInfos.at(i).isGood)
                isGood = true;
            QString gsspTypeResult;
            QString gsspFilterResult;
            QSet<QString> zeroGssp;
            int re = compareGsspWithAlleles(gsspFileInfos.at(i).gsspName, gsspFileInfos.at(i).sequence.data(), sampleInfo.geneName, gsspFileInfos.at(i).exonStartPos, gsspTypeResult, zeroGssp);
            if(result<re)
                result=re;
//            getCombinedAlleles(zeroCombinedAlleles, zeroGssp, i);
            getFilterResult(gsspFilterResult, typeResult, zeroGssp, "/0");
            getCombinedResult(zeroResult, zeroGssp);
//            if(gsspFileInfos.size()==1)
//            {
//                sampleInfo.combinedResult = gsspFilterResult;
//            }
            updateGsspFileResultToRealTimeDatabase(gsspFileInfos.at(i).fileName, re, gsspTypeResult, gsspFilterResult);
//            zeroString.push_back("/0");
        }
        if(gsspFileInfos.size()>0)
        {
//            getFilterResult(sampleInfo.combinedResult, typeResult, zeroCombinedAlleles, zeroString);
            changeAlignMapToAlignString(zeroResult, sampleInfo.combinedResult);
        }
    }
    if(result<MISMATCH)
    {
        if(!isGood)
            result = MATCHBAD;
    }
    if(fileFlag == 1 || gsspFileFlag ==1)
    {
        result = UNMATCH;
    }
    sampleInfo.analysisType = result;
    insertSampleInfoToRealTimeDatabase(sampleInfo);
    return true;
}

void mergeFileSequenceToSampleSequence(char *fileSequence, char *sampleSequence)
{
    int size = (strlen(sampleSequence)<strlen(fileSequence))?strlen(sampleSequence):strlen(fileSequence);
    for(int i=0; i<size; i++)
    {
        if(sampleSequence[i]=='-')
        {
            sampleSequence[i] = fileSequence[i];
            continue;
        }
        sampleSequence[i] = mergeBases(sampleSequence[i], fileSequence[i]);
    }
}
void mergeForwardAndReverseToPattern(char *forwardSeq, char *reverseSeq, char *patternSeq, int exonStartPos, QStringList &frDifferenceList, QStringList &frUnequlList)
{
    int size = strlen(patternSeq);
    for(int i=0; i<size; i++)
    {
        if(forwardSeq[i] == '-')
        {
            patternSeq[i] = reverseSeq[i];
            continue;
        }
        if(reverseSeq[i] == '-')
        {
            patternSeq[i] = forwardSeq[i];
            continue;
        }
        if(forwardSeq[i] == reverseSeq[i])
        {
            patternSeq[i] = forwardSeq[i];
            if(patternSeq[i]=='n')
            {
                frDifferenceList.push_back(QString::number(i+exonStartPos));
            }
            continue;
        }

        if(forwardSeq[i]=='.')
        {
            patternSeq[i]=reverseSeq[i];
        }
        else if(reverseSeq[i]=='.')
        {
            patternSeq[i]=forwardSeq[i];
        }
        else
        {
            patternSeq[i] = mergeBases(forwardSeq[i], reverseSeq[i]);
        }

        if(patternSeq[i] == 'n' || (patternSeq[i] != forwardSeq[i] && patternSeq[i] != reverseSeq[i]))
        {
            frDifferenceList.push_back(QString::number(i+exonStartPos));
        }
        else
        {
            frUnequlList.push_back((QString::number(i+exonStartPos)));
        }
    }
}

void comparePatternWithConsensus(char *patternSeq, char *consensusSeq, int exonStartPos, QStringList &pcDifferenceList)
{
    int size = strlen(patternSeq) < strlen(consensusSeq) ? strlen(patternSeq):strlen(consensusSeq);
    for(int i=0; i<size; i++)
    {
        if(patternSeq[i] == '-' || patternSeq[i] == 'n')
        {
            continue;
        }
        if(!isEqualPC(consensusSeq[i], patternSeq[i]))
        {
            pcDifferenceList.push_back(QString::number(i+exonStartPos));
        }
    }
}
bool isEqualPC(char consensus, char pattern)
{
    unsigned int a = formatMerge(consensus);
    unsigned int b = formatMerge(pattern);
    return (a|b)==a;
}

char mergeBases(char A, char B)
{
    unsigned int a = formatMerge(A);
    unsigned int b = formatMerge(B);
    return reFormatMerge(a|b);
}

unsigned int formatMerge(char A)
{
    switch(A)
    {
    case 'A': return 0x1;
    case 'G': return 0x2;
    case 'T': return 0x4;
    case 'C': return 0x8;
    case 'R': return 0x3;
    case 'Y': return 0xc;
    case 'W': return 0x5;
    case 'S': return 0xa;
    case 'M': return 0x9;
    case 'K': return 0x6;
    case 'B': return 0xe;
    case 'D': return 0x7;
    case 'H': return 0xd;
    case 'V': return 0xb;
    case 'N': return 0xf;
    case '-': return 0x0;
    case '.': return 0x1f;
//    case 'A': return 0x1;
//    case 'G': return 0x2;
//    case 'T': return 0x4;
//    case 'C': return 0x8;
//    case 'R': return 0x3;
//    case 'Y': return 0xc;
//    case 'W': return 0x5;
//    case 'S': return 0xa;
//    case 'M': return 0x9;
//    case 'K': return 0x6;
//    case '-': return 0x0;
//    case '.': return 0x1f;
//    default:
//        return 0xf;
    }
    return 0xf;
}

char reFormatMerge(unsigned int a)
{
    switch(a)
    {
    case 0x0: return '-';
    case 0x1: return 'A';
    case 0x2: return 'G';
    case 0x4: return 'T';
    case 0x8: return 'C';
    case 0x3: return 'R';
    case 0xc: return 'Y';
    case 0x5: return 'W';
    case 0xa: return 'S';
    case 0x9: return 'M';
    case 0x6: return 'K';
    case 0x1f: return '.';
    default:
        return 'n';
    }
    return 'n';
}

int comparePatternWithAllele(char *patternSeq, const QByteArray &geneName, int exonStartPos, int minExonIndex, int maxExonIndex, QMap<int, QString> &typeResult, QSet<QString> &sheildAlleles)
{
    int length = strlen(patternSeq);
    if(length<=0)
        return UNMATCH;
    QVector<AlleleInfo> alleleInfos;
    getAlleleInfosFromStaticDatabase(geneName, exonStartPos, length, minExonIndex, maxExonIndex, alleleInfos, sheildAlleles);
//    if(ignoreIndel)
//    {
//        removIndelAlleles(alleleInfos);
//    }

    if(alleleInfos.size()>100)
    {
        removeAlleleInfoByPattern(patternSeq, alleleInfos);
    }
    if(alleleInfos.size()<=0)
    {
        return MISMATCH;
    }
    QVector< QVector<int> > differentPosInfos;
    getDifferentPositionInfo(patternSeq, alleleInfos, differentPosInfos);
    int alignResult = comparePatternWithAlleleByThread(patternSeq, alleleInfos, differentPosInfos, typeResult);
    return alignResult;
}
int comparePatternWithAlleleByThread(char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector< QVector<int> > &differentPosInfos, QMap<int, QString> &typeResult)
{
    bool isRare=true, isFull=false;
    if(ThreadNum > 1)
    {
        int *splitPos = new int[ThreadNum+1];
        splitPos[0] = alleleInfos.size();
        splitPos[ThreadNum]=0;
        for(int i=1; i<ThreadNum; i++)
        {
            splitPos[i] = int ((1.0-sqrt((double)i)/sqrt((double)ThreadNum)) * splitPos[0]);
        }
        QMap<int, QString> *result = new QMap<int, QString> [ThreadNum];
        QFuture<bool> *thread = new QFuture<bool> [ThreadNum];
        for(int i=0; i<ThreadNum; i++)
        {
            PairStartEnd pair;
            pair.startIndex = splitPos[i+1];
            pair.endIndex = splitPos[i];
            thread[i] = QtConcurrent::run(runComparePA, patternSeq, alleleInfos, differentPosInfos, &result[i], pair);

        }
        for(int i=0; i<ThreadNum; i++)
        {
            thread[i].waitForFinished();
        }
        for(int i=0; i<ThreadNum; i++)
        {
            for(QMap<int, QString>::iterator it=result[i].begin(); it!=result[i].end(); it++)
            {
                typeResult.insertMulti(it.key(), it.value());
                if(it.key()==0)
                {
                    isFull = true;
                    if(!it.value().contains("r"))
                    {
                        isRare = false;
                    }
                }
            }
            result[i].clear();
        }
        delete []splitPos;
        delete []result;
        delete []thread;
    }
    else
    {
        PairStartEnd pair;
        pair.startIndex = 0;
        pair.endIndex = alleleInfos.size();
        QMap<int, QString> remap;
        runComparePA(patternSeq, alleleInfos, differentPosInfos, &remap, pair);
        for(QMap<int, QString>::iterator it=remap.begin(); it!=remap.end(); it++)
        {
            typeResult.insertMulti(it.key(), it.value());
            if(it.key()==0)
            {
                isFull = true;
                if(!it.value().contains("r"))
                {
                    isRare = false;
                }
            }
        }
        remap.clear();
    }

    if(isFull)
    {
        if(isRare)
        {
            return MATCHRARE;
        }
        return MATCHTOTAL;
    }
    return MISMATCH;
}
bool runComparePA(const char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector< QVector<int> > &differentPosInfos, QMap<int, QString> *typeResult, PairStartEnd pair)
{
    int startIndex = pair.startIndex;
    int endIndex = pair.endIndex;
    int size = alleleInfos.size();
    int rLimit = 500;
    int maxTop = 10000;
    for(int i=startIndex; i<endIndex; i++)
    {
        for(int j=i; j<size; j++)
        {
            QString alignInfo;
            int mis = compareByAlleleInfoAndDiffPos(patternSeq, alleleInfos.at(i), alleleInfos.at(j), differentPosInfos.at(i), differentPosInfos.at(j), alignInfo);
            typeResult->insertMulti(mis, alignInfo);
            if(typeResult->size()>=maxTop)
            {
                QMap<int, QString>::iterator itp1, itp2;
                itp1 = typeResult->end();
                itp2 = typeResult->end();
                itp2--;
                for(int i=maxTop; i>rLimit; i--)
                {
                    itp1 = itp2;
                    itp2--;
                    typeResult->erase(itp1);
                }
            }
        }
    }
    return true;
}
int compareByAlleleInfoAndDiffPos(const char *patternSeq, const AlleleInfo &alleleInfoi, const AlleleInfo &alleleInfoj, const QVector<int> &diffInfoi, const QVector<int> &diffInfoj, QString &alignInfo)
{
    const char *alleleSeqi = alleleInfoi.alleleSequence.data();
    const char *alleleSeqj = alleleInfoj.alleleSequence.data();
    alignInfo.append(QString("%1,%2,%3,%4%5%6%7").arg(alleleInfoi.alleleName).arg(alleleInfoj.alleleName).arg(alleleInfoi.isIndel+alleleInfoj.isIndel).arg(alleleInfoi.isRare).arg(alleleInfoj.isRare).arg(alleleInfoi.starInfo).arg(alleleInfoj.starInfo));
    int i=0, j=0, endi=diffInfoi.size(), endj=diffInfoj.size(), mis=0;
    //同一位点不可能同时为星号
    //因为星号不标记为错配
    while(i<endi && j<endj)
    {
        if(diffInfoi[i]<diffInfoj[j])
        {
            if(alleleSeqj[diffInfoi[i]]=='*')
            {
                if(!isEqualPC(patternSeq[diffInfoi[i]], alleleSeqi[diffInfoi[i]]))
                    mis++;
            }
            else
            {
                mis++;
            }
            i++;
        }
        else if(diffInfoj[j]<diffInfoi[i])
        {

            if(alleleSeqi[diffInfoj[j]]=='*')
            {
                if(!isEqualPC(patternSeq[diffInfoj[j]], alleleSeqj[diffInfoj[j]]))
                {
                    mis++;
                }
            }
            else
            {
            mis++;
            }
            j++;
        }
        else
        {
            if(mergeBases(alleleSeqi[diffInfoi[i]], alleleSeqj[diffInfoj[j]]) != patternSeq[diffInfoi[i]])
            {
                mis++;
            }
            i++;j++;
        }
    }
    while(i<endi)
    {
        if(alleleSeqj[diffInfoi[i]]=='*')
        {
            if(!isEqualPC(patternSeq[diffInfoi[i]], alleleSeqi[diffInfoi[i]]))
                mis++;
        }
        else
        {
            mis++;
        }
        i++;
    }
    while(j<endj)
    {
        if(alleleSeqi[diffInfoj[j]]=='*')
        {
            if(!isEqualPC(patternSeq[diffInfoj[j]], alleleSeqj[diffInfoj[j]]))
            {
                mis++;
            }
        }
        else
        {
        mis++;
        }
        j++;
    }
    return mis;
}

void getDifferentPositionInfo(char *patternSeq, QVector<AlleleInfo> &alleleInfos, QVector<QVector<int> > &differentPosInfos)
{
    int size = alleleInfos.size();
    int seqSize = strlen(patternSeq);
    for(int i=0; i<size; i++)
    {
        const char *alleleSeq = alleleInfos.at(i).alleleSequence.data();
        QVector<int> pos;
        for(int j=0; j<seqSize; j++)
        {
            if(alleleSeq[j]!='*' && patternSeq[j]!='-' && patternSeq[j]!='n'  && patternSeq[j] != alleleSeq[j])
            {
                pos.push_back(j);
            }
        }
        differentPosInfos.push_back(pos);
    }
}

void removeAlleleInfoByPattern(char *patternSeq, QVector<AlleleInfo> &alleleInfos)
{
    QVector<int> pos;
    int size = strlen(patternSeq);
    for(int i=0; i<size; i++)
    {
        switch(patternSeq[i])
        {
        case 'A':
            break;
        case 'G':
            break;
        case 'T':
            break;
        case 'C':
            break;
        case '-':
            break;
        case 'n':
            break;
        case '.':
            break;
        default:
            pos.push_back(i);
            break;
        }
    }
    if(pos.size()<=0)
        return;
    int posSize = pos.size();
    for(int i=alleleInfos.size()-1; i>=0; i--)
    {
        const char *alleleSeq = alleleInfos.at(i).alleleSequence.data();
        for(int j=0; j<posSize; j++)
        {
            if(alleleSeq[pos[j]]=='*' || alleleSeq[pos[j]]=='.')
            {
                continue;
            }
            if(!isEqualPC(patternSeq[pos[j]], alleleSeq[pos[j]]))
            {
                alleleInfos.remove(i);
                break;
            }
        }
    }
}

void changeAlignMapToAlignString(QMap<int, QString> &typeResult, QString &result)
{
    int limit = 500;
    int i=0;
    for(QMap<int, QString>::iterator it=typeResult.begin(); it!=typeResult.end(); it++)
    {
        result.push_back(QString("%1,%2;").arg(it.key()).arg(it.value()));
        if(++i>=limit)
            break;
    }
    return;
}

int compareGsspWithAlleles(const QByteArray &gsspName, const char *gsspSequence, const QByteArray &geneName, int exonStartPos, QString &typeResult, QSet<QString> &zeroResult)
{
    int gsspL = strlen(gsspSequence);
    QVector<GsspAlleleInfo> gsspAlleleInfos;
    getGsspAlleleInfosFromStaticDatabase(geneName, exonStartPos, gsspL, gsspAlleleInfos, gsspName);
//    removeUnRelationAllelesByGssp(gsspName, gsspAlleleInfos, exonStartPos, gsspL);

    //没有屏蔽indel
    QMap<int, QString> misInfo;
    for(int i=gsspAlleleInfos.size()-1; i>=0; i--)
    {
        const char *seq = gsspAlleleInfos.at(i).alleleSequence.data();
        int mis=0;
        for(int j=0; j<gsspL; j++)
        {
            //1.3.0定为缺失时, 不算错误.
            if(gsspSequence[j]!=seq[j] && gsspSequence[j]!='-' && gsspSequence[j]!='.')
            {
                mis++;
            }
        }
        misInfo.insertMulti(mis, gsspAlleleInfos.at(i).alleleName);
        if(mis==0)
        {
            zeroResult.insert(gsspAlleleInfos.at(i).alleleName);
        }
    }
    changeAlignMapToAlignString(misInfo, typeResult);
    if(zeroResult.size() > 0)
    {
        return MATCHTOTAL;
    }
    return MISMATCH;
}

void getCombinedAlleles(QSet<QString> &zeroCombinedAlleles, QSet<QString> &zeroGssp, int i)
{
    if(zeroGssp.empty())
    {
        zeroCombinedAlleles.clear();
        return;
    }
    if(i==0)
    {
        if(zeroGssp.size() > 0)
        {
            zeroCombinedAlleles = zeroGssp;
        }
    }
    else
    {

        if(zeroCombinedAlleles.size() > 0)
        {
            QSet<QString> newSet;

            QSet<QString>::iterator it = zeroCombinedAlleles.begin();
            while(it != zeroCombinedAlleles.end())
            {
                if(zeroGssp.contains(*it))
                {
                    newSet.insert(*it);

                }
                it++;
            }
            zeroCombinedAlleles.clear();zeroCombinedAlleles=newSet;
        }
    }
}

void getFilterResult(QString &result, QMap<int, QString> &typeResult, QSet<QString> &zeroAlleles, const QString &name)
{
    if(typeResult.size() <= 0 || zeroAlleles.size() <= 0)
        return;

    int limit = 500;
    int i = 0;
    for(QMap<int ,QString>::iterator it = typeResult.begin(); it!= typeResult.end(); it++)
    {
        QStringList line = it.value().split(",");
        if(zeroAlleles.contains(line.at(0)) || zeroAlleles.contains(line.at(1)))
        {
            result.push_back(QString("%1%2,%3;").arg(it.key()).arg(name).arg(it.value()));
            if(++i>=limit)
                break;
        }
    }
}

void shieldAllele(const QString &sampleName, const QString &alleles)
{
    insertShieldAllelesToSampleTable(sampleName.toAscii(), alleles);
}

void setOpenThread(bool isOpen)
{
    openThread = isOpen;
}

bool getOpenTrhead()
{
    return openThread;
}

void removeUnRelationAllelesByGssp(QString gsspName, QVector<GsspAlleleInfo> &gsspAlleleInfos, int start, int length)
{
    int gsspPos;
    QString gsspSeq;
    getGsspPosAndSeqFromGsspDatabase(gsspName, gsspPos, gsspSeq);

    int pos = gsspPos - 0;

    for(int i=0; i<gsspSeq.size(); i++)
    {
        char A = gsspSeq.at(i).toAscii();
        pos+=i;
        for(int j=gsspAlleleInfos.size()-1; j>=0; j--)
        {
            if(gsspAlleleInfos.at(j).alleleSequence.at(pos) != A)
            {
                gsspAlleleInfos.remove(j);
            }
        }
    }
    for(int i=0; i<gsspAlleleInfos.size(); i++)
    {
        gsspAlleleInfos[i].alleleSequence = gsspAlleleInfos[i].alleleSequence.mid(start, length);
    }
}

void removIndelAlleles(QVector<AlleleInfo> &alleleInfos)
{
    for(int i=alleleInfos.size()-1; i>=0; i--)
    {
        if(alleleInfos.at(i).isIndel != 0)
        {
            alleleInfos.remove(i);
        }
    }
}

void changeThreadNum(int num)
{
    if(num<=0)
        num=1;
    ThreadNum = num;
}

void getCombinedResult(QMap<int, QString> &typeResult, QSet<QString> &zeroAlleles)
{
    if(typeResult.size() <= 0)
        return;
    if(zeroAlleles.size()==0)
    {
        typeResult.clear();
        return;
    }

    QMap<int, QString> newMap;
    for(QMap<int ,QString>::iterator it = typeResult.begin(); it!= typeResult.end(); it++)
    {
        QStringList line = it.value().split(",");
        if(zeroAlleles.contains(line.at(0)) || zeroAlleles.contains(line.at(1)))
        {
            newMap.insertMulti(it.key(), it.value());
        }
    }
    typeResult.clear();
    for(QMap<int ,QString>::iterator it = newMap.begin(); it!= newMap.end(); it++)
    {
        typeResult.insertMulti(it.key(), it.value());
    }
    newMap.clear();
}

void getZeroResultFromTypeResult(QMap<int, QString> &typeResult, QMap<int, QString> &zeroResult)
{
    QMap<int, QString> newMap;
    for(QMap<int ,QString>::iterator it = typeResult.begin(); it!= typeResult.end(); it++)
    {
        if(it.key()==0)
        {
            newMap.insertMulti(it.key(), it.value());
        }
        else
        {
            break;
        }
    }
    for(QMap<int ,QString>::iterator it = newMap.begin(); it!= newMap.end(); it++)
    {
        zeroResult.insertMulti(it.key(), it.value());
    }
}
