#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qscrollarea.h>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class SampleTreeWidget;
class MatchListWidget;
class MultiPeakWidget;
class ExonNavigatorWidget;
class BaseAlignTableWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitData();
private:
    void SetStatusbar();                    //显示底部左侧状态栏信息
    void ConnectSignalandSlot();            //连接信号与槽函数
    void DisConnectSignalandSolt();         //断开信号与槽函数连接
    virtual void closeEvent(QCloseEvent *e);
    void InitUI();

public slots:
    void slotSampleTreeItemChanged(QTreeWidgetItem *item, int col);
    void slotExonFocusPosition(int startpos, int selectpos, int exonstartpos, int index);
    void slotAlignTableFocusPosition(QTableWidgetItem *item);
    void slotPeakFocusPosition(int index, int colnum, QPoint &pos);

    void slotShowSaveDlg();
    void slotShowLoadFileDlg();
    void slotShowOpenDlg();
    void slotShowDeleteDlg();
    void slotShowExportDlg();

    void slotReset();
    void slotMisPosForward();
    void slotMisPosBackward();
    void slotMarkAllSampleApproved();
    void slotMarkAllSampleReviewed();
    void slotAlignPair();
    void slotAlignLab();
    void slotUpdateDatabase();
    void slotControl();
    void slotSetExonTrim();

    void slotyRangeRoomUp();
    void slotyRangeRoomDown();
    void slotyRoomUp();
    void slotyRoomDown();
    void slotxRoomUp();
    void slotxRoomDown();
    void slotResetRoom();

    void slotApplyOne();
    void slotApplyAll();
    void slotAnalyseLater();
    void slotAnalyseNow();
    void slotanalyse();

    void slotAbout();
    void slotHelp();

    void slotAllelePairChanged(QString &, QString &);
    void slotTypeMisMatchPostion(QSet<int> &typeMismatchPos, int type);
    void slotShowStatusBarMsg(const QString &msg);

    void slotChangeDB(const QString &str_samplename);
    void slotChangeDBByFile(QVector<QString> &vec_samplename);

    void slotClearAll();//清空当前显示信息

    void slotPeakAct(int type);
    void slotChangePeak(QString &str_file);
private:
    Ui::MainWindow *ui;
    SampleTreeWidget *m_pSampleTreeWidget;
    MatchListWidget *m_pMatchListWidget;
    MultiPeakWidget *m_pMultiPeakWidget;
    ExonNavigatorWidget *m_pExonNavigatorWidget;
    BaseAlignTableWidget *m_pBaseAlignTableWidget;
    QScrollArea *m_pPeak_area;
    QString m_str_SelectFile;           //保存样品列表选中的文件名称
    QString m_str_SelectSample;         //保存样品列表选中的样品名称
    QTreeWidgetItem *m_pSelectItem;     //保存样品列表选中的item
    QString m_str_GeneVer;              //保存基因表的版本号
    bool m_bChangeDB;
};

class Tipbox : public QMessageBox
{
    Q_OBJECT
public:
    explicit Tipbox(QWidget *parent = nullptr);
    ~Tipbox();
    void doTask(const QString &str_samplename);
};

#endif // MAINWINDOW_H
