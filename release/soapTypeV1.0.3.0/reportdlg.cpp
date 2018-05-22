#include "reportdlg.h"
#include "realtimedatabase.h"
#include "staticdatabase.h"
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

void ReportDlg::slotClickCheckBox(bool check)
{
    ignoreIndel = check;
}

void ReportDlg::slotClickSaveBt()
{
    printReport(fileName, keepNum, ignoreIndel);
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

void printReport(const QString &outFile, int keepNum, bool ignoreIndel)
{
    QFile file(outFile);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug()<<"printReport open file error:"<<outFile;
    }
    QTextStream stream(&file);
    //qDebug()<<"start";
    if(keepNum == 0) //²»Êä³ö´íÅäÊý
        stream<<"Sample\tAllele1\tAllele2\tInfo\tIndel\tType\n";
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
                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Soft"<<"\n";
                    }
                    else
                        break;
                }
                else if(keepNum !=0)
                {
                    isdo = true;
                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Soft"<<"\n";
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
                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\n";
                    }
                    else
                        break;
                }
                else if(keepNum !=0)
                {
                    isdo = true;
                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"<<line.at(3)<<"\t"<<"Filter"<<"\n";
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
