#include "alignmentdlg.h"
#include "ui_alignmentdlg.h"
#include "DataBase/soaptypingdb.h"
#include <QMessageBox>
#include <QPainter>
#include <QtDebug>

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
    fontHeight_ = 16; //改fontHeight_ = 12;
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
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    if(alleNames_.size()<=1)
        return;
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
            //改painter.drawText(QRect(xx_[i], yy_[i]-fontHeight_, 35, fontHeight_), QString::number(i+1));
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
        if(allele.isEmpty())
        {
            qDebug()<<"error"<<firstPair_.name;
            continue;
        }
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



AlignmentDlg::AlignmentDlg(QWidget *parent,const QString &strver,const QString &strgene) :
    QDialog(parent),
    ui(new Ui::AlignmentDlg),
    m_str_genever(strver),
    m_str_selgene(strgene)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint|Qt::WindowMaximizeButtonHint);
    InitUI();
    ConnectSignalandSlot();
    setGeneBoxData();
}

AlignmentDlg::~AlignmentDlg()
{
    delete ui;
    if(m_pAlignPaintWgt)
    {
        delete m_pAlignPaintWgt;
        m_pAlignPaintWgt = nullptr;
    }
}

void AlignmentDlg::InitUI()
{
    QString windowTitle = QString("Soap Typing Allele Alignment (Version %1)").arg(m_str_genever);
    setWindowTitle(windowTitle);

    m_pAlignPaintWgt = new AlignPaintWgt();
    ui->scrollArea->setWidget(m_pAlignPaintWgt);
    //m_pAlignPaintWgt->setMinimumSize(QSize(603,558));

    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    m_map_coden.insert("A", -24);
    m_map_coden.insert("B", -24);
    m_map_coden.insert("C", -24);
    m_map_coden.insert("DQB1", -32);

    m_map_coden.insert("DRB1", -29);
    m_map_coden.insert("DRB3", -29);
    m_map_coden.insert("DRB4", -29);
    m_map_coden.insert("DRB5", -29);

    m_map_coden.insert("DQA1", -23);
    m_map_coden.insert("DPB1", -29);
    m_map_coden.insert("DPA1", -29);
    m_map_coden.insert("G", -24);
}

void AlignmentDlg::ConnectSignalandSlot()
{
    connect(ui->comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this, &AlignmentDlg::slotGeneBoxItemChanged);
    connect(ui->btnInfo, &QPushButton::clicked, this, &AlignmentDlg::slotClickShowInfoButton);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &AlignmentDlg::slotListSelectionChanged);
    connect(ui->btnClear, &QPushButton::clicked, this, &AlignmentDlg::slotClickClearAllButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &AlignmentDlg::close);
    connect(ui->checkBox, &QCheckBox::clicked, this, &AlignmentDlg::slotClickShowCodonBox);
}

void AlignmentDlg::setGeneBoxData()
{
    QStringList geneNames;
    SoapTypingDB::GetInstance()->getGeneNames(geneNames);
    ui->comboBox->addItems(geneNames);
    if(m_str_selgene.isEmpty())
    {
        ui->comboBox->setCurrentIndex(0);
    }
    else
    {
        int index = ui->comboBox->findText(m_str_selgene);
        ui->comboBox->setCurrentIndex(index);
    }
}

void AlignmentDlg::slotGeneBoxItemChanged(const QString &gene)
{
    m_map_pair.clear();
    m_vec_exon.clear();
    m_strlist_alleleNames.clear();
    m_strlist_alleleSeqs.clear();
    m_vec_misPositions.clear();
    SoapTypingDB::GetInstance()->getExonPositionIndexFromStaticDatabase(gene, m_vec_exon);
    SoapTypingDB::GetInstance()->getAlleleNamesAndSeqsByGeneName(gene, m_strlist_alleleNames,
                                                                 m_strlist_alleleSeqs, m_vec_misPositions);
    m_str_geneInfo.clear();
    m_str_geneInfo.append(QString("Gene: %1\n").arg(gene));
    m_str_geneInfo.prepend(QString("Alleles: %1\n").arg(m_strlist_alleleNames.size()));
    m_str_geneInfo.prepend(QString("Sequence Length: %1\n").arg(m_vec_exon.last()));
    for(int i=0;i<m_vec_exon.size();i++)
    {
        m_str_geneInfo.append(QString("Exon %1 at: %2\n").arg(i+1).arg(m_vec_exon[i]+1));

    }

    AlleleNameSeqPair firstPair_;
    firstPair_.name = m_strlist_alleleNames.at(0);
    SoapTypingDB::GetInstance()->getAlleleSequenceByAlleleName(firstPair_.name, firstPair_.seq);

    ui->listWidget->clear();
    ui->listWidget->addItems(m_strlist_alleleNames);

    m_pAlignPaintWgt->setExonPositionInfo(m_vec_exon);
    m_pAlignPaintWgt->setFirstPair(firstPair_);
    if(m_map_coden.contains(gene))
    {
        m_pAlignPaintWgt->setCodonStart(m_map_coden.find(gene).value());
    }
    else
    {
        m_pAlignPaintWgt->setCodonStart(0);
    }
}

void AlignmentDlg::slotClickShowInfoButton()
{
    if(m_str_geneInfo.isEmpty())
    {
        QMessageBox::warning(this, "Soap Typing", "Please select a gene first!");
    }
    else
    {
        QMessageBox::warning(this, "Soap Typing", m_str_geneInfo);
    }
}

void AlignmentDlg::slotListSelectionChanged()
{
    int i=ui->listWidget->currentRow();
    if(ui->listWidget->item(i)->isSelected())
    {
        AlleleNameSeqPair pair;
        pair.name = m_strlist_alleleNames.at(i);
        pair.seq = m_strlist_alleleSeqs.at(i);
        pair.mis = m_vec_misPositions.at(i);
        m_map_pair.insert(i, pair);
    }
    else
    {
        m_map_pair.remove(i);
    }
    m_pAlignPaintWgt->setSelectAlleles(m_map_pair);
    m_pAlignPaintWgt->update();
}

void AlignmentDlg::slotClickClearAllButton()
{
    m_map_pair.clear();
    ui->listWidget->clearSelection();
    m_pAlignPaintWgt->setSelectAlleles(m_map_pair);
    m_pAlignPaintWgt->update();
}

void AlignmentDlg::slotClickShowCodonBox(bool status)
{
    m_pAlignPaintWgt->setShowCodon(status);
}

void AlignmentDlg::resizeEvent(QResizeEvent *e)
{
    m_pAlignPaintWgt->getSize();
}
