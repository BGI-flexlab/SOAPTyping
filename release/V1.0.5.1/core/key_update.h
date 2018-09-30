#ifndef KEY_UPDATE_H
#define KEY_UPDATE_H
#include "TAesClass.h"
#include "QString"
#include "QMessageBox"
#include "QDate"
#include "QTime"
#include <stdio.h>
#include "QtNetwork/QTcpSocket"
#include "QStringList"

struct  When_Now
{
    int id ;
    QDate date;
    QTime time;
};
class Key_Update
{
public:
    Key_Update();
    bool DateIsRigth();
    bool Initdate();
    bool AllIsRigth();
    bool AllIsRigthNet();
    bool Updatetofileofdata();
    bool KeyIsRight();
    int Getmouth2()
    {
        return mouth2;
    }
private:

    bool GetDateFromFile();
    bool GettimeformNet();
    bool IsOutofTime();
    bool RenewDate();
    void errormessge(QString where);
    //bool GetKeyFromFile();
    When_Now FistTime;
    When_Now Last_Time;
    When_Now Now_Time;
    QString keystr;
    QDate currentdate;
    QTime currentime;
    int mouth;
    int mouth2;
    double versionsStart;
    double versionsEnd;
    UCHAR key[1024];
    char miwen[1024];
    bool updatabyNet;
};

#endif // KEY_UPDATE_H
