#ifndef BASEALIGNTABLEWIDGET_H
#define BASEALIGNTABLEWIDGET_H

#include <QTableWidget>
#include "all_base_struct.h"

class BaseAlignTableWidget:public QTableWidget
{
    Q_OBJECT
public:
    BaseAlignTableWidget();
    ~BaseAlignTableWidget();
    void SetAlignTableData(QString &str_samplename, QString &str_file,
                           QString str_info, int col);
    void SetAllelePairData(QString &allele1, QString &allele2);
    void SetRefresh(bool refresh){m_bRefresh = refresh;}
    void ClearBaseAlignTable();
private:
    void InitUI();
    void clearBaseAlignTableSampleItem();
    void getTableHead(QStringList &head, int length, int start);
    void getTypeAlignResult(char *result, char *pattern, char *alleleSeq1, char *alleleSeq2, QSet<int> &misMatch,
                            int alignStart1, int alignStart2);
    void updateTopTableViewGeometry();
    void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;

signals:
    void signalTypeMisMatchPosition(QSet<int> &typeMisMatchPositions, int type);
private:
    int m_iRowNum;
    int m_iColNum;
    bool m_bRefresh; //是否要求重新刷新界面
    QString m_str_file;
    QString m_str_info;
    int m_i_col;
    QString m_str_SampleName;
    QStringList m_sl_defaulthead;
    BaseAlignSampleInfo m_BaseAlignSampleInfo;
    QTableView *m_topTableView;
};

#endif // BASEALIGNTABLEWIDGET_H
