#ifndef SAMPLETREEWIDGET_H
#define SAMPLETREEWIDGET_H

#include <QTreeWidget>
#include <QMap>
#include "all_base_struct.h"

class SampleTreeWidget:public QTreeWidget
{
    Q_OBJECT

public:
    SampleTreeWidget(QWidget *parent = 0);
    ~SampleTreeWidget();
    void SetTreeData();
    void SetSelectItem(int index, const QString &str_sample);
private:
    void InitUI();
    void CreateRightMenu();
    void ConnectSignalandSlot();
    void markSampleType(int markType);
    void markSampleTypeAndClear(int markType);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void signalChangeDBByFile(QVector<QString> &vec_samplename);
    void signalClearAll();
private slots:
    void slotQuickSaveAndClear();
    void slotQuickSave();
    void slotSaveByDate();
    void slotDelete();

    void slotUserComments();
    void slotDeleteSelectedItem();
    void slotCopyName();

    void slotMarkPendingReview();
    void slotMarkReviewed();
    void slotMarkApproved();
    void slotUnlock();

    void slotMarkPendingAndClear();
    void slotMarkReviewedAndClear();
    void slotMarkApprovedAndClear();
private:
    QMap<QString,SampleTreeInfo_t> m_map_SampleTreeInfo;
    QMenu *m_pRightMenu;
    QAction *m_pActSaveAndClear;
    QAction *m_pActQuickSave;
    QAction *m_pActSaveByDate;
    QAction *m_pActDelete;
    QAction *m_pActUserComments;
    QAction *m_pActDelSelItem;
    QAction *m_pActMarkPendingReview;
    QAction *m_pActMarkReviewed;
    QAction *m_pActMarkApproved;
    QAction *m_pActUnlock;
    QAction *m_pActMarkPendingAndClear;
    QAction *m_pActMarkReviewedAndClear;
    QAction *m_pActMarkApprovedAndClear;
    QAction *m_pActCopy;
    QTreeWidgetItem *m_pSelByRightItem;     //右键选中的item
};

#endif // SAMPLETREEWIDGET_H
