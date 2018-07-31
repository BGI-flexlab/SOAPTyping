#include "basealigntable.h"
#include <QtGui>
#include <QtCore>
#include "core/key_update.h"
#include "ui_basealigntablewidget.h"
//改动较大，细节
/***********************************************************
 *                 public fuctions                         *
 ***********************************************************/
BaseAlignTable::BaseAlignTable(QWidget *parent)
    : QTableWidget(parent)
    ,ui(new Ui::BaseAlignTableWidget) //新增
{
    ui->setupUi(this);
    Key_Update key;
    if(!key.AllIsRigth())
    {
        return ;
    }
    rowNum_ = 9;
    colNum_ = 1200;
    startColumn_ = 1;
    setBaseAlignTableDefault();
    connect(this, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotSentBasePos(QTableWidgetItem*)));
    //新增
    this->horizontalHeader()->setHighlightSections(false);
    connect(this->horizontalScrollBar(),SIGNAL(sliderPressed()),this,SLOT(slotSliderPressed()));
    connect(this->horizontalScrollBar(),SIGNAL(sliderReleased()),this,SLOT(slotSliderReleased()));
    connect(this->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(onSliderMoved(int)));
}

//新增
BaseAlignTable::~BaseAlignTable()
{
    delete ui;
}
/***********************************************************
 *                 private fuctions                        *
 ***********************************************************/
void BaseAlignTable::setBaseAlignTableDefault()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectColumns);
    this->verticalHeader()->setVisible(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setStyleSheet("selection-background-color:#3CBADA");
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalHeader()->setDefaultSectionSize(20); //设置行高
    this->horizontalHeader()->setFixedHeight(20); //设置表头的高度
    this->setRowCount(rowNum_);
    this->setColumnCount(colNum_);
    QFont f("微软雅黑");
    f.setBold(true);

    this->setFont(f);
    //    for(int i=0; i<rowNum_; i++)
    //    {
    //        this->setRowHeight(i,15);
    //    }
    for(int i=0; i<colNum_; i++)
    {
        this->setColumnWidth(i,20);
        defaultHead_<<"";
    }

    this->setColumnWidth(0,230);
    this->setHorizontalHeaderLabels(defaultHead_);

    ui->m_MarkTableWidget->hide();
    ui->m_MarkTableWidget->setMinimumWidth(230);
    ui->m_MarkTableWidget->setMaximumWidth(230);
    ui->m_MarkTableWidget->setRowCount(rowNum_);
    ui->m_MarkTableWidget->setColumnCount(1);
    ui->m_MarkTableWidget->setStyleSheet("selection-background-color:#3CBADA");
    ui->m_MarkTableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->m_MarkTableWidget->verticalScrollBar()->setHidden(true);//设置垂直滚动条不可见
    ui->m_MarkTableWidget->horizontalHeader()->setFixedHeight(20); //设置表头的高度
    ui->m_MarkTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//使列完全填充并平分
    ui->m_MarkTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->m_MarkTableWidget->setHorizontalHeaderLabels(QStringList(""));
    ui->m_MarkTableWidget->setColumnWidth(0,230);
    ui->m_MarkTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QTableWidgetItem *markArray = new QTableWidgetItem[rowNum_*1];
    for(int i = 0;i < rowNum_;i++)
    {
        ui->m_MarkTableWidget->setRowHeight(i,20);
        for(int j = 0;j < 1;j++)
        {
            markArray->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            ui->m_MarkTableWidget->setItem(i,j,markArray++);
        }
    }

    ui->m_MarkTableWidget->item(0, 0)->setTextColor(QColor(0, 169,157));
    ui->m_MarkTableWidget->item(1, 0)->setTextColor(QColor(140,98,57));
    ui->m_MarkTableWidget->item(2, 0)->setTextColor(QColor(0,113,188));
    ui->m_MarkTableWidget->item(3, 0)->setTextColor(QColor(247,147,30));
    ui->m_MarkTableWidget->item(4, 0)->setTextColor(QColor(102,45,145));
    ui->m_MarkTableWidget->item(5, 0)->setTextColor(QColor(57,181,74));
    ui->m_MarkTableWidget->item(6, 0)->setTextColor(QColor(237,30,121));

    QFont font1;
    font1.setBold(true);
    font1.setPointSize(12);
    for(int i=0; i<9; i++)
    {
        ui->m_MarkTableWidget->item(i, 0)->setFont(font1);
    }

    ui->m_MarkTableWidget->item(0, 0)->setText("  Consensus Sequence");
    ui->m_MarkTableWidget->item(1, 0)->setText("  Forward Sequence");
    ui->m_MarkTableWidget->item(2, 0)->setText("  Reverse Sequence");
    ui->m_MarkTableWidget->item(3, 0)->setText("  GSSP Sequence");
    ui->m_MarkTableWidget->item(4, 0)->setText("  Consensus Alignment");
    ui->m_MarkTableWidget->item(5, 0)->setText("  Pattern Sequence");

    QTableWidgetItem *itemArray = new QTableWidgetItem[rowNum_ * colNum_];
    for(int i=0; i<rowNum_; i++)
    {
        for(int j=0; j<colNum_; j++)
        {

            if(j == 0)
            {
                itemArray->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            }
            else {
                itemArray->setTextAlignment(Qt::AlignCenter);
            }
            this->setItem(i, j, itemArray++);
        }
    }
    for(int j=1; j<colNum_; j++)
    {
        this->item(0, j)->setTextColor(QColor(0, 169,157));
        this->item(1, j)->setTextColor(QColor(140,98,57));
        this->item(2, j)->setTextColor(QColor(0,113,188));
        this->item(3, j)->setTextColor(QColor(247,147,30));
        this->item(4, j)->setTextColor(QColor(102,45,145));
        this->item(5, j)->setTextColor(QColor(57,181,74));
        this->item(6, j)->setTextColor(QColor(237,30,121));
    }

    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    for(int i=0; i<9; i++)
    {
        item(i, 0)->setFont(font);
    }

    this->item(0,0)->setTextColor(QColor(0, 169,157));
    this->item(1,0)->setTextColor(QColor(140,98,57));
    this->item(2,0)->setTextColor(QColor(0,113,188));
    this->item(3,0)->setTextColor(QColor(247,147,30));
    this->item(4,0)->setTextColor(QColor(102,45,145));
    this->item(5,0)->setTextColor(QColor(57,181,74));
    this->item(6,0)->setTextColor(QColor(237,30,121));

    //    this->item(0, 0)->setTextColor(QColor(142, 171, 189));
    //    this->item(3, 0)->setTextColor(QColor(196, 152, 51));
    //    this->item(4, 0)->setTextColor(QColor(138, 87, 58));
    //    this->item(5, 0)->setTextColor(QColor(139, 136, 119));
    //    this->item(6, 0)->setTextColor(QColor(67, 118, 119));

    for(int j=1; j<colNum_; j++)
    {
        this->item(0, j)->setBackgroundColor(QColor(0, 169,157,51));
        this->item(1, j)->setBackgroundColor(QColor(140,98,57,51));
        this->item(2, j)->setBackgroundColor(QColor(0,113,188,51));
        this->item(3, j)->setBackgroundColor(QColor(247,147,30,51));
        this->item(4, j)->setBackgroundColor(QColor(102,45,145,51));
        this->item(5, j)->setBackgroundColor(QColor(57,181,74,51));
        this->item(6, j)->setBackgroundColor(QColor(237,30,121,51));
        this->item(7, j)->setBackgroundColor(Qt::white);
        this->item(8, j)->setBackgroundColor(Qt::white);
    }
    this->item(0, 0)->setText("  Consensus Sequence");
    this->item(1, 0)->setText("  Forward Sequence");
    this->item(2, 0)->setText("  Reverse Sequence");
    this->item(3, 0)->setText("  GSSP Sequence");
    this->item(4, 0)->setText("  Consensus Alignment");
    this->item(5, 0)->setText("  Pattern Sequence");
}

void BaseAlignTable::setBaseAlignTableSampleData()
{
    this->clearBaseAlignTableSampleItem();
    baseAlignSampleInfo_.clear();
    getBaseAlignSampleInfo(signalInfo_.sampleName, baseAlignSampleInfo_);
    startColumn_ = 1;
    endColumn_ = baseAlignSampleInfo_.alignEndPos - baseAlignSampleInfo_.alignStartPos + 1;
    if(baseAlignSampleInfo_.alignStartPos == baseAlignSampleInfo_.alignEndPos)
        return;
    QStringList header;
    header<<signalInfo_.sampleName;
    getTableHead(header, baseAlignSampleInfo_.alignEndPos - baseAlignSampleInfo_.alignStartPos, baseAlignSampleInfo_.alignStartPos);
    header[0] = "  "+header[0];
    this->setHorizontalHeaderLabels(header);
    ui->m_MarkTableWidget->setHorizontalHeaderLabels(header);//hcj
    QString &line = baseAlignSampleInfo_.consensusSeq;
    for(int i=0; i<line.size(); i++)
    {
        this->item(0, i+startColumn_)->setText(line.at(i));
        this->item(4, i+startColumn_)->setText("-");
    }

    line = baseAlignSampleInfo_.forwardSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)=='-')
            continue;
        this->item(1, i+startColumn_)->setText(line.at(i));
    }
    line = baseAlignSampleInfo_.reverseSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)=='-')
            continue;
        this->item(2, i+startColumn_)->setText(line.at(i));
    }
    line = baseAlignSampleInfo_.patternSeq;
    for(int i=0; i<line.size(); i++)
    {
        if(line.at(i)=='-')
            continue;
        this->item(5, i+startColumn_)->setText(line.at(i));
    }

    for(QSet<int>::iterator it=baseAlignSampleInfo_.editPostion.begin(); it!=baseAlignSampleInfo_.editPostion.end(); it++)
    {
        char A = baseAlignSampleInfo_.forwardSeq[(*it)-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char B = baseAlignSampleInfo_.reverseSeq[(*it)-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char C = mergeBases(A, B);
        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setText(QString("%1").arg(C));
        //        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setBackgroundColor(QColor(39, 132, 68));
    }

    for(QSet<int>::iterator it=baseAlignSampleInfo_.frMisMatchPostion.begin(); it!=baseAlignSampleInfo_.frMisMatchPostion.end(); it++)
    {
        char A = baseAlignSampleInfo_.forwardSeq[(*it)-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char B = baseAlignSampleInfo_.reverseSeq[(*it)-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char C = mergeBases(A, B);
        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setText(QString("%1").arg(C));
        //        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setBackgroundColor(QColor(39, 113, 132));
    }

    for(QSet<int>::iterator it=baseAlignSampleInfo_.pcMisMatchPostion.begin(); it!=baseAlignSampleInfo_.pcMisMatchPostion.end(); it++)
    {
        char A = baseAlignSampleInfo_.forwardSeq[*it-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char B = baseAlignSampleInfo_.reverseSeq[*it-baseAlignSampleInfo_.alignStartPos].toLatin1();
        char C = mergeBases(A, B);
        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setText(QString("%1").arg(C));
        //        this->item(4, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setBackgroundColor(QColor(206, 23, 48));
    }

    for(QMap<QString, BaseAlignGsspInfo>::iterator it=baseAlignSampleInfo_.gsspInfoMap.begin(); it!=baseAlignSampleInfo_.gsspInfoMap.end(); it++)
    {
        BaseAlignGsspInfo &info = it.value();
        line = info.gsspFileSeq;
        for(int i=0; i<line.size(); i++)
        {
            if(line.at(i)=='-')
                continue;
            this->item(3, i+info.gsspFileAlignStartPos - baseAlignSampleInfo_.alignStartPos + startColumn_)->setText(line.at(i));
        }
        line = info.gsspSeq;
        for(int i=0; i<line.size(); i++)
        {
            this->item(3, i+info.gsspPostion - baseAlignSampleInfo_.alignStartPos + startColumn_)->setText(line.at(i));
        }
    }
    return;
}

void BaseAlignTable::setBaseAlignTableTypeData(int type)
{
    this->clearBaseAlignTableTypeItem();
    if(allele1Name_.isEmpty() && allele2Name_.isEmpty())
    {
        QSet<int> misMatch;
        emit signalTypeMisMatchPosition(misMatch, type);
        return;
    }
    QByteArray allele1Seq, allele2Seq, result;
    getAlleleSequence(allele1Name_, allele1Seq, baseAlignSampleInfo_.alignStartPos,baseAlignSampleInfo_.alignEndPos-baseAlignSampleInfo_.alignStartPos);
    if(allele1Name_ == allele2Name_)
    {
        allele2Seq = allele1Seq;
    }
    else
    {
        getAlleleSequence(allele2Name_, allele2Seq, baseAlignSampleInfo_.alignStartPos,baseAlignSampleInfo_.alignEndPos-baseAlignSampleInfo_.alignStartPos);
    }
    result.fill('-', allele1Seq.size());
    QSet<int> misMatch;
    if(signalInfo_.isGssp && !signalInfo_.isGsspFilter)
    {
        QMap<QString, BaseAlignGsspInfo>::iterator it = baseAlignSampleInfo_.gsspInfoMap.find(signalInfo_.fileName);
        if(it != baseAlignSampleInfo_.gsspInfoMap.end())
        {
            QByteArray gsspSeq = it.value().gsspFileSeq.toAscii();
            getTypeAlignResult(result.data(), gsspSeq.data(), allele1Seq.data(), allele2Seq.data(), misMatch, baseAlignSampleInfo_.alignStartPos, it.value().gsspFileAlignStartPos);
        }
    }
    else
    {
        QByteArray patternSeq = baseAlignSampleInfo_.patternSeq.toAscii();
        getTypeAlignResult(result.data(), patternSeq.data(), allele1Seq.data(), allele2Seq.data(), misMatch, baseAlignSampleInfo_.alignStartPos, baseAlignSampleInfo_.alignStartPos);
    }

    QString results = QString(result);
    QString allele1Seqs = QString(allele1Seq);
    QString allele2Seqs = QString(allele2Seq);
    for(int i=0; i<result.size(); i++)
    {
        this->item(6, i+startColumn_)->setText(results.at(i));
        this->item(7, i+startColumn_)->setText(allele1Seqs.at(i));
        this->item(8, i+startColumn_)->setText(allele2Seqs.at(i));
    }
    this->item(6, 0)->setText("  Type Result");
    this->item(7, 0)->setText("  "+allele1Name_);
    this->item(8, 0)->setText("  "+allele2Name_);
    ui->m_MarkTableWidget->item(6, 0)->setText("  Type Result");
    ui->m_MarkTableWidget->item(7, 0)->setText("  "+allele1Name_);
    ui->m_MarkTableWidget->item(8, 0)->setText("  "+allele2Name_);
    for(QSet<int>::iterator it=misMatch.begin(); it!=misMatch.end(); it++)
    {
        this->item(6, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setTextColor(QColor(255,255,255));
        this->item(6, (*it)-baseAlignSampleInfo_.alignStartPos+startColumn_)->setBackgroundColor(QColor(255,0,0));
    }
    emit signalTypeMisMatchPosition(misMatch, type);
}


void BaseAlignTable::clearBaseAlignTableSampleItem()
{
    this->setHorizontalHeaderLabels(defaultHead_);
    for(int i=1; i<colNum_; i++)
    {
        this->item(0, i)->setText("");
        this->item(1, i)->setText("");
        this->item(2, i)->setText("");
        this->item(3, i)->setText("");
        this->item(4, i)->setText("");
        this->item(5, i)->setText("");
        this->item(4, i)->setBackgroundColor(QColor(223, 223, 223));
    }
}

void BaseAlignTable::clearBaseAlignTableTypeItem()
{
    for(int i=1; i<colNum_; i++)
    {
        this->item(6, i)->setText("");
        this->item(7, i)->setText("");
        this->item(8, i)->setText("");
        this->item(6, i)->setBackgroundColor(QColor(223, 223, 223));
    }
    this->item(6,0)->setText("");
    this->item(7,0)->setText("");
    this->item(8,0)->setText("");
    ui->m_MarkTableWidget->item(6, 0)->setText("");
    ui->m_MarkTableWidget->item(7, 0)->setText("");
    ui->m_MarkTableWidget->item(8, 0)->setText("");
}


void BaseAlignTable::getTableHead(QStringList &head, int length, int start)
{
    bool startWithNull = false;
    start += 1;
    for(int i=0; i<length; i++)
    {
        int pos = i + start;
        int TPos = (pos+2) / 1000;
        int HPos = ((pos+1) % 1000) / 100;
        int DPos = (pos % 100) / 10;
        int Ipos = (pos-1) % 10;
        if(pos/100)
        {
            Ipos = (pos-2) % 10;
        }
        switch(Ipos)
        {
        case 1:
            if(startWithNull)
            {
                head<<QString("%1").arg(1);
            }
            else
            {
                head<<QString("");
                //                startWithNull = true;
            }
            break;
        case 0:
            if(startWithNull&&(HPos>0 || TPos >0 || DPos >0))
            {
                head<<QString("%1").arg(DPos);
            }
            else
            {
                //                startWithNull = true;
                head<<"";
            }
            break;
        case 9:
            if(startWithNull&&(TPos>0 || HPos>0))
            {
                head<<QString("%1").arg(HPos);
            }
            else
            {
                //                startWithNull = true;
                head<<"";
            }
            break;
        case 8:
            if(TPos>0&&startWithNull)
            {
                head<<QString("%1").arg(TPos);
            }
            else
            {
                //                startWithNull = true;
                head<<"";
            }
            break;
        default:
            startWithNull = true;
            head<<"";
        }
    }

    if(colNum_>length+1)
    {
        for(int i=length+1; i<colNum_; i++)
        {
            head<<"";
        }
    }
}

int BaseAlignTable::getExonNum(int basePos)
{
    int exon = 0;
    for(int i=0; i<exonPosition_.size(); i++)
    {
        if(basePos>=exonPosition_.at(i) && basePos<exonPosition_.at(i+1))
        {
            exon = i+1;
            break;
        }
    }
    return exon;
}

/***********************************************************
 *                 public slots fuctions                   *
 ***********************************************************/

/***********************************************************
 *                 private slots fuctions                  *
 ***********************************************************/
void BaseAlignTable::slotSentBasePos(QTableWidgetItem* item0)
{
    int nowPos = item0->column();
    if(nowPos>=startColumn_ && nowPos<endColumn_)
    {
        int basePos = nowPos - startColumn_ + baseAlignSampleInfo_.alignStartPos;
        int exon = getExonNum(basePos);
        //qDebug()<<basePos<<exon<<exonPosition_;
        int leftDistance = nowPos*20+220-this->horizontalScrollBar()->sliderPosition();//168
        emit signalFocusPosition(basePos, exon, leftDistance);
    }
}

void BaseAlignTable::slotSliderPressed()
{
    int pos = this->verticalScrollBar()->value();
    ui->m_MarkTableWidget->show();
    ui->m_MarkTableWidget->verticalScrollBar()->setValue(pos);
}

void BaseAlignTable::slotSliderReleased()
{
    ui->m_MarkTableWidget->hide();
}

void BaseAlignTable::onSliderMoved(int pos)
{
    ui->m_MarkTableWidget->verticalScrollBar()->setValue(pos);
}
void BaseAlignTable::slotFocusPosition(int basePosition, int exonIndex, int leftDistance)
{
    int columnPos = basePosition + startColumn_ - baseAlignSampleInfo_.alignStartPos;
    int sliderPos = columnPos*20+220-leftDistance;//168
    this->horizontalScrollBar()->setSliderPosition(sliderPos);
    this->selectColumn(columnPos);
}
void BaseAlignTable::slotAllelePairChanged(QString &allele1, QString &allele2)
{
    allele1Name_ = allele1;
    allele2Name_ = allele2;
    setBaseAlignTableTypeData(0);
}
void BaseAlignTable::slotFileChanged(SignalInfo &signalInfo, int type)
{
//20180608close qDebug("basealigntable slotFileChanged start");
    if(signalInfo_.geneName != signalInfo.geneName)
    {
        exonPosition_.clear();
        getExonPositionIndexFromStaticDatabase(signalInfo.geneName.toAscii(), exonPosition_);
    }
    if(signalInfo_.sampleName != signalInfo.sampleName)
    {
        signalInfo_ = signalInfo;
        allele1Name_.resize(0);
        allele2Name_.resize(0);
        setBaseAlignTableSampleData();
        setBaseAlignTableTypeData(0);
    }
    else
    {
        if(signalInfo_.isUndefined != signalInfo.isUndefined
                || signalInfo_.isGssp != signalInfo.isGssp
                || signalInfo_.isGsspFilter != signalInfo.isGsspFilter
                || signalInfo_.isCombined != signalInfo.isCombined)
            //               || signalInfo_.exonIndex != signalInfo.exonIndex) //这一条是可以省略的.
        {
            allele1Name_.resize(0);
            allele2Name_.resize(0);
            if(type==0)
            {
                setBaseAlignTableTypeData(0);
            }
        }
        signalInfo_ = signalInfo;
        if(type!=0)
        {
            setBaseAlignTableSampleData();
            setBaseAlignTableTypeData(type);
        }
    }
//20180608close qDebug("basealigntable slotFileChanged end");
}

void getTypeAlignResult(char *result, char *pattern, char *alleleSeq1, char *alleleSeq2, QSet<int> &misMatch, int alignStart1, int alignStart2)
{
    int size =strlen(pattern);
    int num = alignStart2 - alignStart1;
    for(int i=0; i<size; i++)
    {
        if(pattern[i]=='-')
            continue;
        if(alleleSeq1[i+num] == '*')
        {
            if(alleleSeq2[i+num]=='*')
                continue;
            else if (alleleSeq2[i+num] != pattern[i])
            {
                result[i+num]=alleleSeq2[i+num];
                misMatch.insert(i+alignStart2);
            }
        }
        else
        {
            if(alleleSeq2[i+num]=='*' || alleleSeq1[i+num]==alleleSeq2[i+num])
            {
                if(alleleSeq1[i+num] != pattern[i])
                {
                    result[i+num]=alleleSeq1[i+num];
                    misMatch.insert(i+alignStart2);
                }
            }
            else
            {
                char A = mergeBases(alleleSeq1[i+num],alleleSeq2[i+num]);
                if(A != pattern[i])
                {
                    result[i+num]=A;
                    misMatch.insert(i+alignStart2);
                }
            }
        }
    }
}
