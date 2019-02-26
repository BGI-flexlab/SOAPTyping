#ifndef MULTIPEAKWIDGET_H
#define MULTIPEAKWIDGET_H

#include <QWidget>
#include "Core/fileTablebase.h"
#include <QMenu>

typedef struct _tagGeneLetter
{
    char type;
    char oldtype;
    int qual;
    int a_signal;
    int t_signal;
    int g_signal;
    int c_signal;
    QPoint pos;
}GeneLetter;

class PeakLine
{
public:
    PeakLine(long size);
    ~PeakLine();
    int getSize();
    void SetBasePoint(char type, double x, double y);
    QPolygonF& GetBasePoint(char type);
    void AddGeneLetter(GeneLetter &geneletter);
    QVector<GeneLetter>& GetGeneLetter();

    void SetFileName(QString &str);
    QString& GetFileName();

    void SetAlignPos(int start, int end);
    void SetExcludePos(int left, int right);
    void GetExcludePos(int &left, int &right);

    void SetOffset(int offset);
    int GetOffset();

    void SetGssp(bool isGssp);
    bool GetGssp();

    void setAvgWidth(float width);
    float getAvgWidth();

    void setAvgSignal(float signal);
    float getAvgSignal();

    void setPeakYSetp(int y);
    int getPeakYStep();

    void setPeakHeight(int height);
    int getPeakHeight();

    void setXLeft(int x);
    int getXLeft();

    void setDelpos(int pos);
    QVector<int> &getDelpos();
private:
    bool m_bGssp;
    int m_left_exclude;
    int m_right_exclude;
    int m_iAlignStart;
    int m_iAlignEnd;

    int m_y_step;            //y轴间距
    int m_iPeakHeight;      //峰图实际高度

    int m_x_left;           //保存左边距离exon的长度

    long m_lsize;
    long m_loffset;
    float m_f_avgwidth;
    float m_f_avgsignal;
    QString m_strFileName;
    QPolygonF m_vec_baseA;
    QPolygonF m_vec_baseT;
    QPolygonF m_vec_baseG;
    QPolygonF m_vec_baseC;
    QVector<GeneLetter> m_vec_GeneLetter;
    QVector<int> m_vec_del;
};



class MultiPeakWidget:public QWidget
{
    Q_OBJECT

public:
    MultiPeakWidget(QWidget *parent = nullptr);
    ~MultiPeakWidget();
    void SetPeakData(const QString &str_samplename, int index, const QString &str_file);
    void SetSelectPos(int subpos,int x=0);
    void AdjustPeakHeight(int height);
    void AdjustPeakY(int y);
    void AdjustPeakX(int x);
    void RestorePeak();
    void SetRefresh(bool refresh){m_bRefresh = refresh;}
    void ClearMultiPeak();
private:
    void CreateRightMenu();
    void ConnectSignalandSlot();
    void paintEvent(QPaintEvent *event);
    void DrawPeakLines(QPainter *pter);
    void DrawHLines(QPainter *pter);
    void DrawPeakHead(QPainter *pter);
    void DrawBasePosition(QPainter *pter);
    void DrawSelectFrame(QPainter *pter);
    void DrawExcludeArea(QPainter *pter);
    void GetBaseColor(QPainter *pter, const QChar &base);
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void SetPeakLineData();
    void ExcludeArea(int type); //type:1代表左排除，2代表右排除,3代表恢复
    void loadFontFromFile();
public slots:
    void slotApplyOne();
    void slotApplyAll();
    void slotAnalyseLater();
    void slotAnalyseNow();
    void slotActanalyze();
private slots:
    void slotDelteThisFile();
    void slotActApplyOne();
    void slotActApplyAll();  
    void slotActanalyzeLater();  
    void slotActanalyzeNow();
    void slotHighLightLeftPart();
    void slotHighLightRightPart();
    void slotResetExclude();

signals:
    //index:当前导航条index colnum：选中的位置和left_exclude的差值
    void signalPeakFocusPosition(int index, int colnum);
    void signalChangeDB(const QString &str_samplename);
//    void signalActApplyOne();
//    void signalActApplyAll();
//    void signalActanalyzeLater();
    void signalPeakAct(int type);
    void signalSendStatusBarMsg(QString &msg);
    void signalChangeDBByFile(QVector<QString> &vec_samplename);
    void SignalChangePeak(QString &str_file);
private:
    QVector<Ab1FileTableBase> m_vec_filetable;
    bool m_bRefresh;
    bool m_bApplyOne;        //true调整单个峰图,false调整所有峰图
    bool m_bEditOne;         //true单个编辑后立即分析，false多个编辑后分析
    int m_x_step;            //x轴间距
    QPoint m_select_pos;    //选中碱基的位置
    bool m_bIsSelect;      //是否选中了一个碱基
    int m_index_Select;    //选中碱基的下标
    int m_index_PeakLine;   //当前选中的峰图下标
    long m_l_xSize;         //x轴的长度
    int m_iPeakHeightTotal;   //所有峰图的高度之和
    QString m_str_SampleName;//样品名称
    int m_index_Exon;        //碱基index
    int m_maxleft;    //记录左边界最大值
    int m_start_exon;
    int m_end_exon;
    int m_x_index;  //记录以startexon为起点的下标
    QVector<QSharedPointer<PeakLine>> m_vec_Peakline;
    QString m_str_fontName;
    QMenu *m_pRightMenu;
    QAction *m_pActDelete;
    QAction *m_pActInsertBaseN;
    QAction *m_pActHideTraceDisplay;
    QAction *m_pActApplyOne;
    QAction *m_pActApplyAll;
    QAction *m_pActanalyzeLater;
    QAction *m_pActanalyzeNow;
    QAction *m_pActanalyze;
    QAction *m_pActExcludeLeft;
    QAction *m_pActExcludeRight;
    QAction *m_pActResetExclude;
};

#endif // MULTIPEAKWIDGET_H
