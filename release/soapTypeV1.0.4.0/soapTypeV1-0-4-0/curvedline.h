#ifndef CURVEDLINE_H
#define CURVEDLINE_H

#include <QWidget>
#include <qpainter.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <QLabel>
#include <QList>
#include "All_Base_Struct.h"
#include "QFile"
#include "QPoint"
#include "QFrame"
#include "soapletter.h"

#define Ccolor QColor( 0,0,255)
#define Gcolor QColor( 0,0,0)
#define Acolor QColor( 57,181,74)
#define Tcolor QColor( 255,0,0)
//#define Wcolor QColor( 102,102,102)
#define Wcolor QColor( 193,39,45)

//#define Ccolor QColor( 0,0,255,150 )
//#define Gcolor QColor( 0,0,0,150 )
//#define Acolor QColor( 0,255,0,150 )
//#define Tcolor QColor( 255,0,0,150 )
//#define Wcolor QColor( 139, 0, 0)
#define LENIMAGE 30000

//#define baseItemWidth 20

class CurvedLine : public QWidget
{
    Q_OBJECT
public:
    explicit CurvedLine(int current,
                        RegionShowData * regionShowData
                        ,RegionShowData *regionShowDataBak,SaveData *,QWidget *parent = 0);
    void drawAll();
    void  reDrawAll();
    int GetWidth(){return CurvedWidth;}
    void SetWidth(int width){CurvedWidth=width;}
    void addordecrease_xAxisExpand(double num)
    {


        if(num>0&&GetWidth()>6*LENIMAGE)
            return;
        regionShow_xAxisExpand+=num;
        if(regionShow_xAxisExpand<=0&&num<0)
        {
            regionShow_xAxisExpand=1;
            return;
        }
    }
    void addordecrease_yAxisExpand(double num)
    {
        regionShow_yAxisExpand+=num;
        if((regionShow_yAxisExpand<=0&&num<0)||regionShow_yAxisExpand>10)
        {
            regionShow_yAxisExpand=0.1;
            return;
        }
    }
    void set_xAxisExpand(double num)
    {regionShow_xAxisExpand=num;}
    double getxAxisExpand()
    {return regionShow_xAxisExpand;}
    double getyAxisExpand()
    {return regionShow_yAxisExpand;}
    void set_yAxisExpand(double num)
    {regionShow_yAxisExpand=num;}
    double getregionShow_xAxisExpand()
    {return regionShow_xAxisExpand;}
    int getCurvedHight(){return CurvedHight;}
    void addordecrease_CurvedHight(int num){CurvedHight+=num;}
    int getLineHight(){return LineHight;}

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QSize maximumSize() const;
    void InsertPeaks(QVector<Peak>&,char ,double ,double ,int ,
                     int ,double ,int which,int);
    void analysisDataforpeak();
    void setCurvedLinePos(QVector<CurvedLine*> *CurvedLinePos){m_pCurvedLinePos = CurvedLinePos;}

    QList<SoapLetter*> getSoapLetterPos(){return m_pLetterList;}
    void getAllSoapLetterPos();
    RegionShowData     *getCuvRegionShowData(){return     cuv_regionShowData;}
   ~CurvedLine();
protected:

    bool myabs(double value)
    {
        if((value>= 0&&value<1)||(value<=0 &&value>-1))
        {
           return true;
        }
        return false;
    }
    virtual void paintEvent( QPaintEvent * );

    void drawContents(  QVector <Each_Base_Signal> &baseT ,
                        QVector <Each_Base_Signal> &baseC,
                        QVector <Each_Base_Signal> &baseG ,
                        QVector <Each_Base_Signal> &baseA ,
                        QPaintDevice * drawmap,int pos);
    void drawContents1(  QVector <Each_Base_Signal> &baseT ,
                        QVector <Each_Base_Signal> &baseC,
                        QVector <Each_Base_Signal> &baseG ,
                        QVector <Each_Base_Signal> &baseA ,
                        QPaintDevice * drawmap,int pos);
    bool drawletter(QPainter *paint);
    void drawline(QPainter *paint,int );
    void drawFileNameAndNum(QPainter *paint);
    void analysisData();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    QString getbasesequence();
    void PosToExonPos( int  MoseClickPos ,int &exonPos );
    void mouseReleaseEvent(QMouseEvent *);
    void ab1PosToExonPos(int  MoseClickPos );
signals:
    void mouserelease(QMouseEvent *);
    void signalfocusrepian();
    void signalgrabpostoscore(double,double,double,QImage *);
    void signalClickLetter(int,int);
    void formbasechanged(char,int,int);

public slots:
    void slotstartgrabwhichon(int whichone,int hy_ho)
    {
        grabWhichone=whichone;
        ishyorho=hy_ho;
        frame->hide();
    }
    void slotbasechanged(char c,int a,int b);
private:

    void  SetColor(QPainter *paint ,char type);
    void  SetColor(QColor &color,char type);
    QVector<QImage * >drawmap;
    QFrame * frame;
    QMouseEvent * e;
    QPoint StartPos,endPos,middlePos;
    bool pressed;
    bool moved;
    int xAxisExpand;
   // QImage * drawmap,*drawmap2,*drawmap3;//波形图临时文件
    RegionShowData     * cuv_regionShowData;
    RegionShowData     * cuv_regionShowDataBak;
    int     cuvLeftLimit;
    int     cuvRightLimit;
    int     cuvLeftExclude;
    int     cuvRightExclude;
    int     currentfile;//当前波形图是第几个文件
    double  maxValue;
    double compress;
    int     LineHight;//显示碱基行的高度
    int     CurvedWidth;//当前窗体的宽度
    int     CurvedHight;//当前窗体的高度
    int startexon,endexon,exonPos;
    int indexnow;
    bool mousepress;
    int grabWhichone,ishyorho;
    QImage *saveanaly;
    QPoint start,end,middle;
    double      regionShow_xAxisExpand  ;//当前窗体x轴扩大的倍数
    double      regionShow_yAxisExpand  ;//当前窗体y轴扩大的倍数
    QVector<BaseInfor> cur_baseinfor;
    QVector<Base_EditInfo>  cur_changeinfo;
    QList<SoapLetter*> m_pLetterList;
    QVector<CurvedLine*> *m_pCurvedLinePos;
    QList< QList<SoapLetter*> > m_AllLetterList;
    SoapLetter **m_ppLetter;

};

#endif // CURVEDLINE_H
