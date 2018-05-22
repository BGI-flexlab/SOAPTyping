#ifndef MATCHLIST_H
#define MATCHLIST_H
#include <QtCore>
#include <QtGui>
#include "realtimedatabase.h"
#include "staticdatabase.h"

class MatchListWidget : public QTableWidget
{
    Q_OBJECT
public:
    MatchListWidget(QWidget *parent=0);
    void setConfig();
    QStringList getTypeResult(){return m_typeResult;}
signals:
    void signalFileChanged(SignalInfo &signalInfo, int type);
    void signalIndelPostion(int indelPos);
    void signalAllelePair(QString &allele1, QString &allele2);
public slots:
    void slotFileChanged(SignalInfo &signalInfo, int type);
    void slotShieldIndelAllelePair(bool isShield);
private slots:
    void slotClickIndelItem(QTableWidgetItem*);
    void slotRowChanged(QTableWidgetItem*);
    void slotShowGsspZCode();
    void slotSetFinalType();
protected:
    void contextMenuEvent(QContextMenuEvent *);
private:
    void clearItem();
    void setMatchListWidgetDefault();
    void setData(QStringList &typeResult);
    void setGssp();
private:
    bool isShieldIndel_;
    int rowNum_;
    int colNum_;
    int rowNumTmp_;
    int oldRow_;
    int alignStart_;
    int alignEnd_;
    QMenu *menuRight_;
    QAction *actionShowGSSPZCode_;
//    QAction *actionShowUndefinedExon_;
//    QAction *actionSetGsspInfo_;
    QAction *actionSetFinalType_;
    SignalInfo signalInfo_;
    QStringList defaultHead_;
    QVector<QStringList> gsspInfos_;
    int m_ListRow;//上一次被选中Row值
    QStringList m_typeResult;
};

#endif // MATCHLIST_H
