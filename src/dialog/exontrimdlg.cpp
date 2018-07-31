#include "exontrimdlg.h"
ExonTimDlg::ExonTimDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("SoapTyping"));
    setDefault();
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hLayout->addWidget(geneBox);
    hLayout->addSpacerItem(spacer);
    hLayout->addWidget(resetBt);
    hLayout->addWidget(sureBt);
    hLayout->addWidget(cancelBt);

    vLayout->addWidget(table);
    vLayout->addLayout(hLayout);
    setGeneBoxData();
    slotGeneBoxIndexChanged(geneBox->currentText());
    connect(geneBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotGeneBoxIndexChanged(QString)));
    connect(resetBt, SIGNAL(clicked()), this, SLOT(slotClickResetBt()));
    connect(sureBt, SIGNAL(clicked()), this, SLOT(slotClickSureBt()));
    connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotClickTableItem()));
    connect(cancelBt, SIGNAL(clicked()), this, SLOT(close()));
    resize(650, 350);

}

void ExonTimDlg::slotGeneBoxIndexChanged(QString gene)
{
    QVector<ExonTrimTable> etList;
    getExonTrimListByGeneName(gene, etList);
    setTableData(etList);
}

void ExonTimDlg::setTableData(QVector<ExonTrimTable> &etList)
{
    table->clear();
    QStringList theader;
    theader<<"Gene"<<"Exon"<<"R/F"<<"Default Start"<<"Default End"<<"Exclude Left Num"<<"Exclude Right Num";
    table->setHorizontalHeaderLabels(theader);

    int size = etList.size();
    if(size<0)
        return;
    table->setRowCount(size);
    for(int i=0; i<size; i++)
    {
        table->setRowHeight(i, 18);
        const ExonTrimTable &etTable = etList.at(i);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(etTable.geneName);
        table->setItem(i, 0, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonIndex);
        table->setItem(i, 1, item);

        item = new QTableWidgetItem;
        item->setText(etTable.fOrR);
        table->setItem(i, 2, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonStart);
        table->setItem(i, 3, item);

        item = new QTableWidgetItem;
        item->setText(etTable.exonEnd);
        table->setItem(i, 4, item);

        item = new QTableWidgetItem;
        item->setText(etTable.excludeLeft);
        table->setItem(i, 5, item);

        item = new QTableWidgetItem;
        item->setText(etTable.excludeRight);
        table->setItem(i, 6, item);
    }
}

void ExonTimDlg::setGeneBoxData()
{
    QStringList geneNames;
    getGeneNames(geneNames);
    geneBox->addItems(geneNames);
    geneBox->setCurrentIndex(0);
}

void ExonTimDlg::setDefault()
{
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::DoubleClicked);
    table->setSelectionBehavior(QAbstractItemView::SelectItems);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList theader;
    theader<<"Gene"<<"Exon"<<"R/F"<<"Default Start"<<"Default End"<<"Exclude Left Num"<<"Exclude Right Num";
    table->verticalHeader()->setVisible(false);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(theader);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth(0, 70);
    table->setColumnWidth(1, 50);
    table->setColumnWidth(2, 40);
    table->setColumnWidth(3, 100);
    table->setColumnWidth(4, 95);
    table->setColumnWidth(5, 130);
    table->setColumnWidth(6, 100);
    table->resize(600, 300);

    resetBt = new QPushButton("Reset", this);
    sureBt = new QPushButton("Apply", this);
    cancelBt = new QPushButton("Exit", this);

    geneBox = new QComboBox(this);
}

void ExonTimDlg::slotClickTableItem()
{
    int c = table->currentColumn();
    if (c<=4)
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        table->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void ExonTimDlg::slotClickResetBt()
{
    for(int i=0; i<table->rowCount(); i++)
    {
        table->item(i, 5)->setText("0");
        table->item(i, 6)->setText("0");
    }

}

void ExonTimDlg::slotClickSureBt()
{
    for(int i=0; i<table->rowCount(); i++)
    {
        ExonTrimTable etTable;
        etTable.geneName=table->item(i, 0)->text();
        etTable.exonIndex=table->item(i, 1)->text();
        etTable.fOrR=table->item(i, 2)->text();
        etTable.exonStart=table->item(i, 3)->text();
        etTable.exonEnd=table->item(i, 4)->text();
        etTable.excludeLeft=table->item(i, 5)->text();
        etTable.excludeRight=table->item(i, 6)->text();
        etTable.etKey=QString("%1_%2_%3").arg(etTable.geneName).arg(etTable.exonIndex).arg(etTable.fOrR);
        updateExonTrim(etTable);
    }
}

