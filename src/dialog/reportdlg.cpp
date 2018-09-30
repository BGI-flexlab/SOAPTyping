#include "reportdlg.h"
#include "ui_reportdlg.h"
#include "Core/core.h"
#include "DataBase/soaptypingdb.h"
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QTextStream>

ReportDlg::ReportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDlg)
{
    ui->setupUi(this);
    ConnectSignalandSlot();
    setDefaultPath();
}

ReportDlg::~ReportDlg()
{
    delete ui;
}


void ReportDlg::ConnectSignalandSlot()
{
    connect(ui->btnBrowse, &QPushButton::clicked, this, &ReportDlg::slotClickScanBt);
    connect(ui->btnSave, &QPushButton::clicked, this, &ReportDlg::slotClickSaveBt);
    connect(ui->btnExit, &QPushButton::clicked, this, &ReportDlg::close);
}

void ReportDlg::setDefaultPath()
{
    QString strDir;
    Core::GetInstance()->GetConfig("path/ReportDir",strDir);
    if(!strDir.isEmpty())
    {
        QString strPath = QString("%1%2%3%4.xls").arg(strDir).arg(QDir::separator())
                .arg(QDate::currentDate().toString("yyyy_MM_dd"))
                .arg(QTime::currentTime().toString("hh_mm_ss_zzz"));

        ui->lineEdit_path->setText(strPath);
    }
}

void ReportDlg::setVersion(const QString &strVer)
{
    m_str_Version = strVer;
}

void ReportDlg::slotClickScanBt()
{
    QString strDir;
    Core::GetInstance()->GetConfig("path/ReportDir",strDir);

    QString strSelectDir= QFileDialog::getExistingDirectory(this, tr("Export"), strDir);
    if (!strSelectDir.isEmpty())
    {
        Core::GetInstance()->SetConfig("path/ReportDir",strSelectDir);

        QString strPath = QString("%1%2%3%4.xls").arg(strSelectDir).arg(QDir::separator())
                .arg(QDate::currentDate().toString("yyyy_MM_dd"))
                .arg(QTime::currentTime().toString("hh_mm_ss_zzz"));

        ui->lineEdit_path->setText(strPath);
    }
}

void ReportDlg::printReport()
{
    QString strPath = ui->lineEdit_path->text();
    int i_num = ui->lineEdit_count->text().toInt();
    bool bignoreIndel = ui->checkBox->checkState();
    QFile file(strPath);
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }

    QTextStream stream(&file);

    QString strtime;
    strtime.append(QDate::currentDate().toString("MM/dd/yyyy").trimmed());//"yyyy/MM/dd"
    strtime.append(" ");
    strtime.append(QTime::currentTime().toString("hh:mm:ss").trimmed());
    QString version = QString("Reported by Soaptyping V%1 %2").arg(m_str_Version).arg(strtime);
    stream<<version<<"\n"<<"\n"<<"Final Typing Summary\n";

    if(i_num == 0) //不输出错配数
        stream<<"Sample\tAllele1\tAllele2\tInfo\tIndel\tType\tAllele1-H\tAllele2-H\n"; //改，增加后两列
    else
        stream<<"Sample\tAllele1\tAllele2\tMismatch\tInfo\tIndel\tType\n";

    QStringList sampleNames;
    SoapTypingDB::GetInstance()->getSampleNamesFromRealTimeDatabase(sampleNames);
    int start, end;
    for(int i=0; i<sampleNames.size(); i++)
    {
        SoapTypingDB::GetInstance()->getSampleStartEndBySampleName(sampleNames.at(i), start, end);
        stream<<sampleNames.at(i);
        QString result;
        bool isdo=false;
        int type = SoapTypingDB::GetInstance()->getResultFromRealTimeDatabaseBySampleName(sampleNames.at(i), result);
        if(type==0)
        {
            QStringList list = result.split(";", QString::SkipEmptyParts);
            int num=0;
            for(int i=0; i<list.size(); i++)
            {
                QStringList line = list.at(i).split(",", QString::SkipEmptyParts);
                QStringList data1 = line.at(1).split(":",QString::SkipEmptyParts); //新增
                QStringList data2 = line.at(2).split(":",QString::SkipEmptyParts); //新增
                if(bignoreIndel && line.at(3) != "0")
                {
                    if(SoapTypingDB::GetInstance()->isIndelInRange(line.at(1),start, end) ||
                       SoapTypingDB::GetInstance()->isIndelInRange(line.at(2), start, end))
                        continue;
                }
                if(i_num==0)
                {
                    if(line.at(0)=="0")
                    {
                        isdo = true;

                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)
                             <<"\t"<<"Soft"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    }
                    else
                        break;
                }
                else if(i_num !=0)
                {
                    isdo = true;

                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)
                         <<"\t"<<"Soft"<<"\t"<<line.at(3)<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    num++;
                    if(num>=i_num)
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
                if(bignoreIndel && line.at(3) != "0")
                {
                    if(SoapTypingDB::GetInstance()->isIndelInRange(line.at(1),start, end) ||
                       SoapTypingDB::GetInstance()->isIndelInRange(line.at(2), start, end))
                        continue;
                }
                if(i_num==0)
                {
                    if(line.at(0).at(0) =='0')
                    {
                        isdo = true;
                        stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(4)<<"\t"<<line.at(3)
                             <<"\t"<<"Filter"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";
                    }
                    else
                        break;
                }
                else if(i_num !=0)
                {
                    isdo = true;

                    stream<<"\t"<<line.at(1)<<"\t"<<line.at(2)<<"\t"<<line.at(0)<<"\t"<<line.at(4)<<"\t"
                         <<line.at(3)<<"\t"<<"Filter"<<"\t"<<data1[0] + ":" + data1[1]<<"\t"<<data2[0]+":"+data2[1]<<"\n";

                    num++;
                    if(num>=i_num)
                        break;
                }
            }
        }
        else if (type == 2)
        {
            QStringList pair = result.split(",", QString::SkipEmptyParts);
            isdo = true;
            if(i_num == 0)
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

    file.close();
}

void ReportDlg::slotClickSaveBt()
{
    printReport();
    close();
}
