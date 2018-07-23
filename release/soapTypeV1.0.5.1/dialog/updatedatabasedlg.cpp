#include "updatedatabasedlg.h"
UpdateDlg::UpdateDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("SoapTyping"));
    QVBoxLayout *hLayout = new QVBoxLayout(this);
    tab = new QTabWidget(this);
    setDefaultData();
    setw1();
    setw2();
    setw3();
    //隐藏式修改
    //tab->addTab(w1, "Whole Update");
    w1->setHidden(true);
    tab->addTab(w2, "Selective Update ");
    tab->addTab(w3, "Insert GSSP");

    hLayout->addWidget(tab);
    resize(600, 350);
}

void UpdateDlg::setDefaultData()
{
    w1 = new QWidget(this);
    w2 = new QWidget(this);
    w3 = new QWidget(this);
    t1Line1 = new QLineEdit(w1);
    t2Line1 = new QLineEdit(w2);
    t2Line2 = new QLineEdit(w2);
    t2Line3 = new QLineEdit(w2);
    t2Line4 = new QLineEdit(w2);
    t2Line5 = new QLineEdit(w2);
    t2Line6 = new QLineEdit(w2);
    t2Line7 = new QLineEdit(w2);
    t3Line1 = new QLineEdit(w3);
    t3Line2 = new QLineEdit(w3);
    t3Line3 = new QLineEdit(w3);
    t3Line4 = new QLineEdit(w3);
    t3Line5 = new QLineEdit(w3);
    t3Line6 = new QLineEdit(w3);
    t1BrowseBt1 = new QPushButton("Browse", w1);
    t2BrowseBt1 = new QPushButton("Browse", w2);
    t2BrowseBt2 = new QPushButton("Browse", w2);
    t2BrowseBt3 = new QPushButton("Browse", w2);
    t2BrowseBt4 = new QPushButton("Browse", w2);
    t2BrowseBt5 = new QPushButton("Browse", w2);
    t2BrowseBt6 = new QPushButton("Browse", w2);
    t2BrowseBt7 = new QPushButton("Browse", w2);
    t1ApplyBt = new QPushButton("Apply", w1);
    t1ExitBt = new QPushButton("Exit", w1);
    t2ApplyBt = new QPushButton("Apply", w2);
    t2ExitBt = new QPushButton("Exit", w2);
    t3ClearBt = new QPushButton("Clear", w3);
    t3InsertBt = new QPushButton("Insert", w3);
    t3ExitBt = new QPushButton("Exit", w3);
    waitBt1 = new QPushButton("It is Processing, Please Wait ...", this);
    waitBt1->setHidden(true);
    waitBt1->setMinimumHeight(150);
    waitBt2 = new QPushButton("It is Processing, Please Wait ...", this);
    waitBt2->setHidden(true);
}

void UpdateDlg::setw1()
{
    w1->setAutoFillBackground(true);
    QVBoxLayout *vL = new QVBoxLayout(w1);
    QHBoxLayout *hL1 = new QHBoxLayout;
    QHBoxLayout *hL2 = new QHBoxLayout;
    QLabel *label = new QLabel("Database Files Dir:", w1);
    QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    hL1->addWidget(label);
    hL1->addWidget(t1Line1);
    hL1->addWidget(t1BrowseBt1);
    hL2->addSpacerItem(spacer1);
    hL2->addWidget(t1ApplyBt);
    hL2->addWidget(t1ExitBt);
    vL->addLayout(hL1);
    vL->addSpacerItem(spacer2);
    vL->addWidget(waitBt1);
    vL->addLayout(hL2);
    connect(t1ApplyBt, SIGNAL(clicked()), this, SLOT(slotClickT1ApplyBt()));
    connect(t1ExitBt, SIGNAL(clicked()), this, SLOT(close()));
    connect(t1BrowseBt1, SIGNAL(clicked()), this, SLOT(slotClickT1BrowseBt1()));
}

void UpdateDlg::setw2()
{
    //隐藏式修改
    w2->setAutoFillBackground(true);
    QVBoxLayout *vL = new QVBoxLayout(w2);
    QGridLayout *gL = new QGridLayout;
    gL->addWidget(new QLabel("Gene File:"), 0, 0, 1, 1);
    //gL->addWidget(new QLabel("Old Gene File:"), 1, 0, 1, 1);
    gL->addWidget(new QLabel("Allele File:"), 2, 0, 1, 1);
    gL->addWidget(new QLabel("Gssp File:"), 3, 0, 1, 1);
    gL->addWidget(new QLabel("Lab Align File:"), 4, 0, 1, 1);
    //gL->addWidget(new QLabel("Exon Trim File:"), 5, 0, 1, 1);
    gL->addWidget(new QLabel("Common File:"), 6, 0, 1, 1);
    gL->addWidget(t2Line1, 0, 1, 1, 1);
    //gL->addWidget(t2Line2, 1, 1, 1, 1);
    t2Line2->setHidden(true);
    gL->addWidget(t2Line3, 2, 1, 1, 1);
    gL->addWidget(t2Line4, 3, 1, 1, 1);
    gL->addWidget(t2Line5, 4, 1, 1, 1);
    //gL->addWidget(t2Line6, 5, 1, 1, 1);
    t2Line6->setHidden(true);
    gL->addWidget(t2Line7, 6, 1, 1, 1);
    gL->addWidget(t2BrowseBt1, 0, 2, 1, 1);
    //gL->addWidget(t2BrowseBt2, 1, 2, 1, 1);
    t2BrowseBt2->setHidden(true);
    gL->addWidget(t2BrowseBt3, 2, 2, 1, 1);
    gL->addWidget(t2BrowseBt4, 3, 2, 1, 1);
    gL->addWidget(t2BrowseBt5, 4, 2, 1, 1);
    //gL->addWidget(t2BrowseBt6, 5, 2, 1, 1);
    t2BrowseBt6->setHidden(true);
    gL->addWidget(t2BrowseBt7, 6, 2, 1, 1);
    QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QHBoxLayout *hL = new QHBoxLayout;
    hL->addSpacerItem(spacer1);
    hL->addWidget(t2ApplyBt);
    hL->addWidget(t2ExitBt);
    vL->addLayout(gL);
    vL->addSpacerItem(spacer2);
    vL->addWidget(waitBt2);
    vL->addLayout(hL);
    connect(t2ApplyBt, SIGNAL(clicked()), this, SLOT(slotClickT2ApplyBt()));
    connect(t2ExitBt, SIGNAL(clicked()), this, SLOT(close()));

    connect(t2BrowseBt1, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt1()));
    //connect(t2BrowseBt2, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt2()));
    connect(t2BrowseBt3, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt3()));
    connect(t2BrowseBt4, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt4()));
    connect(t2BrowseBt5, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt5()));
    //connect(t2BrowseBt6, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt6()));
    connect(t2BrowseBt7, SIGNAL(clicked()), this, SLOT(slotClickT2BrowseBt7()));
}

void UpdateDlg::setw3()
{
    w3->setAutoFillBackground(true);
    QVBoxLayout *vL = new QVBoxLayout(w3);
    QGridLayout *gL = new QGridLayout;
    gL->addWidget(new QLabel("GSSP Name      :"), 0, 0, 1, 1);
    gL->addWidget(new QLabel("Gene Name      :"), 0, 1, 1, 1);
    gL->addWidget(new QLabel("Exon Index[1,8]:"), 0, 2, 1, 1);
    gL->addWidget(new QLabel("F or R(F/R)    :"), 2, 0, 1, 1);
    gL->addWidget(new QLabel("GSSP Position  :"), 2, 1, 1, 1);
    gL->addWidget(new QLabel("GSSP Bases     :"), 2, 2, 1, 1);
    gL->addWidget(t3Line1, 1, 0, 1, 1);
    gL->addWidget(t3Line2, 1, 1, 1, 1);
    gL->addWidget(t3Line3, 1, 2, 1, 1);
    gL->addWidget(t3Line4, 3, 0, 1, 1);
    gL->addWidget(t3Line5, 3, 1, 1, 1);
    gL->addWidget(t3Line6, 3, 2, 1, 1);

    QSpacerItem *spacer1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QHBoxLayout *hL = new QHBoxLayout;
    hL->addSpacerItem(spacer1);
    hL->addWidget(t3ClearBt);
    hL->addWidget(t3InsertBt);
    hL->addWidget(t3ExitBt);
    vL->addLayout(gL);
    vL->addSpacerItem(spacer2);
    vL->addLayout(hL);

    connect(t3InsertBt, SIGNAL(clicked()), this, SLOT(slotClickT3InsertBt()));
    connect(t3ExitBt, SIGNAL(clicked()), this, SLOT(close()));
    connect(t3ClearBt, SIGNAL(clicked()), this, SLOT(slotClickT3ClearBt()));
}

void UpdateDlg::slotClickT1ApplyBt()
{
    QString databaseDir = t1Line1->text();
    if(databaseDir.isEmpty())
    {
        showWarningBox(QString("Database files dir is empty!"));
        return;
    }

    QString geneFile = databaseDir + QDir::separator()+"geneTable.txt";
    QString oldGeneFile = databaseDir + QDir::separator() + "oldGeneTable.txt";
    QString alleleFile = databaseDir + QDir::separator() + "alleleTable.txt";
    QString gsspFile = databaseDir + QDir::separator() + "gsspTable.txt";
    QString labAlignFile = databaseDir + QDir::separator() + "labAlignTable.txt";
    QString exonTrimFile = databaseDir + QDir::separator() + "exonTrimTable.txt";

    QFileInfo info1(geneFile), info2(oldGeneFile), info3(alleleFile),
            info4(gsspFile), info5(labAlignFile), info6(exonTrimFile);
    if(!info1.exists() || !info2.exists() || !info3.exists()
            || !info4.exists() || !info5.exists() || !info6.exists())
    {
        showWarningBox(QString("The dir must include following documents:\ngeneTable.txt\noldGeneTable.txt\nalleleTable.txt\ngsspTable.txt\nlabAlignTable.txt\nexonTrimTable.txt"));
        return;
    }

    waitBt1->setHidden(false);
    SetDisableAll(true);
    repaint();

    updateStaticDatabase(geneFile, oldGeneFile, alleleFile, labAlignFile, exonTrimFile);
    updateGsspDatabase(gsspFile);

    showGeneTable();
    showAlleleTable();
    showLabAlignTable();
    showGsspTable();
    showExonTrimTable();

    showInfomationBox(QString("Update the whole database has been completed!"));
    //t1Line1->setText("");
    SetDisableAll(false);
    waitBt1->setHidden(true);
}

void UpdateDlg::slotClickT2ApplyBt()
{
    QString geneFile = t2Line1->text();
    QString oldGeneFile = t2Line2->text();
    QString alleleFile = t2Line3->text();
    QString gsspFile = t2Line4->text();
    QString labAlignFile = t2Line5->text();
    QString exonTrimFile = t2Line6->text();
    QString CommGsspFile = t2Line7->text();
    if(geneFile.isEmpty() && oldGeneFile.isEmpty()
            && alleleFile.isEmpty() &&gsspFile.isEmpty()
            && labAlignFile.isEmpty() && exonTrimFile.isEmpty()
            && CommGsspFile.isEmpty())
    {
        showWarningBox(QString("Please set one file at least!"));
        return;
    }

    if(isFileExists(geneFile)<0 || isFileExists(oldGeneFile)<0
            || isFileExists(alleleFile)<0|| isFileExists(gsspFile)<0
            || isFileExists(labAlignFile)<0 || isFileExists(exonTrimFile)<0
            || isFileExists(CommGsspFile)<0)
    {
        return;
    }

    QString database1 = QString("Database%1soapTypingStaticDatabase").arg(QDir::separator());
    QString database2 = QString("Database%1soapTypingGsspDatabase").arg(QDir::separator());
    if(isFileExists(database1) != 0 || isFileExists(database2)!=0)
    {
        return;
    }

    waitBt2->setHidden(false);
    SetDisableAll(true);
    repaint();

    if(isFileExists(geneFile)==0)
    {
        //qDebug()<<geneFile;
        readGeneTableTxtFile(geneFile);
    }
    if(isFileExists(oldGeneFile)==0)
    {
        //qDebug()<<oldGeneFile;
        readOldGeneTableTxtFile(oldGeneFile);
    }
    if(isFileExists(alleleFile)==0)
    {
        //qDebug()<<alleleFile;
        readAlleleTableTxtFile(alleleFile);
    }
    if(isFileExists(gsspFile)==0)
    {
        //qDebug()<<gsspFile;
        readGsspTableTxtFile(gsspFile);
    }
    if(isFileExists(labAlignFile)==0)
    {
        //qDebug()<<labAlignFile;
        readLabAlignTableTxtFile(labAlignFile);
    }
    if(isFileExists(exonTrimFile)==0)
    {
        //qDebug()<<exonTrimFile;
        readExonTrimTableTxtFile(exonTrimFile);
    }
    if(isFileExists(CommGsspFile)==0)
    {
        //新增，这个好扯
        //createCommonGsspTable();
        readCommonGsspTableTxt(CommGsspFile);
    }


    showInfomationBox(QString("Update database has been completed!"));
    SetDisableAll(false);
    waitBt2->setHidden(true);
}

void UpdateDlg::slotClickT3InsertBt()
{
    GsspTable gsspTable;
    if(t3Line1->text().isEmpty() || t3Line2->text().isEmpty() || t3Line3->text().isEmpty()
            || t3Line4->text().isEmpty() || t3Line5->text().isEmpty() || t3Line6->text().isEmpty())
    {
        showWarningBox(QString("All blanks must be filled!"));
        return;
    }
    gsspTable.gsspName = t3Line1->text().toAscii();
    gsspTable.exonIndex = t3Line3->text().toInt();
    gsspTable.rOrF = t3Line4->text().toAscii();
    gsspTable.position = t3Line5->text().toInt();
    gsspTable.base = t3Line6->text().toAscii();
    if(t3Line2->text().contains(":")){
        QStringList nameList = t3Line2->text().trimmed().split(":", QString::SkipEmptyParts);
        for(int i=0; i<nameList.size(); i++){
            gsspTable.geneName = nameList.at(i).toAscii();
            gsspTable.gsspKey = gsspTable.gsspName + "_" + gsspTable.geneName;
            insertGsspToGsspTable(gsspTable);
        }
    }else{
        gsspTable.geneName = t3Line2->text().toAscii();
        gsspTable.gsspKey  = t3Line1->text()+"_"+t3Line2->text();
        insertGsspToGsspTable(gsspTable);
    }
    CommonGsspTable commonTable;
    commonTable.gsspName = gsspTable.gsspName;
    commonTable.geneName = t3Line2->text().toAscii();
    commonTable.exonIndex = t3Line3->text().toAscii();
    commonTable.fOrR = gsspTable.rOrF;
    insertCommonGsspTable(commonTable);

    showInfomationBox(QString("Insert GSSP into database has been completed!"));
}

void UpdateDlg::slotClickT1BrowseBt1()
{
    QString dir0 = QFileDialog::getExistingDirectory(this, "Open", dir);
    if(!dir0.isEmpty())
    {
        t1Line1->setText(dir0);
        dir=dir0;
    }
}

void UpdateDlg::slotClickT2BrowseBt1()
{
    getOpenFile(t2Line1);
}

void UpdateDlg::slotClickT2BrowseBt2()
{
    getOpenFile(t2Line2);
}

void UpdateDlg::slotClickT2BrowseBt3()
{
    getOpenFile(t2Line3);
}

void UpdateDlg::slotClickT2BrowseBt4()
{
    getOpenFile(t2Line4);
}

void UpdateDlg::slotClickT2BrowseBt5()
{
    getOpenFile(t2Line5);
}

void UpdateDlg::slotClickT2BrowseBt6()
{
    getOpenFile(t2Line6);
}

void UpdateDlg::slotClickT2BrowseBt7()
{
    getOpenFile(t2Line7);
}

void UpdateDlg::getOpenFile(QLineEdit *line)
{
    QString file = QFileDialog::getOpenFileName(this, "Open", dir2);
    if(!file.isEmpty())
    {
        line->setText(file);
        QFileInfo f(file);
        dir2=f.absoluteFilePath();
    }
}

void UpdateDlg::showWarningBox(const QString &info)
{
    QMessageBox::warning(this, tr("SoapTyping"),info);
}

void UpdateDlg::showInfomationBox(const QString &info)
{
    QMessageBox::information(this, tr("SoapTyping"),info);
}

void UpdateDlg::SetDisableAll(bool t)
{
    tab->setDisabled(t);
    //    t1Line1->setDisabled(t);
    //    t2Line1->setDisabled(t);
    //    t2Line2->setDisabled(t);
    //    t2Line3->setDisabled(t);
    //    t2Line4->setDisabled(t);
    //    t2Line5->setDisabled(t);
    //    t2Line6->setDisabled(t);
    //    t3Line1->setDisabled(t);
    //    t3Line2->setDisabled(t);
    //    t3Line3->setDisabled(t);
    //    t3Line4->setDisabled(t);
    //    t3Line5->setDisabled(t);
    //    t3Line6->setDisabled(t);
    //    t1BrowseBt1->setDisabled(t);
    //    t2BrowseBt1->setDisabled(t);
    //    t2BrowseBt2->setDisabled(t);
    //    t2BrowseBt3->setDisabled(t);
    //    t2BrowseBt4->setDisabled(t);
    //    t2BrowseBt5->setDisabled(t);
    //    t2BrowseBt6->setDisabled(t);
    //    t1ApplyBt->setDisabled(t);
    //    t1ExitBt->setDisabled(t);
    //    t2ApplyBt->setDisabled(t);
    //    t2ExitBt->setDisabled(t);
}

int UpdateDlg::isFileExists(QString file)
{
    if(file.isEmpty())
        return 1;
    QFileInfo info(file);
    if(info.exists())
        return 0;
    else
        showWarningBox(QString("%1 does not exists!").arg(file));
    return -1;
}

void UpdateDlg::slotClickT3ClearBt()
{
    t3Line1->setText("");
    t3Line2->setText("");
    t3Line3->setText("");
    t3Line4->setText("");
    t3Line5->setText("");
    t3Line6->setText("");
}
