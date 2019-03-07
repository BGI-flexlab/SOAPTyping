#include "analysisab1threadtask.h"
#include <QDebug>
#include <QThread>
#include "Core/Ab1.h"
#include "Core/core.h"
#include "all_base_struct.h"
#include "DataBase/soaptypingdb.h"

AnalysisAB1ThreadTask::AnalysisAB1ThreadTask(Ab1FileTableBase *pfiletable):m_pFiletable(pfiletable)
{

}

AnalysisAB1ThreadTask::~AnalysisAB1ThreadTask()
{
    if(m_pFiletable)
    {
        delete m_pFiletable;
        m_pFiletable = nullptr;
    }
}

void AnalysisAB1ThreadTask::run()
{
    Ab1 ab1(m_pFiletable->getFilePath());
    if(!ab1.IsAb1File())
    {
        return;
    }
    if(!ab1.AnalysisFile())
    {
        return;
    }
    ab1.getFWO();

    int i_basenum = ab1.getNumBase();
    m_pFiletable->setBaseNumber(i_basenum);
    int i_signalnum = ab1.getNumTraceData();
    m_pFiletable->setSignalNumber(i_signalnum);
    float f_basewidth = ab1.getBaseSpace();
    m_pFiletable->setAverageBaseWidth(f_basewidth);
    bool b_isgood = ab1.isAb1Good();
    m_pFiletable->setIsGood(b_isgood);

    int i_base_pos_offset = ab1.getBasePosOffset();
    int i_base_offset = ab1.getBaseOffset();
    int i_qual_offset = ab1.getQualOffset();

    char *orignal_seq = (char*)malloc(sizeof(char)*i_basenum+1);
    int *quality = (int*)malloc(sizeof(int)*i_basenum);
    int *base_positions = (int*)malloc(sizeof(int)*i_basenum);
    Baseo* base = (Baseo*)malloc(sizeof(Baseo)*i_basenum);
    Baseo* baseG = (Baseo*)malloc(sizeof(Baseo)*i_signalnum);
    Baseo* baseA = (Baseo*)malloc(sizeof(Baseo)*i_signalnum);
    Baseo* baseT = (Baseo*)malloc(sizeof(Baseo)*i_signalnum);
    Baseo* baseC = (Baseo*)malloc(sizeof(Baseo)*i_signalnum);

    ab1.getBaseSeq(i_base_offset, i_basenum, orignal_seq);
    int i_maxQuality = 0;
    ab1.getBaseQual(i_qual_offset, i_basenum, quality, i_maxQuality);
    m_pFiletable->setMaxQuality(i_maxQuality);
    ab1.getBasePosition(i_base_pos_offset, i_basenum, base_positions);

    long l_trace_offsets[4] = {0};
    ab1.getTraceDataOffset(l_trace_offsets);
    int i_maxSignal = 0;
    ab1.getTraceData(baseG, baseA, baseT, baseC, l_trace_offsets, i_signalnum, i_maxSignal);
    m_pFiletable->setMaxSignal(i_maxSignal);
    ab1.getBaseData(base, i_basenum, quality, orignal_seq, base_positions, baseG, baseA, baseT, baseC);
    if(!m_pFiletable->getIsGssp())
    {
        ab1.getPeak(baseG, baseA, baseT, baseC, base, base_positions, i_basenum);
        float avgsignal =ab1.signalRatio(baseG, baseA, baseT, baseC, base, base_positions, i_basenum);
        m_pFiletable->setAvgsignal(avgsignal);
    }

    QChar c_ROrF = m_pFiletable->getROrF();
    if (c_ROrF == 'R')
    {
        base  = ab1.reverseComplement(base,  i_basenum, i_signalnum, 1);
        baseG = ab1.reverseComplement(baseG, i_basenum, i_signalnum, 0);
        baseA = ab1.reverseComplement(baseA, i_basenum, i_signalnum, 0);
        baseT = ab1.reverseComplement(baseT, i_basenum, i_signalnum, 0);
        baseC = ab1.reverseComplement(baseC, i_basenum, i_signalnum, 0);
    }

    QStringList basePostion_t, baseQuality_t;
    for (int i = 0; i < i_basenum; i++)
    {
        basePostion_t << QString::number(base[i].pos);
        baseQuality_t << QString::number(quality[i]);
    }
    m_pFiletable->setBasePostion(basePostion_t.join(":"));
    m_pFiletable->setBaseQuality(baseQuality_t.join(":"));


    QStringList baseASignal_t, baseGSignal_t, baseTSignal_t, baseCSignal_t;

    if(c_ROrF == 'F')
    {
        for (int i = 0; i < i_signalnum; i++)
        {
            baseGSignal_t << QString::number(baseG[i].signal);
            baseASignal_t << QString::number(baseA[i].signal);
            baseTSignal_t << QString::number(baseT[i].signal);
            baseCSignal_t << QString::number(baseC[i].signal);
        }
    }
    else
    {
        for (int i = 0; i < i_signalnum; i++)
        {
            baseCSignal_t << QString::number(baseG[i].signal);
            baseTSignal_t << QString::number(baseA[i].signal);
            baseASignal_t << QString::number(baseT[i].signal);
            baseGSignal_t << QString::number(baseC[i].signal);
        }
    }

    m_pFiletable->setBaseGSignal(baseGSignal_t.join(":"));
    m_pFiletable->setBaseASignal(baseASignal_t.join(":"));
    m_pFiletable->setBaseTSignal(baseTSignal_t.join(":"));
    m_pFiletable->setBaseCSignal(baseCSignal_t.join(":"));

	QByteArray tmpbuf;
    for (int i = 0; i < i_basenum; i++)
    {
		tmpbuf.append(base[i].basetype);
    }
    m_pFiletable->setBaseSequence(tmpbuf);

    ExonInfoS exonInfos;
    SoapTypingDB::GetInstance()->GetExonInfo(m_pFiletable->getGeneName(), m_pFiletable->getExonIndex(), exonInfos);
    m_pFiletable->setExonStartPos(exonInfos.exonStartPos);
    m_pFiletable->setExonEndPos(exonInfos.exonEndPos);


    FileAlignNew file_align_new;
    file_align_new.consensus = exonInfos.consensusSeq.data();//从数据库表GeneTable获取的一段序列，起始位置是exonStartPos和exonEndPos
    file_align_new.raw_seq = m_pFiletable->getBaseSequence().data();//从文件获取的经过计算的原始序列（getBaseData（））
    file_align_new.consensus_start = exonInfos.exonStartPos;
    file_align_new.consensus_end = exonInfos.exonEndPos;
    file_align_new.exclude_left_num =0;
    file_align_new.exclude_right_num =0;

    if(!m_pFiletable->getIsGssp())
    {
        QStringList pathlist = m_pFiletable->getFilePath().split("_");
        if(pathlist[2] == "S117")
        {
            size_t len = strlen(file_align_new.raw_seq);
            file_align_new.exclude_right_num =abs((int)len - 270);
        }
    }


    FileAlignResult fileAlignResult;
    if(i_basenum >= 20) // 一些ab1文件里面碱基序列为空或很短
    {
        Core::GetInstance()->GetFileAlignResult(file_align_new, fileAlignResult, true);
        m_pFiletable->setUsefulSequence(fileAlignResult.sampleAlign);//保存的是consensus和raw_seq的最大公共子序列
        if(fileAlignResult.isUnDefined)
        {
            qDebug()<<m_pFiletable->getFilePath()<<"isUnDefined";
        }
        if(!m_pFiletable->getIsGssp())
        {
            m_pFiletable->setAlignResult(fileAlignResult.isUnDefined);
        }
        else
        {
            if(fileAlignResult.isUnDefined)
            {
                m_pFiletable->setAlignResult(4);
            }
            else
            {
                m_pFiletable->setAlignResult(0);
            }
        }
    }
    else
    {
        if(!m_pFiletable->getIsGssp())
        {
            m_pFiletable->setAlignResult(1);
        }
        else
        {
            m_pFiletable->setAlignResult(4);
        }
    }

    QStringList alignInfo_t;
    int i_ret = m_pFiletable->getAlignResult();
    if(i_ret >0)
    {
        m_pFiletable->setAlignStartPos(i_basenum-1);
        m_pFiletable->setAlignEndPos(i_basenum-1);

        for (int i = 0; i < i_basenum; i++)
        {
            alignInfo_t << QString::number(-1);
        }
    }
    else
    {
        m_pFiletable->setAlignStartPos(fileAlignResult.leftLimit);
        m_pFiletable->setAlignEndPos(fileAlignResult.rightLimit);

        for (int i = 0; i < i_basenum; i++)
        {
            alignInfo_t << QString::number(fileAlignResult.baseMatchConsensusPos[i]);
        }

        if(!m_pFiletable->getIsGssp())
        {
            QString str_query = QString("%1_%2_%3").arg(m_pFiletable->getGeneName())
                                                   .arg(m_pFiletable->getExonIndex())
                                                   .arg(m_pFiletable->getROrF());
            int leftpos = 0,rightpos = 0;
            SoapTypingDB::GetInstance()->GetExcludePosition(str_query, leftpos, rightpos);
            m_pFiletable->setExcludeLeft(leftpos); //存放的是左排除的个数
            m_pFiletable->setExcludeRight(rightpos);//存放的是右排除的个数
        }
    }

    m_pFiletable->setAlignInfo(alignInfo_t.join(":"));
    if(!m_pFiletable->getIsGssp())
    {
        SoapTypingDB::GetInstance()->InsertOneFileTable(*m_pFiletable);
    }
    else
    {
        SoapTypingDB::GetInstance()->insertOneGsspFileTable(*m_pFiletable);
    }

    free(orignal_seq);
    free(quality);
    free(base_positions);
    free(base);
    free(baseG);
    free(baseA);
    free(baseT);
    free(baseC);
    emit analysisfinished();
}
