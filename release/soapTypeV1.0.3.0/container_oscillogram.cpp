#include "container_oscillogram.h"

Container_Oscillogram::Container_Oscillogram(
        RegionShowData * regionShowData,
        RegionShowData *regionShowDataBak
        ,SaveData * befordata,QWidget *parent
        ) :
    QWidget(parent)
{
    container_savedata =befordata;
    Container_regionShowDataBak=regionShowDataBak;
    ChangeFormshow=false;
    //FocusPos=0;
    //addordecreaseOne=false;
    currenteditfile=regionShowData->currentfile;
    baseItemWidth=15;
    Container_regionShowData=regionShowData;
    focusAb1Pos = 0;
    FocusPos=0;
    focusCurvePosToab1Edge=QApplication::desktop()->width()/2;;


    //regLeftLimit                          = regionShowData->leftLimits;
    //regRightLimit                         = regionShowData->rightLimits;
    filecount = regionShowData->fileNames.size();

    curvedLines.clear();
    for( int i=0 ; i < filecount ; i++ )
    {

        CurvedLine *renderArea =
                new CurvedLine( i,regionShowData,regionShowDataBak
                                ,container_savedata,this);
        curvedLines.push_back( renderArea );
        connect(renderArea,SIGNAL(mouserelease(QMouseEvent *)),this,SLOT(mouseRelease(QMouseEvent *)));
    }
    // alignPosByExonPos(focusPos,focusAb1Pos);
    mainLayout = new QGridLayout;

#if defined( Q_OS_SYMBIAN ) || defined( Q_WS_MAEMO_5 ) || defined( Q_WS_SIMULATOR )
    mainLayout -> setSizeConstraint( QLayout::SetNoConstraint );
#endif
    mainLayout -> setMargin( 0 );
    mainLayout -> setSpacing( 0 );
    int j = 0;
    for ( int h = 0 ; h < filecount ; h++ )
    {
        mainLayout -> addWidget( curvedLines[h] , j++ , 0 );
    }
    setLayout( mainLayout );
    setthiswidth();
}


void Container_Oscillogram :: paintEvent ( QPaintEvent *)
{
    QPainter paint( this );
    HighLightGivenFile(paint,currenteditfile);

}


void Container_Oscillogram::slotChangeFormClosed()
{
    ChangeFormshow=false;
}
void Container_Oscillogram::slotChangeFormShow()
{
    ChangeFormshow=true;
}
//void Container_Oscillogram::slotFocusclik(int x,int y)
//{
//    getwhichFile(x,y);
//}
void  Container_Oscillogram::getwhichFile(int x,int y)
{



    int FocusMarkxPos = x;
    int FocusMarkyPos = y;
    //FocusPos=FocusMarkyPos;
    int clickfile=0, yhight=0;
    for(int i=0;i<curvedLines.count();++i)
    {
        if(FocusMarkyPos>yhight&&
                FocusMarkyPos<(yhight+curvedLines[i]->getCurvedHight()))
        {
            clickfile=i;
            break;
        }
        yhight+=curvedLines[i]->getCurvedHight();
    }
    if(clickfile!=currenteditfile)
    {
        repaint();
        currenteditfile=clickfile;
        emit signalfilechange(true);
    }
    int exonPos = 0;
    ab1PosToExonPos(FocusMarkxPos,exonPos);
    if(exonPos != -1)
    {

        //emit signalFocusPos(exonPos,FocusMarkxPos);//跳到RegionShow 中的slotFocusPos函数
        Repaint_Curved(exonPos,FocusMarkxPos);
        //            emit signalMoveFocusBoxForm(mapToParent(QPoint(FocusMarkxPos,0)).rx());

    }

    this->repaint();

    //qDebug()<<"x= "<<x<<"y=  "<<y;
    return;

}
void Container_Oscillogram ::mouseReleaseEvent( QMouseEvent* event )//鼠标单击移动对齐框
{    
    if( event -> button() == Qt::LeftButton &&!ChangeFormshow)
    {
        getwhichFile( event -> pos().x(),event -> pos().y());
    }
    return;
}



void Container_Oscillogram :: HighLightGivenFile( QPainter &paint ,int whichfile)
{
    try{
        int higth=0;
        for(int i=0;i<filecount;++i)
        {
            higth+=curvedLines[i]->getCurvedHight();
        }
        this->setFixedHeight(higth+100);
        higth=0;
        for(int i=0;i<filecount;++i)
        {
            curvedLines[i]->move(0,higth+1);
            higth+=curvedLines[i]->getCurvedHight();
        }
        paint.setPen( QColor(204,204,204) );
        paint.setBrush( QColor(204,204,204) );
        int yhigth=0;
        for(int i=0;i<whichfile;++i)
        {
            yhigth+=curvedLines[i]->getCurvedHight();
        }
        paint.drawRect( 0 , yhigth , curvedLines[whichfile]->GetWidth() , curvedLines[whichfile]->getLineHight() );

    }
    catch(...)
    {
        ErrorWrite::errorocur("alignPosByExonPos");
    }
}


void Container_Oscillogram :: slotEdited()
{
    emit signalTableEdited();
}

void Container_Oscillogram::addordecrease_CurvedHight(int num)
{
    try{
        if(!container_savedata->addordecreaseOne)
        {
            container_savedata->curedHigth+=num;
            if(container_savedata->curedHigth<10&&num<0)
            {
                container_savedata->curedHigth=10;
            }
        }
        if(container_savedata->addordecreaseOne)
        {
            curvedLines[currenteditfile]->addordecrease_CurvedHight(num);
            curvedLines[currenteditfile]->reDrawAll();
            //container_savedata->curedHigth[currenteditfile]+=num;

        }
        else
        {
            for(int i=0;i<filecount;++i)
            {
                curvedLines[i]->addordecrease_CurvedHight(num);

                curvedLines[i]->reDrawAll();

            }
        }

        //Repaint_Curved(FocusPos,focusCurvePosToab1Edge);
        int higth=0;
        for(int i=0;i<filecount;++i)
        {
            higth+=curvedLines[i]->getCurvedHight();
        }
        this->setFixedHeight(higth+100);
        higth=0;
        for(int i=0;i<filecount;++i)
        {
            curvedLines[i]->move(0,higth+1);
            higth+=curvedLines[i]->getCurvedHight();
        }

    }
    catch(...)
    {
        ErrorWrite::errorocur("alignPosByExonPos");
    }
}
void Container_Oscillogram::addordecrease_xAxisExpand(double num)
{
    try
    {
        if(!container_savedata->addordecreaseOne)
        {
            container_savedata->filesxExpand+=num;
            if(container_savedata->filesxExpand<1&&num<0)
            {
                container_savedata->filesxExpand=1;
            }
        }
        if(container_savedata->addordecreaseOne)
        {
            curvedLines[currenteditfile]->addordecrease_xAxisExpand(num);
            //curvedLines[currenteditfile]->reDrawAll();
            //container_savedata->filesxExpand[currenteditfile]+=num;
        }
        else
        {
            for(int i=0 ;i<filecount;++i)
            {
                curvedLines[i]->addordecrease_xAxisExpand(num);
                //container_savedata-> filesxExpand[i]+=num;

            }
        }

        Move_Curved_Center(FocusPos,focusCurvePosToab1Edge);
        setthiswidth();
        int higth=0;
        for(int i=0;i<filecount;++i)
        {
            higth+=curvedLines[i]->getCurvedHight();
        }
        this->setFixedHeight(higth+100);
        higth=0;
        for(int i=0;i<filecount;++i)
        {
            curvedLines[i]->move(0,higth+1);
            higth+=curvedLines[i]->getCurvedHight();
        }

    }
    catch(...)
    {
        ErrorWrite::errorocur("alignPosByExonPos");
    }


}
void Container_Oscillogram::setthiswidth()
{
    int mostwidth=0;
    for(int i=0;i<filecount;++i)
    {
        if(mostwidth<curvedLines[i]->GetWidth())
        {
            mostwidth=curvedLines[i]->GetWidth();
        }
    }
    this->setFixedWidth(mostwidth+QApplication::desktop()->width());
}
void Container_Oscillogram::addordecrease_yAxisExpand(double num)
{
    try{
        if(!container_savedata->addordecreaseOne)
        {
            container_savedata->filesyExpand+=num;
            if(container_savedata->filesyExpand<0&&num<0)
            {
                container_savedata->filesyExpand=0.1;
            }
        }
        if(container_savedata->addordecreaseOne)
        {
            curvedLines[currenteditfile]->addordecrease_yAxisExpand(num);
            curvedLines[currenteditfile]->reDrawAll();
            //container_savedata->filesyExpand[currenteditfile]+=num;
        }
        else
        {
            for(int i=0 ;i<filecount;++i)
            {
                curvedLines[i]->addordecrease_yAxisExpand(num);
                curvedLines[i]->reDrawAll();
                //container_savedata->filesyExpand[i]+=num;
            }
        }
        int higth=0;
        for(int i=0;i<filecount;++i)
        {
            higth+=curvedLines[i]->getCurvedHight();
        }
        this->setFixedHeight(higth+100);
        higth=0;
        for(int i=0;i<filecount;++i)
        {
            curvedLines[i]->move(0,higth+1);
            higth+=curvedLines[i]->getCurvedHight();
        }

    }
    catch(...)
    {
        ErrorWrite::errorocur("alignPosByExonPos");
    }

    //Repaint_Curved(FocusPos,focusCurvePosToab1Edge);
}

void Container_Oscillogram::slotfocusrepian()
{
    emit signalfocusrepian();
}
void Container_Oscillogram :: Repaint_Curved( int focusPos /*外显子的位置*/,
                                              int focusCurvePos/*鼠标点击距离左边边框的距离*/ )
{

    //int sliderPos = this->horizontalScrollBar()->sliderPosition();
    //int focusCurveToLeftDis = focusCurvePos - sliderPos;
    FocusPos=focusPos;
    alignPosByExonPos(focusPos,focusAb1Pos);

    foreach(CurvedLine * one ,curvedLines)
    {
        one->reDrawAll();
    }
    //regionShow_;

    focusCurvePosToab1Edge=mapToParent(QPoint(focusCurvePos,0)).rx();
    emit signalMoveFocusBoxForm(mapToParent(QPoint(focusCurvePos,0)).rx());
    emit signalMoveScollBar(focusCurvePos,
                            focusAb1Pos,
                            curvedLines[maxPosfile]->
                            getregionShow_xAxisExpand());
    //emit signalFocuschange(mapToParent(QPoint(focusCurvePos,0)).rx(),focusPos);
    emit signal_inform_other(focusPos,focusCurvePosToab1Edge);


}

void Container_Oscillogram::xAxisExpand_aligning(int focusPos, int focusCurvePos)
{
    if(focusPos<0)
        return;
    alignPosByExonPos(focusPos,focusAb1Pos);
    foreach(CurvedLine * one ,curvedLines)
    {
        one->reDrawAll();
    }
    emit signalMoveScollBar(mapFromParent(QPoint(focusCurvePos,0)).rx(),
                            focusAb1Pos,
                            curvedLines[maxPosfile]->
                            getregionShow_xAxisExpand());
}
void Container_Oscillogram :: Move_Curved_Center(int focusPos /*外显子的位置*/,
                                                 int focusCurvePos/*鼠标点击距离左边边框的距离*/ )
{


    if(focusPos<0)
    {
        return;
    }
    FocusPos=focusPos;


    alignPosByExonPos(focusPos,focusAb1Pos);

    foreach(CurvedLine * one ,curvedLines)
    {
        one->reDrawAll();
    }
    //regionShow_;

    focusCurvePosToab1Edge=focusCurvePos;
    emit signalMoveFocusBoxForm(mapToParent(QPoint(focusCurvePos,0)).rx());
    //qDebug()<<"Move_Curved_Center1";
    emit signalMoveScollBar(mapFromParent(QPoint(focusCurvePos,0)).rx(),
                            focusAb1Pos
                            ,curvedLines[maxPosfile]->
                            getregionShow_xAxisExpand());
    //  qDebug()<<"Move_Curved_Center2";

}


void Container_Oscillogram :: ab1PosToExonPos( int &MoseClickPos , int &exonPos )//根据鼠标位置找到ba1文件中对应的碱基
{
    try{
        int i =currenteditfile ;/*(int)( FocusMarkyPos / ( 3*LineHight + regionShow_curveLineHigh ) )*/
        double FocusMarkxPos2 =
                MoseClickPos/curvedLines[i]->getregionShow_xAxisExpand();
        if( ( FocusMarkxPos2 >= Container_regionShowData->leftLimits[i]  )
                && ( FocusMarkxPos2 <= Container_regionShowData->rightLimits[i] ) )
        {
            // FileInfo_ly fileInfo = findFileInfoByFileName(Container_regionShowData->fileNames[i]);
            //QStringList baseMatchConsensusPoss = fileInfo.baseMatchConsensusPos.split(";");
            int basePosSize = Container_regionShowData->base[i].size();
            int j=0;
            for ( ; j < basePosSize; j++ )
            {
                if ( Container_regionShowData->base[i][j].Matchpos != -1 &&
                     Container_regionShowData->base[i][j].pos
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
                if( Container_regionShowData->base[i][h].Matchpos != -1 )
                {
                    break;
                }
            }
            if(h == -1)
            {
                exonPos = -1;
                return;
            }
            if( ( FocusMarkxPos2 - Container_regionShowData->base[i][h].pos ) >
                    ( Container_regionShowData->base[i][j].pos
                      - Container_regionShowData->base[i][h].pos)/2 )
            {
                if( Container_regionShowData->base[i][j].pos
                        > Container_regionShowData->rightLimits[i] )
                {
                    exonPos = Container_regionShowData->base[i][h].Matchpos;
                    //baseMatchConsensusPoss[h].toInt();
                    return;
                }
                exonPos = Container_regionShowData->base[i][j].Matchpos;
                //baseMatchConsensusPoss[j].toInt();
                return;
            }
            else
            {
                if( Container_regionShowData->base[i][j-1].pos
                        < Container_regionShowData->leftLimits[i] )
                {
                    exonPos =Container_regionShowData->base[i][j].Matchpos;;
                    //baseMatchConsensusPoss[j].toInt();
                    return;
                }
                exonPos =Container_regionShowData->base[i][h].Matchpos;
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

/*
bool Container_Oscillogram :: judgeExonPosIsIn( int &exonPos )
{
    for (int i=0; i<Container_regionShowData->fileNames.size(); i++)
    {
        int start = 0;
        int end   = 0;
        FileInfo_ly fileInfo = findFileInfoByFileName(Container_regionShowDataBak->fileNames[i]);
        QStringList baseMatchConsensusPosS = fileInfo.baseMatchConsensusPos.split(";");
        int j=0;
        for(;j<baseMatchConsensusPosS.size();j++)
        {
            if(baseMatchConsensusPosS[j]==QString::number(-1)){continue;}
            else{
                if(start >= baseMatchConsensusPosS[j].toInt()){start = baseMatchConsensusPosS[j].toInt();}
                break;}
        }
        if(j!=0)
            j--;
        for(;j<baseMatchConsensusPosS.size();j++)
        {
            if(end <= baseMatchConsensusPosS[j].toInt())
            {
                end = baseMatchConsensusPosS[j].toInt();
            }
        }
        if((start <= exonPos)&&(end >= exonPos))
        {
            return true;
        }
    }
    return false;
}

*/

void Container_Oscillogram :: alignPosByExonPos( int &exonPos
                                                 /*对应到一致性序列的下标位置*/ ,
                                                 int &maxCorrespondingBasePos )
{
    try
    {
        // 判断该外显子位置是否存在。
        if(exonPos<0)
        {
            qDebug() << "The exonPos is not in the current exon!";
            return;
        }

        // 获得该外显子位置在AB1文件中对应的最大坐标。
        // 定义数组correspondingBasePosS保存该外显子对应相应文件的ab1位置坐标。
        QVector <int> correspondingBasePosS;
        // 定义maxCorrespondingBasePos，获取最大ab1位置坐标。
        maxCorrespondingBasePos = 0;
        maxPosfile=0;
        // 对每一个文件进行分析
        for (int i=0; i<Container_regionShowDataBak->fileNames.size(); i++)
        {
            if(Container_regionShowData->infor[i].isUndefined)
            {
                continue;
            }


            QVector<BaseInfor>basePosStmp;
            foreach( const BaseInfor & base,Container_regionShowDataBak->base[i])
            {
                if (base.Matchpos!=-1)
                {
                    basePosStmp.push_back(base);

                }
            }
            int baseNum = basePosStmp.size();


            if(baseNum<1)
            {

                correspondingBasePosS.push_back(-1);
                qDebug()<<"error by data in  alignPosByExonPos";
                continue;
            }
            // 如果该文件第一个比对位置都大于等于外显子位置，那么取第一个比对位置坐标为该外显子位置对应的坐标。
            if(basePosStmp[0].Matchpos >= exonPos)
            {

                int basePostemp = basePosStmp[0].pos*curvedLines[i]->getregionShow_xAxisExpand();
                if(basePostemp > maxCorrespondingBasePos)
                {
                    maxCorrespondingBasePos = basePostemp;
                    maxPosfile=i;
                }
                correspondingBasePosS.push_back(basePosStmp[0].pos);
            }// 如果该文件最后一个比对位置都小于等于外显子位置，那么取最后一个比对位置坐标为该外显子位置对应的坐标。
            else if (basePosStmp[baseNum-1].Matchpos <= exonPos)
            {
                int basePostemp = basePosStmp[baseNum-1].pos*curvedLines[i]->getregionShow_xAxisExpand();
                if(basePostemp > maxCorrespondingBasePos)
                {
                    maxCorrespondingBasePos = basePostemp;
                    maxPosfile=i;
                }
                correspondingBasePosS.push_back(basePosStmp[baseNum-1].pos);
            }
            else
            {
                int j=0;
                for(;j<baseNum;j++)
                {

                    if(basePosStmp[j].Matchpos == exonPos)
                    {
                        int basePostemp = basePosStmp[j].pos*curvedLines[i]->getregionShow_xAxisExpand();
                        if(basePostemp > maxCorrespondingBasePos)
                        {
                            maxCorrespondingBasePos = basePostemp;
                            maxPosfile=i;
                        }
                        // qDebug()<<basePosStmp[j].Matchpos;
                        correspondingBasePosS.push_back(basePosStmp[j].pos);
                        break;
                    }
                    else if(basePosStmp[j].Matchpos > exonPos)
                    {
                        int basePostemp = basePosStmp[j].pos*curvedLines[i]->getregionShow_xAxisExpand();
                        if(basePostemp > maxCorrespondingBasePos)
                        {
                            maxCorrespondingBasePos = basePostemp;
                            maxPosfile=i;
                        }
                        //qDebug()<<basePosStmp[j].Matchpos<<"big";
                        correspondingBasePosS.push_back(basePosStmp[j].pos+5);
                        break;
                    }

                }


            }
        }

        // 根据最大位置坐标调整数据位置。
        for (int i=0; i<Container_regionShowDataBak->fileNames.size(); i++)
        {
            if(Container_regionShowData->infor[i].isUndefined)
            {
                continue;
            }
            if(correspondingBasePosS[i]<0)
            {
                continue;
            }
            int movePos = (maxCorrespondingBasePos/curvedLines[i]->getregionShow_xAxisExpand()
                           - correspondingBasePosS[i]);
            //int move =movePos-Container_regionShowData->movePos[i];
            // Container_regionShowData->movePos[i]=move;
            //        qDebug()<<"befor move "<<Container_regionShowData->movePos[i];
            //        qDebug()<< "now move"<<movePos;
            //        qDebug()<<"move"<<move;
            //        qDebug()<<"move bak"<<Container_regionShowDataBak->leftLimits[i];
            //        qDebug()<<"move data "<<Container_regionShowData->leftLimits[i];
            //        qDebug()<<"last bak"<<Container_regionShowDataBak->leftLimits[i]+movePos;
            //        qDebug()<<"last data"<<Container_regionShowData->leftLimits[i]+move;
            //        if(movePos==0)
            //            continue;
            for(int j=0;j<Container_regionShowData->base[i].size();j++)
            {
                Container_regionShowData->base[i][j].pos=Container_regionShowDataBak->base[i][j].pos+movePos;
            }
            int baseAnum = Container_regionShowData->baseA[i].size();
            for(int j=0;j<baseAnum;j++)
            {
                Container_regionShowData->baseA[i][j].pos=Container_regionShowDataBak->baseA[i][j].pos+movePos;
                Container_regionShowData->baseC[i][j].pos=Container_regionShowDataBak->baseC[i][j].pos+movePos;
                Container_regionShowData->baseG[i][j].pos=Container_regionShowDataBak->baseG[i][j].pos+movePos;
                Container_regionShowData->baseT[i][j].pos=Container_regionShowDataBak->baseT[i][j].pos+movePos;
            }

            Container_regionShowData->leftLimits[i]=Container_regionShowDataBak->leftLimits[i]+movePos;
            Container_regionShowData->rightLimits[i]=Container_regionShowDataBak->rightLimits[i]+movePos;
            Container_regionShowData->excludeleft[i]=Container_regionShowDataBak->excludeleft[i]+movePos;
            Container_regionShowData->excludeRight[i]=Container_regionShowDataBak->excludeRight[i]+movePos;
            Container_regionShowData->moves[i]=movePos;
//            qDebug()<<"has move"<<Container_regionShowData->baseC[0][0].pos;
        }
        maxCorrespondingBasePos/=curvedLines[maxPosfile]->getregionShow_xAxisExpand();
        AdjustPeak();
    }
    catch(...)
    {
        ErrorWrite::errorocur("alignPosByExonPos");
    }
    // qDebug()<<maxCorrespondingBasePos;
}
void Container_Oscillogram::slotIntelligent_Analysis()
{
    if(Container_regionShowData->base.count()!=Container_regionShowData->Peaksinform.count()
            ||Container_regionShowData->infor[currenteditfile].isUndefined)
        return;
    bool eimtmessge=false;
    for(int i=0;i<Container_regionShowData->base.count();++i)
    {
        QString updatastr="";
        for(int j=0;j<Container_regionShowData->base[i].count();++j)
        {

            if(Container_regionShowData->base[i][j].Matchpos==-1) continue;
            for(int k=0;k<Container_regionShowData->Peaksinform[i].count();++k)
            {

                if(Container_regionShowData->Peaksinform[i][k].matchPos<
                        Container_regionShowData->base[i][j].Matchpos)
                    continue;
                if(Container_regionShowData->Peaksinform[i][k].matchPos>
                        Container_regionShowData->base[i][j].Matchpos)
                    break;
                if(Container_regionShowData->Peaksinform[i][k].matchPos==
                        Container_regionShowData->base[i][j].Matchpos)
                {
                    if(Container_regionShowData->base[i][j].basetype!=
                            Container_regionShowData->Peaksinform[i][k].base&&
                            Container_regionShowData->Peaksinform[i][k].base!='L')
                    {
                        eimtmessge=true;
                        updatastr="";
                        char word =Container_regionShowData->Peaksinform[i][k].base;
                        int pos=Container_regionShowData->base[i][j].Matchpos;
                        bool find=false;
                        for(int mi=0; mi<Container_regionShowData->edit_info[i].count();++mi)
                        {
                            if(pos==Container_regionShowData->edit_info[i][mi].Consen_Pos)
                            {
                                Container_regionShowData->edit_info[i][mi].base_type= word;
                                find=true;
                            }
                            updatastr.append(QString::number(Container_regionShowData->edit_info[i][mi].Consen_Pos));
                            updatastr.append(":");
                            updatastr.append(QString(Container_regionShowData->edit_info[i][mi].base_type));
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
                            Container_regionShowData->edit_info[i].push_back(infor);
                        }
                        //qDebug()<<"excute times.";
//                        qDebug()<<updatastr;
                        // container_oscillogram->bpchanged();
                        //

                        //                        Container_regionShowData->base[i][j].basetype =
                        //                                Container_regionShowData->Peaksinform[i][k].base;
                    }
                }
            }
        }
//        qDebug()<<"emit";
        if(eimtmessge)
        emit signal_analyse( i,updatastr);
    }
    if(eimtmessge)
    emit signal_analyse();
}
void Container_Oscillogram::slotReset_Analysis()
{
    for(int i=0;i<Container_regionShowData->Peaksinform.count();++i)
    {
        Container_regionShowData->edit_info[i].clear();
        emit signal_analyse( i,"");
    }
    emit signal_analyse();
}

void Container_Oscillogram::AdjustPeak()
{

    if(Container_regionShowData->Peaksinform.count()>1)
    {
        for(int i=0;i<Container_regionShowData->Peaksinform.count();++i)
        {
            // bool find=false;
            for(int j=0;j<Container_regionShowData->Peaksinform.count();++j)
            {
                if(Container_regionShowData->infor[i].isgssp==false&&
                        Container_regionShowData->infor[i].exonnum==
                        Container_regionShowData->infor[j].exonnum&&
                        Container_regionShowData->infor[i].samplename==
                        Container_regionShowData->infor[j].samplename&&
                        ((Container_regionShowData->infor[i].rOrF=="F"&&
                          Container_regionShowData->infor[j].rOrF=="R")||
                         (Container_regionShowData->infor[i].rOrF=="R"&&
                          Container_regionShowData->infor[j].rOrF=="F")&&i!=j)
                        )
                {
                    QVector <Peakinfo>  & tempeakchange=Container_regionShowData->Peaksinform[i];
                    QVector <Peakinfo>  & tempeak=Container_regionShowData->Peaksinform[j];
                    for(int m=0;m<tempeakchange.count()-1;++m)
                    {
                        for(int n=0;n<tempeak.count();++n)
                        {
                            if(tempeakchange[m].matchPos>tempeak[n].matchPos) continue;
                            if(tempeakchange[m].matchPos<tempeak[n].matchPos) break;
                            if(tempeakchange[m].matchPos==tempeak[n].matchPos&&tempeak[n].matchPos!=-1)
                            {
                                if(tempeak[n].PeaksVector.count()==2&&tempeakchange[m+1].PeaksVector.count()==2
                                        &&tempeakchange[m].PeaksVector.count()==1)
                                {

                                    int relativepos=tempeak[n].PeaksVector[1].maxsignalpos-tempeak[n].PeaksVector[0].maxsignalpos;
                                    if(relativepos<0)
                                    {
                                        double  tempos=tempeak[n].PeaksVector[1].endpos;
                                        int relativepos2=tempeakchange[m+1].PeaksVector[1].maxsignalpos-tempeakchange[m+1].PeaksVector[0].maxsignalpos;
                                        if(relativepos2<0)
                                        {
                                            double tempos2=tempeakchange[m+1].PeaksVector[0].startpos;
                                            if(tempeak[n].PeaksVector[0].maxsignalpos<tempos
                                                    &&tempeakchange[m+1].PeaksVector[1].maxsignalpos<tempos2)
                                            {
//                                                qDebug()<<"change each other"<<tempeak[n].PeaksVector[0].base<<tempeak[n].PeaksVector[1].base
//                                                       <<tempeakchange[m+1].PeaksVector[0].base<<tempeakchange[m+1].PeaksVector[1].base
//                                                      <<tempeakchange[m].PeaksVector[0].base;
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                }
//                                                qDebug()<<"change each other"<<tempeak[n].PeaksVector[0].base<<tempeak[n].PeaksVector[1].base
//                                                       <<tempeakchange[m+1].PeaksVector[0].base<<tempeakchange[m].PeaksVector[0].base
//                                                      <<tempeakchange[m].PeaksVector[1].base;
                                            }
                                        }
                                        else
                                        {
                                            double tempos2=tempeakchange[m+1].PeaksVector[1].startpos;
                                            if(tempeak[n].PeaksVector[0].maxsignalpos<tempos
                                                    &&tempeakchange[m+1].PeaksVector[0].maxsignalpos<tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                }

                                            }
                                        }

                                    }
                                    else
                                    {

                                        double tempos=tempeak[n].PeaksVector[0].endpos;
                                        int relativepos2=tempeakchange[m+1].PeaksVector[1].maxsignalpos-tempeakchange[m+1].PeaksVector[0].maxsignalpos;
                                        if(relativepos2<0)
                                        {
                                            double tempos2=tempeakchange[m+1].PeaksVector[0].startpos;
                                            if(tempeak[n].PeaksVector[1].maxsignalpos<tempos
                                                    &&tempeakchange[m+1].PeaksVector[1].maxsignalpos<tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            double tempos2=tempeakchange[m+1].PeaksVector[1].startpos;
                                            if(tempeak[n].PeaksVector[1].maxsignalpos<tempos
                                                    &&tempeakchange[m+1].PeaksVector[0].maxsignalpos<tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                        tempeakchange[m+1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                        tempeakchange[m+1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m+1].maxone=0;
                                                    }
                                                }

                                            }
                                        }

                                    }

                                    /* int temrange=tempeak[n].PeaksVector[tempeak[n].maxone].endpos-tempeak[n].PeaksVector[tempeak[n].maxone].startpos;
                                    temrange=temrange/2;

                                    int relativepos2=tempeakchange[m+1].PeaksVector[1].maxsignalpos-tempeakchange[m+1].PeaksVector[0].maxsignalpos;
                                    if(relativepos2<0)
                                        relativepos2=-relativepos2;
                                    int temrange2=tempeakchange[m+1].PeaksVector[tempeakchange[m+1].maxone].endpos-tempeakchange[m+1].PeaksVector[tempeakchange[m+1].maxone].startpos;
                                    temrange2=temrange2/2;
                                    qDebug()<<"         match pos"<<tempeakchange[m].matchPos<<tempeak[n].matchPos;
                                    if(relativepos<temrange&&relativepos2>temrange2)
                                    {
                                        if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                        {
                                            if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[0].base)
                                            {
                                                tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                tempeakchange[m+1].PeaksVector.remove(0);
                                                if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                        >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                {
                                                    tempeakchange[m].maxone=0;
                                                }
                                                else
                                                    tempeakchange[m].maxone=1;
                                                tempeakchange[m+1].maxone=0;
                                            }
                                            else if(tempeak[n].PeaksVector[1].base==tempeakchange[m+1].PeaksVector[1].base)
                                            {
                                                tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                tempeakchange[m+1].PeaksVector.remove(1);
                                                if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                        >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                {
                                                    tempeakchange[m].maxone=0;
                                                }
                                                else
                                                    tempeakchange[m].maxone=1;
                                                tempeakchange[m+1].maxone=0;
                                            }

                                        }
                                        else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                        {
                                            if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[0].base)
                                            {
                                                tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[0]);
                                                tempeakchange[m+1].PeaksVector.remove(0);
                                                if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                        >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                {
                                                    tempeakchange[m].maxone=0;
                                                }
                                                else
                                                    tempeakchange[m].maxone=1;
                                                tempeakchange[m+1].maxone=0;
                                            }
                                            else if(tempeak[n].PeaksVector[0].base==tempeakchange[m+1].PeaksVector[1].base)
                                            {
                                                tempeakchange[m].PeaksVector.push_back(tempeakchange[m+1].PeaksVector[1]);
                                                tempeakchange[m+1].PeaksVector.remove(1);
                                                if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                        >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                {
                                                    tempeakchange[m].maxone=0;
                                                }
                                                else
                                                    tempeakchange[m].maxone=1;
                                                tempeakchange[m+1].maxone=0;
                                            }
                                        }
                                    }
                                    */
                                }

                            }
                        }
                    }
                }
            }
        }
    }

    if(Container_regionShowData->Peaksinform.count()>1)
    {
        for(int i=0;i<Container_regionShowData->Peaksinform.count();++i)
        {
            // bool find=false;
            for(int j=0;j<Container_regionShowData->Peaksinform.count();++j)
            {
                if(Container_regionShowData->infor[i].isgssp==false&&
                        Container_regionShowData->infor[i].exonnum==
                        Container_regionShowData->infor[j].exonnum&&
                        Container_regionShowData->infor[i].samplename==
                        Container_regionShowData->infor[j].samplename&&
                        ((Container_regionShowData->infor[i].rOrF=="F"&&
                          Container_regionShowData->infor[j].rOrF=="R")||
                         (Container_regionShowData->infor[i].rOrF=="R"&&
                          Container_regionShowData->infor[j].rOrF=="F")&&i!=j)
                        )
                {
                    QVector <Peakinfo>  & tempeakchange=Container_regionShowData->Peaksinform[i];
                    QVector <Peakinfo>  & tempeak=Container_regionShowData->Peaksinform[j];
                    for(int m=1;m<tempeakchange.count();++m)
                    {
                        for(int n=0;n<tempeak.count();++n)
                        {
                            if(tempeakchange[m].matchPos>tempeak[n].matchPos) continue;
                            if(tempeakchange[m].matchPos<tempeak[n].matchPos) break;
                            if(tempeakchange[m].matchPos==tempeak[n].matchPos&&tempeak[n].matchPos!=-1)
                            {
                                if(tempeak[n].PeaksVector.count()==2&&tempeakchange[m-1].PeaksVector.count()==2
                                        &&tempeakchange[m].PeaksVector.count()==1)
                                {

                                    int relativepos=tempeak[n].PeaksVector[1].maxsignalpos-tempeak[n].PeaksVector[0].maxsignalpos;
                                    if(relativepos<0)
                                    {
                                        double  tempos=tempeak[n].PeaksVector[0].startpos;
                                        int relativepos2=tempeakchange[m-1].PeaksVector[1].maxsignalpos-tempeakchange[m-1].PeaksVector[0].maxsignalpos;
                                        if(relativepos2<0)
                                        {
                                            double tempos2=tempeakchange[m-1].PeaksVector[1].endpos;
                                            if(tempeak[n].PeaksVector[1].maxsignalpos>tempos
                                                    &&tempeakchange[m-1].PeaksVector[0].maxsignalpos>tempos2)
                                            {
//                                                qDebug()<<"change each other 1"<<tempeak[n].PeaksVector[0].base<<tempeak[n].PeaksVector[1].base
//                                                       <<tempeakchange[m-1].PeaksVector[0].base<<tempeakchange[m-1].PeaksVector[1].base
//                                                      <<tempeakchange[m].PeaksVector[0].base;
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                }
//                                                qDebug()<<"change each other 1"<<tempeak[n].PeaksVector[0].base<<tempeak[n].PeaksVector[1].base
//                                                       <<tempeakchange[m-1].PeaksVector[0].base<<tempeakchange[m].PeaksVector[0].base
//                                                      <<tempeakchange[m].PeaksVector[1].base;
                                            }
                                        }
                                        else
                                        {
                                            double tempos2=tempeakchange[m-1].PeaksVector[0].endpos;
                                            if(tempeak[n].PeaksVector[1].maxsignalpos>tempos
                                                    &&tempeakchange[m-1].PeaksVector[1].maxsignalpos>tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                }

                                            }
                                        }

                                    }
                                    else
                                    {

                                        double tempos=tempeak[n].PeaksVector[1].startpos;
                                        int relativepos2=tempeakchange[m-1].PeaksVector[1].maxsignalpos-tempeakchange[m-1].PeaksVector[0].maxsignalpos;
                                        if(relativepos2<0)
                                        {
                                            double tempos2=tempeakchange[m-1].PeaksVector[1].endpos;
                                            if(tempeak[n].PeaksVector[0].maxsignalpos>tempos
                                                    &&tempeakchange[m-1].PeaksVector[0].maxsignalpos>tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            double tempos2=tempeakchange[m-1].PeaksVector[0].endpos;
                                            if(tempeak[n].PeaksVector[0].maxsignalpos>tempos
                                                    &&tempeakchange[m-1].PeaksVector[1].maxsignalpos>tempos2)
                                            {
                                                if(tempeak[n].PeaksVector[0].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[1].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }

                                                }
                                                else if(tempeak[n].PeaksVector[1].base==tempeakchange[m].PeaksVector[0].base)
                                                {
                                                    if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[0].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[0]);
                                                        tempeakchange[m-1].PeaksVector.remove(0);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                    else if(tempeak[n].PeaksVector[0].base==tempeakchange[m-1].PeaksVector[1].base)
                                                    {
                                                        tempeakchange[m].PeaksVector.push_back(tempeakchange[m-1].PeaksVector[1]);
                                                        tempeakchange[m-1].PeaksVector.remove(1);
                                                        if(tempeakchange[m].PeaksVector[0].maxsiganl
                                                                >tempeakchange[m].PeaksVector[1].maxsiganl)
                                                        {
                                                            tempeakchange[m].maxone=0;
                                                        }
                                                        else
                                                            tempeakchange[m].maxone=1;
                                                        tempeakchange[m-1].maxone=0;
                                                    }
                                                }

                                            }
                                        }

                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    // qDebug()<<"         middle  AdjustPeak";
    for(int i=0;i<Container_regionShowData->Peaksinform.count();++i)
    {
        QVector <Peakinfo>  & Peakinfos=Container_regionShowData->Peaksinform[i];
        for(int count =0;count<Peakinfos.count();++count)
        {
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
//                            qDebug()<<"don't know "<<Peakinfos[count].PeaksVector[0].base
//                                   <<Peakinfos[count].PeaksVector[1].base;
                        }
                    }
                    else
                    {
                        //qDebug()<<"0.25  error";
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
//                for(int i=0;i<Peakinfos[count].PeaksVector.count();++i)
//                {
//                    qDebug()<<"how many peaks "<<Peakinfos[count].PeaksVector[i].base<<Peakinfos[count].PeaksVector[i].startpos
//                           <<Peakinfos[count].PeaksVector[i].maxsiganl;
//                }


            }
        }
    }


    // qDebug()<<"         end  AdjustPeak";

}


Container_Oscillogram::~Container_Oscillogram()
{
}






