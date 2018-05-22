#include "key_update.h"
#include <QFile>
#include "QDataStream"
#include "All_Base_Struct.h"





Key_Update::Key_Update()
{
    FistTime.id=0;
    Last_Time.id=0;
    Now_Time.id=0;
    mouth=0;
    currentime=QTime::currentTime();
    currentdate=QDate::currentDate();
    mouth2=0;
}
bool Key_Update::Initdate()
{
    FistTime.id=1;
    FistTime.date=QDate::currentDate();
    FistTime.time=QTime::currentTime();
    Last_Time.id=2;
    Last_Time.date=QDate::currentDate();
    Last_Time.time=QTime::currentTime();
    Now_Time.id=3;
    Now_Time.date=QDate::currentDate();
    Now_Time.time=QTime::currentTime();
    Updatetofileofdata();
    return true;
}
bool Key_Update::Updatetofileofdata()
{
    try{
        FILE * fp;
        if((fp=fopen("keyfile","wb"))==NULL)
        {
            errormessge("Updatetofileofdata");
            exit(0);
        }
        if(fwrite(&versionsStart,sizeof(double),1,fp)!=1)
            errormessge("Updatetofileofdata1");
        if(fwrite(key,sizeof(UCHAR),1024,fp)!=1024)
            errormessge("Updatetofileofdata2");
        if(fwrite(miwen,sizeof(char),1024,fp)!=1024)
            errormessge("Updatetofileofdata3");
        if(fwrite(&FistTime.id,sizeof(int),1,fp)!=1)
            errormessge("Updatetofileofdata4");
        if(fwrite(&Last_Time.id,sizeof(int),1,fp)!=1)
            errormessge("Updatetofileofdata5");
        if(fwrite(&Now_Time.id,sizeof(int),1,fp)!=1)
            errormessge("Updatetofileofdata6");
        if(fwrite(&FistTime.date,sizeof(QDate),1,fp)!=1)
            errormessge("Updatetofileofdata7");
        if(fwrite(&Last_Time.date,sizeof(QDate),1,fp)!=1)
            errormessge("Updatetofileofdata8");
        if(fwrite(&Now_Time.date,sizeof(QDate),1,fp)!=1)
            errormessge("Updatetofileofdata9");
        if(fwrite(&FistTime.time,sizeof(QTime),1,fp)!=1)
            errormessge("Updatetofileofdata10");
        if(fwrite(&Last_Time.time,sizeof(QTime),1,fp)!=1)
            errormessge("Updatetofileofdata11");
        if(fwrite(&Now_Time.time,sizeof(QTime),1,fp)!=1)
            errormessge("Updatetofileofdata12");
        if(fwrite(&versionsEnd,sizeof(double),1,fp)!=1)
            errormessge("Updatetofileofdata13");
        fclose(fp);

    }
    catch(...)
    {
        ErrorWrite::errorocur("Updatetofileofdata");
        return false;
    }
    return true;

}
bool Key_Update::GettimeformNet()
{

    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost("time.nist.gov", 13);
    //socket->killTimer(60);
    if (socket->waitForConnected(3000))
    {
        if (socket->waitForReadyRead())
        {

            QString str(socket->readAll());
            str = str.trimmed();
            QString date= str.section(" ", 1, 1);
            QString time= str.section(" ", 2, 2);
            int yy=QDate::currentDate().year()/100;
            int dd=0,mm=0;
            QStringList bb=time.split(":");
            int hh=0,ms=0,ss=0;
            if(bb.count()<3) return 1;
            hh=(bb[0].toInt()+8)%24;
            ms=bb[1].toInt();
            ss=bb[2].toInt();
            QTime ti(hh,ms,ss);

            QStringList aa= date.split("-");
            if(aa.count()<3) return false;
            yy=yy*100+aa[0].toInt();
            mm=aa[1].toInt();
            dd=aa[2].toInt();
            QDate da(yy,mm,dd);
            if(bb[0].toInt()>16) da.addDays(1);
            if(currentdate!=da)
            {
                QMessageBox :: warning(0 , " ERROR" , "System date is wrong" );
                exit(0);
            }
            int hour=currentime.hour()-ti.hour();
            if(hour<0)
                hour=-hour;
            if(hour>1)
            {
                QMessageBox :: warning(0 , " ERROR" , "System time is wrong" );
                exit(0);
            }
            if(strlen((char *)key)>7)
            {
                int nowkey=key[7]-'0';
                if(nowkey>1)
                {
                    key[7]='1';
                }
            }
        }
        socket->close();
        delete socket;
        return true;
    }
    return false;

}
bool Key_Update::GetDateFromFile()
{
    try{
        FILE * fp;
        if((fp=fopen("keyfile","rb"))==NULL)
        {
            errormessge("1GetDateFromFile");
        }
        if(fread(&versionsStart,sizeof(double),1,fp)!=1)
        {
            errormessge("2GetDateFromFile");
        }
        long leng=sizeof(double)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(key,sizeof(UCHAR),1024,fp)!=1024)
        {
            errormessge("3GetDateFromFile");
        }
        leng+=sizeof(UCHAR)*1024L;
        fseek(fp, leng, SEEK_SET);
        if(fread(miwen,sizeof(UCHAR),1024,fp)!=1024)
        {
            errormessge("4GetDateFromFile");
        }
        leng+=sizeof(UCHAR)*1024L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&FistTime.id,sizeof(int),1,fp)!=1)
        {
            errormessge("5GetDateFromFile");
        }
        leng+= sizeof(int)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Last_Time.id,sizeof(int),1,fp)!=1)
        {
            errormessge("6GetDateFromFile");
        }
        leng+= sizeof(int)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Now_Time.id,sizeof(int),1,fp)!=1)
        {
            errormessge("7GetDateFromFile");
        }
        leng+= sizeof(int)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&FistTime.date,sizeof(QDate),1,fp)!=1)
        {
            errormessge("8GetDateFromFile");
        }
        leng+=sizeof(QDate)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Last_Time.date,sizeof(QDate),1,fp)!=1)
        {
            errormessge("9GetDateFromFile");
        }
        leng+=sizeof(QDate)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Now_Time.date,sizeof(QDate),1,fp)!=1)
        {
            errormessge("10GetDateFromFile");
        }
        leng+=sizeof(QDate)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&FistTime.time,sizeof(QTime),1,fp)!=1)
        {
            errormessge("11GetDateFromFile");
        }
        leng+=sizeof(QTime)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Last_Time.time,sizeof(QTime),1,fp)!=1)
        {
            errormessge("12GetDateFromFile");
        }
        leng+=sizeof(QTime)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&Now_Time.time,sizeof(QTime),1,fp)!=1)
        {
            errormessge("13GetDateFromFile");
        }
        leng+=sizeof(QTime)*1L;
        fseek(fp, leng, SEEK_SET);
        if(fread(&versionsEnd,sizeof(double),1,fp)!=1)
        {
            errormessge("14GetDateFromFile");
        }
        fclose(fp);
    }
    catch(...)
    {
        ErrorWrite::errorocur("GetDateFromFile");
        return false;
    }
    return true;
}
void Key_Update::errormessge(QString where)
{
    QMessageBox :: warning(0 , " ERROR" , "Keyfile was missing , please contact developers for help!" );
    ErrorWrite::errorocur(where);
    exit(0);
}
bool Key_Update::RenewDate()
{


    Last_Time.date=Now_Time.date;
    Last_Time.time=Now_Time.time;
    Now_Time.date=currentdate;
    Now_Time.time=currentime;

    return  true;

}

bool Key_Update::AllIsRigthNet()
{
    if(!GetDateFromFile()) {return false;exit(0);}
    if(!KeyIsRight()) {return false;exit(0);}
    GettimeformNet();
    if(!IsOutofTime()) {return false;exit(0);}
    if(!RenewDate()) {return false;exit(0);}
    if(!Updatetofileofdata()) {return false;exit(0);}
    return true;
}
bool Key_Update::AllIsRigth()
{
    if(!GetDateFromFile())
    {return false;exit(0);}
    if(!KeyIsRight()) {return false;exit(0);}
    return true;

}
bool Key_Update::IsOutofTime()
{

    if(FistTime.id!=1||Last_Time.id!=2||Now_Time.id!=3||mouth<1)
    {
        QMessageBox :: warning(0 , " ERROR" , "Keyfile was missing!" );
        return false;
    }

    if(Last_Time.date>currentdate)
    {
        QMessageBox :: warning(0 , " ERROR" , "Please update system date" );
        return false;
    }
    if(Last_Time.date==currentdate && (Last_Time.time.hour()-currentime.hour())>1)
    {
        QMessageBox :: warning(0 , " ERROR" , "Please update system time" );
        return false;
    }
    double temmouth=(double)(currentdate.day()-FistTime.date.day())/30
            +(currentdate.year()-FistTime.date.year())*12+
            currentdate.month()-FistTime.date.month();

    if(mouth<temmouth)
    {
        QMessageBox :: warning(0 , "ERROR" , "Keyfile was modified, please contact developers for help!" );
        return false;
    }
    return true;
}
bool Key_Update::KeyIsRight()
{
    try{

        TAesClass *aes = new TAesClass;
        char result[1024];
        if(strlen((char *)key)<6)
        {
            QMessageBox :: warning(0 , "ERROR" , "Keyfile was modified, please contact developers for help!" );
            ErrorWrite::errorocur("KeyIsRight");
        }
        key[3]='_';
        int nowvalue=0;
        if(strlen((char *)key)>7)
        {
            nowvalue= key[7]-'0';
            if(nowvalue>0)
                key[7]='1';
        }
        UCHAR *p = key;
        aes->InitializePrivateKey(16, p); //进行初始
        aes->OnAesUncrypt((LPVOID)miwen, (DWORD)sizeof(miwen),(LPVOID)result); //进行解密
        free(aes);
        aes=0;
        char begin[1024]="bgi chenyongsheng sss type";
        if(strcmp(begin,result)!=0)
        {
            QMessageBox :: warning(0 , "ERROR" , "Keyfile was modified, please contact developers for help!" );
            ErrorWrite::errorocur("KeyIsRight");
            return false;
        }

        mouth=(key[4]-'0')*12+(key[5]-'0');
        if(strlen((char *)key)<8)
        {
            mouth2=0;
            QMessageBox :: warning(0 , "ERROR" , "Keyfile is old, please use the new!" );
            ErrorWrite::errorocur("KeyIsRight");
            return false;
        }
        else
        {

            if(nowvalue>0&&nowvalue<=20)
            {
                mouth2=1;
                key[7]='0'+nowvalue+1;
            }
            else
            {
                mouth2=0;
            }
        }

        return true;
    }
    catch(...)
    {
        ErrorWrite::errorocur("KeyIsRight");
        return false;
    }

}








