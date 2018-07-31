#include"Main_Oscillogram.h"
#include "curvedline.h"
#include "QClipboard"
#include <QDebug>

CurvedLine::CurvedLine(int current
                       ,RegionShowData  *   regionShowData
                       ,RegionShowData *regionShowDataBak,
                       SaveData  * curved_savedata
                       ,QWidget *parent) :
    QWidget(parent)
{
//20180608close qDebug()<<"curvedLine begin";
    currentfile=current;
    LineHight     =20;//改14修改线高
    xAxisExpand=0;
    xAxisExpand=curved_savedata->rightMenuX;
    CurvedHight=curved_savedata->curedHigth;
    regionShow_xAxisExpand=curved_savedata->filesxExpand;
    regionShow_yAxisExpand=curved_savedata->filesyExpand;
    cuv_regionShowData=regionShowData;
    cuv_regionShowDataBak=regionShowDataBak;
    //regionShow_xAxisExpand=5;
//20180608close qDebug()<<"curvedLine xxx1";
    drawAll();
//20180608close qDebug()<<"curvedLine xxx2";
    mousepress=false;
    startexon=0;
    endexon=0;
    grabWhichone=0;
    saveanaly=NULL;

    frame=new QFrame((QWidget *)this->parent());
    frame->setFrameStyle( QFrame::Box | QFrame::Raised );
    frame->setStyleSheet("color:rbg(100,250,250)");
    frame->setWindowFlags(Qt::WindowStaysOnTopHint);
    frame->hide();
    pressed=false;
    moved=false;
    /*新增Start**/
    int num = cur_baseinfor.size();
    m_ppLetter = NULL;
    m_ppLetter = new SoapLetter*[num];
    int i = 0;
//20180608close qDebug()<<"curvedLine xxx3";
    foreach(const BaseInfor & Eachbase,cur_baseinfor)
    {

        QColor color;
        if(Eachbase.Matchpos!=-1)
            SetColor(color,Eachbase.basetype);
        else
            color = QColor(Qt::gray);
        int x1 =  (Eachbase.pos)*regionShow_xAxisExpand -11;
        m_ppLetter[i] = new SoapLetter(Eachbase.basetype,color,this);
        m_ppLetter[i]->show();
        m_ppLetter[i]->setMinimumSize(20,20);
        m_ppLetter[i]->setMaximumSize(20,20);
        m_ppLetter[i]->move(x1,LineHight);
        m_ppLetter[i]->setLetterInfo(currentfile,Eachbase.Matchpos);
        m_ppLetter[i]->setAllLetterDataList(&m_AllLetterList);
        m_pLetterList.append(m_ppLetter[i]);
        connect(m_ppLetter[i],SIGNAL(signalClickLetter(int,int)),this,SIGNAL(signalClickLetter(int,int)));
        connect(m_ppLetter[i],SIGNAL(formbasechanged(char,int,int)),this,SIGNAL(formbasechanged(char,int,int)));
        connect(m_ppLetter[i],SIGNAL(formbasechanged(char,int,int)),this,SLOT(slotbasechanged(char,int,int)));
        i++;
    }
//20180608close qDebug()<<"curvedLine xxx4 end";
    /**新增End**/
}

void CurvedLine::drawAll()
{

//20180608close qDebug()<<"curvedline draw all begin";
    try{

        QVector <Each_Base_Signal>
                &baseG =cuv_regionShowData->baseG[currentfile],
                &baseA =cuv_regionShowData->baseA[currentfile],
                &baseT =cuv_regionShowData->baseT[currentfile],
                &baseC =cuv_regionShowData->baseC[currentfile];

//20180608close qDebug()<<"curvedline draw all xx1";
        int regionShow_traceNum = baseG.size();
        if(regionShow_traceNum<1)
        {
            //qDebug()<<"data empty";
            return;
        }
        /**
          * 修正峰图左右屏蔽部分，灰色位置。取两个碱基的中间值
          **/
        int leftLimitTmp = cuv_regionShowData->leftLimits[currentfile];
        int leftExcludeTmp = cuv_regionShowData->excludeleft[currentfile];
        int rightLimitTmp = cuv_regionShowData->rightLimits[currentfile];
        int rightExcludeTmp = cuv_regionShowData->excludeRight[currentfile];
        cuvLeftLimit = leftLimitTmp;
        cuvLeftExclude = leftExcludeTmp;
        cuvRightLimit = rightLimitTmp;
        cuvRightExclude = rightExcludeTmp;
//        cuvLeftLimit  = cuv_regionShowData->leftLimits[currentfile]*regionShow_xAxisExpand;
//        cuvRightLimit = cuv_regionShowData->rightLimits[currentfile]*regionShow_xAxisExpand;
//        cuvLeftExclude=cuv_regionShowData->excludeleft[currentfile]*regionShow_xAxisExpand;
//        cuvRightExclude=cuv_regionShowData->excludeRight[currentfile]*regionShow_xAxisExpand;
        cur_baseinfor=cuv_regionShowData->base[currentfile];
        cur_changeinfo=cuv_regionShowData->edit_info[currentfile];
        /**
          *0位置和最大位置忽略不处理
          */
        for(int i=cur_baseinfor.count()-2;i>0;i--){
            if(cur_baseinfor[i].pos==leftLimitTmp){
                /**
                  *由于左切除，该位点记录的是被切除的位点的右边一个位点的位置，所以取i-1和i的中间位置。
                  */
                cuvLeftLimit = (cur_baseinfor[i-1].pos+cur_baseinfor[i].pos)/2;
            }
            if(cur_baseinfor[i].pos==leftExcludeTmp){
                /**
                  * 由于左屏蔽，是屏蔽鼠标点击位点左侧的碱基，而leftExclude记录的是鼠标位点，所以取I-1和I位置
                  */
                cuvLeftExclude = (cur_baseinfor[i-1].pos+cur_baseinfor[i].pos)/2;
            }
            if(cur_baseinfor[i].pos==rightLimitTmp){
                /**
                  *由于右切除，该位点记录的是被切除的位点，所以取i-1和i的中间位置。
                  */
                cuvRightLimit = (cur_baseinfor[i-1].pos+cur_baseinfor[i].pos)/2;
            }
            if(cur_baseinfor[i].pos==rightExcludeTmp){
                /**
                  * 由于右屏蔽，是屏蔽鼠标点击位点右侧的碱基，而rightExclude记录的是鼠标位点，所以取I和I+1位置
                  */
                cuvRightExclude = (cur_baseinfor[i+1].pos+cur_baseinfor[i].pos)/2;
            }
        }
//20180608close qDebug()<<"curvedline draw all xx2";
        cuvLeftLimit = cuvLeftLimit * regionShow_xAxisExpand;
        cuvLeftExclude = cuvLeftExclude * regionShow_xAxisExpand;
        cuvRightLimit = cuvRightLimit * regionShow_xAxisExpand;
        cuvRightExclude = cuvRightExclude * regionShow_xAxisExpand;
        for(int i=0;i<cur_changeinfo.count();++i)
        {
            for(int j=0;j<cur_baseinfor.count();++j )
            {
                if(cur_baseinfor[j].Matchpos==
                        cur_changeinfo[i].Consen_Pos)
                {
                    char temp=cur_changeinfo[i].base_type;
                    cur_changeinfo[i].Consen_Pos=cur_baseinfor[j].pos;
                    cur_changeinfo[i].base_type=cur_baseinfor[j].basetype;
                    cur_baseinfor[j].basetype=temp;
                    break;
                }
            }
        }
//20180608close qDebug()<<"curvedline draw all xx3";
        int maxWidth=baseG[regionShow_traceNum-1].pos;
        if(baseA[baseA.size()-1].pos>maxWidth) maxWidth=baseA[baseA.size()-1].pos;
        if(baseT[baseT.size()-1].pos>maxWidth) maxWidth=baseT[baseT.size()-1].pos;
        if(baseC[baseC.size()-1].pos>maxWidth) maxWidth=baseC[baseC.size()-1].pos;
        CurvedWidth=maxWidth*regionShow_xAxisExpand;
        int start=0,end=cuv_regionShowData->base[currentfile][cuv_regionShowData->base[currentfile].count()-1].pos;
        if(!cuv_regionShowData->infor[currentfile].isUndefined)
        {
            for(int i=0;i<cuv_regionShowData->base[currentfile].count();++i)
            {
                if(cuv_regionShowData->base[currentfile][i].Matchpos!=-1)
                {
                    start=cuv_regionShowData->base[currentfile][i].pos;
                    break;
                }
            }
            for(int i=cuv_regionShowData->base[currentfile].count()-1;i>0;--i)
            {
                if(cuv_regionShowData->base[currentfile][i].Matchpos!=-1)
                {
                    end=cuv_regionShowData->base[currentfile][i].pos;
                    break;
                }
            }
        }


//20180608close qDebug()<<"curvedline draw all yy1";
        for( int i = 0 ; i < regionShow_traceNum ; i++)
        {
            if(baseG[i].pos<start) continue;
            if(baseG[i].pos>end) break;
            maxValue >= baseG[i].signal ? maxValue = maxValue : maxValue = baseG[i].signal;
            maxValue >= baseA[i].signal ? maxValue = maxValue : maxValue = baseA[i].signal;
            maxValue >= baseT[i].signal ? maxValue = maxValue : maxValue = baseT[i].signal;
            maxValue >= baseC[i].signal ? maxValue = maxValue : maxValue = baseC[i].signal;
        }
//20180608close qDebug()<<"curvedline draw all yy2";
        double mapcount=(double)baseG[baseG.count()-1].pos*regionShow_xAxisExpand/LENIMAGE;
        compress=((double)(CurvedHight-3*LineHight)/maxValue)*1;
//20180608close qDebug()<<"curvedline draw all yy3 "<<cuv_regionShowData->infor[currentfile].isUndefined;
        if(cuv_regionShowData->infor[currentfile].isUndefined==false)
            analysisDataforpeak();
        compress=((double)(CurvedHight-3*LineHight)/maxValue)*regionShow_yAxisExpand;
#ifdef Q_WS_WIN
        for(int i=0 ;i<drawmap.count();i++)
        {
            delete  drawmap[i];
            drawmap[i]=NULL;
        }
        drawmap.clear();
//20180608close qDebug()<<"curvedline draw all yy4 ";
        for(int i=0;i<mapcount;++i)
        {
            QImage * one=new QImage(LENIMAGE,
                                    CurvedHight-3*LineHight,QImage::Format_ARGB32);
            drawmap.push_back(one);
            drawContents(baseT,baseC,baseG,baseA,drawmap[i],i*LENIMAGE);
        }
#endif

        //drawContents1(baseT,baseC,baseG,baseA,NULL,LENIMAGE);
        this->setFixedHeight(CurvedHight);

    }
    catch(...)
    {
        ErrorWrite::errorocur("drawAll");
    }
//20180608close qDebug()<<"curvedline draw all xx4";
    /**新增Start**/
    if(m_pLetterList.size() != 0)
    {
        int i = 0;
        foreach(const BaseInfor & Eachbase,cur_baseinfor)
        {
            QColor color;
            if(Eachbase.Matchpos!=-1)
                SetColor(color,Eachbase.basetype);
            else
                color = QColor(Qt::gray);
            m_pLetterList[i]->setColor(color);
            int x1 =  (Eachbase.pos)*regionShow_xAxisExpand -11;
            m_pLetterList[i]->move(x1,LineHight);
            m_pLetterList[i]->m_IsClicked = false;
            i++;
        }
    }
//20180608close qDebug()<<"curvedline draw all xx5";
    /**新增End**/
}

void CurvedLine::analysisDataforpeak()
{
    //    cuv_regionShowData->PeaksA.clear();
    //    cuv_regionShowData->PeaksC.clear();
    //    cuv_regionShowData->PeaksG.clear();
    //    cuv_regionShowData->Peaksinform.clear();
    //    cuv_regionShowData->PeaksT.clear();

//20180608close qDebug()<<"curvedline analysisDataforpeak begin file: "<<currentfile;
    if(cuv_regionShowData->PeaksA.count()>=cuv_regionShowData->base.count())
        return;
    int m=currentfile;
    QVector <Each_Base_Signal>
            &baseG =cuv_regionShowDataBak->baseG[m],
            &baseA =cuv_regionShowDataBak->baseA[m],
            &baseT =cuv_regionShowDataBak->baseT[m],
            &baseC =cuv_regionShowDataBak->baseC[m];
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye baseA ";
    for(int j=1; j<baseA.count();++j)
    {
        Each_Base_Signal & basesig=baseA[j-1];
        Each_Base_Signal & basesig2=baseA[j];
        basesig2.relative=(double)(basesig2.signal-basesig.signal)*compress;
    }

//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye baset ";
    for(int j=1; j<baseT.count();++j)
    {
        Each_Base_Signal & basesig=baseT[j-1];
        Each_Base_Signal & basesig2=baseT[j];
        basesig2.relative=(double)(basesig2.signal-basesig.signal)*compress;
    }
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye baseg ";
    for(int j=1; j<baseC.count();++j)
    {
        Each_Base_Signal & basesig=baseC[j-1];
        Each_Base_Signal & basesig2=baseC[j];
        basesig2.relative=(double)(basesig2.signal-basesig.signal)*compress;
    }
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye basec ";
    for(int j=1; j<baseG.count();++j)
    {
        Each_Base_Signal & basesig=baseG[j-1];
        Each_Base_Signal & basesig2=baseG[j];
        basesig2.relative=(double)(basesig2.signal-basesig.signal)*compress;
    }
    QVector<Peak> oneA;
    QVector<Peak> oneT;
    QVector<Peak> oneC;
    QVector<Peak> oneG;
    oneA.clear();
    oneT.clear();
    oneC.clear();
    oneG.clear();
    double max=-200.0,min=200.0,maxsignal=-200.0;
    int startindex=0,endindex=0,maxsingalpos=0;
    max=-200;min=200;maxsignal=-200;startindex=0;endindex=0;maxsingalpos=0;
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye baseA ";
    for(int i=1; i<baseA.count();++i)
    {
        if(baseA[i].relative>=max)
        {
            max=baseA[i].relative;
            startindex=i;
        }
        else
        {
            min=baseA[i].relative;
            for(;i<baseA.count();++i)
            {
                if(baseA[i].relative<=min)
                {
                    min=baseA[i].relative;
                    endindex=i;
                }
                else
                {
                    if(!myabs(max)||!myabs(min))
                    {
                        for(int j=startindex;j<=endindex;++j)
                        {
                            if(baseA[j].signal*compress>maxsignal)
                            {
                                maxsignal=baseA[j].signal*compress;
                                maxsingalpos=baseA[j].pos;
                            }
                        }
                        InsertPeaks(oneA,'A',max,min,startindex,endindex,maxsignal,m,maxsingalpos);
                    }
                    max=-200;min=200;maxsignal=-200;startindex=0;endindex=0;maxsingalpos=0;
                    break;
                }
            }
        }
    }
    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye baset ";
    for(int i=1; i<baseT.count();++i)
    {
        if(baseT[i].relative>=max)
        {
            max=baseT[i].relative;
            startindex=i;
        }
        else
        {
            min=baseT[i].relative;
            for(;i<baseT.count();++i)
            {
                if(baseT[i].relative<=min)
                {
                    min=baseT[i].relative;
                    endindex=i;
                }
                else
                {
                    if(!myabs(max)||!myabs(min))
                    {
                        for(int j=startindex;j<=endindex;++j)
                        {
                            if(baseT[j].signal*compress>maxsignal)
                            {
                                maxsignal=baseT[j].signal*compress;
                                maxsingalpos=baseT[j].pos;
                            }

                        }
                        InsertPeaks(oneT,'T',max,min,startindex,endindex,maxsignal,m,maxsingalpos);
                    }
                    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
                    break;
                }
            }
        }
    }
    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye basec ";
    for(int i=1; i<baseC.count();++i)
    {
        if(baseC[i].relative>=max)
        {
            max=baseC[i].relative;
            startindex=i;
        }
        else
        {
            min=baseC[i].relative;
            for(;i<baseC.count();++i)
            {
                if(baseC[i].relative<=min)
                {
                    min=baseC[i].relative;
                    endindex=i;
                }
                else
                {
                    if(!myabs(max)||!myabs(min))
                    {
                        for(int j=startindex;j<=endindex;++j)
                        {
                            if(baseC[j].signal*compress>maxsignal)
                            {
                                maxsignal=baseC[j].signal*compress;
                                maxsingalpos=baseC[j].pos;
                            }

                        }
                        InsertPeaks(oneC,'C',max,min,startindex,endindex,maxsignal,m,maxsingalpos);
                    }
                    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
                    break;
                }
            }
        }
    }
    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye basec ";
    for(int i=1; i<baseG.count();++i)
    {
        if(baseG[i].relative>=max)
        {
            max=baseG[i].relative;
            startindex=i;
        }
        else
        {
            min=baseG[i].relative;
            for(;i<baseG.count();++i)
            {
                if(baseG[i].relative<=min)
                {
                    min=baseG[i].relative;
                    endindex=i;
                }
                else
                {
                    if(!myabs(max)||!myabs(min))
                    {
                        for(int j=startindex;j<=endindex;++j)
                        {
                            if(baseG[j].signal*compress>maxsignal)
                            {
                                maxsignal=baseG[j].signal*compress;
                                maxsingalpos=baseG[j].pos;
                            }
                        }
                        InsertPeaks(oneG,'G',max,min,startindex,endindex,maxsignal,m,maxsingalpos);
                    }
                    max=-200;min=200;maxsignal=-200.0;startindex=0;endindex=0;maxsingalpos=0;
                    break;
                }
            }
        }
    }
    /*qDebug()<<"endPos"<<oneA[oneA.count()-1].endpos<<oneT[oneT.count()-1].endpos
           <<oneC[oneC.count()-1].endpos<<oneG[oneG.count()-1].endpos;*/
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye diyibu ";
    cuv_regionShowData->PeaksA.push_back(oneA);
    cuv_regionShowData->PeaksT.push_back(oneT);
    cuv_regionShowData->PeaksC.push_back(oneC);
    cuv_regionShowData->PeaksG.push_back(oneG);


    QVector<Peak> oneAll;
    int a=0,t=0,c=0,g=0;
    bool aout=false,tout=false,cout=false,gout=false;
    //double minstartpos=0.0;
    int minstartpos = 0;
    //qDebug()<<oneA.count()<<oneT.count()<<oneC.count()<<oneG.count();
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye diyibu "<<oneA.count()<<","<<oneT.count()<<","<<oneC.count()<<","<<oneG.count()
           //<<";"<<baseA.count()<<","<<baseT.count()<<","<<baseC.count()<<","<<baseG.count()<<",";
    if((oneA.count()|oneT.count()|oneG.count()|oneC.count())==0){
        /**
          *你大爷啊，画个峰图不知道你到底要干啥，一遍一遍的重复代码，看不懂，但是不知道为啥会出现4个one都为0的场景，导致报错崩溃
          */
//20180608close qDebug()<<"nimabi, zhixingdao zheli le?";
    }else{
        while(true)
        {
            if(aout&&tout&&cout&&gout) break;
            int whichone=0;
            minstartpos=cuv_regionShowDataBak->baseA[currentfile].last().pos;//change shi value can change all;
            if(!aout)
                minstartpos=oneA[a].startpos;
            if(!tout&&oneT[t].startpos<=minstartpos)
            {
                minstartpos=oneT[t].startpos;
                whichone=1;
            }
            if(!cout&&oneC[c].startpos<=minstartpos)
            {
                minstartpos=oneC[c].startpos;
                whichone=2;
            }
            if(!gout&&oneG[g].startpos<=minstartpos)
            {
                minstartpos=oneG[g].startpos;
                whichone=3;
            }
            //qDebug()<<a<<t<<c<<g<<whichone;
            switch(whichone)
            {
            case 0: {oneAll.push_back(oneA[a]); ++a; if(a>=oneA.count()) aout=true;}   break;
            case 1: {oneAll.push_back(oneT[t]); ++t; if(t>=oneT.count()) tout=true;}   break;
            case 2: {oneAll.push_back(oneC[c]); ++c; if(c>=oneC.count()) cout=true;}   break;
            case 3: {oneAll.push_back(oneG[g]); ++g; if(g>=oneG.count()) gout=true;}   break;
            }
            //qDebug()<<a<<t<<c<<g<<whichone;
        }
    }
    //qDebug()<<oneAll.count();
    //      QString aa="";
    //    for(int i=1;i<oneAll.count();++i)
    //    {
    //        aa.append("  ");
    //        aa.append(QString::number( oneAll[i].startpos));
    //        aa.append("--");
    //        aa.append(QString::number(oneAll[i].maxsiganl,'f',1));
    //    }
    //     qDebug()<<aa;


    QVector <Peakinfo> Peakinfos;
    QVector <hypeak> hypeakVector;
    //qDebug()<<"one ";
    int temstart,temend;
//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye dierbu ";
    for(int j=1;j<cuv_regionShowData->base[currentfile].count();++j)
    {
        if(cuv_regionShowData->base[currentfile][j].Matchpos==-1) continue;
        temstart=j; break;
    }
    for(int k=cuv_regionShowData->base[currentfile].count()-2;k>temstart;--k)
    {
        if(cuv_regionShowData->base[currentfile][k].Matchpos==-1)continue;
        temend=k;
        break;
    }
    for(int n=temstart;n<=temend+1;++n)
    {
        Peakinfo onepeakinfo;
        Peakinfos.push_back(onepeakinfo);
    }
    //qDebug()<<" temStart"<<temstart<<temend;
    //qDebug()<<"peakinfos count"<<Peakinfos.count();
    int i=currentfile;
    a=0;
    for(int n=temstart,count=0;n<temend&&count<Peakinfos.count();++n,++count)
    {

        Peakinfos[count].Pos=cuv_regionShowDataBak->base[currentfile][n].pos;
        Peakinfos[count].matchPos=cuv_regionShowDataBak->base[currentfile][n].Matchpos;
        for(;a<oneAll.count()-1;)
        {
            if(oneAll[a].startpos>cuv_regionShowDataBak->base[i][n+1].pos)
                break;
            else
            {
                if(oneAll[a].startpos<cuv_regionShowDataBak->base[i][n-1].pos)
                {
                    ++a;
                    continue;
                }
                else if(oneAll[a].startpos>=cuv_regionShowDataBak->base[i][n-1].pos)
                {

                    bool putin=false;
                    if(oneAll[a].endpos<=cuv_regionShowDataBak->base[i][n+1].pos)
                    {
                        Peak tempeak=oneAll[a];
                        Peakinfos[count].PeaksVector.push_back(tempeak);
                        putin =true;
                    }
                    else
                    {
                        if(oneAll[a].endpos<=cuv_regionShowDataBak->base[i][n+2].pos)
                        {
                            if(oneAll[a].startpos>=cuv_regionShowDataBak->base[i][n].pos)
                            {
                                Peak tempeak=oneAll[a];
                                Peakinfos[count+1].PeaksVector.push_back(tempeak);
                                putin =true;
                            }
                            else {

                                int y=0;
                                for(;y<Peakinfos[count].PeaksVector.count();++y)
                                {
                                    if(Peakinfos[count].PeaksVector[y].base==oneAll[a].base)
                                    {
                                        Peak tempeak=oneAll[a];
                                        Peakinfos[count+1].PeaksVector.push_back(tempeak);
                                        putin =true;
                                        break;
                                    }
                                }
                                if(y>=Peakinfos[count].PeaksVector.count())
                                {
                                    hypeak onehy;
                                    onehy.onepeak=oneAll[a];
                                    onehy.startindex=count;
                                    hypeakVector.push_back(onehy);
                                    //qDebug()<<"y==="<<oneAll[a].startpos;
                                    putin =true;
                                }
                            }
                        }
                    }
                    if(putin)
                        ++a;
                    else
                    {
                        break;
                        //qDebug()<<"lost peak of last >n+2"<<currentfile<<oneAll[a].maxsiganl<<oneAll[a].startpos<<oneAll[a].base<<oneAll[a].scroe;

                    }


                }
            }
        }

    }
    for(int i=0;i<Peakinfos.count();++i)
    {
        if(Peakinfos[i].PeaksVector.count()>0)
        {
            int max=0,maxindex=0;
            for(int j=0;j<Peakinfos[i].PeaksVector.count();++j)
            {
                if(Peakinfos[i].PeaksVector[j].maxsiganl>max)
                {
                    max=Peakinfos[i].PeaksVector[j].maxsiganl;
                    maxindex=j;
                }
            }
            Peakinfos[i].maxone=maxindex;
        }
        else
            Peakinfos[i].maxone=0;
    }
    //qDebug()<<"  hyvector  "<<hypeakVector.count();
    //调整峰
    for(int i=0;i<hypeakVector.count();++i)
    {

        if(Peakinfos[hypeakVector[i].startindex].PeaksVector.count()==0)
        {
            Peakinfos[hypeakVector[i].startindex].PeaksVector.push_back(hypeakVector[i].onepeak);
            //qDebug()<<i<<" +  "<<hypeakVector[i].onepeak.base<<hypeakVector[i].onepeak.maxsiganl<<hypeakVector[i].onepeak.startpos;
            continue;
        }
        if(hypeakVector[i].startindex+1>Peakinfos.count()) {continue;}
        if(Peakinfos[hypeakVector[i].startindex+1].PeaksVector.count()==0)
        {
            Peakinfos[hypeakVector[i].startindex+1].PeaksVector.push_back(hypeakVector[i].onepeak);
            // qDebug()<<i<<" +1  "<<hypeakVector[i].onepeak.base<<hypeakVector[i].onepeak.maxsiganl<<hypeakVector[i].onepeak.startpos;
            continue;
        }
        int hy=hypeakVector[i].onepeak.maxsignalpos;
        int  hybefor=Peakinfos[hypeakVector[i].startindex].PeaksVector[Peakinfos[hypeakVector[i].startindex].maxone].maxsignalpos;
        int  hybehind=Peakinfos[hypeakVector[i].startindex+1].PeaksVector[Peakinfos[hypeakVector[i].startindex+1].maxone].maxsignalpos;
        if(hy-hybefor<hybehind-hy)
        {
            Peakinfos[hypeakVector[i].startindex].PeaksVector.push_back(hypeakVector[i].onepeak);
        }
        else
        {
            Peakinfos[hypeakVector[i].startindex+1].PeaksVector.push_back(hypeakVector[i].onepeak);
        }

    }
    //z再次移动峰图

    for(int count=0;count<Peakinfos.count()-1;++count)
    {
        if(Peakinfos[count].PeaksVector.count()>1)
        {
            double leftest=cuv_regionShowDataBak->baseA[currentfile].last().pos;
            for (int i=0;i<Peakinfos[count].PeaksVector.count();++i)
            {
                double temp=Peakinfos[count].PeaksVector[i].maxsignalpos;
                if(temp<leftest)
                    leftest=temp;
            }
            double nextpeakleft=cuv_regionShowDataBak->baseA[currentfile].last().pos;
            for (int i=0;i<Peakinfos[count+1].PeaksVector.count();++i)
            {
                double temp=Peakinfos[count+1].PeaksVector[i].maxsignalpos;
                if(temp<nextpeakleft)
                    nextpeakleft=temp;
            }
            for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
            {
                double temp=Peakinfos[count].PeaksVector[i].maxsignalpos;

                if(temp-leftest>nextpeakleft-temp)
                {
                    bool move=true;
                    for(int i=0;i<cuv_regionShowDataBak->base[currentfile].count();++i)
                    {
                        if(Peakinfos[count].matchPos==cuv_regionShowDataBak->base[currentfile][i].Matchpos)
                        {
                            int abc= cuv_regionShowDataBak->base[currentfile][i].pos-temp;
                            if(abc<0)abc=-abc;
                            if(abc<=3*xAxisExpand) move=false;
                            break;
                        }
                    }
                    if(move)
                    {
                        Peakinfos[count+1].PeaksVector.push_back(Peakinfos[count].PeaksVector[i]);
                        Peakinfos[count].PeaksVector.remove(i);
                        i--;
                    }
                }
            }
        }
    }


    //再次清峰
    for(int count =0;count<Peakinfos.count();++count)
    {
        if(Peakinfos[count].PeaksVector.count()>1)
        {
            double maxsig=0;
            for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
            {
                if(Peakinfos[count].PeaksVector[i].maxsiganl>maxsig)
                    maxsig=Peakinfos[count].PeaksVector[i].maxsiganl;
                for(int j=i+1;j<Peakinfos[count].PeaksVector.count();++j)
                {
                    if(Peakinfos[count].PeaksVector[j].base
                            ==Peakinfos[count].PeaksVector[i].base)
                    {

                        Peakinfos[count].PeaksVector.remove(j);
                        j--;
                        //qDebug()<<" error of  the same peak";
                    }
                }
            }
            for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
            {
                double pre=(double)Peakinfos[count].PeaksVector[i].maxsiganl/maxsig;
                if(pre<0.25)
                {
                    Peakinfos[count].PeaksVector.remove(i);
                    --i;
                }
            }

        }
        while(true)
        {
            if(Peakinfos[count].PeaksVector.count()>2)
            {
                //qDebug()<<"some question"<<Peakinfos[count].PeaksVector.count();
                int min=200,minindex=0;
                for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
                {
                    /*qDebug()<<"  question"<<currentfile<<Peakinfos[count].PeaksVector[i].base
                           <<Peakinfos[count].PeaksVector[i].maxsiganl
                          <<Peakinfos[count].PeaksVector[i].startpos
                         <<Peakinfos[count].PeaksVector[i].endpos<<count;*/
                    if(Peakinfos[count].PeaksVector[i].maxsiganl<min)
                    {
                        min=Peakinfos[count].PeaksVector[i].maxsiganl;
                        minindex=i;
                    }

                }
                Peakinfos[count].PeaksVector.remove(minindex);
            }
            else
                break;
        }

    }
    for(int i=0;i<Peakinfos.count();++i)
    {
        if(Peakinfos[i].PeaksVector.count()>0)
        {
            int max=0,maxindex=0;
            for(int j=0;j<Peakinfos[i].PeaksVector.count();++j)
            {
                if(Peakinfos[i].PeaksVector[j].maxsiganl>max)
                {
                    max=Peakinfos[i].PeaksVector[j].maxsiganl;
                    maxindex=j;
                }
            }
            Peakinfos[i].maxone=maxindex;
        }
        else
            Peakinfos[i].maxone=0;
    }
    //   出现三个峰要移峰

    for(int count =0;count<Peakinfos.count();++count)
    {
        //清除峰
        //        if(Peakinfos[count].matchPos>263&&Peakinfos[count].matchPos<268)
        //        {
        //            for(int i=0;i<cuv_regionShowDataBak->base[currentfile].count();++i)
        //            {
        //                if(Peakinfos[count].matchPos==cuv_regionShowDataBak->base[currentfile][i].Matchpos)
        //                {

        //                    qDebug()<<cuv_regionShowDataBak->base[currentfile][i].pos;
        //                }
        //            }
        //            for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
        //            {
        //                qDebug()<<"     match      "<<Peakinfos[count].matchPos
        //                       <<Peakinfos[count].PeaksVector[i].base<<Peakinfos[count].PeaksVector[i].maxsiganl
        //                      <<Peakinfos[count].PeaksVector[i].scroe<<Peakinfos[count].PeaksVector[i].maxsignalpos;
        //            }
        //        }
        if(Peakinfos[count].PeaksVector.count()==1)
        {
            Peakinfos[count].base=Peakinfos[count].PeaksVector[0].base;
            //qDebug()<<"one "<<Peakinfos[count].base;
        }
        else if(Peakinfos[count].PeaksVector.count()==2)
        {
            if(Peakinfos[count].PeaksVector[0].maxsiganl
                    >Peakinfos[count].PeaksVector[1].maxsiganl)
            {
                Peakinfos[count].percent=Peakinfos[count].PeaksVector[1].maxsiganl/
                        Peakinfos[count].PeaksVector[0].maxsiganl;
            }
            else
            {
                Peakinfos[count].percent=Peakinfos[count].PeaksVector[0].maxsiganl/
                        Peakinfos[count].PeaksVector[1].maxsiganl;
            }

            bool findnow=false;
            if(Peakinfos[count].percent<0.4&&Peakinfos[count].percent>0.25)
            {
                int judgescroe=60;
                if(xAxisExpand!=1)
                    judgescroe=0;
                if(Peakinfos[count].PeaksVector[0].scroe<judgescroe&&Peakinfos[count].PeaksVector[1].scroe<judgescroe)
                {
                    if(Peakinfos[count].PeaksVector[0].scroe<Peakinfos[count].PeaksVector[1].scroe)
                        Peakinfos[count].base=Peakinfos[count].PeaksVector[1].base;
                    else
                        Peakinfos[count].base=Peakinfos[count].PeaksVector[0].base;
                    findnow=true;

                }
                else if(Peakinfos[count].PeaksVector[0].scroe<judgescroe)
                {
                    Peakinfos[count].base=Peakinfos[count].PeaksVector[1].base;
                    findnow=true;
                }
                else if(Peakinfos[count].PeaksVector[1].scroe<judgescroe)
                {
                    Peakinfos[count].base=Peakinfos[count].PeaksVector[0].base;
                    findnow=true;
                }
            }
            if(!findnow)
            {

                //qDebug()<<Peakinfos[count].percent;
                if(Peakinfos[count].percent>=0.25)
                {
                    if((Peakinfos[count].PeaksVector[0].base=='A'&&
                        Peakinfos[count].PeaksVector[1].base=='G')||
                            (Peakinfos[count].PeaksVector[1].base=='A'&&
                             Peakinfos[count].PeaksVector[0].base=='G'))
                    {
                        Peakinfos[count].base='R';
                    }
                    else if((Peakinfos[count].PeaksVector[0].base=='T'&&
                             Peakinfos[count].PeaksVector[1].base=='C')||
                            (Peakinfos[count].PeaksVector[1].base=='T'&&
                             Peakinfos[count].PeaksVector[0].base=='C'))
                    {
                        Peakinfos[count].base='Y';
                    }
                    else if((Peakinfos[count].PeaksVector[0].base=='G'&&
                             Peakinfos[count].PeaksVector[1].base=='T')||
                            (Peakinfos[count].PeaksVector[1].base=='G'&&
                             Peakinfos[count].PeaksVector[0].base=='T'))
                    {
                        Peakinfos[count].base='K';
                    }
                    else if((Peakinfos[count].PeaksVector[0].base=='A'&&
                             Peakinfos[count].PeaksVector[1].base=='C')||
                            (Peakinfos[count].PeaksVector[1].base=='A'&&
                             Peakinfos[count].PeaksVector[0].base=='C'))
                    {
                        Peakinfos[count].base='M';
                    }
                    else if((Peakinfos[count].PeaksVector[0].base=='G'&&
                             Peakinfos[count].PeaksVector[1].base=='C')||
                            (Peakinfos[count].PeaksVector[1].base=='G'&&
                             Peakinfos[count].PeaksVector[0].base=='C'))
                    {
                        Peakinfos[count].base='S';
                    }
                    else if((Peakinfos[count].PeaksVector[0].base=='A'&&
                             Peakinfos[count].PeaksVector[1].base=='T')||
                            (Peakinfos[count].PeaksVector[1].base=='A'&&
                             Peakinfos[count].PeaksVector[0].base=='T'))
                    {
                        Peakinfos[count].base='W';
                    }
                    else
                    {
                        Peakinfos[count].base='L';
                        /* qDebug()<<"don't know "<<Peakinfos[count].PeaksVector[0].base
                               <<Peakinfos[count].PeaksVector[1].base;*/
                    }
                }
                else
                {
                    // qDebug()<<"0.25  error";
                    if(Peakinfos[count].PeaksVector[0].maxsiganl>
                            Peakinfos[count].PeaksVector[1].maxsiganl)
                    {
                        Peakinfos[count].base=Peakinfos[count].PeaksVector[0].base;
                    }
                    else
                        Peakinfos[count].base=Peakinfos[count].PeaksVector[1].base;
                }
            }

        }
        else
        {

            Peakinfos[count].base='L';
            for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
            {
                //qDebug()<<"how many peaks "<<Peakinfos[count].PeaksVector[i].base<<Peakinfos[count].PeaksVector[i].startpos
                //       <<Peakinfos[count].PeaksVector[i].maxsiganl;
            }


        }

    }
    //qDebug()<< "    end one   "<<Peakinfos.count();


    cuv_regionShowData->Peaksinform.push_back(Peakinfos);
    //cuv_regionShowDataBak->Peaksinform.push_back(Peakinfos);

    //    }

//20180608close qDebug()<<"curvedline analysisDataforpeak nidaye xxbu ";


}
/**新增Start**/
void CurvedLine::getAllSoapLetterPos()
{
    for(int i = 0;i < m_pCurvedLinePos->size();i++)
    {
        m_AllLetterList.append(m_pCurvedLinePos->at(i)->getSoapLetterPos());
    }
}
/**新增End**/


void CurvedLine::InsertPeaks(QVector<Peak>&onevector,
                             char base, double max, double min,
                             int startindex, int endindex,
                             double maxsignal,int which,int maxsigpos)
{
    Peak one ;
    one.maxrelat=max;
    one.minrelat=min;
    one.startindex=startindex;
    one.endindex=endindex;
    one.maxsiganl=maxsignal;
    one.base=base;
    one.maxsignalpos=maxsigpos;
    if(min<1&&min>-1&&max>4)
    {
        one.scroe=80;
    }
    else if (min<-4&&max>-1&&max<1)
    {
        one.scroe=80;
    }
    else if (min<-4&&max>4)
    {
        one.scroe=80;
    }
    else if(min<-2&&min>-4&&max<4&&max>2)
    {
        one.scroe=60;
    }
    else if(min<-1&&min>-2&&max<2&&max>1)
    {
        one.scroe=50;
    }
    else if(min<-1&&min>-2&&max>0&&max<1)
    {

        one.scroe=40;
    }
    else if(min<0&&min>-1&&max<2&&max>1)
    {
        one.scroe=40;
    }
    else if(min<-1&&max>=4)
    {
        one.scroe=90;
    }
    else if (min<=-4&&max>1)
    {
        one.scroe=90;
    }
    else if(min<1&&min>=-2&&max<4&&max>=2)
    {
        one.scroe=59;
    }
    else if(min<=-2&&min>-4&&max<=2&&max>-1)
    {
        one.scroe=59;
    }
    else
    {
        //qDebug()<<base<<","<<min<<","<<max<<","<<startindex<<","<<endindex;
        //qDebug()<<one.base<<QString::number( max,'f',2)<<QString::number(min,'f',2) ;
        for(int i=0;i<cuv_regionShowData->base[which].count();++i)
        {
            if(cuv_regionShowDataBak->base[which][i].pos>cuv_regionShowDataBak->baseA[which][one.startindex].pos
                    &&cuv_regionShowDataBak->base[which][i].pos<cuv_regionShowDataBak->baseA[which][one.endindex].pos)
            {
                //qDebug()<<"know"<<cuv_regionShowDataBak->base[which][i].Matchpos
                //      <<cuv_regionShowDataBak->base[which][i].basetype<<base<<maxsignal<<maxsigpos
                //     <<cuv_regionShowDataBak->base[which][i].pos;
            }
        }
        return ;
    }
    switch(base)
    {
    case 'A':{
        one.startpos=cuv_regionShowDataBak->baseA[which][one.startindex].pos;
        one.endpos=cuv_regionShowDataBak->baseA[which][one.endindex].pos;
        onevector.push_back(one);
        break;}
    case 'T':{
        one.startpos=cuv_regionShowDataBak->baseT[which][one.startindex].pos;
        one.endpos=cuv_regionShowDataBak->baseT[which][one.endindex].pos;
        onevector.push_back(one);
        break;}
    case 'C':{
        one.startpos=cuv_regionShowDataBak->baseC[which][one.startindex].pos;
        one.endpos=cuv_regionShowDataBak->baseC[which][one.endindex].pos;
        onevector.push_back(one);
        break;}
    case 'G':{
        one.startpos=cuv_regionShowDataBak->baseG[which][one.startindex].pos;
        one.endpos=cuv_regionShowDataBak->baseG[which][one.endindex].pos;
        onevector.push_back(one);
        break;}
    }
}





void CurvedLine::analysisData()
{
    if(grabWhichone<1) return;
    if(exonPos<1) return ;

    QVector <Each_Base_Signal>
            &baseG =cuv_regionShowData->baseG[currentfile],
            &baseA =cuv_regionShowData->baseA[currentfile],
            &baseT =cuv_regionShowData->baseT[currentfile],
            &baseC =cuv_regionShowData->baseC[currentfile];

    if(saveanaly!=NULL) {delete saveanaly ;saveanaly=NULL;}
    int imagewitdh= (endexon-startexon)*5;
    int myhight=200;
    saveanaly=new QImage(imagewitdh,200,QImage::Format_ARGB32);
    QPainter  painter(saveanaly);
    QVector <BaseInfor>& base=cuv_regionShowData->base[currentfile];
    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(0,0,imagewitdh+10,myhight);
    painter.drawText(2,LineHight-2,QString::number(indexnow-1));
    painter.drawText((base[indexnow].pos-base[indexnow-1].pos)*regionShow_xAxisExpand,LineHight-2,QString::number(indexnow));
    painter.drawText((base[indexnow+1].pos-base[indexnow-1].pos)*regionShow_xAxisExpand-16,LineHight-2,QString::number(indexnow+1));

    SetColor(& painter,base[indexnow-1].basetype);
    painter.drawText(2,2*LineHight-2,QString(base[indexnow-1].basetype));
    SetColor(& painter,base[indexnow].basetype);
    painter.drawText((base[indexnow].pos-base[indexnow-1].pos)*regionShow_xAxisExpand,2*LineHight-2,QString(base[indexnow].basetype));
    SetColor(& painter,base[indexnow+1].basetype);
    painter.drawText((base[indexnow+1].pos-base[indexnow-1].pos)*regionShow_xAxisExpand-16,2*LineHight-2,QString(base[indexnow+1].basetype));
    painter.translate(0,myhight);
    double y1=0,y2=0,higthpre=0,x1=0,x2=0;
    double min=0,max=0,maxhight=0;
    int Starpos;
    int j=0,k=0,relaStartPos=0,relaEndpos=0;
    for(int i=0;i<cuv_regionShowData->baseA[currentfile].count();++i)
    {
        if( cuv_regionShowData->base[currentfile][indexnow].pos-
                cuv_regionShowData->baseA[currentfile][i].pos<5
                )
        {
            relaStartPos=--i;
            break;
        }

    }
    for(int i=0;i<cuv_regionShowData->baseA[currentfile].count();++i)
    {
        if(cuv_regionShowData->baseA[currentfile][i].pos-
                cuv_regionShowData->base[currentfile][indexnow].pos>5)
        {
            relaEndpos=--i;
            break;
        }


    }
    Starpos=baseA[--relaStartPos].pos*regionShow_xAxisExpand;
    for(int i=0;i<cuv_regionShowData->baseA[currentfile].count();++i)
    {
        if(cuv_regionShowData->baseA[currentfile][i].pos*regionShow_xAxisExpand>start.x())
        {
            j=--i;
            break;
        }

    }
    for(int i=0;i<cuv_regionShowData->baseA[currentfile].count();++i)
    {
        if(cuv_regionShowData->baseA[currentfile][i].pos*regionShow_xAxisExpand>end.x())
        {
            k=i;
            break;
        }
    }

    painter.setPen(QPen(Acolor , 1  ));

    for(int i=j;i<k;++i)
    {

        x1=regionShow_xAxisExpand * baseA[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseA[i+1].pos-Starpos;
        y1=- baseA[i].signal*compress;
        y2=- baseA[i+1].signal*compress;
        if(grabWhichone==1)
        {
            //qDebug()<<-y1;
            double value=y1-y2;
            if(value<min)min=value;
            if(value>max)max=value;
            if(-y1>maxhight) maxhight=-y1;
        }
        if(x1>0)
        {
            painter.drawLine(x1,y1,x2,y2);
        }
    }


    painter.setPen(QPen(Tcolor , 1  ));
    //Starpos=baseT[j-1].pos*regionShow_xAxisExpand;
    for(int i=j;i<k;++i)
    {

        x1=regionShow_xAxisExpand * baseT[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseT[i+1].pos-Starpos;
        y1=- baseT[i].signal*compress;
        y2=- baseT[i+1].signal*compress;
        if(grabWhichone==2)
        {
            //qDebug()<<-y1;
            double value=y1-y2;
            if(value<min)min=value;
            if(value>max)max=value;
            if(-y1>maxhight) maxhight=-y1;
        }
        if(x1>0)
        {
            painter.drawLine(x1,y1,x2,y2);
        }
    }

    painter.setPen(QPen(Ccolor , 1  ));
    //Starpos=baseC[j-1].pos*regionShow_xAxisExpand;
    for(int i=j;i<k;++i)
    {

        x1=regionShow_xAxisExpand * baseC[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseC[i+1].pos-Starpos;
        y1=- baseC[i].signal*compress;
        y2=- baseC[i+1].signal*compress;
        if(grabWhichone==3)
        {
            //qDebug()<<-y1;
            double value=y1-y2;

            if(value<min)min=value;
            if(value>max)max=value;
            if(-y1>maxhight) maxhight=-y1;
        }
        if(x1>0)
        {
            painter.drawLine(x1,y1,x2,y2);
        }
    }

    painter.setPen(QPen(Gcolor , 1  ));
    //Starpos=baseG[j-1].pos*regionShow_xAxisExpand;
    for(int i=j;i<k;++i)
    {

        x1=regionShow_xAxisExpand * baseG[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseG[i+1].pos-Starpos;
        y1=- baseG[i].signal*compress;
        y2=- baseG[i+1].signal*compress;
        if(grabWhichone==4)
        {
            //qDebug()<<-y1;
            double value=y1-y2;
            if(value<min)min=value;
            if(value>max)max=value;
            if(-y1>maxhight) maxhight=-y1;
        }
        if(x1>0)
        {
            painter.drawLine(x1,y1,x2,y2);
        }
    }


    double relativemax=0;
    if(ishyorho)
    {
        if(relaStartPos>j)relaStartPos=j;
        if(relaEndpos<k)relaEndpos=k;
        for(int i=relaStartPos;i<relaEndpos;++i)
        {
            if(baseG[i].signal*compress>relativemax)relativemax=baseG[i].signal*compress;
            if(baseA[i].signal*compress>relativemax)relativemax=baseA[i].signal*compress;
            if(baseT[i].signal*compress>relativemax)relativemax=baseT[i].signal*compress;
            if(baseC[i].signal*compress>relativemax)relativemax=baseC[i].signal*compress;
            /*qDebug()<<baseT[i].signal*compress<<" T "<<baseG[i].signal*compress<<" G "
                   <<baseA[i].signal*compress<<" A "<<baseC[i].signal*compress<<"C";*/
        }

    }
    painter.drawText(2,LineHight*3,QString::number(min,'f',2));
    painter.drawText(20,LineHight*3,QString::number(max,'f',2));
    //qDebug()<<maxhight<<" max&rela  "<<relativemax;
    if(relativemax!=0)
    {
        higthpre=maxhight/relativemax *100.0;
        painter.drawText(40,LineHight*3,QString::number(higthpre,'f',2));
    }

    emit signalgrabpostoscore(min,max,higthpre,saveanaly);

}


void CurvedLine::mousePressEvent(QMouseEvent * event)
{
    pressed=true;
    StartPos.setX(event->pos().x());
    StartPos.setY(event->pos().y());
    StartPos=this->mapToParent(StartPos);
}

void CurvedLine::mouseReleaseEvent(QMouseEvent * event)
{
    if(pressed&&moved)
    {
        endPos.setX(event->pos().x());
        endPos.setY(event->pos().y());
        frame->hide();
        QClipboard *board = QApplication::clipboard();
        board->setText(getbasesequence());
    }
    pressed=false;
    moved=false;
    const  QPoint mousepoint=this->mapToParent(event->pos());
    e=new QMouseEvent(event->type(), mousepoint,event->button(),event->buttons(),event->modifiers());
    emit mouserelease(e);
}


void CurvedLine::mouseMoveEvent(QMouseEvent * event)
{
    if(pressed)
    {
        moved=true;
        middlePos.setX(event->pos().x());
        middlePos.setY(event->pos().y());
        middlePos=this->mapToParent(middlePos);
        if(middlePos.y()<StartPos.y()||middlePos.x()<StartPos.x())
        {
            moved=false;
        }
        else
        {
            frame->move(StartPos);
            frame->setFixedSize(middlePos.x()-StartPos.x(),middlePos.y()-StartPos.y());
            frame->show();
        }
    }
}
QString CurvedLine::getbasesequence()
{
    QString  base;
    int i=-1,j=-1;
    PosToExonPos(StartPos.x(),i);
    PosToExonPos(endPos.x(),j);
    //qDebug()<<i<<j;
    if(i<0||j<0) return "";
    while(i<=j)
    {
        base.append( cuv_regionShowData->base[currentfile][i].basetype);
        ++i;
    }
    return base;

}

void CurvedLine :: PosToExonPos( int  MoseClickPos ,int &exonPos )//根据鼠标位置找到ba1文件中对应的碱基
{
    try{
        int i =currentfile ;/*(int)( FocusMarkyPos / ( 3*LineHight + regionShow_curveLineHigh ) )*/
        double FocusMarkxPos2 =
                MoseClickPos/regionShow_xAxisExpand;
        if( ( FocusMarkxPos2 >= cuv_regionShowData->leftLimits[i]  )
                && ( FocusMarkxPos2 <= cuv_regionShowData->rightLimits[i] ) )
        {
            int basePosSize = cuv_regionShowData->base[i].size();
            int j=0;
            for ( ; j < basePosSize; j++ )
            {
                if ( cuv_regionShowData->base[i][j].Matchpos != -1 &&
                     cuv_regionShowData->base[i][j].pos
                     >= FocusMarkxPos2)
                {
                    break;
                }
            }
            if(j == basePosSize)
            {
                exonPos = -1;

                return;
            }
            int h = j-1;
            for( ; h >= 0 ; h-- )
            {
                if( cuv_regionShowData->base[i][h].Matchpos != -1 )
                {
                    break;
                }
            }
            if(h == -1)
            {
                exonPos = h;

                return;
            }
            if( ( FocusMarkxPos2 - cuv_regionShowData->base[i][h].pos ) >
                    ( cuv_regionShowData->base[i][j].pos
                      - cuv_regionShowData->base[i][h].pos)/2 )
            {
                if( cuv_regionShowData->base[i][j].pos
                        > cuv_regionShowData->rightLimits[i] )
                {
                    //                    exonPos = cuv_regionShowData->base[i][h].pos;
                    //                    startexon=cuv_regionShowData->base[i][h-1].pos;
                    //                    endexon=cuv_regionShowData->base[i][h+1].pos;
                    exonPos=h;
                    //baseMatchConsensusPoss[h].toInt();
                    return;
                }
                //                exonPos = cuv_regionShowData->base[i][j].pos;
                //                startexon=cuv_regionShowData->base[i][j-1].pos;
                //                endexon=cuv_regionShowData->base[i][j+1].pos;
                exonPos=j;
                //baseMatchConsensusPoss[j].toInt();
                return;
            }
            else
            {
                if( cuv_regionShowData->base[i][j-1].pos
                        < cuv_regionShowData->leftLimits[i] )
                {
                    //                    exonPos =cuv_regionShowData->base[i][j].pos;
                    //                    startexon=cuv_regionShowData->base[i][j-1].pos;
                    //                    endexon=cuv_regionShowData->base[i][j+1].pos;
                    exonPos=j;
                    //baseMatchConsensusPoss[j].toInt();
                    return;
                }
                //                exonPos =cuv_regionShowData->base[i][h].pos;
                //                startexon=cuv_regionShowData->base[i][h-1].pos;
                //                endexon=cuv_regionShowData->base[i][h+1].pos;
                exonPos=h;
                //baseMatchConsensusPoss[h].toInt();
                return;
            }
        }

        else
        {
            exonPos = -1;
            return;
        }
    }
    catch(...)
    {
        ErrorWrite::errorocur("ab1PosToExonPos");
    }
}


void CurvedLine :: ab1PosToExonPos( int  MoseClickPos )//根据鼠标位置找到ba1文件中对应的碱基
{
    try{
        int i =currentfile ;/*(int)( FocusMarkyPos / ( 3*LineHight + regionShow_curveLineHigh ) )*/
        double FocusMarkxPos2 =
                MoseClickPos/regionShow_xAxisExpand;
        if( ( FocusMarkxPos2 >= cuv_regionShowData->leftLimits[i]  )
                && ( FocusMarkxPos2 <= cuv_regionShowData->rightLimits[i] ) )
        {
            int basePosSize = cuv_regionShowData->base[i].size();
            int j=0;
            for ( ; j < basePosSize; j++ )
            {
                if ( cuv_regionShowData->base[i][j].Matchpos != -1 &&
                     cuv_regionShowData->base[i][j].pos
                     >= FocusMarkxPos2)
                {
                    break;
                }
            }
            if(j == basePosSize)
            {
                exonPos = -1;
                indexnow=j;
                return;
            }
            int h = j-1;
            for( ; h >= 0 ; h-- )
            {
                if( cuv_regionShowData->base[i][h].Matchpos != -1 )
                {
                    break;
                }
            }
            if(h == -1)
            {
                exonPos = -1;
                indexnow=h;
                return;
            }
            if( ( FocusMarkxPos2 - cuv_regionShowData->base[i][h].pos ) >
                    ( cuv_regionShowData->base[i][j].pos
                      - cuv_regionShowData->base[i][h].pos)/2 )
            {
                if( cuv_regionShowData->base[i][j].pos
                        > cuv_regionShowData->rightLimits[i] )
                {
                    exonPos = cuv_regionShowData->base[i][h].pos;
                    startexon=cuv_regionShowData->base[i][h-1].pos;
                    endexon=cuv_regionShowData->base[i][h+1].pos;
                    indexnow=h;
                    //baseMatchConsensusPoss[h].toInt();
                    return;
                }
                exonPos = cuv_regionShowData->base[i][j].pos;
                startexon=cuv_regionShowData->base[i][j-1].pos;
                endexon=cuv_regionShowData->base[i][j+1].pos;
                indexnow=j;
                //baseMatchConsensusPoss[j].toInt();
                return;
            }
            else
            {
                if( cuv_regionShowData->base[i][j-1].pos
                        < cuv_regionShowData->leftLimits[i] )
                {
                    exonPos =cuv_regionShowData->base[i][j].pos;
                    startexon=cuv_regionShowData->base[i][j-1].pos;
                    endexon=cuv_regionShowData->base[i][j+1].pos;
                    indexnow=j;
                    //baseMatchConsensusPoss[j].toInt();
                    return;
                }
                exonPos =cuv_regionShowData->base[i][h].pos;
                startexon=cuv_regionShowData->base[i][h-1].pos;
                endexon=cuv_regionShowData->base[i][h+1].pos;
                indexnow=h;
                //baseMatchConsensusPoss[h].toInt();
                return;
            }
        }

        else
        {
            exonPos = -1;
            return;
        }
    }
    catch(...)
    {
        ErrorWrite::errorocur("ab1PosToExonPos");
    }
}
/**新增Start**/
void CurvedLine::slotbasechanged(char c, int a, int b)
{
    drawAll();
    repaint();
}
/**新增End**/
void CurvedLine::reDrawAll()
{
    drawAll();
    repaint();
}

void CurvedLine::drawContents1(  QVector <Each_Base_Signal> &baseT ,
                                 QVector <Each_Base_Signal> &baseC,
                                 QVector <Each_Base_Signal> &baseG ,
                                 QVector <Each_Base_Signal> &baseA ,
                                 QPaintDevice * drawmap,int Starpos)
{


    double x1,x2,y1,y2;
    int myhight=200;
    //compress=((double)(myhight-3*LineHight)/maxValue)*regionShow_yAxisExpand;
    QVector <BaseInfor>& base=cuv_regionShowData->base[currentfile];
    Starpos=0;
    regionShow_xAxisExpand=5;

    for(int j=1;j<base.count()-1;++j)
    {
        double imagewitdh=(base[j+1].pos-base[j-1].pos)*regionShow_xAxisExpand;
        QImage * mydrawmap=new QImage(imagewitdh+10,
                                      myhight,QImage::Format_ARGB32);
        QImage * mydrawmap2=new QImage(imagewitdh*2+50,
                                       myhight,QImage::Format_ARGB32);
        QPainter  painter(mydrawmap);
        QPainter  painter2(mydrawmap2);

        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawRect(0,0,imagewitdh+10,myhight);
        painter2.setBrush(QBrush(QColor(255,255,255)));
        painter2.drawRect(0,0,imagewitdh*2+50,myhight);



        painter.drawText(2,LineHight-2,QString::number(j-1));
        painter.drawText((base[j].pos-base[j-1].pos)*regionShow_xAxisExpand,LineHight-2,QString::number(j));
        painter.drawText((base[j+1].pos-base[j-1].pos)*regionShow_xAxisExpand-16,LineHight-2,QString::number(j+1));

        SetColor(& painter,base[j-1].basetype);
        painter.drawText(2,2*LineHight-2,QString(base[j-1].basetype));
        SetColor(& painter,base[j].basetype);
        painter.drawText((base[j].pos-base[j-1].pos)*regionShow_xAxisExpand,2*LineHight-2,QString(base[j].basetype));
        SetColor(& painter,base[j+1].basetype);
        painter.drawText((base[j+1].pos-base[j-1].pos)*regionShow_xAxisExpand-16,2*LineHight-2,QString(base[j+1].basetype));

        painter.translate(0,myhight);
        painter2.translate(0,myhight);
        /*
        painter.setPen(Gcolor);
        for (int i =0; i<baseG.size()-1;++i)
        {
            if(baseG[i].pos>base[j+1].pos) break;
            if(baseG[i].pos<base[j-1].pos) continue;
            Starpos=base[j-1].pos*regionShow_xAxisExpand;
            x1=regionShow_xAxisExpand * baseG[i].pos-Starpos;
            x2=regionShow_xAxisExpand * baseG[i+1].pos-Starpos;
            y1=- baseG[i].signal*compress;
            y2=- baseG[i+1].signal*compress;
            if(x1>0)
            {
                painter.drawLine(x1,y1,x2,y2);
            }

        }

       painter.setPen(Acolor);
       for (int i =0; i<baseA.size()-1;++i)
        {
          if(baseA[i].pos<base[j-1].pos) continue;
          if(baseA[i].pos>base[j+1].pos) break;
          Starpos=base[j-1].pos*regionShow_xAxisExpand;
          x1=regionShow_xAxisExpand * baseA[i].pos-Starpos;
          x2=regionShow_xAxisExpand * baseA[i+1].pos-Starpos;
          y1=- baseA[i].signal*compress;
          y2=- baseA[i+1].signal*compress;
          if(x1>0)
          {
              painter.drawLine(x1,y1,x2,y2);
          }
        }
      painter.setPen(Tcolor);
      for (int i =0; i<baseT.size()-1;++i)
       {
          if(baseT[i].pos<base[j-1].pos) continue;
          if(baseT[i].pos>base[j+1].pos) break;
          Starpos=base[j-1].pos*regionShow_xAxisExpand;
          x1=regionShow_xAxisExpand * baseT[i].pos-Starpos;
          x2=regionShow_xAxisExpand * baseT[i+1].pos-Starpos;
          y1=- baseT[i].signal*compress;
          y2=- baseT[i+1].signal*compress;
          if(x1>0)
          {
              painter.drawLine(x1,y1,x2,y2);
          }
       }
      painter.setPen(Ccolor);
      for (int i =0; i<baseC.size()-1;++i)
       {
          if(baseC[i].pos<base[j-1].pos) continue;
          if(baseC[i].pos>base[j+1].pos) break;
          Starpos=base[j-1].pos*regionShow_xAxisExpand;
          x1=regionShow_xAxisExpand * baseC[i].pos-Starpos;
          x2=regionShow_xAxisExpand * baseC[i+1].pos-Starpos;
          y1=- baseC[i].signal*compress;
          y2=- baseC[i+1].signal*compress;
          if(x1>0)
          {
            painter.drawLine(x1,y1,x2,y2);
          }

       }*/
        painter.setPen(QPen(Gcolor , 2  ));
        painter2.setPen(QPen(Gcolor , 1  ));
        //painter.setPen(Gcolor);
        //imagewitdh=imagewitdh+20;
        for (int i =0,m=0,n=0; i<baseG.size()-1;++i)
        {
            if(baseG[i].pos>base[j+1].pos) break;
            if(baseG[i].pos<base[j-1].pos) continue;
            Starpos=base[j-1].pos*regionShow_xAxisExpand;
            x1=regionShow_xAxisExpand * baseG[i].pos-Starpos;
            x2=regionShow_xAxisExpand * baseG[i+1].pos-Starpos;
            y1=- baseG[i].signal*compress;
            y2=- baseG[i+1].signal*compress;
            double value=y2-y1;
            if(x1>0)
            {
                ++n;
                painter.drawPoint(x1,y1);
                painter.drawPoint(x2,y2);
                QString strvalue=QString::number(-value,'f',1);
                for(int i=0 ;i<2/*strvalue.length()*/;++i)
                {
                    painter2.drawText(10+x2*2,y2-4*10+i*10,QString(strvalue[i]));
                }
            }

        }
        painter.setPen(QPen(Acolor , 2  ));
        painter2.setPen(QPen(Acolor , 1  ));
        for (int i =0,m=0,n=0; i<baseA.size()-1;++i)
        {
            if(baseA[i].pos<base[j-1].pos) continue;
            if(baseA[i].pos>base[j+1].pos) break;
            Starpos=base[j-1].pos*regionShow_xAxisExpand;
            x1=regionShow_xAxisExpand * baseA[i].pos-Starpos;
            x2=regionShow_xAxisExpand * baseA[i+1].pos-Starpos;
            y1=- baseA[i].signal*compress;
            y2=- baseA[i+1].signal*compress;
            double value=y2-y1;
            if(x1>0)
            {
                ++n;
                painter.drawPoint(x1,y1);
                painter.drawPoint(x2,y2);
                QString strvalue=QString::number(-value,'f',1);
                for(int i=0 ;i<2/*strvalue.length()*/;++i)
                {
                    painter2.drawText(10+x2*2,y2-4*10+i*10,QString(strvalue[i]));
                }

            }
        }
        painter.setPen(QPen(Tcolor , 2  ));
        painter2.setPen(QPen(Tcolor , 1  ));
        for (int i =0,m=0,n=0; i<baseT.size()-1;++i)
        {
            if(baseT[i].pos<base[j-1].pos) continue;
            if(baseT[i].pos>base[j+1].pos) break;
            Starpos=base[j-1].pos*regionShow_xAxisExpand;
            x1=regionShow_xAxisExpand * baseT[i].pos-Starpos;
            x2=regionShow_xAxisExpand * baseT[i+1].pos-Starpos;
            y1=- baseT[i].signal*compress;
            y2=- baseT[i+1].signal*compress;
            double value=y2-y1;
            if(x1>0)
            {
                ++n;
                painter.drawPoint(x1,y1);
                painter.drawPoint(x2,y2);
                QString strvalue=QString::number(-value,'f',1);
                for(int i=0 ;i<2/*strvalue.length()*/;++i)
                {
                    painter2.drawText(10+x2*2,y2-4*10+i*10,QString(strvalue[i]));
                }
            }
        }
        painter.setPen(QPen(Ccolor , 2  ));
        painter2.setPen(QPen(Ccolor , 1  ));
        for (int i =0,m=0,n=0; i<baseC.size()-1;++i)
        {
            if(baseC[i].pos<base[j-1].pos) continue;
            if(baseC[i].pos>base[j+1].pos) break;
            Starpos=base[j-1].pos*regionShow_xAxisExpand;
            x1=regionShow_xAxisExpand * baseC[i].pos-Starpos;
            x2=regionShow_xAxisExpand * baseC[i+1].pos-Starpos;
            y1=- baseC[i].signal*compress;
            y2=- baseC[i+1].signal*compress;
            double value=y2-y1;
            if(x1>0)
            {
                ++n;
                painter.drawPoint(x1,y1);
                painter.drawPoint(x2,y2);
                QString strvalue=QString::number(-value,'f',1);
                for(int i=0 ;i<2/*strvalue.length()*/;++i)
                {
                    painter2.drawText(10+x2*2,y2-4*10+i*10,QString(strvalue[i]));
                }
            }

        }
        if(base[j].Matchpos>0)
        {
            QString aa="img/";
            cuv_regionShowData->base[currentfile];
            aa.append(((cuv_regionShowData->fileNames[currentfile].split("_"))[0]));
            aa.append("_exon");
            aa.append(QString::number( cuv_regionShowData->infor[currentfile].exonnum));
            aa.append("_pos");
            aa.append(QString::number(base[j].Matchpos));
            aa.append("_file");
            aa.append(QString::number(currentfile));
            aa.append("_pos2");
            aa.append(QString::number(j));
            aa.append(".jpg");
            mydrawmap->save(aa);
            QString aa2="img/";
            cuv_regionShowData->base[currentfile];
            aa2.append(((cuv_regionShowData->fileNames[currentfile].split("_"))[0]));
            aa2.append("_exon");
            aa2.append(QString::number( cuv_regionShowData->infor[currentfile].exonnum));
            aa2.append("_pos");
            aa2.append(QString::number(base[j].Matchpos));
            aa2.append("_file");
            aa2.append(QString::number(currentfile));
            aa2.append("_pos2");
            aa2.append(QString::number(j));
            aa2.append("_");
            aa2.append(QString::number(0));
            aa2.append(".jpg");
            mydrawmap2->save(aa2);

        }
    }

}



void CurvedLine::drawContents(  QVector <Each_Base_Signal> &baseT ,
                                QVector <Each_Base_Signal> &baseC,
                                QVector <Each_Base_Signal> &baseG ,
                                QVector <Each_Base_Signal> &baseA ,
                                QPaintDevice * drawmap,int Starpos)
{
    QPainter  painter(drawmap);
    //    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(0,CurvedHight-3*LineHight);

    double x1,x2,y1,y2;

    // qDebug()<<regionShow_yAxisExpand<<"   compress  "<<compress;
    /*
    painter.setPen(QPen(Qt::black , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksG[currentfile].count();++i)
    {

        x1= baseG[cuv_regionShowData->PeaksG[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseG[cuv_regionShowData->PeaksG[currentfile][i].startindex].signal*compress;
        x2= baseG[cuv_regionShowData->PeaksG[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseG[cuv_regionShowData->PeaksG[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint( x1,y1);
            //painter.drawPoint(x2,y2);
        }
    }
    painter.setPen(QPen(Qt::red , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksG[currentfile].count();++i)
    {

        x1= baseG[cuv_regionShowData->PeaksG[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseG[cuv_regionShowData->PeaksG[currentfile][i].startindex].signal*compress;
        x2= baseG[cuv_regionShowData->PeaksG[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseG[cuv_regionShowData->PeaksG[currentfile][i].endindex].signal*compress;
        if(x2>0){
            //painter.drawPoint( x1,y1);
            painter.drawPoint(x2,y2);
        }
    }
    painter.setPen(QPen(Qt::black , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksA[currentfile].count();++i)
    {

        x1= baseA[cuv_regionShowData->PeaksA[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseA[cuv_regionShowData->PeaksA[currentfile][i].startindex].signal*compress;
        x2= baseA[cuv_regionShowData->PeaksA[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseA[cuv_regionShowData->PeaksA[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint( x1,y1);

            //painter.drawPoint(x2,y2);
        }
    }
    painter.setPen(QPen(Qt::red , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksA[currentfile].count();++i)
    {

        x1= baseA[cuv_regionShowData->PeaksA[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseA[cuv_regionShowData->PeaksA[currentfile][i].startindex].signal*compress;
        x2= baseA[cuv_regionShowData->PeaksA[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseA[cuv_regionShowData->PeaksA[currentfile][i].endindex].signal*compress;
        if(x2>0){
            //painter.drawPoint( x1,y1);
            painter.drawPoint(x2,y2);
        }
    } painter.setPen(QPen(Qt::black , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksT[currentfile].count();++i)
    {

        x1= baseT[cuv_regionShowData->PeaksT[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseT[cuv_regionShowData->PeaksT[currentfile][i].startindex].signal*compress;
        x2= baseT[cuv_regionShowData->PeaksT[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseT[cuv_regionShowData->PeaksT[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint( x1,y1);
            //painter.drawPoint(x2,y2);
        }
    }
    painter.setPen(QPen(Qt::red , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksT[currentfile].count();++i)
    {

        x1= baseT[cuv_regionShowData->PeaksT[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseT[cuv_regionShowData->PeaksT[currentfile][i].startindex].signal*compress;
        x2= baseT[cuv_regionShowData->PeaksT[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseT[cuv_regionShowData->PeaksT[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint(x2,y2);
        }
    } painter.setPen(QPen(Qt::black , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksC[currentfile].count();++i)
    {

        x1= baseC[cuv_regionShowData->PeaksC[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseC[cuv_regionShowData->PeaksC[currentfile][i].startindex].signal*compress;
        x2= baseC[cuv_regionShowData->PeaksC[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseC[cuv_regionShowData->PeaksC[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint( x1,y1);
        }
    }
    painter.setPen(QPen(Qt::red , 5  ));
    for(int i=0;i<cuv_regionShowData->PeaksC[currentfile].count();++i)
    {

        x1= baseC[cuv_regionShowData->PeaksC[currentfile][i].startindex].pos*regionShow_xAxisExpand-Starpos;
        y1=-baseC[cuv_regionShowData->PeaksC[currentfile][i].startindex].signal*compress;
        x2= baseC[cuv_regionShowData->PeaksC[currentfile][i].endindex].pos*regionShow_xAxisExpand-Starpos;
        y2=-baseC[cuv_regionShowData->PeaksC[currentfile][i].endindex].signal*compress;
        if(x2>0){
            painter.drawPoint(x2,y2);
        }
    }
    */
    painter.setPen(Gcolor);
    for (int i =0; i<baseG.size()-1;++i)
    {

        x1=regionShow_xAxisExpand * baseG[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseG[i+1].pos-Starpos;
        y1=- baseG[i].signal*compress;
        y2=- baseG[i+1].signal*compress;
        if(x2>0)
            painter.drawLine(x1,y1,x2,y2);
        if(x1>LENIMAGE) break;

    }
    painter.setPen(Acolor);
    for (int i =0; i<baseA.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseA[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseA[i+1].pos-Starpos;
        y1=- baseA[i].signal*compress;
        y2=- baseA[i+1].signal*compress;
        if(x2>0)
            painter.drawLine(x1,y1,x2,y2);
        if(x1>LENIMAGE) break;
    }
    painter.setPen(Tcolor);
    for (int i =0; i<baseT.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseT[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseT[i+1].pos-Starpos;
        y1=- baseT[i].signal*compress;
        y2=- baseT[i+1].signal*compress;
        if(x2>0)
            painter.drawLine(x1,y1,x2,y2);
        if(x1>LENIMAGE) break;
    }
    painter.setPen(Ccolor);
    for (int i =0; i<baseC.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseC[i].pos-Starpos;
        x2=regionShow_xAxisExpand * baseC[i+1].pos-Starpos;
        y1=- baseC[i].signal*compress;
        y2=- baseC[i+1].signal*compress;
        if(x2>0)
            painter.drawLine(x1,y1,x2,y2);
        if(x1>LENIMAGE) break;

    }

}



QSize CurvedLine :: minimumSizeHint() const
{
    return QSize( CurvedWidth , CurvedHight );
}


QSize CurvedLine :: sizeHint() const
{
    return QSize( CurvedWidth , CurvedHight );
}


QSize CurvedLine :: maximumSize() const
{
    return QSize( CurvedWidth , CurvedHight );
}



void CurvedLine :: paintEvent( QPaintEvent *event )
{
    QWidget :: paintEvent( event );
    QPainter painter( this );
    drawFileNameAndNum(&painter);
    painter.setPen( Qt::gray );
    painter.setBrush( Qt::gray );
    painter.drawRect( 0 , 3*LineHight , cuvLeftLimit , height()-3*LineHight );
    //qDebug()<<"linyi"<<cuvLeftExclude<<"  "<<cuvRightExclude<<"  "<<cuvLeftLimit<<"  "<<cuvRightLimit;
    if(cuvLeftExclude>cuvLeftLimit&&cuvLeftExclude<cuvRightLimit)
        painter.drawRect(cuvLeftLimit,3*LineHight,cuvLeftExclude-cuvLeftLimit,height()-3*LineHight );
    painter.drawRect( cuvRightLimit , 3*LineHight , CurvedWidth - cuvRightLimit , height()-3*LineHight );
    if(cuvRightExclude<cuvRightLimit&&cuvRightExclude>cuvLeftLimit)
        painter.drawRect( cuvRightExclude , 3*LineHight ,cuvRightLimit-cuvRightExclude , height()-3*LineHight );

#ifdef Q_WS_MAC
    QVector <Each_Base_Signal>
            &baseG =cuv_regionShowData->baseG[currentfile],
            &baseA =cuv_regionShowData->baseA[currentfile],
            &baseT =cuv_regionShowData->baseT[currentfile],
            &baseC =cuv_regionShowData->baseC[currentfile];
    double x1,x2,y1,y2;
    painter.setPen(Gcolor);
    for (int i =0; i<baseG.size()-1;++i)
    {

        x1=regionShow_xAxisExpand * baseG[i].pos;
        x2=regionShow_xAxisExpand * baseG[i+1].pos;
        y1=CurvedHight-(baseG[i].signal*compress);
        y2=CurvedHight-(baseG[i+1].signal*compress);
        painter.drawLine(x1,y1,x2,y2);

    }
    painter.setPen(Acolor);
    for (int i =0; i<baseA.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseA[i].pos;
        x2=regionShow_xAxisExpand * baseA[i+1].pos;
        y1=CurvedHight-(baseA[i].signal*compress);
        y2=CurvedHight-(baseA[i+1].signal*compress);
        painter.drawLine(x1,y1,x2,y2);
    }
    painter.setPen(Tcolor);
    for (int i =0; i<baseT.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseT[i].pos;
        x2=regionShow_xAxisExpand * baseT[i+1].pos;
        y1=CurvedHight-(baseT[i].signal*compress);
        y2=CurvedHight-(baseT[i+1].signal*compress);
        painter.drawLine(x1,y1,x2,y2);
    }
    painter.setPen(Ccolor);
    for (int i =0; i<baseC.size()-1;++i)
    {
        x1=regionShow_xAxisExpand * baseC[i].pos;
        x2=regionShow_xAxisExpand * baseC[i+1].pos;
        y1=CurvedHight-(baseC[i].signal*compress);
        y2=CurvedHight-(baseC[i+1].signal*compress);
        painter.drawLine(x1,y1,x2,y2);
    }
#endif
#ifdef Q_WS_WIN
    for(int i=0;i<drawmap.count();++i)
    {
        painter.drawImage(LENIMAGE*i,3*LineHight,
                          *(drawmap[i]),0,0,LENIMAGE);
        //        painter.setPen(QPen(Qt::red , 10 , Qt::SolidLine));
        //        painter.drawLine(i*LENIMAGE-10,0,i*LENIMAGE-10,CurvedHight);
    }
#endif
    painter.setClipRect( contentsRect() );
    drawletter(&painter);
    painter.setPen( QPen( Qt::black , 1 , Qt::SolidLine ) );
    painter.drawLine( 0,CurvedHight-1,CurvedWidth ,CurvedHight-1 );
    //drawline(&painter,0);
    drawline(&painter,LineHight);
    drawline(&painter,2*LineHight);
    drawline(&painter,3*LineHight);

    //emit signalfocusrepian();

}


void CurvedLine::drawFileNameAndNum(QPainter *paint)//画间隔值
{
    QString filename=cuv_regionShowData->fileNames[currentfile];
    if(filename.length()<=50)
        paint->drawText(0,LineHight-2,filename);
    else
    {
        filename=filename.left(48)+".....";
        paint->drawText(0,LineHight-2,filename);
    }
}


void CurvedLine::drawline(QPainter *paint,int height)//画分隔线
{
    paint -> setPen( QPen( Qt::gray , 1 , Qt::DashLine ) );
    paint -> drawLine( 0,height,CurvedWidth ,height );
}


bool CurvedLine ::drawletter(QPainter *paint)//画碱基字母
{
    //改
    //    for(int i=20;i<=cuv_regionShowData->base[currentfile].count()-1;i+=10)
    //    {
    //        paint->drawText(cuv_regionShowData->base[currentfile][i].pos*regionShow_xAxisExpand/*+(move-regionShowData.leftLimits[currentfile])*/
    //                        ,LineHight-2,QString::number(i));


    //    }
    //    foreach(const BaseInfor & Eachbase,cur_baseinfor)
    //    {

    //        if(Eachbase.Matchpos!=-1)
    //            this->SetColor(paint,Eachbase.basetype);
    //        else
    //            paint->setPen(Qt::gray);
    //        paint->drawText((Eachbase.pos)*regionShow_xAxisExpand
    //                        ,2*LineHight-2,QString(Eachbase.basetype));
    //    }

    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setLetterSpacing(QFont::AbsoluteSpacing,2);
    paint->setFont(font);
    for(int i=20;i<=cuv_regionShowData->base[currentfile].count()-1;i+=10)
    {
        int x1 = cuv_regionShowData->base[currentfile][i].pos*regionShow_xAxisExpand - 11;
        paint->setPen(QColor(102,102,102));
        paint->setBrush(Qt::NoBrush);
        paint->drawText(x1/*+(move-regionShowData.leftLimits[currentfile])*/,LineHight-6,QString::number(i));
    }



    for(int i=0,j=0;i<cuv_regionShowData->base[currentfile].count();++i,++j)
    {
        if(cuv_regionShowData->Peaksinform.count()<1) break;
        if(j>=cuv_regionShowData->Peaksinform[currentfile].count()) {break;}
        Peakinfo & one=cuv_regionShowData->Peaksinform[currentfile][j];
        for(;i<cuv_regionShowData->base[currentfile].count();++i)
        {
            if(one.Pos+cuv_regionShowData->moves[currentfile]==cuv_regionShowData->base[currentfile][i].pos) {break;}
        }
        if(i>=cuv_regionShowData->base[currentfile].count()) break;
        BaseInfor & Eachbase=cuv_regionShowData->base[currentfile][i];
        if(one.Pos+cuv_regionShowData->moves[currentfile]==Eachbase.pos&&one.base!=Eachbase.basetype&&one.base!='L')
        {
            this->SetColor(paint,one.base);
            int x1=(one.Pos+cuv_regionShowData->moves[currentfile])
                    *regionShow_xAxisExpand;
            paint->drawText(x1-7,4*LineHight-4,QString(one.base));//改paint->drawText(x1,4*LineHight-2,QString(one.base));
        }
    }



    foreach(const Base_EditInfo & one,cur_changeinfo)
    {
        this->SetColor(paint,one.base_type);
        //改paint->drawText(one.Consen_Pos*regionShow_xAxisExpand,3*LineHight-2,QString(one.base_type));
        int x1 = one.Consen_Pos*regionShow_xAxisExpand - 6;
        paint->drawText(x1,3*LineHight-2,QString(one.base_type));
    }


    return true;
}

void CurvedLine:: SetColor(QPainter *paint ,char type)//设置碱基的颜色
{
    QFont font ;
    font.setPointSize(12);
    if( type == 'G' )
    {
        font.setBold(false);
        paint->setFont(font);
        paint->setPen(QPen(Gcolor,1)  );
    }
    else if( type == 'A' )
    {
        font.setBold(false);
        paint->setFont(font);
        paint->setPen(QPen(Acolor,1) );
    }
    else if( type == 'T' )
    {
        font.setBold(false);
        paint->setFont(font);
        paint->setPen(QPen(Tcolor,1) );
    }
    else if( type == 'C' )
    {
        font.setBold(false);
        paint->setFont(font);
        paint->setPen(QPen(Ccolor,1 ));
    }
    else if( ( type == 'R' ) ||
             ( type == 'Y' ) ||
             ( type == 'K' )||
             ( type == 'M' ) ||
             ( type == 'S' ) ||
             ( type == 'D' ) ||
             ( type== 'W' )  ||
             ( type == 'H' ) ||
             ( type == 'B' ) ||
             ( type == 'V' ) ||
             ( type == 'N')||
             ( type == 'L'))
    {
        font.setBold(true);
        paint->setFont(font);
        paint->setPen(QPen(Wcolor,3)   );
    }
    else
    {
        //        QString outInfor = "The change base also contain the basetype:    ";
        //        outInfor +=  type;
        //        QMessageBox::warning( this , tr("Base ERROR") , outInfor );
        //wutcsl@163.com
        return;
    }

}
/**新增Start**/
void CurvedLine::SetColor(QColor &color, char type)
{
    if( type == 'G' )
    {
        color = Gcolor;
    }
    else if( type == 'A' )
    {
        color = Acolor;
    }
    else if( type == 'T' )
    {
        color = Tcolor;
    }
    else if( type == 'C' )
    {
        color = Ccolor;
    }
    else if( ( type == 'R' ) ||
             ( type == 'Y' ) ||
             ( type == 'K' )||
             ( type == 'M' ) ||
             ( type == 'S' ) ||
             ( type == 'D' ) ||
             ( type== 'W' )  ||
             ( type == 'H' ) ||
             ( type == 'B' ) ||
             ( type == 'V' ) ||
             ( type == 'N')||
             ( type == 'L'))
    {
        color = Wcolor;
    }
    else
    {
        //        QString outInfor = "The change base also contain the basetype:    ";
        //        outInfor +=  type;
        //        QMessageBox::warning( this , tr("Base ERROR") , outInfor );
        //wutcsl@163.com
        return;
    }
}
/**新增End**/

CurvedLine::~CurvedLine()
{
    for(int i=0 ;i<drawmap.count();i++)
    {
        delete  drawmap[i];
        drawmap[i]=NULL;
    }
    drawmap.clear();
    /**新增Start**/
    if(m_ppLetter)
    {
        int num = cur_baseinfor.size();
        for(int i = 0;i < num;i++)
        {
            delete m_ppLetter[i];
        }
        delete []m_ppLetter;
    }
    m_pLetterList.clear();
    /**新增End**/
}

