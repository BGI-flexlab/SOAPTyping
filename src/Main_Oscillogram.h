#ifndef MAIN_OSCILLOGRAM_H
#define MAIN_OSCILLOGRAM_H

#include <QScrollArea>
#include <QPainter>
#include "container_oscillogram.h"
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <QContextMenuEvent>
#include "realtimedatabase.h"
#include "analysis.h"
#include<QKeyEvent>

class Main_Oscillogram : public QScrollArea
{
    Q_OBJECT
public:
    Main_Oscillogram();
    ~Main_Oscillogram();
    void contextMenuEvent(QContextMenuEvent *event);
    void rigthMenuShow(QContextMenuEvent *event)
    {
        contextMenuEvent(event);
    }

    void updatasignalInfo(int currentfile);//�����������Ϣ���ݵ�����
    bool Getiscontainer_create()
    {return iscontainer_create;}
    bool Getcontainer_focusPosOk()//��ȡ��ǰ����λ���Ƿ���ȷ
    {
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
            return false;
        }
       return  true;
    }
    int GetMarkType(){return getMarkTypeBySampleName(currentFileSignalInfo.sampleName.toAscii());}

    void DisconnectFocusBox();
private:
    void create_draw();
    bool getRegionShowData( SignalInfo& fileSignal,bool);
    void analysisData();//������ǰ��ͼ����
    void createAction();//�����Ҽ��˵�������Ӧ
    void createMenu(QMenu       *);//�����Ҽ��˵�
    void moveCenterPos(int sliderPos);//�ƶ���ǰ���㵽��Ļ����
    void resetAll();
    void errorocur(QString);
    void getAB1Signal();//��ȡ����ź�ֵ
    QString getAcidFromBase(QString base);//��ȡ�����Ӧ�İ�����
    QVector<int> m_SignalA;
    QVector<int> m_SignalT;
    QVector<int> m_SignalG;
    QVector<int> m_SignalC;
    QVector<int> m_Qual;

signals:
    void signalchangedbp();
    void signaladdordecreaseOne(bool);
    void signalchangedbp(SignalInfo &,int);
    //void signaladdordecreaseOne(bool);
    void signalEdited( SignalInfo& );
    void signalFocusPos(int,int);
    void signalchangedleftorright(SignalInfo,int );
    void signal_toinform_other(int ,int,int );
    void signalactApplyOne();
    void signalactApplyAll();
    void signalactanalyseLater();
    void signalactanalyseNow();
    void signalAB1Information(QString msgstr);//���������ͼ����ǰ��Ϣ



   // void signalFocusPos( int focusCurveToLeftDis,int focusPos );
//    void signal_add_xAxisExpand(double);
//    void signal_decrease_xAxisExpand(double);
//    void signal_add_yAxisExpand(double);
//    void signal_decrease_yAxisExpand(double);



public slots:
    void slot_inform_other(int,int);
    void slotMoveFocusBoxForm(int xPos);
    void slotaddordecreaseOne();
    void slotaddordecreaseAll();
    void slotDelteThisFile();
    void slotanalyseLater()
    {
        analyse=false;
        actanalyseLater->setDisabled(true);
        actanalyseNow->setDisabled(false);
        actanalyseNow->setIconVisibleInMenu(false);
        actanalyseLater->setIconVisibleInMenu(true);

    }
    void slotanalyseNow()
    {
        analyse=true;
        actanalyseLater->setDisabled(false);
        actanalyseNow->setDisabled(true);
        actanalyseNow->setIconVisibleInMenu(true);
        actanalyseLater->setIconVisibleInMenu(false);
    }
    void slotIntelligent_Analysis(int,QString);
    void slotanalyse();
    void slotactApplyOne()
    {
        actApplyOne->setDisabled(true);
        actApplyAll->setDisabled(false);
        actApplyOne->setIconVisibleInMenu(true);
        actApplyAll->setIconVisibleInMenu(false);
        slotaddordecreaseOne();
        emit signalactApplyOne();
    }
    void slotactApplyAll()
    {
        actApplyOne->setIconVisibleInMenu(false);
        actApplyAll->setIconVisibleInMenu(true);
        actApplyOne->setDisabled(false);
        actApplyAll->setDisabled(true);
        slotaddordecreaseAll();
        emit signalactApplyAll();
    }
    void slotactanalyseLater()
    {
        actanalyseNow->setIconVisibleInMenu(false);
        actanalyseLater->setIconVisibleInMenu(true);
        actanalyseLater->setDisabled(true);
        actanalyseNow->setDisabled(false);
        slotanalyseLater();
        emit signalactanalyseLater();
    }
    void slotactanalyseNow()
    {
        actanalyseNow->setIconVisibleInMenu(true);
        actanalyseLater->setIconVisibleInMenu(false);
        actanalyseLater->setDisabled(false);
        actanalyseNow->setDisabled(true);
        slotanalyseNow();

    }
    void slotactanalyse()
    {
        actanalyse->setDisabled(true);
        slotanalyse();
    }

    void slotfilechange(bool is_emit);
    void slotReset();
    void slotResetExclude();
    void slotmousepressed(int focusCurveToLeftDis,int focusPos );

    void slotslidermovebymouse(int );
    void slotslidermousepress();
    void slotMoveScollBar(
                       int focusCurvePos/*�����������߱߿�ľ���*/
            ,int Ab1Pos,int);
    void slotHighLightLeftPart();
    void slotHighLightRightPart();
    //void slotResetExclude();
    void slotEdited();
    void slotFocusMark (int , int , int )  ; // ����������Ĳ���ʱ�򴫵ݵ�slotֵ
    //void slotFocusMark ( int focusBasePos , int exonNum) ;
    void slotyRoomUp();
    void slotyRoomDown();
    void slotyRangeRoomDown();
    void slotyRangeRoomUp();
    void slotxRoomUp();
    void slotxRoomDown();
    void resetRoomSetting();
    void slotRegionShowDataChanged(
            SignalInfo& fileSignalInfo ,int);//important
    void AB1FrameClicked();

    void slotClickLetter(int fileindex,int match);
    void slotbasechanged(char );
    void slotBasechanged(char basetype,int fileindex,int match);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    Container_Oscillogram     *container_oscillogram;
    QGridLayout     *mainLayout;
    QScrollBar * hscoll;

    QMenu       *rightMenu;
    QAction     *actInsertBaseN;
    QAction     *actHideTraceDisplay;
    QAction     *actFilterByCurrentBase;
    QAction     *actRemoveLastBaseFilter;
    QAction     *actRemoveAllBaseFilters;
    QAction     *actRemoveLastNullAlleleFilter;

    QAction     *actApplyOne;
    QAction     *actApplyAll;
    QAction     *actanalyseNow;
    QAction     *actanalyseLater;
    QAction     *actanalyse;

    QAction     *actDelete;



    QAction     *actExcludeLeft;
    QAction     *actExcludeRight;
    QAction     *actResetExclude;
    QAction     *actIntelligent_Analysis;
    QAction     *actReset_Analysis;
    QAction     *actChangeExonOrDirectionToThisTrace;
    QAction     *actRemoveThisTrace;



    //int         sampleMaktype;
    bool        analyse;
    int         rightMenuX;
    int         rightMenuX2;
    int         rightMenuY;
    int         sliderPos;
    int         current_exonNum; /* ����������*/
    int         mfocusBasePos;
    bool  iscontainer_create;
    RegionShowData     regionShowData,regionShowDataBak;
    SignalInfo     currentFileSignalInfo;
    bool data_is_ready;
    double xAxisExpandEverytime;
    double yAxisExpandEverytime;
    int curvedHigthEverytime;
    SaveData befordata;
    bool done;
    QMutex mutex;
    int couldnotmove;
    int XPos;

    bool isClickTable;
    bool isFileChange;
//    int * filesxExpandold;
//    int * filesyExpandold;
    double Space;
    QList<FileInfo_ly> m_FileInfo;
    int alignStart_;
    int alignEnd_;
};


#endif // MAIN_OSCILLOGRAM_H
