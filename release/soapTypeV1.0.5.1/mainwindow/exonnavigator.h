#ifndef EXONNAVIGATOR_H
#define EXONNAVIGATOR_H
#include <QtCore>
#include <QtGui>
#include "database/realtimedatabase.h"
#include "database/staticdatabase.h"

class ExonNavigator : public QWidget
{
    Q_OBJECT
public:
    ExonNavigator(QWidget *parent=0);
signals:
    void signalFocusPosition(int basePosition, int exonIndex, int leftDistance);
public slots:
    void slotFileChanged(SignalInfo &signalInfo, int type);
    void slotFocusPosition(int basePosition, int exonIndex, int leftDistance);
    void slotTypeMisMatchPostionChanged(QSet<int> &typeMismatchPositions, int type);
    void slotIndelPosition(int basePos);
    void slotMisPosForward();
    void slotMisPosBackward();
protected:
    QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
private:
    int getExonNum(int basePos);
    int oldPosToNewPos(int oldPos);
    int newPosToOldPos(int newPos);
    void setData();
    int getLastXBySampleMisPos(int type);
private:
    bool isDown_;
    int lastX_;
    int gap_;
    int newLength_;
    qreal xscale_;
    ExonNavigatorInfo exonInfo_;
    SignalInfo signalInfo_;
    QVector<int> exonPositionIndex_;
    QMap<int, int> typeMisPosition_;
    QMap<int, int> sampleMisPostion_;
    QMap<int, int>::iterator typePoint_;
    QMap<int, int>::iterator samplePoint_;
};




#endif // EXONNAVIGATOR_H
