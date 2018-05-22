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
    void mouseReleaseEvent(QMouseEvent *);//��굥���ƶ������
    void ab1PosToExonPos( int &MoseClickPos , int &exonPos );//�������λ���ҵ�ba1�ļ��ж�Ӧ�ļ��
    void HighLightGivenFile( QPainter &paint ,int whichfile);
    int getcurrenteditfile(){return currenteditfile;}
    void setcurrenteditfile(int num){currenteditfile=num;}
    int getbaseItemWidth(){return baseItemWidth;}
   // bool judgeExonPosIsIn( int &exonPos );
    void alignPosByExonPos( int &exonPos , int &maxCorrespondingBasePos );
    void Move_Curved_Center(int focusPos /*�����ӵ�λ��*/,
                            int focusCurvePos/*�����������߱߿�ľ���*/ );

    void Repaint_Curved(int focusPos /*�����ӵ�λ��*/,
                        int focusCurvePos/*�����������߱߿�ľ���*/ );
    void xAxisExpand_aligning(int focusPos /*�����ӵ�λ��*/,
                              int focusCurvePos/*�����������߱߿�ľ���*/);
    void addordecrease_xAxisExpand(double num);
    void addordecrease_yAxisExpand(double num);
    void addordecrease_CurvedHight(int );
    int getfocusCurvePosToab1Edge(){return focusCurvePosToab1Edge;}
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
    void AdjustPeak();

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
        int                             FocusPos ; /*��ǰ���������λ�ã�����ǰ�����Ľ���*/

        bool   ChangeFormshow;

        int currenteditfile;
        int filecount;
        int baseItemWidth;
        int focusAb1Pos ;/*��ʼ״̬���������ab1�ļ��е�λ��*/
        int focusCurvePosToab1Edge;
//        double * filesxExpand;//��ָ�벻�ͷ����ϼ����ͷ�
//        double * filesyExpand;
//        bool  addordecreaseOne;
        SaveData *container_savedata;/*��ָ�벻�ͷ����ϼ����ͷ�*/
        int maxPosfile;


    
};

#endif // CONTAINER_OSCILLOGRAM_H
