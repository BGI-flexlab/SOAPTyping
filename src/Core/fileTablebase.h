#ifndef FILEINFOBASE_H
#define FILEINFOBASE_H

#include <QString>

#define DEF_SET_GET(varType,varName,funName)          \
    private : varType varName;                             \
    public : void set##funName(const varType &varName){this->varName = varName;}      \
    public : varType get##funName(void){return this->varName;}

#define DEF_SET_GET_REF(varType,varName,funName)          \
    private : varType varName;                             \
    public : void set##funName(const varType &varName){this->varName = varName;}      \
    public : varType &get##funName(void){return this->varName;}

#define DEF_SET_GET_VIRTUAL(varType,funName)          \
    public : virtual void  set##funName(const varType &){}      \
    public : virtual varType get##funName(void){return 0;}

#define DEF_SET_GET_VIRTUAL_REF(varType,funName)          \
    public : virtual void  set##funName(const varType &){}      \
    public : virtual varType &get##funName(void){static varType tmp;return tmp;}


class Ab1FileTableBase
{
public:
    Ab1FileTableBase();
    virtual ~Ab1FileTableBase();

    DEF_SET_GET(bool, m_bGssp, IsGssp)
    DEF_SET_GET(QChar, m_cROrF, ROrF)

    DEF_SET_GET(int, m_iExonIndex, ExonIndex)
    DEF_SET_GET(int, m_iExonStartPos, ExonStartPos)
    DEF_SET_GET(int, m_iExonEndPos, ExonEndPos)
    DEF_SET_GET(int, m_iBaseNumber, BaseNumber)
    DEF_SET_GET(int, m_iSignalNumber, SignalNumber)
    DEF_SET_GET(int, m_iMaxSignal, MaxSignal)
    DEF_SET_GET(int, m_iMaxQuality, MaxQuality)
    DEF_SET_GET(int, m_iIsGood, IsGood)
    DEF_SET_GET(int, m_iAlignResult, AlignResult)
    DEF_SET_GET(int, m_iAlignStartPos, AlignStartPos)
    DEF_SET_GET(int, m_iAlignEndPos, AlignEndPos)
    DEF_SET_GET(int, m_iExcludeLeft, ExcludeLeft)
    DEF_SET_GET(int, m_iExcludeRight, ExcludeRight)

    DEF_SET_GET(float, m_fAverageBaseWidth, AverageBaseWidth)

    DEF_SET_GET_REF(QString, m_strFileName, FileName)
    DEF_SET_GET_REF(QString, m_strSampleName, SampleName)
    DEF_SET_GET_REF(QString, m_strFilePath, FilePath)
    DEF_SET_GET_REF(QString, m_strGeneName, GeneName)
    DEF_SET_GET_REF(QString, m_strUsefulSequence, UsefulSequence)
    DEF_SET_GET_REF(QByteArray, m_strBaseSequence, BaseSequence)
    DEF_SET_GET_REF(QString, m_strBasePostion, BasePostion)
    DEF_SET_GET_REF(QString, m_strBaseQuality, BaseQuality)
    DEF_SET_GET_REF(QString, m_strBaseASignal, BaseASignal)
    DEF_SET_GET_REF(QString, m_strBaseTSignal, BaseTSignal)
    DEF_SET_GET_REF(QString, m_strBaseGSignal, BaseGSignal)
    DEF_SET_GET_REF(QString, m_strBaseCSignal, BaseCSignal)
    DEF_SET_GET_REF(QString, m_strAlignInfo, AlignInfo)
    DEF_SET_GET_REF(QString, m_strEditInfo, EditInfo)

//    DEF_SET_GET_VIRTUAL(int, ExtraFile)
//    DEF_SET_GET_VIRTUAL(float, Avgsignal)

//    DEF_SET_GET_VIRTUAL_REF(QString, GsspName)
//    DEF_SET_GET_VIRTUAL_REF(QString, TypeResult)
//    DEF_SET_GET_VIRTUAL_REF(QString, FilterResult)
    DEF_SET_GET(int, m_iExtraFile, ExtraFile)
    DEF_SET_GET(float, m_fAvgsignal, Avgsignal)

    DEF_SET_GET_REF(QString, m_strGsspName, GsspName)
    DEF_SET_GET_REF(QString, m_strTypeResult, TypeResult)
    DEF_SET_GET_REF(QString, m_strFilterResult, FilterResult)

};


class Ab1NormalFileTable:public Ab1FileTableBase
{
public:
    Ab1NormalFileTable();
    ~Ab1NormalFileTable();

    DEF_SET_GET(int, m_iExtraFile, ExtraFile)
    DEF_SET_GET(float, m_fAvgsignal, Avgsignal)
};

class Ab1GsspFileTable:public Ab1FileTableBase
{
public:
    Ab1GsspFileTable();
    ~Ab1GsspFileTable();

    DEF_SET_GET_REF(QString, m_strGsspName, GsspName)
    DEF_SET_GET_REF(QString, m_strTypeResult, TypeResult)
    DEF_SET_GET_REF(QString, m_strFilterResult, FilterResult)
};

class SampleTable
{
public:
    SampleTable();
    ~SampleTable();

    DEF_SET_GET_REF(QString, m_strSampleName, SampleName)
    DEF_SET_GET_REF(QString, m_strGeneName, GeneName)

    DEF_SET_GET(int, m_iFileType, FileType)
    DEF_SET_GET(int, m_iMarkType, MarkType)
    DEF_SET_GET(int, m_iAnalysisType, AnalysisType)
    DEF_SET_GET(int, m_iMinExonIndex, MinExonIndex)
    DEF_SET_GET(int, m_iMaxExonIndex, MaxExonIndex)
    DEF_SET_GET(int, m_iExonStartPos, ExonStartPos)
    DEF_SET_GET(int, m_iExonEndPos, ExonEndPos)

    DEF_SET_GET_REF(QString, m_strConsensusSequence, ConsensusSequence)
    DEF_SET_GET_REF(QString, m_strForwardSequence, ForwardSequence)
    DEF_SET_GET_REF(QString, m_strReverseSequence, ReverseSequence)
    DEF_SET_GET_REF(QString, m_strPatternSequence, PatternSequence)
    DEF_SET_GET_REF(QString, m_strMismatchBetweenPC, MismatchBetweenPC)
    DEF_SET_GET_REF(QString, m_strMismatchBetweenFR, MismatchBetweenFR)
    DEF_SET_GET_REF(QString, m_strMmismatchBetweenFR, MmismatchBetweenFR)
    DEF_SET_GET_REF(QString, m_strEditPostion, EditPostion)
    DEF_SET_GET_REF(QString, m_strTypeResult, TypeResult)
    DEF_SET_GET_REF(QString, m_strGsspInfo, GsspInfo)
    DEF_SET_GET_REF(QString, m_strShieldAllele, ShieldAllele)
    DEF_SET_GET_REF(QString, m_strSetResult, SetResult)
    DEF_SET_GET_REF(QString, m_strSetNote, SetNote)
    DEF_SET_GET_REF(QString, m_strSetGSSP, SetGSSP)
    DEF_SET_GET_REF(QString, m_strCombinedResult, CombinedResult)
};

#endif // FILEINFOBASE_H
