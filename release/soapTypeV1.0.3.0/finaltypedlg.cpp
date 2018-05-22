#include "finaltypedlg.h"
#include "realtimedatabase.h"
FinalTypeDlg::FinalTypeDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Set Final Type"));
    setFinalTypeDlgDefault();
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QVBoxLayout *vLayout1 = new QVBoxLayout;
    QVBoxLayout *vLayout2 = new QVBoxLayout;
    QVBoxLayout *vLayout3 = new QVBoxLayout;


    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QHBoxLayout *hLayout4 = new QHBoxLayout;
    QHBoxLayout *hLayout5 = new QHBoxLayout;
    QHBoxLayout *hLayout6 = new QHBoxLayout;

    QLabel *sampleLabel = new QLabel("Sample Name:", this);
    QLabel *allele1Label = new QLabel("Allele 1:",this);
    QLabel *allele2Label = new QLabel("Allele 2:",this);

    QSpacerItem *spacer1 = new QSpacerItem(50, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *spacer2 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QTextEdit *info = new QTextEdit(this);
    info->setText("Select an allele pair.\nAnd use '->' button to set\nfinal type.");
    info->setEnabled(false);

    hLayout1->addWidget(sampleLabel);
    hLayout1->addWidget(sampleNameLine_);

    vLayout1->addLayout(hLayout1);
    vLayout1->addWidget(table_);

    hLayout2->addWidget(allele1Label);
    hLayout2->addWidget(allele1Line_);

    hLayout3->addWidget(allele2Label);
    hLayout3->addWidget(allele2Line_);

    vLayout2->addLayout(hLayout2);
    vLayout2->addLayout(hLayout3);

    hLayout4->addWidget(setButton_);
    hLayout4->addLayout(vLayout2);

    hLayout5->addSpacerItem(spacer2);
    hLayout5->addWidget(saveButton_);

    hLayout6->addSpacerItem(spacer3);
    hLayout6->addWidget(exitButton_);

    vLayout3->addWidget(info);
    vLayout3->addLayout(hLayout4);
    vLayout3->addSpacerItem(spacer1);
    vLayout3->addLayout(hLayout5);
    vLayout3->addLayout(hLayout6);

    hLayout->addLayout(vLayout1);
    hLayout->addLayout(vLayout3);
    resize(550,300);
    connect(setButton_, SIGNAL(clicked()), this, SLOT(slotClickSetButton()));
    connect(table_, SIGNAL(itemSelectionChanged()), this, SLOT(slotRowChanged()));
    connect(saveButton_,SIGNAL(clicked()), this, SLOT(slotClickSaveButton()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(close()));
}
void FinalTypeDlg::setSampleName(const QString &sampleName)
{
    sampleName_ = sampleName;
    sampleNameLine_->setText(sampleName);
    QString resultString = getSetResultBySampleName(sampleName.toAscii());
    if(resultString.isEmpty())
        return;
    QStringList sp = resultString.split(",", QString::SkipEmptyParts);
    alleleName1_ = sp.at(0);
    alleleName2_ = sp.at(1);
    slotClickSetButton();
}

void FinalTypeDlg::setAlleleResult(const QVector<QStringList> &result)
{
    int size = result.size();
    if (size <= 0)
        return;
    table_->setRowCount(size);

    for(int i=0; i<size; i++)
    {
        table_->setRowHeight(i, 18);
        for(int j=0; j<3; j++)
        {
            QTableWidgetItem *item =  new QTableWidgetItem;
            item->setText(result.at(i).at(j));
            table_->setItem(i, j, item);
        }
    }
}

void FinalTypeDlg::setFinalTypeDlgDefault()
{
    sampleNameLine_ = new QLineEdit(this);
    sampleNameLine_->setEnabled(false);

    allele1Line_ = new QLineEdit(this);
    allele2Line_ = new QLineEdit(this);
    table_ = new QTableWidget(this);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->verticalHeader()->setVisible(false);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setColumnCount(3);
    QStringList header;
    header<<"Mis"<<"Allele1"<<"Allele2";
    table_->setHorizontalHeaderLabels(header);
    table_->setColumnWidth(0, 50);
    table_->setColumnWidth(1, 100);
    table_->setColumnWidth(2, 100);

    table_->resize(250, 300);

    saveButton_ = new QPushButton("save", this);
    exitButton_ = new QPushButton("exit", this);
    setButton_ = new QPushButton("->", this);
 }

void FinalTypeDlg::slotClickSetButton()
{
    allele1Line_->setText(alleleName1_);
    allele2Line_->setText(alleleName2_);
}

void FinalTypeDlg::slotRowChanged()
{
    int row = table_->currentRow();
    alleleName1_ = table_->item(row, 1)->text();
    alleleName2_ = table_->item(row, 2)->text();
}

void FinalTypeDlg::slotClickSaveButton()
{
    QString result = QString("%1,%2").arg(allele1Line_->text()).arg(allele2Line_->text());
    updateSetResultBySampleName(sampleName_.toAscii(), result);
    close();
}
