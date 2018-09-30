#include "allelepairdlg.h"
#include "database/staticdatabase.h"
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
/**新增Start**/
void AllelePairDlg::setData(QStringList typeResult, QString *alelle1, QString *allele2)
{
    allele1_ = alelle1;
    allele2_ = allele2;
    if(typeResult.size() == 0)
    {
        box1_->addItem("");
        box2_->addItem("");
    }
    else
    {
        int typenum = typeResult.size();
        QStringList line1List;
        QStringList line2List;
        for(int i = 0; i < typenum;i++)
        {
            QStringList line = typeResult.at(i).split(",");
            line1List.append(line[1]);
            line2List.append(line[2]);
        }
        QStringList line1 =  removeSameData(line1List);
        QStringList line2 =  removeSameData(line2List);
        box1_->addItems(line1);
        box2_->addItems(line2);
    }

}

void AllelePairDlg::findComboxItem(QString alelle1, QString allele2)
{
    int index1 = box1_->findText(alelle1);
    int index2 = box2_->findText(allele2);
    box1_->setCurrentIndex(index1);
    box2_->setCurrentIndex(index2);
}

QStringList AllelePairDlg::removeSameData(QStringList list)
{
    QStringList returnList;
    for (int i = 0; i < list.count(); i++)  //外循环是循环的次数
    {
        for (int j = list.count() - 1 ; j > i; j--)  //内循环是 外循环一次比较的次数
        {
            if (list[i] == list[j])
            {
                list.removeAt(j);
            }
        }
    }
    returnList = list;
    return returnList;
}
/**新增End**/

void AllelePairDlg::slotClickOK()
{
    allele1_->push_back(box1_->currentText());
    allele2_->push_back(box2_->currentText());
    close();
}

