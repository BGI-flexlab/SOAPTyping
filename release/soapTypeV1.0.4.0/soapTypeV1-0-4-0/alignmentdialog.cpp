#include "alignmentdialog.h"
#include "staticdatabase.h"
#include "staticdatabase.h"
AlignPaintWgt::AlignPaintWgt(QWidget *parent)
    :QWidget(parent)
{
//     setAttribute(Qt::WA_ShowModal, false);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    brushColor_ = true;
    showCodon_ = false;
    codonStart_ = 0;
    headerWidth_ = 150;
    fontWidth_ = 12;
    fontHeight_ = 16;
    lineBaseNumber_ = 1;
    rowsHeight_ = 1;
    baseNumber_ = 0;
    xx_ = NULL;
    yy_ = NULL;
    exon_ = NULL;
    exonRow_ = NULL;
}
AlignPaintWgt::~AlignPaintWgt()
{
    alleNames_.clear();
    alleSeqs_.clear();
    mis_.clear();
    posToExon_.clear();
    if(xx_ != NULL)
    {
        delete[] xx_;
        xx_ = NULL;
    }
    if(yy_ != NULL)
    {
        delete[] yy_;
        yy_ = NULL;
    }
    if(exon_ != NULL)
    {
        delete[] exon_;
        exon_ = NULL;
    }
    if(exonRow_ != NULL)
    {
        delete[] exonRow_;
        exonRow_ = NULL;
    }
}

QSize AlignPaintWgt::minimumSizeHint() const
{
    return QSize(800, 600);
}

void AlignPaintWgt::slotRepaint()
{
    getSize();
}

void AlignPaintWgt::paintEvent(QPaintEvent *e)
{
    if(alleNames_.size()<=1)
        return;
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    int fileNum = alleNames_.size();

    for(int i=0; i<rowNum_; i++)
    {
        for(int j=0; j<fileNum; j++)
        {
            painter.drawText(QRect(5, i*rowsHeight_+(j+1)*fontHeight_,headerWidth_, fontHeight_), alleNames_.at(j));
        }
    }


    painter.setPen(Qt::blue);
    if(!showCodon_)
    {
        for(int i=0; i<baseNumber_; i+=10)
        {
            if(i ==0)
            {
                continue;
            }
            else if(i%lineBaseNumber_ == 0)
            {
                painter.drawText(QRect(xx_[i - 1], yy_[i - 1]-fontHeight_, 35, fontHeight_), QString::number(i));
            }
            else
            {
                painter.drawText(QRect(xx_[i -1]/*- fontWidth_*/, yy_[i]-fontHeight_, 35, fontHeight_), QString::number(i));
            }
        }
    }
    else
    {
        int j=codonStart_-1;
        for(int i=0; i<baseNumber_; i+=3)
        {
            j++;
            if(j==0)
            {
                j=1;
                painter.drawText(QRect(xx_[i], yy_[i]-fontHeight_, 35, fontHeight_), QString::number(j));
                continue;
            }
            if(j%5==0)
            {
                painter.drawText(QRect(xx_[i], yy_[i]-fontHeight_, 35, fontHeight_), QString::number(j));
            }
        }
    }

    //exon
    int num=0;
    painter.setPen(Qt::yellow);
    painter.setBrush(Qt::darkGray);
    for(QMap<int, int>::iterator it=posToExon_.begin(); it!=posToExon_.end();it++)
    {
        painter.drawRect(QRect(xx_[it.key()]-4, yy_[it.key()]+fileNum*fontHeight_, fontWidth_, fontHeight_));
        painter.drawText(QRect(xx_[it.key()], yy_[it.key()]+fileNum*fontHeight_, fontWidth_, fontHeight_), QString::number(it.value()));
    }

    painter.setPen(Qt::NoPen);
    int lineW1 = lineBaseNumber_ * 4 /3 *fontWidth_;
    int lineW2 = lineW1 + headerWidth_;
    int rowH = fileNum*fontHeight_;
    for(num=0; num<exonNum_;num++)
    {
        if(num%2==0)
        {
            painter.setBrush(QColor(169,208,245));
        }
        else
        {
            painter.setBrush(QColor(189,189,224));
        }
        painter.drawRect(QRect(xx_[exon_[num]]-4, yy_[exon_[num]], lineW2-xx_[exon_[num]], rowH));
        for(int i= exonRow_[num]+1 ; i<=exonRow_[num+1]; i++)
        {
            painter.drawRect(QRect(headerWidth_-4, i*rowsHeight_+fontHeight_, lineW1, rowH ));
        }
    }

    int h;
    painter.setBrush(Qt::red);
    for(int i=0; i<fileNum; i++)
    {
        const QVector<int> &mis = mis_.at(i);
        h=i*fontHeight_;
        for(int j=0; j<mis.size(); j++)
        {
            painter.drawRect(QRect(xx_[mis.at(j)]-4, yy_[mis.at(j)]+h, fontWidth_, fontHeight_));
        }
    }


    painter.setPen(Qt::black);
    for(int i=0; i<fileNum; i++)
    {
        const QString &allele=alleSeqs_.at(i);
        h=i*fontHeight_;
        for(int j=0; j<baseNumber_;j++)
        {
            painter.drawText(QRect(xx_[j], yy_[j]+h,fontWidth_, fontHeight_), allele.at(j));
        }
    }
}

void AlignPaintWgt::setArray()
{
    if(xx_ != NULL)
    {
        delete[] xx_;
        xx_ = NULL;
    }
    if(yy_ != NULL)
    {
        delete[] yy_;
        yy_ = NULL;
    }
    if(exon_ != NULL)
    {
        delete[] exon_;
        exon_ = NULL;
    }
    if(exonRow_ != NULL)
    {
        delete[] exonRow_;
        exonRow_ = NULL;
    }
    if(alleNames_.size()<=1)
        return;
    xx_ = new int[baseNumber_];
    yy_ = new int[baseNumber_];
    int z;
    for(int i=0; i<baseNumber_; i++)
    {
        yy_[i]=(i/lineBaseNumber_)*rowsHeight_+fontHeight_;
        z=i%lineBaseNumber_;
        xx_[i]=headerWidth_+(z/3*4+z%3)*fontWidth_;
    }
    exonNum_ = posToExon_.size();
    exon_ = new int[exonNum_];
    exonRow_ = new int[exonNum_+1];
    int num=0;
    for(QMap<int, int>::iterator it=posToExon_.begin(); it!=posToExon_.end();it++)
    {
        exon_[num]=it.key();
        exonRow_[num++]=it.key()/lineBaseNumber_;
    }
    exonRow_[num]=rowNum_;
}

void AlignPaintWgt::getSize()
{
    QSize pSize = parentWidget()->size();
    if(alleNames_.size()<=1)
    {
        resize(pSize);
        return;
    }
    int w = pSize.width();
//    lineBaseNumber_ = (w-headerWidth_)/4/fontWidth_*3;
    lineBaseNumber_ = (w-headerWidth_)/fontWidth_/4*3;
//    lineBaseNumber_ = 30;
    rowsHeight_ = (2+alleNames_.size())*fontHeight_;
    if(baseNumber_%lineBaseNumber_ > 0)
    {
        rowNum_ = baseNumber_/lineBaseNumber_+1;
    }
    else
    {
        rowNum_ = baseNumber_/lineBaseNumber_;
    }

    int h = rowNum_*rowsHeight_+10;

    setArray();

    if(h>pSize.height())
        pSize.setHeight(h);
    resize(pSize);
}

void AlignPaintWgt::setShowCodon(bool show)
{
    showCodon_ = show;
    repaint();
}
void AlignPaintWgt::setCodonStart(int pos)
{
    codonStart_ = pos;
}

void AlignPaintWgt::setExonPositionInfo(QVector<int> &exonStartPosIndex)
{
    alleNames_.clear();
    alleSeqs_.clear();
    mis_.clear();
    posToExon_.clear();
    repaint();
    for(int i=0; i<exonStartPosIndex.size()-1; i++)
    {
        posToExon_.insert(exonStartPosIndex.at(i), i+1);
    }
    baseNumber_ = exonStartPosIndex.last();
}

void AlignPaintWgt::setFirstPair(const AlleleNameSeqPair &pair)
{
    firstPair_ = pair;
}

void AlignPaintWgt::setSelectAlleles(const QMap<int, AlleleNameSeqPair> &mapPair)
{
    alleNames_.clear();
    alleSeqs_.clear();
    mis_.clear();
    if(mapPair.size()<1)
    {
        getSize();
        return;
    }
    alleNames_.push_back(firstPair_.name);
    alleSeqs_.push_back(firstPair_.seq);
    QVector<int> m;
    mis_.push_back(m);
    for(QMap<int, AlleleNameSeqPair>::const_iterator it=mapPair.begin(); it!=mapPair.end();it++)
    {
        alleNames_.push_back(it.value().name);
        alleSeqs_.push_back(it.value().seq);
        mis_.push_back(it.value().mis);
    }
    getSize();
}







AlignmentDialog::AlignmentDialog(QWidget *parent)
    :QDialog(parent)
{
    setAttribute(Qt::WA_ShowModal, false);
    setWindowFlags(Qt::WindowMinMaxButtonsHint);

    QString windowTitle;
    QString versionname;
    getVersionNames(versionname);
    windowTitle = QString("Soap Typing Allele Alignment (Version %1)").arg(versionname);
    setWindowTitle(windowTitle);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QSplitter *hSplitter = new QSplitter(Qt::Horizontal, this);
    label_ = new QLabel(this);
    label_->setText("Genes:");

    geneBox_ = new QComboBox(this);
    geneBox_->setMinimumWidth(60);
    setGeneBoxData();

    showCodonBox_ = new QCheckBox("Show Codon");
//    showCodonBox_->setEnabled(false);

    clearAllButton_ = new QPushButton("Clear All",this);
    showInfoButton_ = new QPushButton("Info", this);
    exitButton_ = new QPushButton("Exit", this);

    QSpacerItem *spacer = new QSpacerItem(10, 50,QSizePolicy::Expanding, QSizePolicy::Maximum);
    hLayout1->addWidget(label_);
    hLayout1->addWidget(geneBox_);
    hLayout1->addWidget(showCodonBox_);
    hLayout1->addWidget(clearAllButton_);
    hLayout1->addWidget(showInfoButton_);
    hLayout1->addWidget(exitButton_);
    hLayout1->addSpacerItem(spacer);

    listWidget_ = new ListWgt(this);
    listWidget_->setMaximumWidth(130);
    listWidget_->setMinimumWidth(130);
    listWidget_->setSelectionMode(QAbstractItemView::MultiSelection);
//    listWidget_->setStyleSheet("selection-background-color:#FABEF1");

    QScrollArea *scrollArea = new QScrollArea(this);
    alignPaintWdg_ = new AlignPaintWgt(scrollArea);
    scrollArea->setWidget(alignPaintWdg_);
    scrollArea->setBackgroundRole(QPalette::Base);
    scrollArea->setAutoFillBackground(true);
    hSplitter->addWidget(listWidget_);
    hSplitter->addWidget(scrollArea);

    vLayout->addLayout(hLayout1);
    vLayout->addWidget(hSplitter);
    connect(this, SIGNAL(signalResize()), alignPaintWdg_, SLOT(slotRepaint()));
    connect(geneBox_, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotGeneBoxItemChanged(QString)));
    connect(showInfoButton_, SIGNAL(clicked()), this, SLOT(slotClickShowInfoButton()));
    connect(listWidget_, SIGNAL(itemSelectionChanged()), this, SLOT(slotListSelectionChanged()));
    connect(clearAllButton_, SIGNAL(clicked()), this, SLOT(slotClickClearAllButton()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(close()));
    connect(showCodonBox_, SIGNAL(clicked(bool)), this, SLOT(slotClickShowCodonBox(bool)));

    //����ô���� ...
    codenMap_.insert("A", -24);
    codenMap_.insert("B", -24);
    codenMap_.insert("C", -24);
    codenMap_.insert("DQB1", -32);

    codenMap_.insert("DRB1", -29);
    codenMap_.insert("DRB3", -29);
    codenMap_.insert("DRB4", -29);
    codenMap_.insert("DRB5", -29);

    codenMap_.insert("DQA1", -23);
    codenMap_.insert("DPB1", -29);
    codenMap_.insert("DPA1", -29);
    codenMap_.insert("G", -24);

    resize(800, 600);
//    this->setMinimumSize(820,610);
//    this->setMaximumSize(820,610);
}
void AlignmentDialog::setGeneBoxData()
{
    QStringList geneNames;
    getGeneNames(geneNames);
    geneBox_->addItems(geneNames);
    geneBox_->setCurrentIndex(-1);
}
void AlignmentDialog::slotGeneBoxItemChanged(QString gene)
{
    geneInfo_.clear();
    exonStartPosIndex_.clear();
    alleleNames_.clear();
    alleleSeqs_.clear();
    misPositions_.clear();
    pairMap_.clear();
    getExonPositionIndexFromStaticDatabase(gene.toAscii(), exonStartPosIndex_);
    getAlleleNamesAndSeqsByGeneName(gene.toAscii(), alleleNames_, alleleSeqs_, misPositions_);
    int i=0;
    for(i=0; i<exonStartPosIndex_.size()-1;i++)
    {
        geneInfo_.append(QString("Exon %1 at: %2\n").arg(i+1).arg(exonStartPosIndex_.at(i)));
    }
    geneInfo_.prepend(QString("Sequence Length: %1\n").arg(exonStartPosIndex_.at(i)));
    geneInfo_.prepend(QString("Alleles: %1\n").arg(alleleNames_.size()));
    geneInfo_.prepend(QString("Gene: %1\n").arg(gene));

    firstPair_.name = alleleNames_.at(0);
    getAlleleSequenceByAlleleName(firstPair_.name.toAscii(), firstPair_.seq);

    listWidget_->clear();
    listWidget_->addItems(alleleNames_);

    alignPaintWdg_->setExonPositionInfo(exonStartPosIndex_);
    alignPaintWdg_->setFirstPair(firstPair_);
    if(codenMap_.contains(gene))
    {
        alignPaintWdg_->setCodonStart(codenMap_.find(gene).value());
    }
    else
    {
        alignPaintWdg_->setCodonStart(0);
    }
}

void AlignmentDialog::slotClickShowInfoButton()
{
    if(geneInfo_.isEmpty())
    {
        QMessageBox::warning(this, "Soap Typing", "Please select a gene first!");
    }
    else
    {
        QMessageBox::warning(this, "Soap Typing", geneInfo_);
    }
}
void AlignmentDialog::slotListSelectionChanged()
{
    int i=listWidget_->currentRow();
    if(listWidget_->item(i)->isSelected())
    {
        AlleleNameSeqPair pair;
        pair.name = alleleNames_.at(i);
        pair.seq = alleleSeqs_.at(i);
        pair.mis = misPositions_.at(i);
        pairMap_.insert(i, pair);
    }
    else
    {
        pairMap_.remove(i);
    }
    alignPaintWdg_->setSelectAlleles(pairMap_);
}

void AlignmentDialog::slotClickClearAllButton()
{
    pairMap_.clear();
    listWidget_->clearSelection();
}
void AlignmentDialog::slotClickShowCodonBox(bool s)
{
    alignPaintWdg_->setShowCodon(s);
}

void AlignmentDialog::slotGetGeneName(QString gene)
{
    int index = geneBox_->findText(gene);
    if(index == -1)
    {
        return;
    }
    geneBox_->setCurrentIndex(index);
}

void AlignmentDialog::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    emit signalResize();
}
