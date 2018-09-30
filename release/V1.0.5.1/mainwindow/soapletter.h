#ifndef SOAPLETTER_H
#define SOAPLETTER_H

#include <QWidget>
#include <QPainter>
#include <QFont>
#include <QFrame>
#include <QMouseEvent>
//新增类
class SoapLetter : public QWidget
{
    Q_OBJECT
public:
    explicit SoapLetter();
    explicit SoapLetter(char basetype,QColor color,QWidget *parent = 0);
    void setLetterInfo(int fileIndex = 0,int match =0);
    void setColor(QColor &color,char type);
    void setColor(QColor color){m_BaseTypeColor = color;}
    //void setLetterDataList(QList<SoapLetter*> *list){m_pList = list;}
    void setAllLetterDataList(QList< QList<SoapLetter*> > *all_list){m_pAllList = all_list;}
    void initLetterData();

    bool    m_IsClicked;
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    char    m_BaseType;
    QColor  m_BaseTypeColor;

    int     m_LetterFileIndex;
    int     m_LetterMatch;
    //QList<SoapLetter*> *m_pList;
    QList< QList<SoapLetter*> > *m_pAllList;
signals:
    void signalClickLetter(int,int);
    void formbasechanged(char,int,int);
public slots:

};

#endif // SOAPLETTER_H
