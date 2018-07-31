#include "reportdlg.h"
#include "database/realtimedatabase.h"
#include "database/staticdatabase.h"
void  Savereport(QString dir)
{
    QFile data("Database/report.ini");
    if (!data.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }
    QTextStream out(&data);//QDataStream
    out << dir<<"\r\n";
    data.flush();
    data.close();

}

QString  Readreport()
{
    QString dir;
    QFile data("Database/report.ini");
    if (!data.open(QFile::ReadOnly | QFile::Text))
    {
        return "";
    }
    QTextStream in(&data);//QDataStream
    in >> dir;
    data.close();
    return dir;
}
ReportDlg::ReportDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Produce Report"));
    setDefault();
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayou1 = new QHBoxLayout;
    QHBoxLayout *hLayou2 = new QHBoxLayout;
    QHBoxLayout *hLayou3 = new QHBoxLayout;
    QLabel *fileNameLabel = new QLabel("File Name:", this);
    QLabel *alleleCount = new QLabel("Allele Count:", this);
    QSpacerItem *spacer1 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);
    //    QSpacerItem *spacer2 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);

    hLayou1->addWidget(fileNameLabel);
    hLayou1->addWidget(fileLine);
    hLayou1->addWidget(scanBt);

    hLayou2->addWidget(ignoreIndelBox);
    hLayou2->addSpacerItem(spacer1);
    hLayou2->addWidget(alleleCount);
    hLayou2->addWidget(numLine);
    //    hLayou2->addSpacerItem(spacer2);

    hLayou3->addSpacerItem(spacer3);
    hLayou3->addWidget(saveBt);
    hLayou3->addWidget(exitBt);

    vLayout->addLayout(hLayou1);
    vLayout->addLayout(hLayou2);
    vLayout->addLayout(hLayou3);
    resize(500, 100);
    QString dir=Readreport();
    if(dir=="")
        dir.append(".\\");
    else
        dir.append("\\");
    dir.append(QDate::currentDate().toString("yyyy_MM_dd").trimmed());
    dir.append("_");
    dir.append(QTime::currentTime().toString("hh_mm_ss_zzz").trimmed());
    dir.append(".xls");
    dir.trimmed();
    fileLine->setText(dir);
}
void ReportDlg::setDefault()
{
    ignoreIndel = true;
    keepNum = 0;
    fileLine = new QLineEdit(this);
    numLine = new QLineEdit(this);
    numLine->setMaximumWidth(30);
    ignoreIndelBox = new QCheckBox("Ignore Indel", this);
    ignoreIndelBox->setCheckState(Qt::Checked);
    saveBt = new QPushButton("save", this);
    exitBt = new QPushButton("exit", this);
    scanBt = new QPushButton("Browse", this);
    connect(numLine, SIGNAL(textChanged(QString)), this, SLOT(slotNumLineChanged(QString)));
    connect(scanBt, SIGNAL(clicked()), this, SLOT(slotClickScanBt()));
    connect(saveBt, SIGNAL(clicked()), this, SLOT(slotClickSaveBt()));
    connect(exitBt, SIGNAL(clicked()), this, SLOT(close()));
    connect(ignoreIndelBox, SIGNAL(clicked(bool)), this, SLOT(slotClickCheckBox(bool)));
    connect(fileLine, SIGNAL(textChanged(QString)), this, SLOT(slotFileLineChanged(QString)));
}

//新增
void ReportDlg::setVersionName(QString version)
{
    versionName = version;
}

void ReportDlg::slotClickCheckBox(bool check)
{
    ignoreIndel = check;
}

void ReportDlg::slotClickSaveBt()
{
    printReport(fileName, keepNum, ignoreIndel,versionName);
    close();
}



void ReportDlg::slotClickScanBt()
{
    QString dir=Readreport();
    fileLine->setText("");
    QString s= QFileDialog::getExistingDirectory(this, tr("Export"), dir);
    if(s.isEmpty())
        return;
    Savereport(s);
    //qDebug()<<s;
    dir=s;

    dir.append("\\");
    dir.append(QDate::currentDate().toString("yyyy_MM_dd").trimmed());
    dir.append("_");
    dir.append(QTime::currentTime().toString("hh_mm_ss_zzz").trimmed());
    dir.append(".xls");
    dir.trimmed();
    fileLine->setText(dir);


}

void ReportDlg::slotNumLineChanged(QString s)
{
    if(s.isEmpty())
    {
        keepNum =  0;
    }
    else
    {
        keepNum = s.toInt();
    }
}

void ReportDlg::slotFileLineChanged(QString s)
{
    fileName = s;
}

void printReport(const QString &outFile, int keepNum, bool ignoreIndel,QString versionName)
{
    QFile file(outFile);
    if(!file.open(QFile::WriteOnly))
    {
//20180608close qDebug()<<"printReport open file error:"<<outFile;
    }
    QTextStream stream(&file);
    //qDebug()<<"start";
    //新增Start
    QString version;
    QString strtime;
    strtime.append(QDate::currentDate().toString("MM/dd/yyyy").trimmed());//"yyyy/MM/dd"
    strtime.append(" ");
    strtime.append(QTime::currentTime().toString("hh:mm:ss").trimmed());
    version = QString("Reported by Soaptyping V%1 %2").arg(versionName).arg(strtime);
    stream<<version<<"\n"<<"\n"<<"Final Typing Summary\n";
    //新增End

    if(keepNum == 0) //不输出错配数
        stream<<"Sample\tAllele1\tAllele2\tInfo\tIndel\tType\tAllele1-H\tAllele2-H\n"; //改，增加后两列
    else
        stream<<"Sample\tAllele1\tAllele2\tMismatch\tInfo\tIndel\tType\n";
    QStringList sampleNames;
    getSampleNamesFromRealTimeDatabase(sampleNames);
    int start, end;
    //qDebug()<<sampleNames;
    for(int i=0; i<sampleNames.size(); i++)
    {
        getSampleStartEndBySampleName(sampleNames.at(i).toAscii(), start, end);
        stream<<sampleNames.at(i);
        QString result;
        bool isdo=false;
        int type = getResultFromRealTimeDatabaseBySampleName(sampleNames.at(i).toAscii(), result);
        //qDebug()<<type<<result;
        if(type==0)
        {
            QStringList list = result.split(";", QString::SkipEmptyParts);
            int num=0;
            for(int i=0; i<list.size(); i++)
            {
                QStringList line = list.at(i).split(",", QString::SkipEmptyParts);
                QStringList data1 = line.at(1).split(":",QString::SkipEmptyParts); //新增
                QStringList data2 = line.at(2).split(":",QString::SkipEmptyParts); //新增
                if(ignoreIndel && line.at(3) != "0")
                {
                    if(isIndelInRange(line.at(1).toAscii(),start, end) || isIndelInRange(line.at(2).toAscii(), start, end))
                        continue;
                }
                if(keepNum==0)
                {
                    if(line.at(0)=="0")
                    {
                        isdo = true;
                        //改                       stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Soft"<<"\n";
                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Soft"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    }
                    else
                        break;
                }
                else if(keepNum !=0)
                {
                    isdo = true;
                    //改                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Soft"<<"\n";
                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<"Soft"<<"\t"<<line.at(3)<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    num++;
                    if(num>=keepNum)
                        break;
                }
            }
        }
        else if(type==1)
        {
            QStringList list = result.split(";", QString::SkipEmptyParts);
            int num=0;
            for(int i=0; i<list.size(); i++)
            {
                QStringList line = list.at(i).split(",", QString::SkipEmptyParts);
                QStringList data1 = line.at(1).split(":",QString::SkipEmptyParts); //新增
                QStringList data2 = line.at(2).split(":",QString::SkipEmptyParts); //新增
                if(ignoreIndel && line.at(3) != "0")
                {
                    if(isIndelInRange(line.at(1).toAscii(),start, end) || isIndelInRange(line.at(2).toAscii(), start, end))
                        continue;
                }
                if(keepNum==0)
                {
                    if(line.at(0).at(0).toAscii()=='0')
                    {
                        isdo = true;
                        //改                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\n";
                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    }
                    else
                        break;
                }
                else if(keepNum !=0)
                {
                    isdo = true;
                    //改                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\n";
                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";

                    num++;
                    if(num>=keepNum)
                        break;
                }
            }
        }
        else if (type == 2)
        {
            QStringList pair = result.split(",", QString::SkipEmptyParts);
            isdo = true;
            if(keepNum == 0)
                stream<<"\t"<<pair.at(0)<<"\t"<<pair.at(1)<<"\t"<<"\t"<<"\t"<<"User"<<"\n";
            else
                stream<<"\t"<<pair.at(0)<<"\t"<<pair.at(1)<<"\t"<<"\t"<<"\t"<<"\t"<<"User"<<"\n";
        }
        if(isdo)
        {
            stream<<"\n";
        }
        else
        {
            stream<<"\n\n";
        }
    }
    //qDebug()<<"end";
    file.close();
}
