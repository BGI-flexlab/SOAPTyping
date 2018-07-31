#ifndef SAMPLETREEWIDGET_H
#define SAMPLETREEWIDGET_H
#include <QtCore>
#include <QtGui>
#include "database/realtimedatabase.h"

class SampleTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    SampleTreeWidget(QWidget *parent = 0);
signals:
    void signalFileChnaged(SignalInfo &signalInfo, int type);
    void signalGeneName(QString geneName); //新增
public slots:
    void slotFileChanged(SignalInfo &signalInfo, int type);
private slots:
    void slotItemChanged(QTreeWidgetItem *item0, int col);          //鼠标点击不同的item的时候, 关联此槽, 向外发送FileSignal信号.
    void slotQuickSaveAndClear();    //保存样品, 并且从内存中删除该样品
    void slotQuickSave();            //保存样品
    void slotSaveByDate();           //根据日期保存样品
    void slotDelete();               //删除样品
    void slotShowGSSPZCode();        //显示样品的GSSP引用信息, 针对mis=0的所有型别
    void slotUserComments();         //用户自己设置的备注信息
    void slotSetFinalType();         //用户手动设置最终型别
    void slotMarkPendingReview();    //将样品标记为pending状态
    void slotMarkReviewed();         //将样品标记为review状态
    void slotMarkApproved();         //将样品标记为approved状态
    void slotUnlock();               //将样品还原为owned状态
    void slotMarkPendingAndClear();  //将样品标记为pending, 保存样品, 并删除样品
    void slotMarkReviewedAndClear(); //将样品标记为review, 保存样品, 并删除样品
    void slotMarkApprovedAndClear(); //将样品标记为approved, 保存样品, 并删除样品
    void slotDeleteSelectedItem();
    void slotCopyName();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    void setSampleTreeWidgetDefault();
    void setSampleTreeWidgetActionAndConnectionDefault();
    void setData(QVector<SampleTreeInfo> &sampleTreeInfoList);
    void setData(QVector<SampleTreeInfo_t> &sampleTreeInfoList);//新增
    QVector<SampleTreeInfo_t> sortData(QVector<SampleTreeInfo> sampleTreeInfoList);//新增
    QIcon getIcon(int analysisType, int markType);
    void markSampleType(int markType);
    void markSampleTypeAndClear(int markType);
    void expandSample(int type);
    int getSampleIndex(const QString &sampleName);
    int getFileIndex(const QString &fileName, int sampleIndex);
    void emitFileChangedSignal(int sampleIndex, int fileIndex, int type);
    void emitAfterDelete();
private:
    bool onControl;
    QMenu *rightMenu;
    QAction *actionQuickSaveAndClear;
    QAction *actionQuickSave;
    QAction *actionSaveByDate;
    QAction *actionDelete;
    //    QAction *actionShowGSSPZCode;
    QAction *actionUserComments;
    QAction *actionDeleteSelectedItem;
    //    QAction *actionSetFinalType;
    QAction *actionMarkPendingReview;
    QAction *actionMarkReviewed;
    QAction *actionMarkApproved;
    QAction *actionUnlock;
    QAction *actionMarkPendingAndClear;
    QAction *actionMarkReviewedAndClear;
    QAction *actionMarkApprovedAndClear;
    QAction *action_copy;

    SignalInfo signalInfo_;
    QVector<SampleTreeInfo> sampleTreeInfoList_;
    QVector<SampleTreeInfo_t> m_TreeInfoList;//新增
    QTreeWidgetItem *oldItem_;
    int oldColumn_;
};

#endif // SAMPLETREEWIDGET_H
