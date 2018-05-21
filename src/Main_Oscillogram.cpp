#include "Main_Oscillogram.h"
#include "key_update.h"
#include "ab1.h"

Main_Oscillogram::Main_Oscillogram()
{
    rightMenuX2=0;
    Key_Update key;
    if(!key.AllIsRigth())
    {
        return ;
    }
    rightMenuX2=key.Getmouth2();
    this -> setBackgroundRole( QPalette::Base );
    this -> setAutoFillBackground( true );
    sliderPos=0;
    container_oscillogram=NULL;
    //currentFileSignalInfo = NULL;
    hscoll=this->horizontalScrollBar();
    iscontainer_create=false;
    data_is_ready=false;
    done=true;
    analyse=true;
    //sampleMaktype=0;

    this -> setFocusPolicy(Qt::StrongFocus);

    //rightMenuForm=new QMenu( FocusBoxForm );
    rightMenu = new QMenu( this );
    current_exonNum=0;
    xAxisExpandEverytime=1;
    yAxisExpandEverytime=0.2;
    curvedHigthEverytime=10;
    //this->setHorizontalScrollBar();
    //this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    createAction();
    createMenu(rightMenu);
    //createMenu(rightMenuForm);
    befordata.addordecreaseOne=false;
    befordata.filesxExpand=4;//决定两个波峰的宽
    befordata.filesyExpand=1;//决定波峰的高
    befordata.curedHigth=200;//峰图的高
    befordata.rightMenuX=rightMenuX2;
    couldnotmove=0;
    //this->horizontalScrollBar();

    isClickTable = false;
    isFileChange = false;

    connect(actApplyOne,SIGNAL(triggered()),this,SLOT(slotactApplyOne()));
    connect(actApplyAll,SIGNAL(triggered()),this,SLOT(slotactApplyAll()));
    connect(actanalyseLater,SIGNAL(triggered()),this,SLOT(slotactanalyseLater()));
    connect(actanalyseNow,SIGNAL(triggered()),this,SLOT(slotactanalyseNow()));
    connect(actanalyse,SIGNAL(triggered()),this,SLOT(slotactanalyse()));

    connect(actDelete, SIGNAL(triggered()), this, SLOT(slotDelteThisFile()));

    connect(hscoll,SIGNAL(valueChanged(int)),this,SLOT(slotslidermovebymouse(int )));
    connect(hscoll,SIGNAL(sliderPressed()),this,SLOT(slotslidermousepress()));
    connect( actExcludeLeft,SIGNAL(triggered()),
             this,SLOT(slotHighLightLeftPart()));
    connect( actExcludeRight,SIGNAL(triggered()),
             this,SLOT(slotHighLightRightPart()));
    connect( actResetExclude,SIGNAL(triggered()),
             this,SLOT(slotResetExclude()));
    //  slider=this->horizontalScrollBar();
}

void Main_Oscillogram :: create_draw()
{
    if(data_is_ready==false)
        return ;

    container_oscillogram = new Container_Oscillogram(
                &regionShowData,
                &regionShowDataBak,
                &befordata,this);
    iscontainer_create=true;
    this -> setWidget( container_oscillogram );
    this -> setBackgroundRole( QPalette::Base );
    this -> setAutoFillBackground( true );
    connect(container_oscillogram,SIGNAL(signal_analyse()),
            this,SLOT(slotanalyse()));
    connect(container_oscillogram,SIGNAL(signal_analyse(int,QString)),
            this,SLOT(slotIntelligent_Analysis(int,QString)));
    connect(actIntelligent_Analysis,SIGNAL(triggered()),
            container_oscillogram,SLOT(slotIntelligent_Analysis()));
    connect(actReset_Analysis,SIGNAL(triggered()),
            container_oscillogram,SLOT(slotReset_Analysis()));
    connect(container_oscillogram,SIGNAL(signalfilechange(bool)),
            this,SLOT(slotfilechange(bool)));
    connect(container_oscillogram,
            SIGNAL(signal_inform_other(int,int)),
            this,SLOT(slot_inform_other(int,int)));

    connect(container_oscillogram,SIGNAL(signalFocuschange(int,int))
            ,this,SLOT(slotmousepressed(int,int)));
    connect(container_oscillogram,
            SIGNAL(signalMoveScollBar(int,int,int))
            ,this,SLOT(slotMoveScollBar(int,int,int)));
    connect(container_oscillogram,SIGNAL(signalClickLetter(int,int))
            ,this,SLOT(slotClickLetter(int,int)));
    connect(container_oscillogram,
            SIGNAL(formbasechanged(char,int,int))
            ,this,SLOT(slotBasechanged(char,int,int)));

    connect(container_oscillogram,
            SIGNAL(signalMoveFocusBoxForm(int)),
            this,SLOT(slotMoveFocusBoxForm(int))  );
    iscontainer_create=true;
}

Main_Oscillogram::~Main_Oscillogram()
{
    //    delete rightMenu,actInsertBaseN,actHideTraceDisplay,
    //            actFilterByCurrentBase,actRemoveLastBaseFilter,
    //            actRemoveAllBaseFilters,actRemoveLastNullAlleleFilter,
    //            actExcludeLeft,actExcludeRight,actResetExclude,
    //            actSetToDRB101StartPosition,actResetToSeCoreDRB101StartPosition,
    //            actChangeExonOrDirectionToThisTrace,
    //            actRemoveThisTrace;
    //    delete container_oscillogram;
    //    delete FocusBoxForm;
    //    delete mainLayout;
    //    delete befordata.filesxExpand,befordata.filesyExpand;


}
void Main_Oscillogram::slotslidermousepress()
{
    sliderPos=this->horizontalScrollBar()->sliderPosition();
}
void Main_Oscillogram::slotslidermovebymouse(int  pos)
{
    XPos = pos;
    if(iscontainer_create)
    {
        sliderPos=this->horizontalScrollBar()->sliderPosition();

        int currentfile = container_oscillogram->getcurrenteditfile();
        QVector < CurvedLine* > curvedLine = container_oscillogram->getCurvedLine();
        int curvedLineH = 0 ;
        for(int i = 0;i < currentfile;i++)
        {
            curvedLineH += curvedLine[i]->getCurvedHight();
        }

        DisconnectFocusBox();
    }
}
void  Main_Oscillogram::slot_inform_other(int ConsensePos,int mousePos)
{
    emit signal_toinform_other(ConsensePos,current_exonNum,mousePos);
}

void Main_Oscillogram::slotIntelligent_Analysis(int currentfile,QString updatastr)
{
//    qDebug()<<"changebp"<<currentfile<<updatastr;
    updatasignalInfo( currentfile );
    upDatabyChangebp(regionShowData.fileNames[currentfile],updatastr,currentFileSignalInfo.isGssp);

}
void Main_Oscillogram::slotanalyse()
{
//    qDebug()<<"analyse";
    actanalyse->setDisabled(true);
    if(iscontainer_create)
    {
        int currentfile=container_oscillogram->getcurrenteditfile();
        updatasignalInfo( currentfile );
        analysisSample(currentFileSignalInfo.sampleName.toAscii());
        emit signalchangedbp( currentFileSignalInfo,1 );
    }

}

void Main_Oscillogram::slotDelteThisFile()
{
    if(iscontainer_create)
    {
        int currentfile = container_oscillogram->getcurrenteditfile();
        updatasignalInfo(currentfile);
        deleteFile(currentFileSignalInfo.fileName.toAscii());
        analysisSample(currentFileSignalInfo.sampleName.toAscii());
        emit signalchangedbp(currentFileSignalInfo, 1);
    }
}

void Main_Oscillogram::slotfilechange(bool is_emit)
{
    if(iscontainer_create)
    {

        int currentfile=container_oscillogram->getcurrenteditfile();
        updatasignalInfo( currentfile );
        if(is_emit)
            emit signalchangedbp( currentFileSignalInfo,0 );
    }

}
void Main_Oscillogram::updatasignalInfo(int currentfile)
{

    if(iscontainer_create)
    {
        currentFileSignalInfo.fileName=regionShowData.fileNames[currentfile];
        currentFileSignalInfo.rOrF=regionShowData.infor[currentfile].rOrF;
        currentFileSignalInfo.sampleName=regionShowData.infor[currentfile].samplename;
        currentFileSignalInfo.geneName=regionShowData.infor[currentfile].genegname;
        currentFileSignalInfo.isGssp=regionShowData.infor[currentfile].isgssp;
        currentFileSignalInfo.exonIndex=regionShowData.infor[currentfile].exonnum;
        currentFileSignalInfo.isUndefined=regionShowData.infor[currentfile].isUndefined;

    }

}

void Main_Oscillogram::DisconnectFocusBox()
{
    QString strMsg;
    if(isClickTable)
    {
        if(isFileChange)
        {
            strMsg = QString("Ready");
            isFileChange = false;
        }
        else
        {
            int currentfile=container_oscillogram->getcurrenteditfile();//鼠标点击获取当前选中文件
            int pos=container_oscillogram->getfocusPos();
            int match=0;
            bool isMatch = false;
            for(match=0;match<regionShowData.base[currentfile].count();++match)
            {
                if(regionShowData.base[currentfile][match].Matchpos==pos)
                {
                    isMatch = true;
                    break;
                }
            }
            if(!isMatch)
            {
                emit signalAB1Information("Ready");
                return;
            }

            QString Codon;
            QString Code;
            int integer = (pos + 1 - (alignStart_ - 1))/3;
            int Remainder = (pos + 1 - (alignStart_ - 1))%3;
            if(Remainder == 0)
            {
                if(integer == 1)
                {
                    Codon=QString("%1.%2").arg(integer).arg(3);
                    strMsg = QString("Exon:%1 Codon:%2 Pos:%3").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1);
                }
                else
                {
                    QString Code1,Code2,Code3;
                    Code1 = regionShowData.base[currentfile][match-2].basetype;
                    Code2 = regionShowData.base[currentfile][match-1].basetype;
                    Code3 = regionShowData.base[currentfile][match].basetype;
                    for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                    {
                        if(pos- 2==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code1 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos- 1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code2 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code3 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                    }

                    Codon= QString("%1.%2").arg(integer).arg(3);
                    Code = QString("%1%2%3").arg(Code1)
                                            .arg(Code2)
                                            .arg(Code3);
                    Code = Code + QString("(%1)").arg(getAcidFromBase(Code));
                    strMsg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(Code);
                }
            }
            else
            {
                if(integer == 0)
                {
                    Codon=QString("%1.%2").arg(integer+1).arg(Remainder);
                    strMsg = QString("Exon:%1 Codon:%2 Pos:%3").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1);
                }
                else
                {
                    Codon=QString("%1.%2").arg(integer+1).arg(Remainder);
                    if(Remainder==1)
                    {
                        QString Code1,Code2,Code3;
                        Code1 = regionShowData.base[currentfile][match].basetype;
                        Code2 = regionShowData.base[currentfile][match+1].basetype;
                        Code3 = regionShowData.base[currentfile][match+2].basetype;
                        for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                        {
                            if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code1 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                            else if(pos+1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code2 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                            else if(pos+2==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code3 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                        }

                        Code = QString("%1%2%3").arg(Code1)
                                                .arg(Code2)
                                                .arg(Code3);
                    }
                    else if(Remainder==2)
                    {
                        QString Code1,Code2,Code3;
                        Code1 = regionShowData.base[currentfile][match-1].basetype;
                        Code2 = regionShowData.base[currentfile][match].basetype;
                        Code3 = regionShowData.base[currentfile][match+1].basetype;
                        for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                        {
                            if(pos- 1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code1 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                            else if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code2 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                            else if(pos+1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                            {
                                Code3 = regionShowData.edit_info[currentfile][i].base_type;
                            }
                        }
                        Code = QString("%1%2%3").arg(Code1)
                                                .arg(Code2)
                                                .arg(Code3);
                    }
                    Code = Code + QString("(%1)").arg(getAcidFromBase(Code));
                    strMsg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(Code);
                }
            }
        }


        emit signalAB1Information(strMsg);
        isClickTable = false;
    }
    else
    {
        int currentfile=container_oscillogram->getcurrenteditfile();//鼠标点击获取当前选中文件
        int pos=container_oscillogram->getfocusPos();
        int match=0;
        bool isMatch = false;
        for(match=0;match<regionShowData.base[currentfile].count();++match)
        {
            if(regionShowData.base[currentfile][match].Matchpos==pos)
            {
                isMatch = true;
                break;
            }
        }
        if(!isMatch)
        {
            emit signalAB1Information("Ready");
            return;
        }

        QString QV = QString::number(regionShowData.base[currentfile][match].qual);
        QString Codon;
        QString Code;
        int integer = (pos + 1 - (alignStart_ - 1))/3;
        int Remainder = (pos + 1 - (alignStart_ - 1))%3;
        if(Remainder == 0)
        {
            if(integer == 1)
            {
                Codon=QString("%1.%2").arg(integer).arg(3);
                strMsg = QString("Exon:%1 Codon:%2 Pos:%3 QV:%4").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(QV);
            }
            else
            {
                QString Code1,Code2,Code3;
                Code1 = regionShowData.base[currentfile][match-2].basetype;
                Code2 = regionShowData.base[currentfile][match-1].basetype;
                Code3 = regionShowData.base[currentfile][match].basetype;
                for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                {
                    if(pos- 2==regionShowData.edit_info[currentfile][i].Consen_Pos)
                    {
                        Code1 = regionShowData.edit_info[currentfile][i].base_type;
                    }
                    else if(pos- 1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                    {
                        Code2 = regionShowData.edit_info[currentfile][i].base_type;
                    }
                    else if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                    {
                        Code3 = regionShowData.edit_info[currentfile][i].base_type;
                    }
                }
                Codon= QString("%1.%2").arg(integer).arg(3);
                Code = QString("%1%2%3").arg(Code1)
                                        .arg(Code2)
                                        .arg(Code3);
                Code = Code + QString("(%1)").arg(getAcidFromBase(Code));
                strMsg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4 QV:%5").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(Code).arg(QV);
            }
        }
        else
        {
            if(integer == 0)
            {
                Codon=QString("%1.%2").arg(integer+1).arg(Remainder);
                strMsg = QString("Exon:%1 Codon:%2 Pos:%3 QV:%4").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(QV);
            }
            else
            {
                Codon=QString("%1.%2").arg(integer+1).arg(Remainder);
                if(Remainder==1)
                {
                    QString Code1,Code2,Code3;
                    Code1 = regionShowData.base[currentfile][match].basetype;
                    Code2 = regionShowData.base[currentfile][match+1].basetype;
                    Code3 = regionShowData.base[currentfile][match+2].basetype;
                    for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                    {
                        if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code1 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos+1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code2 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos+2==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code3 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                    }
                    Code = QString("%1%2%3").arg(Code1)
                                            .arg(Code2)
                                            .arg(Code3);
                }
                else if(Remainder==2)
                {
                    QString Code1,Code2,Code3;
                    Code1 = regionShowData.base[currentfile][match-1].basetype;
                    Code2 = regionShowData.base[currentfile][match].basetype;
                    Code3 = regionShowData.base[currentfile][match+1].basetype;
                    for(int i =0;i < regionShowData.edit_info[currentfile].count();i++)
                    {
                        if(pos- 1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code1 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code2 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                        else if(pos+1==regionShowData.edit_info[currentfile][i].Consen_Pos)
                        {
                            Code3 = regionShowData.edit_info[currentfile][i].base_type;
                        }
                    }
                    Code = QString("%1%2%3").arg(Code1)
                                            .arg(Code2)
                                            .arg(Code3);
                }
                Code = Code + QString("(%1)").arg(getAcidFromBase(Code));
                strMsg = QString("Exon:%1 Codon:%2 Pos:%3 Code:%4 QV:%5").arg(currentFileSignalInfo.exonIndex).arg(Codon).arg(pos + 1).arg(Code).arg(QV);
            }

        }
        emit signalAB1Information(strMsg);
    }
}
void Main_Oscillogram::slotMoveFocusBoxForm(int xPos)//移动选中窗口
{
    XPos = xPos;
    int FoucusBox=15;
    if(couldnotmove<0)
        xPos=xPos+couldnotmove;
    //qDebug()<<"   "<<couldnotmove<<xPos;
    if(iscontainer_create&&data_is_ready)
    {
        FoucusBox=container_oscillogram->
                getbaseItemWidth();
        int currentfile = container_oscillogram->getcurrenteditfile();
        QVector < CurvedLine* > curvedLine = container_oscillogram->getCurvedLine();
        int curvedLineH = 0 ;
        for(int i = 0;i < currentfile;i++)
        {
            curvedLineH += curvedLine[i]->getCurvedHight();
        }
        DisconnectFocusBox();
    }
}

bool Main_Oscillogram :: getRegionShowData( SignalInfo & fileSignal ,bool getdata)
{
    QList<FileInfo_ly> fileinfo_lyList;
    try{
        current_exonNum=fileSignal.exonIndex;
        if( fileSignal.sampleName!=""&&
                fileSignal.fileName != ""&&
                currentFileSignalInfo.fileName==fileSignal.fileName&&
                fileSignal.exonIndex==currentFileSignalInfo.exonIndex&&
                fileSignal.isGssp==currentFileSignalInfo.isGssp&&
                fileSignal.geneName==currentFileSignalInfo.geneName&&
                fileSignal.isUndefined==currentFileSignalInfo.isUndefined&&
                fileSignal.sampleName==currentFileSignalInfo.sampleName&&
                fileSignal.rOrF==currentFileSignalInfo.rOrF&&
                fileSignal.isCombined==currentFileSignalInfo.isCombined&&
                fileSignal.isGsspFilter==currentFileSignalInfo.isGsspFilter &&getdata==false)
        {
            data_is_ready=true;
            return false;
        }
        regionShowData.base.clear();
        regionShowData.baseA.clear();
        regionShowData.baseT.clear();
        regionShowData.baseC.clear();
        regionShowData.baseG.clear();

        regionShowData.fileNames.clear();
        regionShowData.leftLimits.clear();
        regionShowData.rightLimits.clear();
        regionShowData.edit_info.clear();
        regionShowData.filemax.clear();

        regionShowData.excludeleft.clear();
        regionShowData.excludeRight.clear();
        regionShowData.infor.clear();
        regionShowData.PeaksA.clear();
        regionShowData.PeaksC.clear();
        regionShowData.PeaksG.clear();
        regionShowData.PeaksT.clear();
        regionShowData.Peaksinform.clear();
        regionShowData.moves.clear();



        regionShowDataBak.base.clear();
        regionShowDataBak.baseA.clear();
        regionShowDataBak.baseC.clear();
        regionShowDataBak.baseG.clear();
        regionShowDataBak.baseT.clear();

        regionShowDataBak.edit_info.clear();
        regionShowDataBak.excludeleft.clear();
        regionShowDataBak.excludeRight.clear();
        regionShowDataBak.filemax.clear();
        regionShowDataBak.fileNames.clear();

        regionShowDataBak.leftLimits.clear();
        regionShowDataBak.rightLimits.clear();
        regionShowDataBak.infor.clear();


        regionShowDataBak.PeaksA.clear();
        regionShowDataBak.PeaksC.clear();
        regionShowDataBak.PeaksG.clear();
        regionShowDataBak.PeaksT.clear();
        regionShowDataBak.Peaksinform.clear();
        regionShowDataBak.moves.clear();

        currentFileSignalInfo=fileSignal;
        //qDebug()<<"get data";

        if(fileSignal.isUndefined)
        {
            fileinfo_lyList=OgetOneData(fileSignal.fileName,fileSignal.isGssp);

        }
        else
        {
            fileinfo_lyList =OgetAllData(fileSignal.sampleName,fileSignal.exonIndex);
        }
        m_FileInfo = fileinfo_lyList;
        alignStart_ = 0;
        alignEnd_ = 0;
        getSampleStartEndBySampleName(fileSignal.sampleName.toAscii(), alignStart_, alignEnd_);

        if(fileinfo_lyList.isEmpty())
        {
            qDebug()<<"FileSignalInfo ERROR";
            return false;
        }

    }
    catch(...)
    {
        ErrorWrite::errorocur("getRegionShowData befor");
    }
    int filenum=0;
    foreach (const FileInfo_ly &fileInfo,  fileinfo_lyList)
    {
        try
        {

            filenum++;
            QStringList rawSequence           = fileInfo.baseSequence.split("",QString::SkipEmptyParts);
            QStringList baseMatchConsensusPos = fileInfo.alignInfo.split(":");
            QStringList basePos               = fileInfo.basePostion.split(":");
            QStringList basequality           = fileInfo.baseQuality.split(":");
            QStringList baseGSignal           = fileInfo.baseGSignal.split(":");
            QStringList baseASignal           = fileInfo.baseASignal.split(":");
            QStringList baseTSignal           = fileInfo.baseTSignal.split(":");
            QStringList baseCSignal           = fileInfo.baseCSignal.split(":");
            QStringList editinfo              = fileInfo.editInfo.split(";");
            FileInfomation information;
            information.exonnum=fileInfo.exonIndex;
            information.isgssp=fileInfo.isgssp;
            information.rOrF=fileInfo.rOrF;
            information.genegname=fileInfo.geneName;
            information.samplename=fileInfo.sampleName;
            if((fileInfo.alignResult>0&&fileInfo.isgssp==false)||(fileInfo.alignResult==4&&fileInfo.isgssp==true))
                information.isUndefined=true;
            else information.isUndefined=false;
            regionShowData.infor.push_back(information);
            regionShowData.avgsignal.push_back(fileInfo.avgsignal);
            QVector <Base_EditInfo> eidtlist;
            for(int i=0;i<editinfo.count();++i)
            {
                if(editinfo[i]=="")
                {
                    break;
                }
                QStringList strlist=editinfo[i].split(":");
                Base_EditInfo editstruct;
                editstruct.base_type=((QString)strlist[1]).data()->toLatin1();
                editstruct.Consen_Pos=((QString)strlist[0]).toInt();
                eidtlist.push_back(editstruct);
            }
            regionShowData.currentfile=0;
            regionShowData.edit_info.push_back(eidtlist);
            //qDebug()<<rawSequence;
            int baseNum = fileInfo.baseNumber;
            QString outInfor ="database error";
            if( baseMatchConsensusPos.size() != baseNum )
            {
                qDebug()<<"The number of baseMatchConsensusPos doesn't equal to baseNum in regionShow getRegionShowData()";
                QMessageBox :: warning( this , tr("baseMatchConsensusPos ERROR") , outInfor );
                break;
            }
            if( basePos.size() != baseNum )
            {
                qDebug()<< "The number of basePos doesn't equal to baseNum in regionShow getRegionShowData()";
                QMessageBox :: warning( this , tr("basePos ERROR") , outInfor );
                break;
            }
            int baseGnum = fileInfo.signalNumber;
            if( baseGSignal.size() != baseGnum )
            {
                qDebug()<< "The number of baseGSignal doesn't equal to baseGnum in regionShow getRegionShowData ()";
                QMessageBox :: warning( this , tr("baseGSignal ERROR") , outInfor );
                break;
            }

            if( baseASignal.size() != baseGnum )
            {
                qDebug()<< "The number of baseASignal doesn't equal to baseGnum in regionShow getRegionShowData ()";
                QMessageBox :: warning( this , tr("baseASignal ERROR") , outInfor );
                break;
            }

            if( baseTSignal.size() != baseGnum )
            {
                qDebug()<< "The number of baseTSignal doesn't equal to baseGnum in regionShow getRegionShowData ()";
                QMessageBox :: warning( this , tr("baseTSignal ERROR") , outInfor );
                break;
            }

            if( baseCSignal.size() != baseGnum )
            {
                qDebug()<< "The number of baseCSignal doesn't equal to baseGnum in regionShow getRegionShowData ()";
                QMessageBox :: warning( this , tr("baseCSignal ERROR") , outInfor );
                break;
            }

            regionShowData.fileNames.push_back(fileInfo.fileName);
            MaxValue filemax;
            filemax.maxQuality=fileInfo.maxQuality;
            filemax.maxSgnal=fileInfo.maxSgnal;
            regionShowData.filemax.push_back(filemax);

            foreach(QString one,editinfo)
            {
                Base_EditInfo edit;
                QStringList str_edit=  one.split(":");
                edit.Consen_Pos=str_edit[0].toInt();
                edit.base_type=rawSequence[1].data()->toLatin1();

            }
            QVector < BaseInfor > baseInfors;
            for (int i = 0; i < baseNum; i++)
            {
                BaseInfor baseInfor;
                baseInfor.basetype = rawSequence[i].data()->toLatin1();
                baseInfor.pos      = basePos[i].toInt();
                baseInfor.qual     = basequality[i].toInt();
                baseInfor.Matchpos = baseMatchConsensusPos[i].toInt();
                baseInfor.signalA  = baseASignal[baseInfor.pos];
                baseInfor.signalT  = baseTSignal[baseInfor.pos];
                baseInfor.signalC  = baseCSignal[baseInfor.pos];
                baseInfor.signalG  = baseGSignal[baseInfor.pos];
                baseInfors.push_back( baseInfor );
            }
            if(fileInfo.alignEndPos<baseNum)
            {
                regionShowData.leftLimits.push_back(baseInfors[fileInfo.alignStartPos].pos);
                //qDebug()<<fileInfo.alignStartPos;
                regionShowData.rightLimits.push_back(baseInfors[fileInfo.alignEndPos].pos);
            }
            else if(fileInfo.alignStartPos<baseNum)
            {
                regionShowData.leftLimits.push_back(baseInfors[fileInfo.alignStartPos].pos);
                //qDebug()<<fileInfo.alignStartPos;
                regionShowData.rightLimits.push_back(baseInfors[baseNum-1].pos);
            }
            else
            {
                regionShowData.leftLimits.push_back(baseInfors[baseNum-1].pos);
                regionShowData.rightLimits.push_back(baseInfors[baseNum-1].pos);
            }
            regionShowData.base.push_back( baseInfors );
            //qDebug()<<"read from database"<<fileInfo.excludeleft;
            if(fileInfo.excludeleft==-1)
            {
                if(fileInfo.alignStartPos<baseNum)
                    regionShowData.excludeleft.push_back(baseInfors[fileInfo.alignStartPos].pos);
                else
                    regionShowData.excludeleft.push_back(baseInfors[baseNum-1].pos);
                //qDebug()<<"linyi0"<< baseInfors[fileInfo.alignStartPos].pos;
            }
            else
            {
                int i=0;
                for(;i<baseInfors.count();++i)
                {
                    if(baseInfors[i].Matchpos==fileInfo.excludeleft)
                    {
                        regionShowData.excludeleft.push_back(baseInfors[i+1].pos);
                        //qDebug()<<"linyi1"<< baseInfors[i].pos;
                        break;
                    }
                }
                if(i>=baseInfors.count())
                {
                    if(fileInfo.alignStartPos<baseNum)
                        regionShowData.excludeleft.push_back(baseInfors[fileInfo.alignStartPos].pos);
                    else
                        regionShowData.excludeleft.push_back(baseInfors[baseNum-1].pos);
                     //qDebug()<<"linyi2"<< baseInfors[fileInfo.alignStartPos].pos;
                }
            }
            if(fileInfo.excludeRight==-1)
            {
                if(fileInfo.alignEndPos<baseNum)
                    regionShowData.excludeRight.push_back(baseInfors[fileInfo.alignEndPos].pos);
                else
                    regionShowData.excludeRight.push_back(baseInfors[baseNum-1].pos);
            }
            else
            {
                int i=0;
                for( ;i<baseInfors.count();++i)
                {
                    if(baseInfors[i].Matchpos==fileInfo.excludeRight)
                    {
                        regionShowData.excludeRight.push_back(baseInfors[i-1].pos);
                        break;
                    }
                }
                if(i>=baseInfors.count())
                {
                    if(fileInfo.alignEndPos<baseNum)
                        regionShowData.excludeRight.push_back(baseInfors[fileInfo.alignEndPos].pos);
                    else
                        regionShowData.excludeRight.push_back(baseInfors[baseNum-1].pos);
                }
            }
            //qDebug()<<"read from database"<<fileInfo.excludeleft;
            QVector < Each_Base_Signal > baseGs;
            QVector < Each_Base_Signal > baseAs;
            QVector < Each_Base_Signal > baseTs;
            QVector < Each_Base_Signal > baseCs;
            for (int i = 0; i < baseGnum; i++)
            {
                Each_Base_Signal baseG;
                Each_Base_Signal baseA;
                Each_Base_Signal baseT;
                Each_Base_Signal baseC;
                baseG.pos = i;
                baseG.signal=baseGSignal[i].toInt();
                baseA.pos = i;
                baseA.signal=baseASignal[i].toInt();
                baseT.pos = i;
                baseT.signal=baseTSignal[i].toInt();
                baseC.pos = i;
                baseC.signal=baseCSignal[i].toInt();
                baseGs.push_back( baseG );
                baseAs.push_back( baseA );
                baseTs.push_back( baseT );
                baseCs.push_back( baseC );
                //qDebug()<<i+1;
            }

            regionShowData.baseG.push_back( baseGs );
            regionShowData.baseA.push_back( baseAs );
            regionShowData.baseT.push_back( baseTs );
            regionShowData.baseC.push_back( baseCs );
            regionShowData.moves.push_back(0);
            regionShowDataBak = regionShowData;
            data_is_ready=true;
        }
        catch(...)
        {
            errorocur("getRegionShowData");
            QApplication::exit(0);
        }
        //PressMovePos.push_back(0);
    }

    return true;
}




void Main_Oscillogram :: analysisData()
{
    try{
        int maxLeftLimit = 0;//定义变量 maxLeftLimit 来存储左侧屏蔽位置最大值。

        for(int i=0;i<regionShowData.leftLimits.size();i++)
        {
            if(regionShowData.leftLimits[i]>=maxLeftLimit)
                maxLeftLimit = regionShowData.leftLimits[i];
        }
        for(int i=0;i<regionShowData.leftLimits.size();i++)
        {
            int movePos = maxLeftLimit-regionShowData.leftLimits[i];
            for(int j=0;j<regionShowData.base[i].size();j++)
            {
                regionShowData.base[i][j].pos=regionShowData.base[i][j].pos+movePos;
            }
            int baseAnum = regionShowData.baseA[i].size();
            for(int j=0;j<baseAnum;j++)
            {
                regionShowData.baseA[i][j].pos=regionShowData.baseA[i][j].pos+movePos;
                regionShowData.baseC[i][j].pos=regionShowData.baseC[i][j].pos+movePos;
                regionShowData.baseG[i][j].pos=regionShowData.baseG[i][j].pos+movePos;
                regionShowData.baseT[i][j].pos=regionShowData.baseT[i][j].pos+movePos;
            }
            regionShowData.leftLimits[i]=regionShowData.leftLimits[i]+movePos;
            regionShowData.rightLimits[i]=regionShowData.rightLimits[i]+movePos;
            regionShowData.excludeleft[i]+=movePos;
            regionShowData.excludeRight[i]+=movePos;
        }
    }
    catch(...)
    {
        errorocur("analysisData");
        QApplication::exit(0);
    }
}




void Main_Oscillogram :: createAction()
{

    actDelete                                   = new QAction(tr("Delete Selected File"),this);
    actInsertBaseN                              = new QAction(tr("Insert Base 'N'"),this);
    actHideTraceDisplay                         = new QAction(tr("Hide Trace Display"),this);
    actFilterByCurrentBase                      = new QAction(tr("Filter By Current Base"),this);
    actRemoveLastBaseFilter                     = new QAction(tr("Remove Last Base Filter"),this);
    actRemoveAllBaseFilters                     = new QAction(tr("Remove All Base Filters"),this);
    actRemoveLastNullAlleleFilter               = new QAction(tr("Remove Last Null AlleleFilter"),this);



    actApplyOne                                 = new QAction(QIcon(":/images/apply.png"),tr("Apply One"),this);
    actApplyOne->setIconVisibleInMenu(false);
    actApplyOne->setShortcut(QKeySequence(Qt::ALT+Qt::Key_E));
    actApplyAll                                 = new QAction(QIcon(":/images/apply.png"),tr("Apply All"),this);
    actApplyAll->setDisabled(true);
    actApplyAll->setShortcut(QKeySequence(Qt::ALT+Qt::Key_R));
    actanalyseLater                             = new QAction(QIcon(":/images/apply.png"),tr("Edit Multi"),this);
    actanalyseLater->setIconVisibleInMenu(false);
    actanalyseLater->setShortcut(QKeySequence(Qt::ALT+Qt::Key_M));
    actanalyseNow                               = new QAction(QIcon(":/images/apply.png"),tr("Edit One"),this);
    actanalyseNow->setDisabled(true);
    actanalyseNow->setShortcut(QKeySequence(Qt::ALT+Qt::Key_O));
    actanalyse                                  = new QAction(tr("Analyze"),this);
    actanalyse->setShortcut(QKeySequence(Qt::ALT+Qt::Key_T));
    actanalyse->setDisabled(true);

    actExcludeLeft                              = new QAction(tr("Exclude left"),this);
    actExcludeRight                             = new QAction(tr("Exclude right"),this);
    actResetExclude                             = new QAction(tr("Reset Exclude"),this);
    actIntelligent_Analysis                     = new QAction(tr("Smart Analysis"),this);
    actReset_Analysis                           = new QAction(tr("Reset Analysis"),this);
    actChangeExonOrDirectionToThisTrace         = new QAction(tr("Change Exon Or Direction To This Trace"),this);
    actRemoveThisTrace                          = new QAction(tr("Remove This Trace"),this);
    actInsertBaseN->setDisabled(true);
    actHideTraceDisplay->setDisabled(true);
    actFilterByCurrentBase->setDisabled(true);
    actRemoveLastBaseFilter->setDisabled(true);
    actRemoveAllBaseFilters->setDisabled(true);
    actRemoveLastNullAlleleFilter->setDisabled(true);
    //    actIntelligent_Analysis->setDisabled(true);
    //    actReset_Analysis->setDisabled(true);
    actChangeExonOrDirectionToThisTrace->setDisabled(true);
    actRemoveThisTrace->setDisabled(true);

    actIntelligent_Analysis->setVisible(false);
    actReset_Analysis->setVisible(false);


}




void Main_Oscillogram :: createMenu(QMenu * rightMenu )
{

    rightMenu->addAction(actDelete);
    rightMenu->addAction(actInsertBaseN);
    rightMenu->addAction(actHideTraceDisplay);
    rightMenu->addSeparator();
    rightMenu->addAction(actApplyOne);
    rightMenu->addAction(actApplyAll);
    rightMenu->addSeparator();
    rightMenu->addAction(actanalyseNow);
    rightMenu->addAction(actanalyseLater);
    rightMenu->addSeparator();
    rightMenu->addAction(actanalyse);
    rightMenu->addSeparator();
    rightMenu->addAction(actExcludeLeft);
    rightMenu->addAction(actExcludeRight);
    rightMenu->addAction(actResetExclude);
    rightMenu->addSeparator();
    rightMenu->addAction(actIntelligent_Analysis);
    rightMenu->addAction(actReset_Analysis);
    //    rightMenu->addSeparator();
    //    rightMenu->addAction(actChangeExonOrDirectionToThisTrace);
    //    rightMenu->addAction(actRemoveThisTrace);

}




void Main_Oscillogram::contextMenuEvent(QContextMenuEvent *event)
{


    if(rightMenu != NULL)
    {
        sliderPos = this->horizontalScrollBar()->sliderPosition();
        rightMenuX = sliderPos + event->x();
        rightMenuY = event->y();
        rightMenu->exec(QCursor::pos());
        event->accept();
    }
}

void Main_Oscillogram :: moveCenterPos(int sliderPos)                                      //      移动第五部分中心位置
{
    this->horizontalScrollBar()->setSliderPosition(sliderPos);
}

void Main_Oscillogram:: slotaddordecreaseOne()
{
    actApplyOne->setDisabled(true);
    actApplyAll->setDisabled(false);
    actApplyOne->setIconVisibleInMenu(true);
    actApplyAll->setIconVisibleInMenu(false);
    befordata.addordecreaseOne=true;
}
void Main_Oscillogram::slotaddordecreaseAll()
{
    actApplyOne->setDisabled(false);
    actApplyAll->setDisabled(true);
    actApplyOne->setIconVisibleInMenu(false);
    actApplyAll->setIconVisibleInMenu(true);
    befordata.addordecreaseOne=false;
}


void Main_Oscillogram :: resetAll()
{

    data_is_ready=false;
    getRegionShowData(currentFileSignalInfo,true);
    if(data_is_ready)
    {
        befordata.filesxExpand=2;
        befordata.filesyExpand=1;
        befordata.curedHigth=200;
        analysisData();
        if(iscontainer_create)
            container_oscillogram->~Container_Oscillogram();
        iscontainer_create=false;
        create_draw();
        iscontainer_create=true;
    }
}

void Main_Oscillogram::slotResetExclude()
{
    if(iscontainer_create)
    {
        try{
            if(GetMarkType()==3) return;
            int currentfile= container_oscillogram->getcurrenteditfile();
            updatasignalInfo( currentfile );
            QString leftstr=currentFileSignalInfo.geneName.trimmed();
            //QString rightstr=leftstr;
            leftstr.append("_");
            leftstr.append(QString::number(currentFileSignalInfo.exonIndex));
            leftstr.append("_");
            leftstr.append(currentFileSignalInfo.rOrF.trimmed());
            int left=-1;
            int right=-1;
            if(currentFileSignalInfo.isGssp==false)
            {
                left= getExcludeLeftPosition(leftstr);
                right=getExcludeRightPostion(leftstr);
                if(left!=-1)
                {
                    for(int i=1 ;i<regionShowData.base[currentfile].count();++i)
                    {
                        if(left+1==regionShowData.base[currentfile][i].Matchpos)
                        {
                            regionShowData.excludeleft[currentfile]=regionShowData.base[currentfile][i].pos;
                            regionShowDataBak.excludeleft[currentfile]=regionShowDataBak.base[currentfile][i].pos;
                            //pos=pos-1;
                            break;
                        }
                    }
                }
                else
                {
                    regionShowData.excludeleft[currentfile]=left;
                    regionShowDataBak.excludeleft[currentfile]=left;
                }
                if(right!=-1)
                {
                    for(int i=0 ;i<regionShowData.base[currentfile].count();++i)
                    {
                        if(right-1==regionShowData.base[currentfile][i].Matchpos)
                        {
                            regionShowData.excludeRight[currentfile]=regionShowData.base[currentfile][i].pos;
                            regionShowDataBak.excludeRight[currentfile]=regionShowDataBak.base[currentfile][i].pos;
                            //pos=pos+1;
                            break;
                        }
                    }
                }
                else
                {
                    regionShowData.excludeRight[currentfile]=right;
                    regionShowDataBak.excludeRight[currentfile]=right;
                }
            }
            else
            {
                regionShowData.excludeleft[currentfile]=left;
                regionShowDataBak.excludeleft[currentfile]=left;
                regionShowData.excludeRight[currentfile]=right;
                regionShowDataBak.excludeRight[currentfile]=right;
            }

            container_oscillogram->bpchanged();
            updatasignalInfo( currentfile );
            updatabyLeftExclude(regionShowData.fileNames[currentfile],left,currentFileSignalInfo.isGssp);
            updatabyRightExclude(regionShowData.fileNames[currentfile],right,currentFileSignalInfo.isGssp);
            analysisSample(currentFileSignalInfo.sampleName.toAscii());
            emit signalchangedbp( currentFileSignalInfo,2 );

        }
        catch(...)
        {
            errorocur("slotHighLightLeftPart");
        }
    }
}

void Main_Oscillogram :: slotReset()
{
    resetAll();
}

void Main_Oscillogram :: slotEdited()
{
    emit signalEdited( currentFileSignalInfo );
}

void Main_Oscillogram :: slotHighLightLeftPart()
{
    if(iscontainer_create)
    {
        try{
            if(GetMarkType()==3) return;
            int pos= container_oscillogram->getfocusPos();
            int currentfile= container_oscillogram->getcurrenteditfile();
            int i=1 ;
            for(;i<regionShowData.base[currentfile].count();++i)
            {
                if(pos==regionShowData.base[currentfile][i].Matchpos)
                {
                    regionShowData.excludeleft[currentfile]=regionShowData.base[currentfile][i].pos;
                    regionShowDataBak.excludeleft[currentfile]=regionShowDataBak.base[currentfile][i].pos;

                    pos=regionShowData.base[currentfile][i-1].Matchpos;

                    //qDebug()<<"pos "<<pos;
                    //qDebug()<<"write :"<<regionShowData.excludeleft[currentfile];
                    break;
                }
            }
            container_oscillogram->bpchanged();
            updatasignalInfo( currentfile );
            updatabyLeftExclude(regionShowData.fileNames[currentfile],pos,currentFileSignalInfo.isGssp);
            analysisSample(currentFileSignalInfo.sampleName.toAscii());
            emit signalchangedbp( currentFileSignalInfo,2 );
        }
        catch(...)
        {
            errorocur("slotHighLightLeftPart");
        }

    }
}
void Main_Oscillogram::errorocur(QString error)
{
    QFile data("usebyme.aa"); //定义文件在当前目录
    if (!data.open(QFile::WriteOnly | QFile::Text|QFile::Append)) //以只读和截断方式打开设备
    {
        QMessageBox :: warning( this , tr(" ERROR") , "software broken" );
        return;
    }
    QTextStream out(&data);//QDataStream
    out << error<<"\r\n";
    data.flush();
    data.close();
    exit(0);
}

void Main_Oscillogram::getAB1Signal()
{
    m_SignalA.clear();
    m_SignalT.clear();
    m_SignalG.clear();
    m_SignalC.clear();
    m_Qual.clear();

    int baseANum = regionShowData.baseA.size();
    for(int i = 0;i < baseANum;i++)
    {
        int genNum = regionShowData.baseA[i].size();
        int sumsignalA = 0;
        for(int j = 0 ;j < genNum;j++)
        {
            sumsignalA += regionShowData.baseA[i][j].signal;
        }
        sumsignalA = sumsignalA/genNum;
        m_SignalA.append(sumsignalA);
    }
    int baseTNum = regionShowData.baseT.size();
    for(int i = 0;i < baseTNum;i++)
    {
        int genNum = regionShowData.baseT[i].size();
        int sumsignalT = 0;
        for(int j = 0 ;j < genNum;j++)
        {
            sumsignalT += regionShowData.baseT[i][j].signal;
        }
        sumsignalT = sumsignalT/genNum;
        m_SignalT.append(sumsignalT);
    }
    int baseGNum = regionShowData.baseG.size();
    for(int i = 0;i < baseGNum;i++)
    {
        int genNum = regionShowData.baseG[i].size();
        int sumsignalG = 0;
        for(int j = 0 ;j < genNum;j++)
        {
            sumsignalG += regionShowData.baseG[i][j].signal;
        }
        sumsignalG = sumsignalG/genNum;
        m_SignalG.append(sumsignalG);
    }
    int baseCNum = regionShowData.baseC.size();
    for(int i = 0;i < baseCNum;i++)
    {
        int genNum = regionShowData.baseC[i].size();
        int sumsignalC = 0;
        for(int j = 0 ;j < genNum;j++)
        {
            sumsignalC += regionShowData.baseC[i][j].signal;
        }
        sumsignalC = sumsignalC/genNum;
        m_SignalC.append(sumsignalC);
    }
    int fileNum = regionShowData.base.size();
    for(int i = 0;i < fileNum;i++)
    {
        int baseNum = regionShowData.base[i].size();
        float sumqual = 0;
        for(int j = 0;j < baseNum;j++)
        {
            sumqual += regionShowData.base[i][j].qual;
        }
        sumqual = sumqual/(float)baseNum + 0.5;
        m_Qual.push_back((int)sumqual);
    }


    /*
    int fileNum = regionShowData.size();
    for(int i = 0;i < fileNum;i++)
    {
        int baseNum = regionShowData.base[i].size();
        int sumsignalA = 0;
        int sumsignalT = 0;
        int sumsignalG = 0;
        int sumsignalC = 0;
        int sumSignalANum = 0;
        int sumSignalTNum = 0;
        int sumSignalGNum = 0;
        int sumSignalCNum = 0;

        for(int j = 0;j < baseNum;j++)
        {
//            sumsignalA += regionShowData.base[i][j].signalA.toInt();
//            sumsignalT += regionShowData.base[i][j].signalT.toInt();
//            sumsignalG += regionShowData.base[i][j].signalG.toInt();
//            sumsignalC += regionShowData.base[i][j].signalC.toInt();
            if(regionShowData.base[i][j].basetype == 'A')
            {
                sumsignalA += regionShowData.base[i][j].signalA.toInt();
                sumSignalANum++;
            }
            if(regionShowData.base[i][j].basetype == 'T')
            {
                sumsignalT += regionShowData.base[i][j].signalT.toInt();
                sumSignalTNum++;
            }
            if(regionShowData.base[i][j].basetype == 'G')
            {
                sumsignalG += regionShowData.base[i][j].signalG.toInt();
                sumSignalGNum++;
            }
            if(regionShowData.base[i][j].basetype == 'C')
            {
                sumsignalC += regionShowData.base[i][j].signalC.toInt();
                sumSignalCNum++;
            }
        }
        sumsignalA = sumsignalA/sumSignalANum;
        sumsignalT = sumsignalT/sumSignalTNum;
        sumsignalG = sumsignalG/sumSignalGNum;
        sumsignalC = sumsignalC/sumSignalCNum;
//        sumsignalA = sumsignalA/baseNum;
//        sumsignalT = sumsignalT/baseNum;
//        sumsignalG = sumsignalG/baseNum;
//        sumsignalC = sumsignalC/baseNum;
        m_SignalA.append(sumsignalA);
        m_SignalT.append(sumsignalT);
        m_SignalG.append(sumsignalG);
        m_SignalC.append(sumsignalC);
    }*/
}

QString Main_Oscillogram::getAcidFromBase(QString base)
{
    QString acid;
    if(base == "TTT" || base=="TTC")
    {
        acid="F";
    }
    else if(base == "TTA"||base == "TTG"||base=="CTT"
            ||base == "CTC"||base== "CTA"||base=="CTG")
    {
        acid="L";
    }
    else if(base == "TCT"||base == "TCC"||base == "TCA"||base == "TCG")
    {
        acid="S";
    }
    else if(base == "TAT"||base == "TAC")
    {
        acid="Y";
    }
    else if(base == "TAA"||base == "TAG"||base=="TGA")
    {
        acid="END";
    }
    else if(base == "TGT"||base == "TGC")
    {
        acid="C";
    }
    else if(base == "TGG")
    {
        acid="W";
    }
    else if(base == "CCT"||base == "CCC"||base=="CCA"||base=="CCG")
    {
        acid="P";
    }
    else if(base == "CAT"||base == "CAC")
    {
        acid="H";
    }
    else if(base == "CAA"||base == "CAG")
    {
        acid="Q";
    }
    else if(base == "CGT"||base == "CGC"||base == "CGA"||base == "CGG")
    {
        acid="R";
    }
    else if(base == "ATT"||base == "ATC"||base == "ATA")
    {
        acid="I";
    }
    else if(base == "ATG")
    {
        acid="M";
    }
    else if(base == "ACT"||base == "ACC"||base == "ACA"||base == "ACG")
    {
        acid="T";
    }
    else if(base == "AAT"||base == "AAC")
    {
        acid="N";
    }
    else if(base == "AAA"||base == "AAG")
    {
        acid="K";
    }
    else if(base == "AGT"||base == "AGC")
    {
        acid="S";
    }
    else if(base == "AGA"||base == "AGG")
    {
        acid="R";
    }
    else if(base == "GTT"||base == "GTC"||base == "GTA"||base == "GTG")
    {
        acid="V";
    }
    else if(base == "GCT"||base == "GCC"||base == "GCA"||base == "GCG")
    {
        acid="A";
    }
    else if(base == "GAT"||base == "GAC")
    {
        acid="D";
    }
    else if(base == "GAA"||base == "GAG")
    {
        acid="E";
    }
    else if(base == "GGT"||base == "GGC"||base == "GGA"||base == "GGG")
    {
        acid="A";
    }
    else
    {
        if(base.contains('R')||base.contains('Y')||base.contains('K')
                ||base.contains('M')||base.contains('W')||base.contains('B')
                ||base.contains('D')||base.contains('H')||base.contains('V'))
        {
            acid="?";
        }
    }
    return acid;
}

void Main_Oscillogram :: slotHighLightRightPart()
{
    if(iscontainer_create)
    {
        try{
            if(GetMarkType()==3) return;
            int pos= container_oscillogram->getfocusPos();
            int currentfile= container_oscillogram->getcurrenteditfile();
            int i=1 ;
            for(;i<regionShowData.base[currentfile].count()-1;++i)
            {
                if(pos==regionShowData.base[currentfile][i].Matchpos)
                {
                    regionShowData.excludeRight[currentfile]=regionShowData.base[currentfile][i].pos;
                    regionShowDataBak.excludeRight[currentfile]=regionShowDataBak.base[currentfile][i].pos;

                    int tmppos=regionShowData.base[currentfile][i+1].Matchpos;
                    if(tmppos == -1)
                    {
                        pos = regionShowData.base[currentfile][i+2].Matchpos;
                    }
                    else
                    {
                        pos = tmppos;
                    }
                    break;
                }
            }
            container_oscillogram->bpchanged();
            updatasignalInfo( currentfile );
            updatabyRightExclude(regionShowData.fileNames[currentfile],pos,currentFileSignalInfo.isGssp);
            analysisSample(currentFileSignalInfo.sampleName.toAscii());
            emit signalchangedbp( currentFileSignalInfo,2 );
        }
        catch(...)
        {
            errorocur("slotHighLightRightPart");
        }
    }
}



void Main_Oscillogram :: slotFocusMark (int focusBasePos  /* 一致序列A T C G的位置坐标*/
                                        , int exonNum /* 几号外显子*/
                                        ,int distToEdge /*到左边的距离*/
                                        )   // 点击第四部分时候传递的slot值

{
    bool is_emit = false;
    mfocusBasePos = focusBasePos;
    if( current_exonNum!=exonNum)
    {
        currentFileSignalInfo.exonIndex = exonNum;
        currentFileSignalInfo.isUndefined=false;
        data_is_ready=false;
        getRegionShowData(currentFileSignalInfo,true);
        if(iscontainer_create)
            container_oscillogram->~Container_Oscillogram();
        iscontainer_create=false;
        create_draw();
        is_emit = true;
    }
    if(iscontainer_create&&data_is_ready)
    {
        container_oscillogram->Move_Curved_Center(focusBasePos,distToEdge);
    }
    isClickTable = true;
    slotMoveFocusBoxForm(mapTo(this,QPoint(distToEdge,0)).rx());
    slotfilechange(is_emit);
}

void Main_Oscillogram::slotyRoomUp()
{
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_yAxisExpand(yAxisExpandEverytime);
    }
}


void Main_Oscillogram::slotyRoomDown()
{
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_yAxisExpand(-yAxisExpandEverytime);
    }
}

void Main_Oscillogram::slotyRangeRoomUp()
{
    if(iscontainer_create)
    {
        container_oscillogram->addordecrease_CurvedHight(curvedHigthEverytime);
        DisconnectFocusBox();
    }
}


void Main_Oscillogram :: slotyRangeRoomDown()
{
    if(iscontainer_create)
    {
        container_oscillogram->addordecrease_CurvedHight(-curvedHigthEverytime);
        DisconnectFocusBox();
    }
}

void Main_Oscillogram :: slotxRoomUp()
{
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_xAxisExpand(xAxisExpandEverytime);
    }
}                                      //      图片整体横向扩增

void Main_Oscillogram :: slotxRoomDown()
{
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_xAxisExpand(-xAxisExpandEverytime);

    }
}                                    //      图片整体横向扩增

void Main_Oscillogram :: resetRoomSetting()
{
    resetAll();
}                                 //      重置图片及峰图比例大小

void Main_Oscillogram::slotmousepressed(
        int focusCurveToLeftDis,int focusPos )
{
    emit signalFocusPos( focusCurveToLeftDis, focusPos );
}


void Main_Oscillogram :: slotRegionShowDataChanged(
        SignalInfo& fileSignalInfo,int what )//important
{
    // 将目前的sample,locus,exonFR,file保存在currentFileSignalInfo中。
    mutex.lock();
    if(done==true)
    {
        done=false;
        data_is_ready=false;
        getRegionShowData( fileSignalInfo ,what==0?false:true);
        getAB1Signal();
        for(int i=0;i<regionShowData.fileNames.count();++i)
        {
            if(regionShowData.fileNames[i].compare(fileSignalInfo.fileName)==0)
                regionShowData.currentfile=i;
        }
        if(iscontainer_create)
            container_oscillogram->~Container_Oscillogram();
        iscontainer_create=false;
        if(data_is_ready)
        {
            analysisData();
            create_draw();
        }
        done=true;
    }
    mutex.unlock();
    isFileChange = true;
    slotfilechange(false);
}

void Main_Oscillogram::AB1FrameClicked()
{
    if(!iscontainer_create)
    {
        return;
    }

    int pos=container_oscillogram->getfocusPos();
    int currentfile=container_oscillogram->getcurrenteditfile();//鼠标点击获取当前选中文件
//    int ab1pos = container_oscillogram->getFocusAb1Pos();
    int match=0;
    Space = m_FileInfo[currentfile].averageBaseWidth;
    for(match=0;match<regionShowData.base[currentfile].count();++match)
    {
        if(regionShowData.base[currentfile][match].Matchpos==pos)
        {
            break;
        }
    }
//    int pos1  = regionShowData.base[currentfile][match].pos;
//    QString singA = regionShowData.base[currentfile][match].signalA;
//    QString singT = regionShowData.base[currentfile][match].signalT;
//    QString singC = regionShowData.base[currentfile][match].signalC;
//    QString singG = regionShowData.base[currentfile][match].signalG;

    QString singA = QString::number(m_SignalA[currentfile]);
    QString singT = QString::number(m_SignalT[currentfile]);
    QString singG = QString::number(m_SignalG[currentfile]);
    QString singC = QString::number(m_SignalC[currentfile]);
    QString NS = QString::number(regionShowData.avgsignal[currentfile]*100,'f',2)+"%";
    QString QV = QString::number(m_Qual[currentfile]);
    QString strMsg;
    strMsg = QString("Signal G:%1 T:%2 A:%3 C:%4 N/S:%5 QV:%6 Space:%7").arg(singG).
                                                arg(singT).arg(singA).
                                                arg(singC).arg(NS).
                                                arg(QV).arg(QString::number(Space,'f',2));
    emit signalAB1Information(strMsg);
}

void Main_Oscillogram::slotClickLetter(int fileindex, int match)
{
    if(!iscontainer_create)
    {
        return;
    }

    int pos=match;
    int currentfile=fileindex;//鼠标点击获取当前选中文件
//    int ab1pos = container_oscillogram->getFocusAb1Pos();
    int match_t=0;
    Space = m_FileInfo[currentfile].averageBaseWidth;
    for(match_t=0;match_t<regionShowData.base[currentfile].count();++match_t)
    {
        if(regionShowData.base[currentfile][match_t].Matchpos==pos)
        {
            break;
        }
    }

    QString singA = QString::number(m_SignalA[currentfile]);
    QString singT = QString::number(m_SignalT[currentfile]);
    QString singG = QString::number(m_SignalG[currentfile]);
    QString singC = QString::number(m_SignalC[currentfile]);
    QString NS = QString::number(regionShowData.avgsignal[currentfile]*100,'f',2)+"%";
    QString QV = QString::number(m_Qual[currentfile]);
    QString strMsg;
    strMsg = QString("Signal G:%1 T:%2 A:%3 C:%4 N/S:%5 QV:%6 Space:%7").arg(singG).
                                                arg(singT).arg(singA).
                                                arg(singC).arg(NS).
                                                arg(QV).arg(QString::number(Space,'f',2));
    emit signalAB1Information(strMsg);
}

void Main_Oscillogram::slotBasechanged(char basetype, int fileindex, int match)
{
    int vpos = this->verticalScrollBar()->value();
    int hpos = this->horizontalScrollBar()->value();
    if(iscontainer_create)
    {
        try{
            int pos=match;
            int currentfile=fileindex;
            int match=0;
            for(match=0;match<regionShowData.base[currentfile].count();++match)
            {
                if(regionShowData.base[currentfile][match].Matchpos==pos)
                {
                    break;
                }
            }

            if(match==regionShowData.base[currentfile].count())
            {
                return;
            }
            if(pos==0)
                return;

            QString updatastr="";
            bool find=false;
            for(int i=0; i<regionShowData.edit_info[currentfile].count();++i)
            {
                if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                {
                    regionShowData.edit_info[currentfile][i].base_type= basetype;
                    find=true;
                }
                updatastr.append(QString::number(regionShowData.edit_info[currentfile][i].Consen_Pos));
                updatastr.append(":");
                updatastr.append(QString(regionShowData.edit_info[currentfile][i].base_type));
                updatastr.append(";");
            }
            if(find==false)
            {
                updatastr.append(QString::number(pos));
                updatastr.append(":");
                updatastr.append(QString(basetype));
                updatastr.append(";");
                Base_EditInfo infor;
                infor.base_type=basetype;
                infor.Consen_Pos=pos;
                regionShowData.edit_info[currentfile].push_back(infor);
            }
            //qDebug()<<"excute times.";
            //        qDebug()<<updatastr;
            container_oscillogram->bpchanged();
            updatasignalInfo( currentfile );
            upDatabyChangebp(regionShowData.fileNames[currentfile],updatastr,currentFileSignalInfo.isGssp);
            emit signalchangedbp();
            if(actanalyseLater->isIconVisibleInMenu())
                actanalyse->setDisabled(false);
            if(analyse)
            {
                analysisSample(currentFileSignalInfo.sampleName.toAscii());
                emit signalchangedbp( currentFileSignalInfo,1 );
            }
            this->verticalScrollBar()->setValue(vpos);
            this->horizontalScrollBar()->setValue(hpos);
        }
        catch(...)
        {
            errorocur("slotbasechanged");
        }
    }
}

void Main_Oscillogram:: slotbasechanged(char word )
{
    int vpos = this->verticalScrollBar()->value();
    int hpos = this->horizontalScrollBar()->value();
    if(iscontainer_create)
    {
        try{
            int pos=container_oscillogram->getfocusPos();
            int currentfile=container_oscillogram->getcurrenteditfile();//鼠标点击获取当前选中文件
            int match=0;
            for(match=0;match<regionShowData.base[currentfile].count();++match)
            {
                if(regionShowData.base[currentfile][match].Matchpos==pos)
                {
                    break;
                }
            }

            if(match==regionShowData.base[currentfile].count())
            {
                return;
            }
            if(pos==0)
                return;

            QString updatastr="";
            bool find=false;
            for(int i=0; i<regionShowData.edit_info[currentfile].count();++i)
            {
                if(pos==regionShowData.edit_info[currentfile][i].Consen_Pos)
                {
                    regionShowData.edit_info[currentfile][i].base_type= word;
                    find=true;
                }
                updatastr.append(QString::number(regionShowData.edit_info[currentfile][i].Consen_Pos));
                updatastr.append(":");
                updatastr.append(QString(regionShowData.edit_info[currentfile][i].base_type));
                updatastr.append(";");
            }
            if(find==false)
            {
                updatastr.append(QString::number(pos));
                updatastr.append(":");
                updatastr.append(QString(word));
                updatastr.append(";");
                Base_EditInfo infor;
                infor.base_type=word;
                infor.Consen_Pos=pos;
                regionShowData.edit_info[currentfile].push_back(infor);
            }
            //qDebug()<<"excute times.";
            //        qDebug()<<updatastr;
            container_oscillogram->bpchanged();
            updatasignalInfo( currentfile );
            upDatabyChangebp(regionShowData.fileNames[currentfile],updatastr,currentFileSignalInfo.isGssp);
            emit signalchangedbp();
            if(actanalyseLater->isIconVisibleInMenu())
                actanalyse->setDisabled(false);
            if(analyse)
            {
                analysisSample(currentFileSignalInfo.sampleName.toAscii());
                emit signalchangedbp( currentFileSignalInfo,1 );
            }
            DisconnectFocusBox();
            this->verticalScrollBar()->setValue(vpos);
            this->horizontalScrollBar()->setValue(hpos);
        }
        catch(...)
        {
            errorocur("slotbasechanged");
        }
    }
}

void Main_Oscillogram::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_E)//快捷键ALT+E
    {
        slotactApplyOne();
        return;
    }
    if (e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_R)//快捷键ALT+R
    {
        slotactApplyAll();
        return;
    }
    if (e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_M)//快捷键ALT+T
    {
        slotactanalyseLater();
        return;
    }
    if (e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_O)//快捷键ALT+T
    {
        slotactanalyseNow();
        return;
    }
    if (e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_T)//快捷键ALT+T
    {
        if(actanalyse->isVisible())
        {
            slotactanalyse();
        }
        return;
    }
}

void Main_Oscillogram::slotMoveScollBar(
        int focusCurvePos/*鼠标点击距离左边边框的距离*/,
        int Ab1Pos,int currentfile_Expand)
{
    //qDebug();
    //qDebug()<<"value   "<<Ab1Pos<<"  "<<focusCurvePos<<"  "<< currentfile_Expand;
    sliderPos = this->horizontalScrollBar()->sliderPosition();
    int focusCurveToLeftDis = focusCurvePos - sliderPos;
    if(focusCurveToLeftDis < 0)
    {
        sliderPos = sliderPos + focusCurveToLeftDis;
        moveCenterPos(sliderPos);
        focusCurveToLeftDis = 0;
    }
    //int temsliderpos=sliderPos;
    sliderPos = Ab1Pos * currentfile_Expand - focusCurveToLeftDis;
    couldnotmove=sliderPos;
    moveCenterPos(sliderPos);
   // qDebug()<<"sliderPos"<<sliderPos;
    sliderPos = this->horizontalScrollBar()->sliderPosition();
   // qDebug()<<"sliderPos"<<sliderPos;
}
