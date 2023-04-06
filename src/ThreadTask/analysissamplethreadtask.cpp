#include "analysissamplethreadtask.h"
#include <QDebug>
#include <QThread>
#include <QFuture>
#include <QtConcurrent>
#include <QSet>
#include "Core/core.h"
#include "DataBase/soaptypingdb.h"
const int MAX_THREAD_NUM = 1;
const int I_ROWNUM = 800;
AnalysisSampleThreadTask::AnalysisSampleThreadTask(const QString &str_sample):m_sample(str_sample)
{

}

AnalysisSampleThreadTask::~AnalysisSampleThreadTask()
{
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
        sampleSequence[i] = Core::GetInstance()->mergeBases(sampleSequence[i], fileSequence[i]);
    }
}

void mergeForwardAndReverseToPattern(char *forwardSeq, char *reverseSeq, char *patternSeq,
                                     int exonStartPos, QStringList &frDifferenceList, QStringList &frUnequlList)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
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
            if(patternSeq[i]=='N')
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
            patternSeq[i] = Core::GetInstance()->mergeBases(forwardSeq[i], reverseSeq[i]);
        }

        if(patternSeq[i] == 'N' || (patternSeq[i] != forwardSeq[i] && patternSeq[i] != reverseSeq[i]))
        {
            frDifferenceList.push_back(QString::number(i+exonStartPos));//指不兼容
        }
        else
        {
            frUnequlList.push_back((QString::number(i+exonStartPos)));//指兼容但不同
        }
    }
}

bool isEqualPC(char A, char B) //A包含B
{
    return Core::GetInstance()->isEqualPC(A, B);
}

void comparePatternWithConsensus(char *patternSeq, char *consensusSeq, int exonStartPos, QStringList &pcDifferenceList)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    int size = strlen(patternSeq) < strlen(consensusSeq) ? strlen(patternSeq):strlen(consensusSeq);
    for(int i=0; i<size; i++)
    {
        if(patternSeq[i] == '-' || patternSeq[i] == 'N')
        {
            continue;
        }
        if(!isEqualPC(consensusSeq[i], patternSeq[i]))
        {
            pcDifferenceList.push_back(QString::number(i+exonStartPos));
        }
    }
}

void removeAlleleInfoByPatternNew(char *patternSeq, QVector<AlleleInfo> &alleleInfos)
{
    int size = strlen(patternSeq), alleleSize = alleleInfos.size();
    QMap<int, int> tmpNomalPos;
    QMap<int, int> tmpStarPos;
    int starCount = 0, normalCount = 0, tmpMis = 0, tmpCount=0;

    for(int i=0; i<alleleSize; i++){
        int mis = 0;
        const char *alleleSeq = alleleInfos.at(i).alleleSequence.data();
        if(strlen(alleleSeq) < size)
        {
            qDebug()<<alleleInfos.at(i).alleleName;
            continue;
        }
        for(int j=0; j<size; j++){

//            if(alleleSeq[j]=='*' ||patternSeq[j]=='-' || alleleSeq[j]=='.' || patternSeq[j]=='N' || patternSeq[j]=='.'){
//                continue;
//            }
            if(alleleSeq[j]=='*' ||patternSeq[j]=='-' || patternSeq[j]=='N') continue;
            if(alleleSeq[j]=='.' || patternSeq[j]=='.')
            {
                mis++;
                continue;
            }
            if(!isEqualPC(patternSeq[j], alleleSeq[j]))
            {
                mis++;
            }
        }
       // qDebug()<<alleleInfos.at(i).alleleName<<" mis = "<<mis;
        if(alleleInfos.at(i).starInfo.contains("*")){
            starCount++;
            tmpStarPos.insertMulti(mis, i);
        }else{
            normalCount++;
            tmpNomalPos.insertMulti(mis, i);
        }
    }

    QSet<int> tmpset;//缓存留下的allele
    //取最好的前200个不带有*的序列,最多不超过400，废掉了
    //取前20%最好的，最多不超多50%
    //cut_best and cut_limit
    int cut_best = normalCount*0.2;
    if (cut_best<200){
        cut_best = 200;
    }

    int cut_limit = normalCount*0.5;
    if (cut_limit<500){
        cut_limit = 500;
    }
    for(QMap<int, int>::iterator it=tmpNomalPos.begin(); it!=tmpNomalPos.end(); it++){
        tmpCount++;
       // qDebug()<<"normal it.key = "<<it.key()<<" it.value = "<<alleleInfos.at(it.value()).alleleName<<" Count = "<<tmpMis;
        if(tmpCount>cut_limit || tmpCount>cut_best && it.key()>tmpMis){
                break;
        }else{
            tmpMis = it.key();
        }
        tmpset.insert(it.value());
    }
    tmpCount =0;tmpMis =0;
    //取最好的前200个带有*的序列,最多不超过400,废掉了
    //取前20%最好的，最多不超多50%
    cut_best = starCount*0.2;
    if (cut_best<200){
        cut_best = 200;
    }
    cut_limit = starCount*0.5;
    if (cut_limit<500){
        cut_limit = 500;
    }
    for(QMap<int, int>::iterator it=tmpStarPos.begin(); it!=tmpStarPos.end(); it++){
        tmpCount++;
        if(tmpCount>cut_limit || tmpCount>cut_best && it.key()>tmpMis){
                break;
        }else{
            tmpMis = it.key();
        }
      //  qDebug()<<"star it.key = "<<it.key()<<" it.value = "<<alleleInfos.at(it.value()).alleleName<<" Count = "<<tmpMis;

        tmpset.insert(it.value());
    }


    for(int i=alleleSize-1; i>=0; i--){
        if(!tmpset.contains(i)){
            alleleInfos.remove(i);
        }
    }
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
            if(alleleSeq[j]!='*' && patternSeq[j]!='-' && patternSeq[j]!='N'  && patternSeq[j] != alleleSeq[j])
            {
                pos.push_back(j);
            }
        }
        differentPosInfos.push_back(pos);
    }
}

int compareByAlleleInfoAndDiffPos_new(const char *patternSeq, const AlleleInfo &alleleInfoi,
                                  const AlleleInfo &alleleInfoj, const QVector<int> &diffInfoi,
                                  const QVector<int> &diffInfoj, QSet<int> &difpos)
{
    const char *alleleSeqi = alleleInfoi.alleleSequence.data();
    const char *alleleSeqj = alleleInfoj.alleleSequence.data();
//    alignInfo.append(QString("%1,%2,%3,%4%5%6%7").arg(alleleInfoi.alleleName)
//                     .arg(alleleInfoj.alleleName).arg(alleleInfoi.isIndel+alleleInfoj.isIndel)
//                     .arg(alleleInfoi.isRare).arg(alleleInfoj.isRare)
//                     .arg(alleleInfoi.starInfo).arg(alleleInfoj.starInfo));
    difpos.clear();

    foreach(int pos, diffInfoi)
    {
        difpos.insert(pos);
    }

    foreach(int pos, diffInfoj)
    {
        difpos.insert(pos);
    }

    int mis = 0;
    foreach(int pos, difpos)
    {
        if(alleleSeqi[pos] == alleleSeqj[pos])//型别对碱基相同，但是和patternSeq不同
        {
            mis++;
        }
        else
        {
            if(alleleSeqi[pos] == '*')
            {
                if(!isEqualPC(patternSeq[pos], alleleSeqj[pos]))
                {
                    mis++;
                }
                continue;
            }
            if(alleleSeqj[pos] == '*')
            {
                if(!isEqualPC(patternSeq[pos], alleleSeqi[pos]))
                {
                    mis++;
                }
                continue;
            }

            bool i_ret = isEqualPC(patternSeq[pos], alleleSeqi[pos]);
            bool j_ret = isEqualPC(patternSeq[pos], alleleSeqj[pos]);

            if(!i_ret || !j_ret)
            {
                mis++;
            }
        }
    }
    return  mis;
}

int compareByAlleleInfoAndDiffPos(const char *patternSeq, const AlleleInfo &alleleInfoi,
                                  const AlleleInfo &alleleInfoj, const QVector<int> &diffInfoi,
                                  const QVector<int> &diffInfoj, QString &alignInfo)
{
    const char *alleleSeqi = alleleInfoi.alleleSequence.data();
    const char *alleleSeqj = alleleInfoj.alleleSequence.data();
    alignInfo.append(QString("%1,%2,%3,%4%5%6%7").arg(alleleInfoi.alleleName)
                     .arg(alleleInfoj.alleleName).arg(alleleInfoi.isIndel+alleleInfoj.isIndel)
                     .arg(alleleInfoi.isRare).arg(alleleInfoj.isRare)
                     .arg(alleleInfoi.starInfo).arg(alleleInfoj.starInfo));
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
            if(Core::GetInstance()->mergeBases(alleleSeqi[diffInfoi[i]], alleleSeqj[diffInfoj[j]]) != patternSeq[diffInfoi[i]])
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


bool runComparePA(const char *patternSeq, QVector<AlleleInfo> &alleleInfos,
                  QVector< QVector<int> > &differentPosInfos, QMap<int, QString> *typeResult, PairStartEnd pair)
{
    int startIndex = pair.startIndex;
    int endIndex = pair.endIndex;
    int size = alleleInfos.size();
    qDebug()<<__FUNCTION__<<startIndex<<endIndex<<size;

    QSet<int> difpos;
    QMap<int, QMap<QString,QString>> order_map;
    for(int i=startIndex; i<endIndex; i++)
    {
        for(int j=i+1; j<size; j++)
        {
            QString alignInfo;
//            int mis = compareByAlleleInfoAndDiffPos(patternSeq, alleleInfos.at(i),
//                                                    alleleInfos.at(j), differentPosInfos.at(i),
//                                                    differentPosInfos.at(j), alignInfo);
            int i_indel = alleleInfos.at(i).isIndel + alleleInfos.at(j).isIndel;
            if(i_indel != 0) //屏蔽插入和缺失的型别对
            {
                continue;
            }

            int mis = compareByAlleleInfoAndDiffPos_new(patternSeq, alleleInfos.at(i),
                                                        alleleInfos.at(j), differentPosInfos.at(i),
                                                        differentPosInfos.at(j), difpos);

            typeResult->insertMulti(mis, alignInfo);
//            if(alleleInfos.at(i).alleleName.contains("DQB1*02:02") &&
//               alleleInfos.at(j).alleleName.contains("DQB1*03:02"))
//            {
//                qDebug()<<alleleInfos.at(i).alleleName<<alleleInfos.at(j).alleleName<<mis;
//            }
//            if(typeResult->size()>=maxTop)
//            {
//                QMap<int, QString>::iterator itp1, itp2, itp500;
//                itp1 = typeResult->end();
//                itp2 = typeResult->end();
//                itp500 = typeResult->begin()+500;
//                int mis = itp500.key();
//                itp2--;
//                for(int i=maxTop; i>rLimit; i--)
//                {
//                    if(itp2.key()==mis){
//                        break;
//                    }
//                    itp1 = itp2;
//                    itp2--;
//                    typeResult->erase(itp1);
//                }
//            }
        }
    }

    return true;
}

typedef struct _tagAlignindex
{
    int i;
    int j;
}Alignindex;

typedef struct _tagtmp_str_z
{
    QString str1;
    QString str2;
}tmp_str_z;

int comparePatternWithAllele_OneThread(char *patternSeq, QVector<AlleleInfo> &alleleInfos,
                                 QVector< QVector<int> > &differentPosInfos, QVector<QString> &typeResult)
{
    bool isRare=true, isFull=false;
    int size = alleleInfos.size();
    QSet<int> difpos;
    QMap<int, QMap<int, QMap<int, Alignindex>>> order_map;
#ifndef QT_NO_DEBUG
    QMap<QString, tmp_str_z> map_test;
    QFile file("d:\\test.ini");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString str;
            tmp_str_z str_z;
            in>>str>>str_z.str1>>str_z.str2;
            map_test.insert(str, str_z);
        }
        file.close();
    }

    QStringList str_list = alleleInfos.at(0).alleleName.split('*');

    QString str1,str2;
    if(map_test.contains(str_list[0]))
    {
        str1 = map_test.value(str_list[0]).str1;
        str2 = map_test.value(str_list[0]).str2;
    }
#endif



    //筛选allele,先做i,再做j.
    //特定的allele在这里设置 (A*02:01)
    QString allele_spec = "A*02:01";

    QSet<QString> set_str;
    for(int i=0; i<size; i++)
    {
       // qDebug()<<i<<" and "<<alleleInfos.at(i).alleleName;
        //空掉不是特定的allele
        if(alleleInfos.at(i).alleleName.count(':')<3) //筛选i相似的型别
        {

            if(!set_str.contains(alleleInfos.at(i).alleleName))
            {
                set_str.insert(alleleInfos.at(i).alleleName);
            }
            else
            {
                continue;
            }
        }
        else
        {
            QString tmp_str = alleleInfos.at(i).alleleName.section(':',0,2);
           // qDebug()<<alleleInfos.at(i).alleleName<<" and "<<tmp_str;
            if(!set_str.contains(tmp_str))
            {
                set_str.insert(tmp_str);
            }
            else
            {
                continue;
            }
        }


        QSet<QString> set_str_t;
        for(int j=i+1; j<size; j++)
        {           
            if(alleleInfos.at(j).alleleName.count(':')<3) //筛选j相似的型别
            {
                if(!set_str_t.contains(alleleInfos.at(j).alleleName))
                {
                    set_str_t.insert(alleleInfos.at(j).alleleName);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                QString tmp_str = alleleInfos.at(j).alleleName.section(':',0,2);
                if(!set_str_t.contains(tmp_str))
                {
                    set_str_t.insert(tmp_str);
                }
                else
                {
                    continue;
                }
            }


            //如果i和j都没有"allele_spec"则跳过此项
         /*   if(alleleInfos.at(i).alleleName.section(':',0,1) != allele_spec && alleleInfos.at(j).alleleName.section(':',0,1) != allele_spec)
            {
                continue;
            }

*/

            int mis = compareByAlleleInfoAndDiffPos_new(patternSeq, alleleInfos.at(i),
                                                        alleleInfos.at(j), differentPosInfos.at(i),
                                                        differentPosInfos.at(j), difpos);

#ifndef QT_NO_DEBUG
            if(!str1.isEmpty() &&!str2.isEmpty() && alleleInfos.at(i).alleleName.contains(str1) &&
               alleleInfos.at(j).alleleName.contains(str2))
            {
                qDebug()<<alleleInfos.at(i).alleleName<<alleleInfos.at(j).alleleName<<mis;
            }
#endif

            Alignindex index;
            index.i = i;
            index.j = j;
            if(order_map.contains(mis))
            {
                QMap<int, QMap<int, Alignindex>> &t_t_map = order_map[mis];
                if(t_t_map.contains(i))
                {
                    QMap<int,Alignindex> &t_map = order_map[mis][i];
                    t_map.insert(j, index);
                }
                else
                {
                    QMap<int,Alignindex> t_map;
                    t_map.insert(j, index);
                    t_t_map.insert(i, t_map);
                }
            }
            else
            {
                QMap<int, QMap<int, Alignindex>> t_t_map;
                QMap<int,Alignindex> t_map;
                t_map.insert(j, index);
                t_t_map.insert(i, t_map);
                order_map.insert(mis, t_t_map);
            }
        }
    }

    auto itor = order_map.begin();
    for(;itor != order_map.end();itor++)
    {
        if(itor.key() == 0)
        {
            isFull = true;
        }
        QMap<int, QMap<int, Alignindex>> &t_t_map = order_map[itor.key()];

        auto t_t_itor = t_t_map.begin();
        for(;t_t_itor != t_t_map.end(); t_t_itor++)
        {
            QMap<int, Alignindex> &t_map = t_t_map[t_t_itor.key()];
            foreach(const Alignindex &index, t_map.values())
            {
                const AlleleInfo &i_info = alleleInfos.at(index.i);
                const AlleleInfo &j_info = alleleInfos.at(index.j);

                QString alignInfo = QString("%1,%2,%3,%4,%5%6%7%8;").arg(itor.key()).arg(i_info.alleleName)
                                             .arg(j_info.alleleName).arg(i_info.isIndel+j_info.isIndel)
                                             .arg(i_info.isRare).arg(j_info.isRare)
                                             .arg(i_info.starInfo).arg(j_info.starInfo);

                if(isRare && !alignInfo.contains("r"))
                {
                    isRare = false;
                }
                typeResult.push_back(alignInfo);
                if(typeResult.size()>1000)
                {
                    goto END;
                }
            }
        }
    }

END:

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

int comparePatternWithAlleleByThread(char *patternSeq, QVector<AlleleInfo> &alleleInfos,
                                     QVector< QVector<int> > &differentPosInfos, QMap<int, QString> &typeResult)
{
    bool isRare=true, isFull=false;

    QFuture<bool> *thread = new QFuture<bool> [MAX_THREAD_NUM];
    QMap<int, QString> *result = new QMap<int, QString> [MAX_THREAD_NUM];
    int splitPos[MAX_THREAD_NUM + 1];
    splitPos[0] = alleleInfos.size();
    splitPos[MAX_THREAD_NUM] = 0;
    for (int i = 1; i < MAX_THREAD_NUM; i++)
    {
        splitPos[i] = int((1.0 - sqrt((double)i) / sqrt((double)MAX_THREAD_NUM)) * splitPos[0]);
    }

    for(int i=0;i<MAX_THREAD_NUM;i++)
    {
        PairStartEnd pair;
        pair.startIndex = splitPos[MAX_THREAD_NUM-i];
        pair.endIndex = splitPos[MAX_THREAD_NUM-i-1];
        thread[i] = QtConcurrent::run(runComparePA, patternSeq, alleleInfos, differentPosInfos, &result[i], pair);
    }

    for(int i=0; i<MAX_THREAD_NUM; i++)
    {
        thread[i].waitForFinished();
    }

    QMap<int,int> map_key;
    for(int i=0; i<MAX_THREAD_NUM; i++)
    {
        QList<int> lk = result[i].uniqueKeys();
        foreach(int a, lk)
        {
            map_key.insert(a,0);
        }
    }

    foreach(int a, map_key.keys())
    {
        if(a==0)
        {
            isFull = true;
        }

        for(int i=MAX_THREAD_NUM-1; i>=0; i--)
        {
            QList<QString> ls = result[i].values(a);
            if(ls.size()<=50)
            {
                foreach(const QString &str, ls)
                {
                    if(!str.contains("r"))
                    {
                        isRare = false;
                    }
                    typeResult.insertMulti(a, str);
                }
            }
            else
            {
//                auto itor = ls.end()-10;
//                for(;itor != ls.end();itor++)
//                {
//                    if(!itor->contains("r"))
//                    {
//                        isRare = false;
//                    }
//                    typeResult.insertMulti(a, *itor);
//                }
                QStringRef s_ref;
                int count = 0;
                foreach(const QString &str, ls)
                {
                    QVector<QStringRef> v_sl = str.splitRef(',');
                    if(s_ref == v_sl[0] && count++ > 10)
                    {
                        continue;
                    }
                    else
                    {
                        s_ref = v_sl[0];
                        count = 0;
                    }

                    typeResult.insertMulti(a, str);
                }


            }

            if(typeResult.size() > 800)
            {
                goto CLEAR;
            }
        }
    }

CLEAR:
    for(int i=0; i<MAX_THREAD_NUM; i++)
    {
        result[i].clear();
    }


//    for(int i=0; i<MAX_THREAD_NUM; i++)
//    {
//        for(QMap<int, QString>::iterator it=result[i].begin(); it!=result[i].end(); it++)
//        {
//            typeResult.insertMulti(it.key(), it.value());
//            if(it.key()==0)
//            {
//                isFull = true;
//                if(!it.value().contains("r"))
//                {
//                    isRare = false;
//                }
//            }
//        }
//        result[i].clear();
//    }

    delete []result;
    delete []thread;

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


int AnalysisSampleThreadTask::comparePatternWithAllele(char *patternSeq,  char *consensusSeq,
                                                       const QString &geneName,
                                                       int exonStartPos, int minExonIndex, int maxExonIndex,
                                                       QVector<QString> &typeResult, QSet<QString> &sheildAlleles)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    int length = strlen(patternSeq);
    if(length<=0)
        return UNMATCH;
    QVector<AlleleInfo> alleleInfos;
    //从alleleTable获取信息，数据比较多
    SoapTypingDB::GetInstance()->getAlleleInfosFromStaticDatabase(geneName, exonStartPos, length, minExonIndex, maxExonIndex, alleleInfos, sheildAlleles);

    //qDebug()<<"alleleSize = "<<alleleInfos.size();
    if(alleleInfos.size()>3000){
        /**
          *需要排除兼并序列>一致性序列的点
          */
        //removeAlleleInfoByPattern(patternSeq, alleleInfos, consensusSeq);
        removeAlleleInfoByPatternNew(patternSeq, alleleInfos);
    }
   // qDebug()<<"alleleSize = "<<alleleInfos.size();
    if(alleleInfos.size()<=0)
    {
        return MISMATCH;
    }
    QVector< QVector<int> > differentPosInfos;
    getDifferentPositionInfo(patternSeq, alleleInfos, differentPosInfos);
    //int alignResult = comparePatternWithAlleleByThread(patternSeq, alleleInfos, differentPosInfos, typeResult);
    int alignResult = comparePatternWithAllele_OneThread(patternSeq, alleleInfos, differentPosInfos, typeResult);
    return alignResult;
}

void changeAlignMapToAlignString_vec(QVector<QString> &typeResult, QString &result)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    int limit = I_ROWNUM;
    int i=0;
    for(QVector<QString>::iterator it=typeResult.begin(); it!=typeResult.end(); it++)
    {
        result.push_back(*it);
        if(++i>=limit)
            break;
    }
}

void changeAlignMapToAlignString(QMap<int, QString> &typeResult, QString &result)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    int limit = I_ROWNUM;
    int i=0;
    for(QMap<int, QString>::iterator it=typeResult.begin(); it!=typeResult.end(); it++)
    {
        result.push_back(QString("%1,%2;").arg(it.key()).arg(it.value()));
        if(++i>=limit)
            break;
    }
    return;
}

void getZeroResultFromTypeResult(QVector<QString> &typeResult, QVector<QString> &zeroResult)
{
    for(auto it = typeResult.begin(); it!= typeResult.end(); it++)
    {
        if(it->startsWith('0'))
        {
            QStringRef str = it->midRef(2);
            //zeroResult.insertMulti(0, str.toString());
            zeroResult.push_back(str.toString());
        }
        else
        {
            break;
        }
    }
}

int AnalysisSampleThreadTask::compareGsspWithAlleles(const QByteArray &gsspName, const char *gsspSequence,
                                                     const QString &geneName, int exonStartPos,
                                                     QString &typeResult, QSet<QString> &zeroResult)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    int gsspL = (int)strlen(gsspSequence);
    QVector<GsspAlleleInfo> gsspAlleleInfos;
    SoapTypingDB::GetInstance()->getGsspAlleleInfosFromStaticDatabase(geneName, exonStartPos, gsspL,
                                                                      gsspAlleleInfos, gsspName);
    //没有屏蔽indel
    QMap<int, QString> misInfo;
    for(int i=gsspAlleleInfos.size()-1; i>=0; i--)
    {
        const char *seq = gsspAlleleInfos.at(i).alleleSequence.data();
        int mis=0;
        for(int j=0; j<gsspL; j++)
        {
            //1.3.0定为缺失时, 不算错误.
            if(gsspSequence[j]!='-' && gsspSequence[j]!='.'&& seq[j] != '*' &&gsspSequence[j]!=seq[j])
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

void getFilterResult(QString &result, QVector<QString> &typeResult, QSet<QString> &zeroAlleles, const QString &name)
{
    if(typeResult.size() <= 0 || zeroAlleles.size() <= 0)
        return;

    int limit = I_ROWNUM;
    int i = 0;
    for(auto it = typeResult.begin(); it!= typeResult.end(); it++)
    {
        QStringList line = it->split(",");
        //QVector<QStringRef> v_s = it->splitRef(',');
        if(zeroAlleles.contains(line[1]) || zeroAlleles.contains(line[2]))
        {
            QStringRef str = it->midRef(line[0].length()+1);
            result.push_back(QString("%1%2,%3").arg(line[0]).arg(name).arg(str));
            if(++i>=limit)
                break;
        }
    }
}

void getCombinedResult(QVector<QString> &zeroResult, QSet<QString> &zeroAlleles)
{
    if(zeroResult.size() <= 0)
        return;
    if(zeroAlleles.size()==0)
    {
        zeroResult.clear();
        return;
    }

    QVector<QString> newvec;
    for(auto it = zeroResult.begin(); it!= zeroResult.end(); it++)
    {
        QStringList line = it->split(",");
        if(zeroAlleles.contains(line.at(0)) || zeroAlleles.contains(line.at(1)))
        {
            //newMap.insertMulti(it.key(), it.value());
            newvec.push_back(*it);
        }
    }
    //zeroResult.clear();

    zeroResult.swap(newvec);
//    for(QMap<int ,QString>::iterator it = newMap.begin(); it!= newMap.end(); it++)
//    {
//        typeResult.insertMulti(it.key(), it.value());
//    }
//    newMap.clear();
}

void AnalysisSampleThreadTask::analysisSample(SampleInfo &sampleInfo, ExonInfo &exonInfo,
                    QVector<FileInfo>& fileInfos, QVector<FileInfo>& gsspFileInfos)
{
    //qDebug()<<(long)QThread::currentThreadId<<__FUNCTION__;
    bool isGood;
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
    QVector<QString> typeResult;
    int result = 0;
    if(fileInfos.size()>0)
    {
        sampleInfo.patternSequence.fill('-', sampleInfo.exonEndPos - sampleInfo.exonStartPos);
        //根据forwardSequence和reverseSequence，生成patternSequence
        mergeForwardAndReverseToPattern(sampleInfo.forwardSequence.data(), sampleInfo.reverseSequence.data(),
                                        sampleInfo.patternSequence.data(), sampleInfo.exonStartPos,
                                        frDifferenceList, frUnequlList);
        //从geneTable获取consensusSequence
        SoapTypingDB::GetInstance()->getConsensusSequenceFromStaticDatabase(sampleInfo.geneName,
                                                                            sampleInfo.consensusSequence, sampleInfo.exonStartPos,
                                                                            sampleInfo.exonEndPos-sampleInfo.exonStartPos);
        //比较patternSequence和consensusSequence
        comparePatternWithConsensus(sampleInfo.patternSequence.data(), sampleInfo.consensusSequence.data(),
                                    sampleInfo.exonStartPos, pcDifferenceList);
        QSet<QString> shieldAlleles;
        SoapTypingDB::GetInstance()->getShieldAllelesFromDatabase(sampleInfo.sampleName, shieldAlleles);

        result = comparePatternWithAllele(sampleInfo.patternSequence.data(), sampleInfo.consensusSequence.data(),
                                          sampleInfo.geneName, sampleInfo.exonStartPos, sampleInfo.minExonIndex,
                                          sampleInfo.maxExonIndex, typeResult, shieldAlleles);
        changeAlignMapToAlignString_vec(typeResult, sampleInfo.typeResult);
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
            SoapTypingDB::GetInstance()->getConsensusSequenceFromStaticDatabase(sampleInfo.geneName, sampleInfo.consensusSequence,
                                                   sampleInfo.exonStartPos, sampleInfo.exonEndPos-sampleInfo.exonStartPos);
        }

        //QMap<int, QString> zeroResult;
        QVector<QString> zeroResult;
        getZeroResultFromTypeResult(typeResult, zeroResult);
        for(int i=0; i<gsspFileInfos.size(); i++)
        {
            if(gsspFileInfos.at(i).isGood)
                isGood = true;
            QString gsspTypeResult;
            QString gsspFilterResult;
            QSet<QString> zeroGssp;
            int re = compareGsspWithAlleles(gsspFileInfos.at(i).gsspName, gsspFileInfos.at(i).sequence.data(),
                                            sampleInfo.geneName, gsspFileInfos.at(i).exonStartPos, gsspTypeResult, zeroGssp);
            if(result<re)
                result=re;
            getFilterResult(gsspFilterResult, typeResult, zeroGssp, "/0");
            getCombinedResult(zeroResult, zeroGssp);

           SoapTypingDB::GetInstance()->updateGsspFileResultToRealTimeDatabase(gsspFileInfos.at(i).fileName, re, gsspTypeResult, gsspFilterResult);

        }
        if(gsspFileInfos.size()>0)
        {
            for(auto it=zeroResult.begin(); it!=zeroResult.end(); it++)
            {
                sampleInfo.combinedResult.push_back(QString("%1,%2;").arg(0).arg(*it));
                if(sampleInfo.combinedResult.size() >= I_ROWNUM)
                    break;
            }
        }
    }
    if(result<MISMATCH)
    {
        if(!isGood)
            result = MATCHBAD;
    }
//    if(fileFlag == 1 || gsspFileFlag ==1)
//    {
//        result = UNMATCH;
//    }
    sampleInfo.analysisType = result;
}


void AnalysisSampleThreadTask::run()
{
    ExonInfo exonInfo;
    QVector<FileInfo> fileInfos;
    QVector<FileInfo> gsspFileInfos;

    int fileFlag = SoapTypingDB::GetInstance()->getFileInfoFromRealTimeDatabase(m_sample, fileInfos, exonInfo);
    int gsspFileFlag = SoapTypingDB::GetInstance()->getGsspFileInfoFromRealTimeDatabase(m_sample,gsspFileInfos, exonInfo);
    SampleInfo sampleInfo;
    sampleInfo.sampleName = m_sample;
    if(fileInfos.size()==0 && gsspFileInfos.size()==0)
    {
        if(gsspFileFlag == 1 || fileFlag==1)
        {
            sampleInfo.analysisType = UNMATCH;
            sampleInfo.maxExonIndex = 0;
            sampleInfo.minExonIndex = 0;
            sampleInfo.exonStartPos = 0;
            sampleInfo.exonEndPos = 0;
        }
    }
    else
    {
        analysisSample(sampleInfo, exonInfo, fileInfos, gsspFileInfos);
        SoapTypingDB::GetInstance()->insertSampleInfoToRealTimeDatabase(sampleInfo);
    }

    qDebug()<<m_sample<<"analysisfinished";
    emit analysisfinished();
}
