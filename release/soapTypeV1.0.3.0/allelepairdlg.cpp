#include "allelepairdlg.h"
#include "staticdatabase.h"
AllelePairDlg::AllelePairDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Allele Pair Alignment");

    label_ = new QLabel(tr("   Select a pair of alleles and the sequence \n will be listed in the Sequence result window."), this);
    QFont font;
    font.setPointSize(10);
    label_->setFont(font);
    buttonOK_ = new QPushButton("OK", this);
    box1_ = new QComboBox(this);
    box2_ = new QComboBox(this);
//    box1_->setEditable(true);
//    box2_->setEditable(true);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QHBoxLayout *hLayout3 = new QHBoxLayout;

    QSpacerItem *spacer1 = new QSpacerItem(15, 50, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(15, 50, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(15, 50, QSizePolicy::Maximum, QSizePolicy::Minimum);
    QSpacerItem *spacer4 = new QSpacerItem(10, 50, QSizePolicy::Maximum, QSizePolicy::Minimum);
    QSpacerItem *spacer5 = new QSpacerItem(15, 50, QSizePolicy::Maximum, QSizePolicy::Minimum);
    QSpacerItem *spacer6 = new QSpacerItem(10, 50, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer7 = new QSpacerItem(10, 50, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hLayout1->addSpacerItem(spacer1);
    hLayout1->addWidget(label_);
    hLayout1->addSpacerItem(spacer2);

    hLayout2->addSpacerItem(spacer3);
    hLayout2->addWidget(box1_);
    hLayout2->addSpacerItem(spacer4);
    hLayout2->addWidget(box2_);
    hLayout2->addSpacerItem(spacer5);

    hLayout3->addSpacerItem(spacer6);
    hLayout3->addWidget(buttonOK_);
    hLayout3->addSpacerItem(spacer7);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    setMaximumSize(360,180);
    setMinimumSize(360,180);

    connect(buttonOK_, SIGNAL(clicked()), this, SLOT(slotClickOK()));
}
void AllelePairDlg::setData(const QString &geneName, QString *allele1, QString *allele2)
{
    allele1_ = allele1;
    allele2_ = allele2;
    if(geneName.isEmpty())
    {
        box1_->addItem("");
        box2_->addItem("");

    }
    else
    {
        QStringList alleleNames;
        getAlleleNameListFromStaticDabase(geneName.toAscii(), alleleNames);
        box1_->addItems(alleleNames);
        box2_->addItems(alleleNames);
    }
}

void AllelePairDlg::slotClickOK()
{
    allele1_->push_back(box1_->currentText());
    allele2_->push_back(box2_->currentText());
    close();
}
