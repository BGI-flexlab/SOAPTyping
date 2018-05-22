#include "Main_Oscillogram.h"
#include "key_update.h"


void FocusBoxFrame::mouseDoubleClickEvent(QMouseEvent * event)//弹出改变碱基窗体
{
    //emit mouseClickEvent(event->x(),event->y());

    if(ChangeBp==NULL)
    {
        if(((Main_Oscillogram *)this->parent())->Getiscontainer_create()
                &&((Main_Oscillogram *)this->parent())->Getcontainer_focusPosOk()
                &&((Main_Oscillogram *)this->parent())->GetMarkType()!=3)
        {
            ChangeBp= new ChangeForm();
            ChangeBp->setWindowTitle("Change Base");
            ChangeBp->setFixedSize(300,150);
            int x=mapToGlobal(QPoint(event->x(),event->y()) ).x()-ChangeBp->width();
            int y=mapToGlobal(QPoint(event->x(),event->y()) ).y()-ChangeBp->height();
            if(x<0){x=0;}
            if(y<0){y=0;}
            ChangeBp->move (x,y);

            ChangeBp -> setBackgroundRole( QPalette::Base );
            ChangeBp -> setAutoFillBackground( true );
            ChangeBp->show();
            //ChangeBp->setAttribute(Qt::WA_ShowModal, true);
            connect(ChangeBp,SIGNAL(IHaveClosed()),this,SLOT(ChangeFormClosed()));
            connect(ChangeBp,SIGNAL(formbasechanged(char)),
                    this,SLOT(slotbasechanged(char)));
            emit ChangeFormShow();
        }

    }
}
void FocusBoxFrame::mousePressEvent(QMouseEvent *event)
{
    if( event -> button() == Qt::LeftButton)
        emit mouseClickEvent(event->x(),event->y());

}
void FocusBoxFrame::ChangeFormClosed()
{
    delete ChangeBp;
    ChangeBp=NULL;
    emit ChangeFormClosedto_container();
}
FocusBoxFrame::~FocusBoxFrame()
{
}
void FocusBoxFrame::contextMenuEvent(QContextMenuEvent * e)
{
    ( (Main_Oscillogram*)(this->parent()))->rigthMenuShow(e);
}
void FocusBoxFrame::slotbasechanged(char word)
{
    //qDebug()<<"one time";
    emit basechanged(word);
}
void ChangeForm::closeEvent(QCloseEvent * e)
{
    emit IHaveClosed();
}
void ChangeForm::BpChanged(int WhichOneChoose)
{
    // qDebug()<<WhichOneChoose;

    char changebp=' ';
    switch(WhichOneChoose)
    {
    case 0:
        changebp='A';break;
    case 1:
        changebp='T';break;
    case 2:
        changebp='C';break;
    case 3:
        changebp='G';break;
    case 4:
        changebp='R';break;
    case 5:
        changebp='Y';break;
    case 6:
        changebp='K';break;
    case 7:
        changebp='M';break;
    case 8:
        changebp='S';break;
    case 9:
        changebp='W';break;
    default:
        qDebug()<<"error";

    }
    this->hide();
    if (changebp!=' ')
    {
        // qDebug()<< "BpChanged ";
        emit formbasechanged(changebp);

    }
    this->close();


}



ChangeForm::ChangeForm()//构建修改碱基窗体
{
    setAttribute(Qt::WA_ShowModal, true);
    group=new QButtonGroup(this);
    group->setExclusive(true);
    connect(group,SIGNAL(buttonClicked(int)),this,SLOT(BpChanged(int)));
    above=new QGridLayout();
    //above->setRowMinimumHeight(1,40);
    above->setVerticalSpacing(40);
    below=new QHBoxLayout();
    mainlay=new QVBoxLayout();
    Abp =new QRadioButton("A");
    Abp->setShortcut(QKeySequence(Qt::Key_A));
    above->addWidget(Abp,0,0);
    Tbp =new QRadioButton("T");
    Tbp->setShortcut(QKeySequence(Qt::Key_T));
    above->addWidget(Tbp,0,1);
    Cbp =new QRadioButton("C");
    Cbp->setShortcut(QKeySequence(Qt::Key_C));
    above->addWidget(Cbp,0,2);
    Gbp =new QRadioButton("G");
    Gbp->setShortcut(QKeySequence(Qt::Key_G));
    above->addWidget(Gbp,0,3);
    Rbp =new QRadioButton("R");
    Rbp->setShortcut(QKeySequence(Qt::Key_R));
    above->addWidget(Rbp,0,4);

    Ybp =new QRadioButton("Y");
    Ybp->setShortcut(QKeySequence(Qt::Key_Y));
    above->addWidget(Ybp,1,0);
    Kbp =new QRadioButton("K");
    Kbp->setShortcut(QKeySequence(Qt::Key_K));
    above->addWidget(Kbp,1,1);
    Mbp =new QRadioButton("M");
    Mbp->setShortcut(QKeySequence(Qt::Key_M));
    above->addWidget(Mbp,1,2);
    Sbp =new QRadioButton("S");
    Sbp->setShortcut(QKeySequence(Qt::Key_S));
    above->addWidget(Sbp,1,3);
    Wbp =new QRadioButton("W");
    Wbp->setShortcut(QKeySequence(Qt::Key_W));
    above->addWidget(Wbp,1,4);



    cancel=new QPushButton("&Cancel");
    cancel->setShortcut(Qt::Key_Escape);

    below->addStretch();
    below->addWidget(cancel);

    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
    mainlay->addLayout(above);
    mainlay->addLayout(below);
    setLayout(mainlay);

    group->addButton(Abp,0);
    group->addButton(Tbp,1);
    group->addButton(Cbp,2);
    group->addButton(Gbp,3);
    group->addButton(Rbp,4);
    group->addButton(Ybp,5);
    group->addButton(Kbp,6);
    group->addButton(Mbp,7);
    group->addButton(Sbp,8);
    group->addButton(Wbp,9);


}





ChangeForm::~ChangeForm()
{
    //    delete Abp,Tbp,Cbp,Gbp,Rbp,Ybp,
    //            Kbp,Mbp,Sbp,Wbp,Bbp,Dbp,Hbp,Vbp;
    //    delete above;
    //    delete below;
    //    delete mainlay;
    //    delete group;
    //    delete cancel;
}



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



    FocusBoxForm =new  FocusBoxFrame(this);
    FocusBoxForm->setFixedSize(
                15,
                QApplication::desktop()->height());
    FocusBoxForm->setFrameStyle( QFrame::Box | QFrame::Raised );
    FocusBoxForm->setStyleSheet("color:rbg(100,250,250)");

    FocusBoxForm->move(QPoint(1,1));
    FocusBoxForm->hide();
    FocusBoxForm->setWindowFlags(Qt::WindowStaysOnTopHint);
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
    befordata.filesxExpand=2;
    befordata.filesyExpand=1;
    befordata.curedHigth=200;
    befordata.rightMenuX=rightMenuX2;
    couldnotmove=0;
    //this->horizontalScrollBar();



    connect(actApplyOne,SIGNAL(triggered()),this,SLOT(slotactApplyOne()));
    connect(actApplyAll,SIGNAL(triggered()),this,SLOT(slotactApplyAll()));
    connect(actanalyseLater,SIGNAL(triggered()),this,SLOT(slotactanalyseLater()));
    connect(actanalyseNow,SIGNAL(triggered()),this,SLOT(slotactanalyseNow()));
    connect(actanalyse,SIGNAL(triggered()),this,SLOT(slotactanalyse()));

    connect(actDelete, SIGNAL(triggered()), this, SLOT(slotDelteThisFile()));

    connect(hscoll,SIGNAL(valueChanged(int)),this,SLOT(slotslidermovebymouse(int )));
    connect(FocusBoxForm,SIGNAL(mouseClickEvent(int,int)),
            this,SLOT(slotFocusclik(int,int)));
    connect(hscoll,SIGNAL(sliderPressed()),this,SLOT(slotslidermousepress()));
    connect(FocusBoxForm,SIGNAL(basechanged(char)),this,
            SLOT(slotbasechanged(char)));
    connect( actExcludeLeft,SIGNAL(triggered()),
             this,SLOT(slotHighLightLeftPart()));
    connect( actExcludeRight,SIGNAL(triggered()),
             this,SLOT(slotHighLightRightPart()));
    connect( actResetExclude,SIGNAL(triggered()),
             this,SLOT(slotResetExclude()));
    //  slider=this->horizontalScrollBar();
}


void Main_Oscillogram::slotFocusclik(int x, int y)
{
    int slider=this->verticalScrollBar()->sliderPosition();
    //qDebug()<<"x= "<<x<<"y=  "<<y;
    //qDebug()<<slider<<"slider";

    if(iscontainer_create)
    {
        container_oscillogram->getwhichFile(x,y+slider);
    }
    // this->repaint();

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
    FocusBoxForm->setFixedSize(
                container_oscillogram->getbaseItemWidth(),
                QApplication::desktop()->height());
    //regionShow -> setAttribute( Qt::WA_TranslucentBackground , true );
    this -> setWidget( container_oscillogram );
    this -> setBackgroundRole( QPalette::Base );
    this -> setAutoFillBackground( true );

    //    connect(container_oscillogram,SIGNAL(signalfocusrepian())
    //            ,FocusBoxForm,SLOT(repaint()));
    //connect(slider,SIGNAL(),this,slotslidermovebymouse());
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
    connect(FocusBoxForm,SIGNAL(ChangeFormShow()),
            container_oscillogram,SLOT(slotChangeFormShow()));
    connect(FocusBoxForm,SIGNAL(ChangeFormClosedto_container()),
            container_oscillogram,SLOT(slotChangeFormClosed()));

    connect(container_oscillogram,
            SIGNAL(signalMoveFocusBoxForm(int)),
            this,SLOT(slotMoveFocusBoxForm(int))  );
    //    connect(this,SIGNAL(signaladdordecreaseOne(bool)),
    //            container_oscillogram,
    //            SLOT(slotaddordecreaseOne(bool)));
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


    if(iscontainer_create)
    {

        int movepos=pos-sliderPos;
        int where=FocusBoxForm->pos().x()-movepos;
        FocusBoxForm->move(QPoint(where,1));
        sliderPos=this->horizontalScrollBar()->sliderPosition();
    }
}
void  Main_Oscillogram::slot_inform_other(int ConsensePos,int mousePos)
{
    emit signal_toinform_other(ConsensePos,current_exonNum,mousePos);
}
void Main_Oscillogram:: slotbasechanged(char word )
{
    if(iscontainer_create)
    {
        try{
            int pos=container_oscillogram->getfocusPos();
            int currentfile=container_oscillogram->getcurrenteditfile();
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
        }
        catch(...)
        {
            errorocur("slotbasechanged");
        }
    }
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
void Main_Oscillogram::slotMoveFocusBoxForm(int xPos)
{
    int FoucusBox=15;
    if(couldnotmove<0)
        xPos=xPos+couldnotmove;
    //qDebug()<<"   "<<couldnotmove<<xPos;
    if(iscontainer_create&&data_is_ready)
    {
        FoucusBox=container_oscillogram->
                getbaseItemWidth();
        if(xPos-FoucusBox/2+3<2)
        {
            FocusBoxForm->move(QPoint(1,1));
        }

        FocusBoxForm->move(QPoint(xPos-FoucusBox/2+3,1));
        FocusBoxForm->show();
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
        FocusBoxForm->hide();
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
    actApplyAll                                 = new QAction(QIcon(":/images/apply.png"),tr("Apply All"),this);
    actApplyAll->setDisabled(true);
    actanalyseLater                             = new QAction(QIcon(":/images/apply.png"),tr("Edit Multi"),this);
    actanalyseLater->setIconVisibleInMenu(false);
    actanalyseNow                               = new QAction(QIcon(":/images/apply.png"),tr("Edit One"),this);
    actanalyseNow->setDisabled(true);
    actanalyse                                  = new QAction(tr("Analyze"),this);
 //   actanalyse->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
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

                    pos=regionShowData.base[currentfile][i+1].Matchpos;
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
    if( current_exonNum!=exonNum)
    {
        currentFileSignalInfo.exonIndex = exonNum;
        currentFileSignalInfo.isUndefined=false;
        data_is_ready=false;
        getRegionShowData(currentFileSignalInfo,true);
        FocusBoxForm->hide();
        if(iscontainer_create)
            container_oscillogram->~Container_Oscillogram();
        iscontainer_create=false;
        create_draw();
//        slotFocusMark(focusBasePos,exonNum,distToEdge);
        is_emit = true;
    }
//    else
//    {
        if(iscontainer_create&&data_is_ready)
        {
            container_oscillogram->Move_Curved_Center(focusBasePos,distToEdge);
        }
//    }
    slotMoveFocusBoxForm(mapTo(this,QPoint(distToEdge,0)).rx());
    slotfilechange(is_emit);
}

void Main_Oscillogram::slotyRoomUp()
{
    // sliderPos = this->horizontalScrollBar()->sliderPosition();
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_yAxisExpand(yAxisExpandEverytime);


    }
    //moveCenterPos(sliderPos);
}


void Main_Oscillogram::slotyRoomDown()
{
    //sliderPos = this->horizontalScrollBar()->sliderPosition();
    if(iscontainer_create)
    {
        container_oscillogram->
                addordecrease_yAxisExpand(-yAxisExpandEverytime);
    }
    // moveCenterPos(sliderPos);
}

void Main_Oscillogram::slotyRangeRoomUp()
{
    if(iscontainer_create)
    {
        container_oscillogram->addordecrease_CurvedHight(curvedHigthEverytime);
    }
}


void Main_Oscillogram :: slotyRangeRoomDown()
{
    if(iscontainer_create)
    {
        container_oscillogram->addordecrease_CurvedHight(-curvedHigthEverytime);
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
    //currentFileSignalInfo = fileSignalInfo;
    // 将目前的sample,locus,exonFR,file保存在currentFileSignalInfo中。
    //    if(!what)
    //        return;
    mutex.lock();
    if(done==true)
    {
        done=false;
        data_is_ready=false;
        getRegionShowData( fileSignalInfo ,what==0?false:true);
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
    slotfilechange(false);
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


