#include "gsspinfodlg.h"
#include "realtimedatabase.h"
#include "staticdatabase.h"
#define MIN_GAP 50

GsspInfoDlg::GsspInfoDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Gssp Infomation"));
    setWindowFlags(Qt::WindowMaximizeButtonHint);
    setDefault();
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QSpacerItem *spacer1 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Minimum);
    QLabel *label1 = new QLabel("Sample Name:", this);
    QLabel *label2 = new QLabel("Gssp Recommended:", this);
    QLabel *label3 = new QLabel("Available Gssps:", this);
    QLabel *label4 = new QLabel("Gssp Set By User:", this);
    hLayout1->addWidget(label1);
    hLayout1->addWidget(sampleNameLine_);
    hLayout1->addSpacerItem(spacer1);
    hLayout1->addWidget(label2);
    hLayout1->addWidget(gsspDefaultLine_);

    hLayout2->addWidget(label3);
    hLayout2->addSpacerItem(spacer2);
    hLayout2->addWidget(label4);
    hLayout2->addWidget(gsspUserLine_);

    hLayout3->addSpacerItem(spacer3);
    hLayout3->addWidget(saveDefaultBt_);
    hLayout3->addWidget(saveUserBt_);
    hLayout3->addWidget(exitBt_);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addWidget(table1_);
    vLayout->addWidget(table2_);
    vLayout->addLayout(hLayout3);
    resize(750, 500);
    connect(table1_, SIGNAL(itemSelectionChanged()), this, SLOT(slotSetGsspInfoTable2()));
    connect(saveDefaultBt_, SIGNAL(clicked()), this, SLOT(slotClickSaveDefaultBt()));
    connect(saveUserBt_, SIGNAL(clicked()), this, SLOT(slotClickSaveUserBt()));
    connect(exitBt_, SIGNAL(clicked()), this, SLOT(close()));

}

void GsspInfoDlg::setDefault()
{
    sampleNameLine_ = new QLineEdit(this);
    sampleNameLine_->setEnabled(false);

    gsspDefaultLine_ = new QLineEdit(this);
    gsspDefaultLine_->setEnabled(false);

    gsspUserLine_ = new QLineEdit(this);

    table1_ = new QTableWidget(this);
    table1_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table1_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table1_->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList theader;
    theader<<"Allele 11"<<"Allele 12"<<"="<<"Allele 21"<<"Allele 22"<<"Gssps";
    table1_->verticalHeader()->setVisible(false);
    table1_->setColumnCount(6);
    table1_->setHorizontalHeaderLabels(theader);
    table1_->horizontalHeader()->setStretchLastSection(true);
    table1_->setColumnWidth(0, 100);
    table1_->setColumnWidth(1, 100);
    table1_->setColumnWidth(2, 20);
    table1_->setColumnWidth(3, 100);
    table1_->setColumnWidth(4, 100);
    table1_->setColumnWidth(5, 200);
    table1_->resize(720, 200);

    table2_ = new QTableWidget(this);
    table2_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table2_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table2_->setSelectionMode(QAbstractItemView::SingleSelection);
    table2_->verticalHeader()->setVisible(false);
    table2_->setColumnCount(10);
    QStringList header2;
    header2<<"Allele 11"<<"Allele 12"<<"="<<"Allele 21"<<"Allele 22"<<"Position"<<"Exon"<<"R/F"<<"Gssp"<<"Gssp Seq";
    table2_->setHorizontalHeaderLabels(header2);
    table2_->horizontalHeader()->setStretchLastSection(true);
    table2_->setColumnWidth(0, 100);
    table2_->setColumnWidth(1, 100);
    table2_->setColumnWidth(2, 20);
    table2_->setColumnWidth(3, 100);
    table2_->setColumnWidth(4, 100);
    table2_->setColumnWidth(5, 60);
    table2_->setColumnWidth(6, 40);
    table2_->setColumnWidth(7, 40);
    table2_->setColumnWidth(8, 50);
    table2_->setColumnWidth(9, 100);
    table2_->resize(620, 200);

    saveDefaultBt_ = new QPushButton("Save Default", this);
    saveUserBt_ = new QPushButton("Save User", this);
    exitBt_ = new QPushButton("Eixt", this);
}

void GsspInfoDlg::setSampleName(const QString &sampleName)
{
    sampleName_ = sampleName;
    sampleNameLine_->setText(sampleName_);
}

void GsspInfoDlg::setGsspTable1(QVector<QStringList> &gsspInfos)
{
    int size = gsspInfos.size();
    table1_->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        table1_->setRowHeight(i, 18);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(0));
        table1_->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(1));
        table1_->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText("=");
        table1_->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(2));
        table1_->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(3));
        table1_->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(gsspInfos.at(i).at(4));
        table1_->setItem(i, 5, item);

        QStringList info = gsspInfos.at(i).at(5).split(";", QString::SkipEmptyParts);
        infos_.push_back(info);
    }
}

void GsspInfoDlg::slotSetGsspInfoTable2()
{
    int index = table1_->currentRow();
    table2_->clear();
    QStringList head;
    head<<table1_->item(index, 0)->text()
          <<table1_->item(index, 1)->text()
         <<"="
           <<table1_->item(index, 2)->text()
             <<table1_->item(index, 3)->text()
            <<"position"<<"Exon"<<"R/F"
           <<"Gssp"<<"Gssp Seq";
    table2_->setHorizontalHeaderLabels(head);
    int size = infos_.at(index).size();
    table2_->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        table2_->setRowHeight(i, 18);
        QStringList line = infos_.at(index).at(i).split(":");
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(line.at(0));
        table2_->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(line.at(1));
        table2_->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(line.at(2));
        table2_->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(line.at(3));
        table2_->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(line.at(4));
        table2_->setItem(i, 5, item);

        item = new QTableWidgetItem;
        item->setText(line.at(5));
        table2_->setItem(i, 6, item);

        item = new QTableWidgetItem;
        item->setText(line.at(6));
        table2_->setItem(i, 7, item);

        item = new QTableWidgetItem;
        item->setText(line.at(7));
        table2_->setItem(i, 8, item);

        item = new QTableWidgetItem;
        item->setText(line.at(8));
        table2_->setItem(i, 9, item);
    }
}
void GsspInfoDlg::slotClickSaveDefaultBt()
{
    QString gssp = gsspDefaultLine_->text();
    updateSetGsspBySampleName(sampleName_.toAscii(), gssp);
    close();
}

void GsspInfoDlg::slotClickSaveUserBt()
{
    QString gssp = gsspUserLine_->text();
    updateSetGsspBySampleName(sampleName_.toAscii(), gssp);
    close();

}

void findUsefulGssp(const char *seq11, const char *seq12, const char *seq21, const char *seq22, int exonStart, int exonEnd, QVector<GsspTable> &gsspTables, QStringList &gssps, QStringList &infos)
{
    QVector<int> pos;
    for(int i=exonStart; i<exonEnd; i++)
    {
        if(seq11[i]!=seq12[i])
        {
            pos.push_back(i);
        }
    }
    if(pos.size()==0)
        return;

    QMap<int, int> same1;
    QMap<int, int> same2;
    QMap<int, int> posM;

    for(int i=0; i<pos.size(); i++)
    {
        if(seq11[pos[i]] == seq21[pos[i]])
        {
            same1.insert(pos[i], 0);
        }
        else
        {
            same2.insert(pos[i], 0);
        }
        posM.insert(pos[i], 0);
    }
    if(same1.size()==0 || same2.size()==0)
        return;

    bool flag = true;
    QMap<int, int> *sameP=NULL, *diffP=NULL;
    if(same1.size() >= same2.size())
    {
        sameP = &same1;
        diffP = &same2;
        flag = true; //13, 24
    }
    else
    {
        sameP = &same2;
        diffP = &same1;
        flag = false; //13, 23
    }

    int sb = sameP->begin().key(), se = (sameP->end()--).key();
    int db = diffP->begin().key(), de = (diffP->end()--).key();

    for(int i=0; i<gsspTables.size(); i++)
    {
        int point = gsspTables.at(i).position;
        if(!posM.contains(point))
            continue;

        if(sameP->contains(point))
        {
            if(gsspTables.at(i).rOrF.contains("F"))
            {
                if(de - point < MIN_GAP )
                    continue;
            }
            else
            {
                if(point - db < MIN_GAP)
                    continue;
            }
        }
        else
        {
            if(gsspTables.at(i).rOrF.contains("F"))
            {
                if(se - point < MIN_GAP)
                    continue;
            }
            else
            {
                if(point - sb < MIN_GAP)
                    continue;
            }
        }
        const char *gsspSeq = gsspTables.at(i).base.data();
        bool flag2 = false;
        if(flag)
        {
            for(int j=0; j<strlen(gsspSeq); j++)
            {
                if(seq11[point+j] != gsspSeq[j] || seq21[point+j] != gsspSeq[j])
                {
                    flag2 = true;
                    break;
                }
            }
            if(flag2)
            {
                for(int j=0; j<strlen(gsspSeq); j++)
                {
                    if(seq12[point+j] != gsspSeq[j] || seq22[point+j] != gsspSeq[j])
                    {
                        flag2 = true;
                        break;
                    }
                }
            }
        }
        else
        {
            for(int j=0; j<strlen(gsspSeq); j++)
            {
                if(seq11[point+j] != gsspSeq[j] || seq22[point+j] != gsspSeq[j])
                {
                    flag2 = true;
                    break;
                }
            }
            if(flag2)
            {
                for(int j=0; j<strlen(gsspSeq); j++)
                {
                    if(seq12[point+j] != gsspSeq[j] || seq21[point+j] != gsspSeq[j])
                    {
                        flag2 = true;
                        break;
                    }
                }
            }

        }
        if(flag2)
            continue;
        QString s11,s12,s21,s22;
        for(int j=0; j<strlen(gsspSeq); j++)
        {
            s11.push_back(seq11[point+j]);
            s12.push_back(seq12[point+j]);
            s21.push_back(seq21[point+j]);
            s22.push_back(seq22[point+j]);
        }
        //do something
        gssps.push_back(gsspTables.at(i).gsspName);
        infos.push_back(QString("%1:%2:%3:%4:%5:%6:%7:%8:%9").arg(s11).arg(s12).arg(s21).arg(s22)
                        .arg(point)
                        .arg(gsspTables.at(i).exonIndex).arg(QString(gsspTables.at(i).rOrF))
                        .arg(QString(gsspTables.at(i).gsspName))
                        .arg(QString(gsspTables.at(i).base)));
    }
}

bool processGssp(const QVector<AllelePair> &allelePairs, const QString &sampleName, QVector<QStringList> &result)
{
    if(allelePairs.size()<2)
        return false;
    QMap<QString, QByteArray> seqMap;
    for(int i=0; i<allelePairs.size(); i++)
    {
        if(seqMap.find(allelePairs.at(i).allele1) == seqMap.end())
        {
            QByteArray seq;
            getAlleleSequence(allelePairs.at(i).allele1, seq);
            seqMap.insert(allelePairs.at(i).allele1, seq);
        }
        if(seqMap.find(allelePairs.at(i).allele2) == seqMap.end())
        {
            QByteArray seq;
            getAlleleSequence(allelePairs.at(i).allele2, seq);
            seqMap.insert(allelePairs.at(i).allele2, seq);
        }
    }

    int exonStart, exonEnd;
    QByteArray geneName;
    getExonIndexAndGeneBySampleName(sampleName.toAscii(), exonStart, exonEnd, geneName);

    QVector<GsspTable> gsspTable2;
    QVector<GsspTable> gsspTable3;
    if(exonStart<=2 && exonEnd >=2)
    {
        getGsspTablesFromGsspDatabase(geneName, 2, gsspTable2);
    }
    if(exonStart<=3 && exonEnd >=3)
    {
        getGsspTablesFromGsspDatabase(geneName, 3, gsspTable3);
    }
    QVector<int> exonIndex;
    getExonPositionIndexFromStaticDatabase(geneName, exonIndex);
    for(int i=0; i<allelePairs.size(); i++)
    {
        const char *seq11 = (seqMap.find(allelePairs.at(i).allele1).value()).data();
        const char *seq12 = (seqMap.find(allelePairs.at(i).allele2).value()).data();
        for(int j=i+1; j<allelePairs.size(); j++)
        {
            QStringList gssps, infos;
            const char *seq21 = (seqMap.find(allelePairs.at(j).allele1).value()).data();
            const char *seq22 = (seqMap.find(allelePairs.at(j).allele2).value()).data();

            if(gsspTable2.size()>0)
            {
                findUsefulGssp(seq11, seq12, seq21, seq22,
                               exonIndex.at(1),exonIndex.at(2), gsspTable2, gssps, infos
                               );
            }
            if(gsspTable3.size()>0)
            {
                findUsefulGssp(seq11, seq12, seq21, seq22,
                               exonIndex.at(2),exonIndex.at(3), gsspTable3, gssps, infos
                               );
            }

            if(gssps.size()==0)
                continue;
            QStringList re;
            re.push_back(allelePairs.at(i).allele1);
            re.push_back(allelePairs.at(i).allele2);
            re.push_back(allelePairs.at(j).allele1);
            re.push_back(allelePairs.at(j).allele2);
            re.push_back(gssps.join(":"));
            re.push_back(infos.join(";"));
            result.push_back(re);
        }
    }
    if(result.size()==0)
        return false;
    return true;
}
