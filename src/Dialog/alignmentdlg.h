#ifndef ALIGNMENTDLG_H
#define ALIGNMENTDLG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class AlignmentDlg;
}

struct AlleleNameSeqPair
{
    QString name;
    QString seq;
    QVector <int> mis;
};

class AlignPaintWgt : public QWidget
{
    Q_OBJECT
public:
    AlignPaintWgt(QWidget *parent=0);
    ~AlignPaintWgt();
    void setShowCodon(bool show);
    void setExonPositionInfo(QVector<int> &exonStartPosIndex);
    void setFirstPair(const AlleleNameSeqPair &pair);
    void setSelectAlleles(const QMap<int, AlleleNameSeqPair> &mapPair);
    void setCodonStart(int pos);
    void getSize();
protected:
    QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent *);
public slots:
    void slotRepaint();
private:

    void setArray();
private:
    bool brushColor_;
    bool showCodon_;
    int codonStart_;
    int headerWidth_;
    int fontWidth_;
    int fontHeight_;
    int lineBaseNumber_;
    int rowsHeight_;
    int baseNumber_;
    int rowNum_;
    AlleleNameSeqPair firstPair_;
    QVector<QString> alleNames_;
    QVector<QString> alleSeqs_;
    QVector<QVector<int> >mis_;
    QMap<int, int> posToExon_;
    int exonNum_;
    int *xx_;
    int *yy_;
    int *exon_;
    int *exonRow_;
};

class AlignmentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AlignmentDlg(QWidget *parent,const QString &strver,const QString &strgene);
    ~AlignmentDlg();
protected:
    virtual void resizeEvent(QResizeEvent *) override;
private:
    void InitUI();
    void ConnectSignalandSlot();
    void setGeneBoxData();
private slots:
    void slotGeneBoxItemChanged(const QString &gene);
    void slotClickShowInfoButton();
    void slotListSelectionChanged();
    void slotClickClearAllButton();
    void slotClickShowCodonBox(bool);
private:
    Ui::AlignmentDlg *ui;
    QString m_str_geneInfo;
    QString m_str_genever;
    QVector<int> m_vec_exon;
    QStringList m_strlist_alleleNames;
    QStringList m_strlist_alleleSeqs;
    QVector< QVector<int> > m_vec_misPositions;
    QMap<int, AlleleNameSeqPair> m_map_pair;
    QMap<QString, int> m_map_coden;
    AlignPaintWgt *m_pAlignPaintWgt;
    QString m_str_selgene;
};

#endif // ALIGNMENTDLG_H
