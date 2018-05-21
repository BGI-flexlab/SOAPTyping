#ifndef SAMPLETREEWIDGET_H
#define SAMPLETREEWIDGET_H
#include <QtCore>
#include <QtGui>
#include "realtimedatabase.h"

class SampleTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    SampleTreeWidget(QWidget *parent = 0);
signals:
    void signalFileChnaged(SignalInfo &signalInfo, int type);
    void signalGeneName(QString geneName);
public slots:
    void slotFileChanged(SignalInfo &signalInfo, int type);
private slots:
    void slotItemChanged(QTreeWidgetItem *item0, int col);          //����������ͬ��item��ʱ��, �����˲�, ���ⷢ��FileSignal�ź�.
    void slotQuickSaveAndClear();    //������Ʒ, ���Ҵ��ڴ���ɾ������Ʒ
    void slotQuickSave();            //������Ʒ
    void slotSaveByDate();           //�������ڱ�����Ʒ
    void slotDelete();               //ɾ����Ʒ
    void slotShowGSSPZCode();        //��ʾ��Ʒ��GSSP������Ϣ, ����mis=0�������ͱ�
    void slotUserComments();         //�û��Լ����õı�ע��Ϣ
    void slotSetFinalType();         //�û��ֶ����������ͱ�
    void slotMarkPendingReview();    //����Ʒ����Ϊpending״̬
    void slotMarkReviewed();         //����Ʒ����Ϊreview״̬
    void slotMarkApproved();         //����Ʒ����Ϊapproved״̬
    void slotUnlock();               //����Ʒ��ԭΪowned״̬
    void slotMarkPendingAndClear();  //����Ʒ����Ϊpending, ������Ʒ, ��ɾ����Ʒ
    void slotMarkReviewedAndClear(); //����Ʒ����Ϊreview, ������Ʒ, ��ɾ����Ʒ
    void slotMarkApprovedAndClear(); //����Ʒ����Ϊapproved, ������Ʒ, ��ɾ����Ʒ
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
    void setData(QVector<SampleTreeInfo_t> &sampleTreeInfoList);
    QVector<SampleTreeInfo_t> sortData(QVector<SampleTreeInfo> sampleTreeInfoList);//�����ݰ���һ��˳������
//    QVector<SampleTreeInfo> sortData(QVector<SampleTreeInfo> sampleTreeInfoList);
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
//    QVector<SampleTreeInfo> m_TreeInfoList;
    QVector<SampleTreeInfo_t> m_TreeInfoList;
    QTreeWidgetItem *oldItem_;
    int oldColumn_;
};

#endif // SAMPLETREEWIDGET_H
