#include "soapletter.h"
#include <QDebug>

SoapLetter::SoapLetter()
{

}

SoapLetter::SoapLetter(char basetype, QColor color, QWidget *parent) :
    QWidget(parent)
{
    m_BaseType        = basetype;
    m_BaseTypeColor   = color;
    m_IsClicked       = false;
    m_LetterFileIndex = 0;
    m_LetterMatch     = 0;
    this -> setFocusPolicy(Qt::StrongFocus);
}

void SoapLetter::setLetterInfo(int fileIndex, int match)
{
     m_LetterFileIndex = fileIndex;
     m_LetterMatch     = match;
}

void SoapLetter::setColor(QColor &color, char type)
{
    if( type == 'G' )
    {
        color = QColor( 0,0,255);
    }
    else if( type == 'A' )
    {
        color = QColor( 57,181,74);
    }
    else if( type == 'T' )
    {
        color = QColor( 255,0,0);
    }
    else if( type == 'C' )
    {
        color = QColor( 0,0,255);
    }
    else if( ( type == 'R' ) ||
             ( type == 'Y' ) ||
             ( type == 'K' )||
             ( type == 'M' ) ||
             ( type == 'S' ) ||
             ( type == 'D' ) ||
             ( type== 'W' )  ||
             ( type == 'H' ) ||
             ( type == 'B' ) ||
             ( type == 'V' ) ||
             ( type == 'N')||
             ( type == 'L'))
    {
        color = QColor( 193,39,45);
    }
    else
    {
        //        QString outInfor = "The change base also contain the basetype:    ";
        //        outInfor +=  type;
        //        QMessageBox::warning( this , tr("Base ERROR") , outInfor );
        //wutcsl@163.com
        return;
    }
}

void SoapLetter::initLetterData()
{
//    int num = m_pList->size();
//    for(int i = 0;i < num;i++)
//    {
//        m_pList->at(i)->m_IsClicked = false;
//    }
    int listnum = m_pAllList->size();
    for(int i = 0;i<listnum;i++)
    {
        int num = m_pAllList->at(i).size();
        for(int j = 0;j < num;j++)
        {
            if(m_pAllList->at(i).at(j)->m_IsClicked)
            {
                m_pAllList->at(i).at(j)->m_IsClicked = false;
                m_pAllList->at(i).at(j)->repaint();
            }
//            m_pAllList->at(i).at(j)->m_IsClicked = false;
        }
    }
}

void SoapLetter::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(QColor(211,211,211));
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::Antialiasing, false);
    int widthNum = width();
    int heightNum = height();
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);

    painter.setPen(m_BaseTypeColor);
    painter.drawText(QRect(1,0,widthNum,heightNum),Qt::AlignCenter|Qt::AlignHCenter,QString("%1").arg(m_BaseType));

    if(m_IsClicked)
    {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(QRect(0,0,widthNum-1,heightNum-1));
    }
}

void SoapLetter::mousePressEvent(QMouseEvent *e)
{
    if(m_LetterMatch == -1)
    {
        return;
    }
    if(e->button() == Qt::LeftButton)
    {
        initLetterData();
        m_IsClicked = true;
        signalClickLetter(m_LetterFileIndex,m_LetterMatch);
        repaint();
    }
}

void SoapLetter::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_LetterMatch == -1)
    {
        return;
    }
    if(e->button() == Qt::LeftButton)
    {
        initLetterData();
        m_IsClicked = true;
        repaint();
    }
}

void SoapLetter::keyPressEvent(QKeyEvent *e)
{
    if(!m_IsClicked || m_LetterMatch == -1)
    {
        return;
    }
    int key = e->key();
    char changebp=' ';
    switch(key)
    {
    case 65:
        changebp='A';break;
    case 97:
        changebp='A';break;
    case 84:
        changebp='T';break;
    case 116:
        changebp='T';break;
    case 67:
        changebp='C';break;
    case 99:
        changebp='C';break;
    case 71:
        changebp='G';break;
    case 103:
        changebp='G';break;
    case 82:
        changebp='R';break;
    case 114:
        changebp='R';break;
    case 89:
        changebp='Y';break;
    case 121:
        changebp='Y';break;
    case 75:
        changebp='K';break;
    case 107:
        changebp='K';break;
    case 77:
        changebp='M';break;
    case 109:
        changebp='M';break;
    case 83:
        changebp='S';break;
    case 115:
        changebp='S';break;
    case 87:
        changebp='W';break;
    case 119:
        changebp='W';break;
    default:
        qDebug()<<"error";
        break;
    }
    setColor(m_BaseTypeColor,changebp);
    m_IsClicked = false;

    if (changebp!=' ')
    {
        m_BaseType  = changebp;
        repaint();
        emit formbasechanged(changebp,m_LetterFileIndex,m_LetterMatch);
    }
}
