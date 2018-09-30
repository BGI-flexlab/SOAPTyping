#include "fileTablebase.h"

Ab1FileTableBase::Ab1FileTableBase()
{
    m_bGssp = false;
    m_cROrF = 'F';
    m_iExonIndex = 0;
    m_iExonStartPos = 0;
    m_iExonEndPos = 0;
    m_iBaseNumber = 0;
    m_iSignalNumber = 0;
    m_iMaxSignal = 0;
    m_iMaxQuality = 0;
    m_iIsGood = 0;
    m_iAlignResult = 0;
    m_iAlignStartPos = 0;
    m_iAlignEndPos = 0;
    m_iExcludeLeft = 0;
    m_iExcludeRight = 0;
    m_fAverageBaseWidth = 0.0;

    m_iExtraFile = 0;
    m_fAvgsignal = 0.0;
}

Ab1FileTableBase::~Ab1FileTableBase()
{

}

Ab1NormalFileTable::Ab1NormalFileTable()
{
    m_iExtraFile = 0;
    m_fAvgsignal = 0.0;
}

Ab1NormalFileTable::~Ab1NormalFileTable()
{

}

Ab1GsspFileTable::Ab1GsspFileTable()
{

}

Ab1GsspFileTable::~Ab1GsspFileTable()
{

}

SampleTable::SampleTable()
{
    m_iFileType = 0;
    m_iMarkType = 0;
    m_iAnalysisType = 0;
    m_iMinExonIndex = 0;
    m_iMaxExonIndex = 0;
    m_iExonStartPos = 0;
    m_iExonEndPos = 0;
}

SampleTable::~SampleTable()
{

}
