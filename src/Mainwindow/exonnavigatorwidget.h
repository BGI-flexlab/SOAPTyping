#ifndef EXONNAVIGATORWIDGET_H
#define EXONNAVIGATORWIDGET_H

#include <QWidget>
#include "all_base_struct.h"
#include <set>

typedef struct _tagExon
{
   int i_exonstartpos;    //导航条起始pos
   int i_exonendpos;      //导航条结束pos
   int i_screenstartpos;      //导航条起始x坐标
   int i_screenwidth;        //导航条长度
   int i_exonindex;     //导航条显示index
}Exon;

class ExonNavigatorWidget:public QWidget
{
    Q_OBJECT
public:
    ExonNavigatorWidget(QWidget *parent = nullptr);
    ~ExonNavigatorWidget();
    void SetExonData(QString &str_sample, QString &str_gene);
    void SetSelectPos(int colnum, int &selectpos,int &exonstartpos, int &index);
    void setSelectFramePosition(int index, int &startpos, int &selectpos, int &exonstartpos);
    void SetSelectFramePos(int index, int colnum ,int &columnPos);
    void ActForward();
    void ActBackward();
    void SetTypeMisPos(QSet<int> &typeMismatchPos);
    void SetRefresh(bool refresh){m_bRefresh = refresh;}
    void ClearExonNav();
private:
    void CalcExonData();
    void paintEvent(QPaintEvent *event);
    void DrawExonArea(QPainter &paiter);
    void DrawSelectFrame(QPainter &paiter);
    void DrawExonPos(QPainter &paiter);
    void mousePressEvent(QMouseEvent *event);
    int PeakPosToScreenPos(int oldpos);
    int ScreenPosToPeakPos(int newpos);
    void resizeEvent(QResizeEvent *event);
signals:
    //导航条起始pos,选中的峰图pos，选中的导航条起始pos,选中的导航条index
    void signalExonFocusPosition(int startpos,int selectpos,int exonstart, int index);
private:
    bool m_bRefresh;
    int m_iwidth;
    int m_iheight;
    int m_itop1;
    int m_itop2;
    int m_itop3;
    int m_ih0;
    int m_ih1;
    int m_ih2;
    int m_ih3;
    int m_iSelectPeakPos;   //当前选中的峰图位置
    int m_iMidgap;          //碱基导航条中间间距
    int m_igap;             //碱基导航条两端间距
    int m_isub_pos;         //外显子x坐标，最大最小之差
    int m_iSelectindex;     //当前选中的导航条index
    int m_iStartPeakpos;    //起始峰图位置
    double m_dXscale;       //计算每个碱基的间距
    QVector<Exon> m_vec_Exon; //导航条信息
    QVector<int> m_vecExonIndex;
    ExonNavigatorInfo m_Exoninfo;
    QString m_str_SampleName;
    QString m_str_GeneName;
    QMap<int,int> m_map_mispos; //保存所有错配位置，自动排序
    QMap<int,int> m_map_typemispos; //保存allele错配位置，自动排序
    QMap<int,int> m_map_TotalMisPos; //保存m_map_mispos和m_map_typemispos的集合
};

#endif // EXONNAVIGATORWIDGET_H
