#ifndef ALIGNMENTDIALOG_H
#define ALIGNMENTDIALOG_H
#include <QtGui>
#include <QtCore>

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
protected:
    QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent *);
public slots:
    void slotRepaint();
private:
    void getSize();
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



class ListWgt: public QListWidget
{
    Q_OBJECT
public:
    ListWgt(QWidget *parent=0)
        :QListWidget(parent)
    {
    }

protected:
    void mouseMoveEvent(QMouseEvent *e)
    {
        e->ignore();
        return;
    }
};


class AlignmentDialog : public QDialog
{
    Q_OBJECT
public:
    AlignmentDialog(QWidget *parent=0);
protected:
    void resizeEvent(QResizeEvent *e);
signals:
    void signalResize();
private:
    void setGeneBoxData();
private slots:
    void slotGeneBoxItemChanged(QString gene);
    void slotClickShowInfoButton();
    void slotListSelectionChanged();
    void slotClickClearAllButton();
    void slotClickShowCodonBox(bool);
private:
    QLabel *label_;
    QComboBox *geneBox_;
    QCheckBox *showCodonBox_;
    QPushButton *clearAllButton_;
    QPushButton *showInfoButton_;
    QPushButton *exitButton_;
    ListWgt *listWidget_;
    AlignPaintWgt *alignPaintWdg_;
    QString geneInfo_;
    QVector<int> exonStartPosIndex_;
    AlleleNameSeqPair firstPair_;
    QStringList alleleNames_;
    QStringList alleleSeqs_;
    QVector< QVector<int> > misPositions_;
    QMap<int, AlleleNameSeqPair> pairMap_;
    QMap<QString, int> codenMap_;//神奇的东西...
};

#endif // ALIGNMENTDIALOG_H
