#ifndef MAIN_OSCILLOGRAM_H
#define MAIN_OSCILLOGRAM_H

#include <QScrollArea>
#include "container_oscillogram.h"
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <QContextMenuEvent>
#include "realtimedatabase.h"
#include "analysis.h"





class ChangeForm:public QDialog
{
        Q_OBJECT
private:
    QGridLayout *above;
    QHBoxLayout *below;
    QVBoxLayout *mainlay;
    QButtonGroup *group;
    QRadioButton *Abp,*Tbp,*Cbp,*Gbp,*Rbp,*Ybp,
    *Kbp,*Mbp,*Sbp,*Wbp,*Bbp,*Dbp,*Hbp,*Vbp;
    QPushButton *cancel;
public:
    ChangeForm();
    void closeEvent(QCloseEvent * e);
    ~ChangeForm();
protected:

public slots:
    void BpChanged(int WhichOneChoose);
signals:
    void IHaveClosed();
    void formbasechanged(char word);



};


class FocusBoxFrame :public QFrame
{
        Q_OBJECT
private:
       ChangeForm * ChangeBp;

public:

    FocusBoxFrame(QFrame *parent=0):QFrame(parent)
    {ChangeBp=NULL;}
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *e);
    ~FocusBoxFrame();
    void contextMenuEvent(QContextMenuEvent * e);
signals:
     void mouseClickEvent(int,int);
     void ChangeFormShow();
     void ChangeFormClosedto_container();
     void basechanged(char word);
public slots:
     void ChangeFormClosed();
     void slotbasechanged(char word);
protected:

   // void keyReleaseEvent(QKeyEvent * event);
};



class Main_Oscillogram : public QScrollArea
{
    Q_OBJECT
public:
    Main_Oscillogram();
    void contextMenuEvent(QContextMenuEvent *event);
    void rigthMenuShow(QContextMenuEvent *event)
    {
        contextMenuEvent(event);
    }

    ~Main_Oscillogram();
    void updatasignalInfo(int currentfile);
    bool Getiscontainer_create()
    {return iscontainer_create;}
    bool Getcontainer_focusPosOk()
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

private:
    void create_draw();
    bool getRegionShowData( SignalInfo& fileSignal,bool);
    void analysisData();
    void createAction();
    void createMenu(QMenu       *);
    void moveCenterPos(int sliderPos);
    void resetAll();
    void errorocur(QString);
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




   // void signalFocusPos( int focusCurveToLeftDis,int focusPos );
//    void signal_add_xAxisExpand(double);
//    void signal_decrease_xAxisExpand(double);
//    void signal_add_yAxisExpand(double);
//    void signal_decrease_yAxisExpand(double);



public slots:
    void slotFocusclik(int,int);
    void slot_inform_other(int,int);
    void slotbasechanged(char );
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
                       int focusCurvePos/*鼠标点击距离左边边框的距离*/
            ,int Ab1Pos,int);
    void slotHighLightLeftPart();
    void slotHighLightRightPart();
    //void slotResetExclude();
    void slotEdited();
    void slotFocusMark (int , int , int )  ; // 点击第三、四部分时候传递的slot值
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
private:
    Container_Oscillogram     *container_oscillogram;
    FocusBoxFrame        *FocusBoxForm;
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
    int         current_exonNum; /* 几号外显子*/
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

//    int * filesxExpandold;
//    int * filesyExpandold;
};



#endif // MAIN_OSCILLOGRAM_H
