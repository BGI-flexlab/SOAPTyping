#include "setdlg.h"
#include "core/config.h"
#include <QtGui>
#include "core/analysis.h"
SetDlg::SetDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("setting"));
    QLabel *label1 = new QLabel("Ignore Indel Alleles               :", this);
    QLabel *label2 = new QLabel("Set Open File Thread Numbers       :", this);
    QLabel *label3 = new QLabel("Set Analysis Sample Thread Numbers :", this);

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QHBoxLayout *hLayout4 = new QHBoxLayout;

    QSpacerItem *spacer1 = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer3 = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer4 = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer5 = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    setDefault();

    hLayout1->addWidget(label1);
    hLayout1->addSpacerItem(spacer1);
    hLayout1->addWidget(ignoreIndelBox);

    hLayout2->addWidget(label2);
    hLayout2->addSpacerItem(spacer2);
    hLayout2->addWidget(openFileThreadLine);

    hLayout3->addWidget(label3);
    hLayout3->addSpacerItem(spacer3);
    hLayout3->addWidget(analysisSampleThreadLine);

    hLayout4->addWidget(resetBt);
    hLayout4->addSpacerItem(spacer4);
    hLayout4->addWidget(saveBt);
    hLayout4->addSpacerItem(spacer5);
    hLayout4->addWidget(exitBt);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addLayout(hLayout4);

    resize(300, 200);
    QMap<QString, QString> config;
    readConfig(config);
    setDefaultInfo(config);
    connect(resetBt, SIGNAL(clicked()), this, SLOT(slotClickResetBt()));
    connect(saveBt, SIGNAL(clicked()), this, SLOT(slotClickSaveBt()));
    connect(exitBt, SIGNAL(clicked()), this, SLOT(close()));
}

void SetDlg::setDefault()
{
    ignoreIndelBox = new QCheckBox(this);
    openFileThreadLine = new QLineEdit(this);
    analysisSampleThreadLine = new QLineEdit(this);
    saveBt = new QPushButton("Save", this);
    exitBt = new QPushButton("Exit", this);
    resetBt = new QPushButton("Reset", this);
}

void SetDlg::setDefaultInfo(QMap<QString, QString> &config)
{
    int ig = config.find("IgnoreIndel").value().toInt();
    if(ig==1)
        ignoreIndelBox->setChecked(true);
    else
        ignoreIndelBox->setChecked(false);


    openFileThreadLine->setText(config.find("OpenThread").value());
    analysisSampleThreadLine->setText(config.find("AnalysisThread").value());
}

void SetDlg::slotClickResetBt()
{
    QMap<QString, QString> config;
    setDefaultConfig(config);
    setDefaultInfo(config);
}

void SetDlg::slotClickSaveBt()
{
    int ig = 0;
    if(ignoreIndelBox->checkState())
    {
        ig = 1;
    }

    int oThread = openFileThreadLine->text().toInt();
    int aThread = analysisSampleThreadLine->text().toInt();

    if(oThread < 1 || aThread < 1)
    {
        QMessageBox::warning(this, tr("Soap Typing"),tr("Thread Num must > 0"));
        return;
    }

    QMap<QString, QString> config;
    readConfig(config);
    config.insert("IgnoreIndel", QString::number(ig));
    config.insert("OpenThread", QString::number(oThread));
    config.insert("AnalysisThread", QString::number(aThread));
    changeThreadNum(aThread);
    writeConfig(config);
    close();
}
