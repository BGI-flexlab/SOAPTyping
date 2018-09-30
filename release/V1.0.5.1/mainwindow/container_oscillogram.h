#ifndef CONTAINER_OSCILLOGRAM_H
#define CONTAINER_OSCILLOGRAM_H

#include <QWidget>
#include "curvedline.h"
#include <qgridlayout.h>
#include <qmenu.h>
#include <QMouseEvent>
#include <qdesktopwidget.h>

class Container_Oscillogram : public QWidget
{
    Q_OBJECT
public:
    explicit Container_Oscillogram(
            RegionShowData * regionShowData,
            RegionShowData *regionShowDataBak ,
            SaveData *befordata,QWidget *parent = 0
            );
    ~Container_Oscillogram();
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);//鼠标单击移动对齐框
    void ab1PosToExonPos( int &MoseClickPos , int &exonPos);//根据鼠标位置找到ba1文件中对应的碱基
    void HighLightGivenFile( QPainter &paint ,int whichfile);//标记被操作峰图
    int getcurrenteditfile(){return currenteditfile;}//获取当前峰图
    void setcurrenteditfile(int num){currenteditfile=num;}//设置当前峰图
    int getbaseItemWidth(){return baseItemWidth;}
    // bool judgeExonPosIsIn( int &exonPos );
    void alignPosByExonPos( int &exonPos , int &maxCorrespondingBasePos );//根据鼠标点击位置计算外显子坐标
    void Move_Curved_Center(int focusPos /*外显子的位置*/,
                            int focusCurvePos/*鼠标点击距离左边边框的距离*/ );//移动峰图位置对齐峰图

    void Repaint_Curved(int focusPos /*外显子的位置*/,
                        int focusCurvePos/*鼠标点击距离左边边框的距离*/);//移动峰图位置对齐峰图（内部调用）
    void xAxisExpand_aligning(int focusPos /*外显子的位置*/,
                              int focusCurvePos/*鼠标点击距离左边边框的距离*/);//调整峰图比例后对齐峰图
    void addordecrease_xAxisExpand(double num);//调整峰图x轴缩放系数
    void addordecrease_yAxisExpand(double num);//调整峰图y轴缩放系数
    void addordecrease_CurvedHight(int );//调整曲线的高
    int getfocusCurvePosToab1Edge(){return focusCurvePosToab1Edge;}//获取当前操作位置，当前焦点
    int getfocusPos(){return FocusPos;}
    void setthiswidth();
    double getxAxisExpand()
    {
        return curvedLines[currenteditfile]->getxAxisExpand();
    }
    double getyAxisExpand()
    {
        return curvedLines[currenteditfile]->getyAxisExpand();
    }
    void bpchanged()
    {
        curvedLines[currenteditfile]->reDrawAll();
    }
    void  getwhichFile(int x,int y);
    void  AdjustPeak();

    QVector < CurvedLine* > getCurvedLine(){return curvedLines;} //新增
    //    RegionShowData     *getCuvRegionShowData(){return Container_regionShowData;}
private:
signals:
    void signalfilechange(bool is_emit);
    void signalfocusrepian();
    void signalTableEdited();
    void signalFocusPos(int exonPos , int FocusMarkxPos,int);
    void signalMoveFocusBoxForm(int xPos);
    void signalMoveScollBar(int,int,int);
    void signalFocuschange(int,int);
    void signal_inform_other(int ,int );
    void signal_analyse(int ,QString );
    void signal_analyse();

    void signalClickLetter(int,int);//新增
    void formbasechanged(char,int,int);//新增
public slots:
    void slotReset_Analysis();
    void slotIntelligent_Analysis();
    void mouseRelease(QMouseEvent * event)
    {
        mouseReleaseEvent(event);
    }
    void slotfocusrepian();
    void slotChangeFormClosed();
    void slotChangeFormShow();
    void slotEdited();

    //    void slotaddordecreaseOne(bool change)
    //        { addordecreaseOne=change; }

private:
    QVector < CurvedLine* >         curvedLines;
    QGridLayout*                    mainLayout;

    RegionShowData     * Container_regionShowData,* Container_regionShowDataBak;
    int                             FocusPos ; /*当前操作碱基的位置，及当前操作的焦点*/
    bool   ChangeFormshow;

    int currenteditfile;
    int filecount;
    int baseItemWidth;
    int focusAb1Pos ;/*初始状态点击焦点在ab1文件中的位置*/
    int focusCurvePosToab1Edge;
    //        double * filesxExpand;//该指针不释放又上级类释放
    //        double * filesyExpand;
    //        bool  addordecreaseOne;
    SaveData *container_savedata;/*该指针不释放又上级类释放*/
    int maxPosfile;
    int m_FocusBoxPos; //新增
public:
    int getFocusAb1Pos(){return focusAb1Pos;} //新增
};

#endif // CONTAINER_OSCILLOGRAM_H
