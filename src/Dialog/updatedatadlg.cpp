#include "updatedatadlg.h"
#include "ui_updatedatadlg.h"
#include <QFileDialog>
#include <DataBase/soaptypingdb.h>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <Core/core.h>
#include <QDir>

UpdateDataDlg::UpdateDataDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDataDlg)
{
    ui->setupUi(this);
    ConnectSignalandSlot();
    m_str_dir = "";
}

UpdateDataDlg::~UpdateDataDlg()
{
    delete ui;
}

void UpdateDataDlg::ConnectSignalandSlot()
{
    connect(ui->btnGene, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenFileFlg);
    connect(ui->btnAllele, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenFileFlg);
    connect(ui->btnGssp, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenFileFlg);
    connect(ui->btnLabAlign, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenFileFlg);
    connect(ui->btnCommon, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenFileFlg);
    connect(ui->btnNuc, &QPushButton::clicked, this, &UpdateDataDlg::slotOpenDir);
    connect(ui->btnApply, &QPushButton::clicked, this, &UpdateDataDlg::slotApply);
    connect(ui->btnExit, &QPushButton::clicked, this, &UpdateDataDlg::close);

    connect(ui->btnClear, &QPushButton::clicked, this, &UpdateDataDlg::slotClear);
    connect(ui->btnInsert, &QPushButton::clicked, this, &UpdateDataDlg::slotInsert);
    connect(ui->btnExit_2, &QPushButton::clicked, this, &UpdateDataDlg::close);
}

void UpdateDataDlg::slotOpenDir()
{
    m_str_dir = QFileDialog::getExistingDirectory(this,"Nuc Dir","");
    ui->lineEdit_Nuc->setText(m_str_dir);
}

void UpdateDataDlg::slotOpenFileFlg()
{
    QString file = QFileDialog::getOpenFileName(this, "Open");
    if(!file.isEmpty())
    {
        QPushButton*btn = qobject_cast<QPushButton*>(sender());//获取发射信号的对象
        if("btnGene"== btn->objectName())
        {
            ui->lineEdit_Gene->setText(file);
        }
        else if ("btnAllele"== btn->objectName())
        {
            ui->lineEdit_Allele->setText(file);
        }
        else if ("btnGssp"== btn->objectName())
        {
            ui->lineEdit_Gssp->setText(file);
        }
        else if ("btnLabAlign"== btn->objectName())
        {
            ui->lineEdit_LabAlign->setText(file);
        }
        else if ("btnCommon"== btn->objectName())
        {
            ui->lineEdit_Common->setText(file);
        }
    }
}

bool UpdateDataDlg::isFileExists(const QString &file)
{
    if(file.isEmpty())
    {
       return false;
    }
    return QFileInfo::exists(file);
}

void UpdateDataDlg::slotApply()
{
    ui->tab->setEnabled(false);
    QString geneFile = ui->lineEdit_Gene->text();
    QString alleleFile = ui->lineEdit_Allele->text();
    QString gsspFile = ui->lineEdit_Gssp->text();
    QString labAlignFile = ui->lineEdit_LabAlign->text();
    QString CommGsspFile = ui->lineEdit_Common->text();
    //QString NucPath = ui->lineEdit_Nuc->text();

    if(isFileExists(geneFile))
    {
        SoapTypingDB::GetInstance()->readGeneTableTxtFile(geneFile);
    }

    if(isFileExists(alleleFile))
    {
        SoapTypingDB::GetInstance()->readAlleleTableTxtFile(alleleFile);
    }

    if(isFileExists(gsspFile))
    {
        SoapTypingDB::GetInstance()->readGsspTableTxtFile(gsspFile);
    }

    if(isFileExists(labAlignFile))
    {
        SoapTypingDB::GetInstance()->readLabAlignTableTxtFile(labAlignFile);
    }

    if(isFileExists(CommGsspFile))
    {
        SoapTypingDB::GetInstance()->readCommonGsspTableTxt(CommGsspFile);
    }

    if(!m_str_dir.isEmpty())
    {
        UpdateDatabase();
    }

    ui->tab->setEnabled(true);
    QMessageBox::information(this, tr("SoapTyping"),"Update database has been completed!");
}

void UpdateDataDlg::slotClear()
{
    ui->lineEdit_Gssp_Name->clear();
    ui->lineEdit_Gene_Name->clear();
    ui->lineEdit_Exon->clear();
    ui->lineEdit_FR->clear();
    ui->lineEdit_Pos->clear();
    ui->lineEdit_Base->clear();
}

void UpdateDataDlg::slotInsert()
{
    GsspTable gsspTable;
    gsspTable.gsspName = ui->lineEdit_Gssp_Name->text();
    gsspTable.exonIndex = ui->lineEdit_Exon->text().toInt();
    gsspTable.rOrF = ui->lineEdit_FR->text();
    gsspTable.position = ui->lineEdit_Pos->text().toInt();
    gsspTable.base = ui->lineEdit_Base->text().toLatin1();

    QStringList nameList = ui->lineEdit_Gene_Name->text().trimmed().split(":", QString::SkipEmptyParts);
    for(int i=0; i<nameList.size(); i++){
        gsspTable.geneName = nameList.at(i);
        gsspTable.gsspKey = gsspTable.gsspName + "_" + gsspTable.geneName;
        SoapTypingDB::GetInstance()->insertGsspTable(gsspTable);
    }

    CommonGsspTable commonTable;
    commonTable.gsspName = gsspTable.gsspName;
    commonTable.geneName = ui->lineEdit_Gene_Name->text();
    commonTable.exonIndex = ui->lineEdit_Exon->text();
    commonTable.fOrR = gsspTable.rOrF;
    SoapTypingDB::GetInstance()->insertCommonGsspTable(commonTable);

    QMessageBox::information(this, tr("SoapTyping"),"Insert GSSP into database has been completed!");
}


void ParseClassFile(QMap<QString,QVector<int>> &map_class)
{
    QFile file("./DataBase/altered_all.classes");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QVector<QStringRef> vec = line.splitRef('\t');

            if(map_class.find(vec[0].toString()) != map_class.end())
            {
                map_class[vec[0].toString()].prepend(vec[1].toInt());
            }
            else
            {
                QVector<int> temp;
                temp.reserve(15);
                temp.push_back(vec[1].toInt());
                map_class.insert(vec[0].toString(),temp);
            }
        }
        file.close();
    }
}

void ParseAvailbleFile(QMap<QString,QString> &map_availble)
{
    QFile file("./DataBase/availble.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList strlist = line.split('\t');
            map_availble.insert(strlist[0],strlist[1]);
        }
        file.close();
    }
}

void ParseRareList(QSet<QString> &set_rare)
{
    QFile file("./DataBase/Rare List.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            set_rare.insert(line);
        }
        file.close();
    }
}

void InsertMapInfo(int split_pos, QString &line, QHash<QString, QString> &hash_info)
{
    QString strname = line.left(split_pos).trimmed();
    QString strseq = line.mid(split_pos).remove(' ');
    if(hash_info.find(strname)!=hash_info.end())
    {
        QString &str_seq = hash_info[strname];
        str_seq.append(strseq);
    }
    else
    {
        hash_info.insert(strname, strseq);
    }
}

void ParseNucFile_DRB(QString &strver, QString &str_dir,
                      QMap<QString, QHash<QString,QString>> &map_hash_info)
{
    QString str_path = QString("%1/%2_nuc.txt").arg(str_dir).arg("DRB");
    QString str_start1("DRB1*");
    QString str_start3("DRB3*");
    QString str_start4("DRB4*");
    QString str_start5("DRB5*");

    QHash<QString, QString> hash_DRB1;
    QHash<QString, QString> hash_DRB3;
    QHash<QString, QString> hash_DRB4;
    QHash<QString, QString> hash_DRB5;
    int split_pos = 0;
    bool isfirst_DRB1 = true;
    bool isfirst_DRB3 = true;
    bool isfirst_DRB4 = true;
    bool isfirst_DRB5 = true;
    QString str_firstname_DRB1;
    QString str_firstname_DRB3;
    QString str_firstname_DRB4;
    QString str_firstname_DRB5;

    QFile file(str_path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if(line.startsWith(str_start1))
            {
                if(isfirst_DRB1)
                {
                    isfirst_DRB1 = false;
                    str_firstname_DRB1 = line.left(split_pos).trimmed();
                }
                InsertMapInfo(split_pos, line, hash_DRB1);
            }
            else if (line.startsWith(str_start3)) {
                if(isfirst_DRB3)
                {
                    isfirst_DRB3 = false;
                    str_firstname_DRB3 = line.left(split_pos).trimmed();
                }
                InsertMapInfo(split_pos, line, hash_DRB3);
            }
            else if (line.startsWith(str_start4)) {
                if(isfirst_DRB4)
                {
                    isfirst_DRB4 = false;
                    str_firstname_DRB4 = line.left(split_pos).trimmed();
                }
                InsertMapInfo(split_pos, line, hash_DRB4);
            }
            else if (line.startsWith(str_start5)) {
                if(isfirst_DRB5)
                {
                    isfirst_DRB5 = false;
                    str_firstname_DRB5 = line.left(split_pos).trimmed();
                }
                InsertMapInfo(split_pos, line, hash_DRB5);
            }
            else if(line.contains("version"))
            {
                QStringList strlist = line.split(' ');
                strver = strlist[strlist.size()-1];
            }
            else if (split_pos==0 && line.contains("cDNA")) {
                split_pos = line.length()-1;
            }
        }
        file.close();
    }


    QString &str_DRB1 = hash_DRB1[str_firstname_DRB1];
    QString bty_DRB3 = hash_DRB3[str_firstname_DRB3].toLatin1();
    QString bty_DRB4 = hash_DRB4[str_firstname_DRB4].toLatin1();
    QString bty_DRB5 = hash_DRB5[str_firstname_DRB5].toLatin1();

    for(int i=0;i<str_DRB1.length();i++)
    {
        if(bty_DRB3[i]=='-')
        {
            bty_DRB3[i] = str_DRB1[i];
        }
        if(bty_DRB4[i]=='-')
        {
            bty_DRB4[i] = str_DRB1[i];
        }
        if(bty_DRB5[i]=='-')
        {
            bty_DRB5[i] = str_DRB1[i];
        }
    }

    hash_DRB3[str_firstname_DRB3] = bty_DRB3;
    hash_DRB4[str_firstname_DRB4] = bty_DRB4;
    hash_DRB5[str_firstname_DRB5] = bty_DRB5;

    map_hash_info.insert(str_firstname_DRB1, hash_DRB1);
    map_hash_info.insert(str_firstname_DRB3, hash_DRB3);
    map_hash_info.insert(str_firstname_DRB4, hash_DRB4);
    map_hash_info.insert(str_firstname_DRB5, hash_DRB5);
}

void ParseNucFile(const QString &strgene, QString &strver, QString &str_dir,
                  QMap<QString, QHash<QString,QString>> &map_hash_info)
{
    QString str_path = QString("%1/%2_nuc.txt").arg(str_dir).arg(strgene);
    QString str_start = QString("%1*").arg(strgene);
    int split_pos = 0;
    bool isfirstline = true;
    QString strname;
    QHash<QString, QString> hash_info;

    QFile file(str_path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if(line.startsWith(str_start))
            {
                if(isfirstline)
                {
                    isfirstline = false;
                    strname = line.left(split_pos).trimmed();
                }
                InsertMapInfo(split_pos, line, hash_info);
            }
            else if(line.contains("version"))
            {
                QStringList strlist = line.split(' ');
                strver = strlist[strlist.size()-1];
            }
            else if (split_pos==0 && line.contains("cDNA")) {
                split_pos = line.length()-1;
            }
        }
        file.close();

        map_hash_info.insert(strname, hash_info);
    }
}

void GetExonPos(const QString &str, QVector<int> &vec_pos) //获取分割线'|'的pos
{
    QString line(str);
    QString &first = line.remove('.');
    int pos = 0,icount = 0;
    vec_pos.push_back(0);
    while (true)
    {
        pos = first.indexOf('|',pos);
        if(pos == -1)
        {
            break;
        }
        vec_pos.push_back(pos-icount);
        pos++;
        icount++;
    }
    vec_pos.push_back(line.remove('|').length());
}

void GetDotPos(const QString &str, QMap<int,int> &map_pos) //获取'.'的pos和长度
{
    QString line(str);
    QString &first = line.remove('|');
    int pos_dot=0;
    int lenth_dot=0;
    for(int i=0;i<first.size();i++)
    {
        if (first[i] == '.')
        {
            if(pos_dot == 0)
            {
                pos_dot = i;
            }
            lenth_dot++;
            continue;
        }
        if(pos_dot)
        {
            map_pos.insert(pos_dot,lenth_dot);
            pos_dot = 0;
            lenth_dot = 0;
        }
    }
}

int GetRealPos(QMap<int,int> &map_dotpos,int pos) //去除'.'后，获取变化碱基的实际pos
{
    auto itor = map_dotpos.begin();
    int total = 0;
    for(;itor!=map_dotpos.end();itor++)
    {
        if(pos <= itor.key())
        {
            break;
        }
        total += itor.value();
    }
    return pos - total;
}

//hash_info，入参，传入解析文件获取的信息
//str_firstname,第一行的名称
//vec_exonpos，入参，传入exon的pos
//map_dotpos，入参，传入dot的pos
//set_rare，入参，用以判断是否是稀有类型
//vec_seqline，入\出参，获取每一行allel信息
//vec_set，入\出参，获取每一列的碱基（自动去重）
//map_len，入\出参，获取相同长度(自动去重)
//void CreateSeqLine(QHash<QString,QString> &hash_info, const QString &str_firstname, QVector<int> &vec_exonpos, QMap<int,int> &map_dotpos,
//                   QSet<QString> &set_rare, QVector<SeqLine *> &vec_seqline, QVector<QSet<char>> &vec_set,
//                   QMap<int,int> &map_len)
void CreateSeqLine(QHash<QString,QString> &hash_info, const QString &str_firstname, QVector<int> &vec_exonpos, QMap<int,int> &map_dotpos,
                   QSet<QString> &set_rare, QMap<QString, QSharedPointer<SeqLine>> &map_seqline, QVector<QSet<char>> &vec_set,
                   QMap<int,int> &map_len)
{
    QString firstline = hash_info[str_firstname];
    firstline.remove('|');
    int f_size = firstline.size();
    vec_set.reserve(f_size);
    for(int i=0;i<f_size;i++)
    {
        QSet<char> set;
        vec_set.push_back(set);
    }

//    QString str = str_firstname.split('*').at(0);
//    QFile file(QString("F:/test/%1.txt").arg(str));
//    file.open(QIODevice::WriteOnly);
//    QTextStream out(&file);

    QByteArray temp("ATGC");
    auto itor = hash_info.begin();
    for(;itor!=hash_info.end();itor++)
    {
        QString &arry = itor.value().remove('|');
        //out<<arry<<"\r\n";
        int pos_change = 0,pos_del = 0, pos_inl = 0;

        //SeqLine *pseqline = new SeqLine(itor.key());
        //vec_seqline.push_back(pseqline);
        QSharedPointer<SeqLine> pseqline = QSharedPointer<SeqLine>(new SeqLine(itor.key()));
        map_seqline.insert(itor.key(), pseqline);

        if(set_rare.find(itor.key())!=set_rare.end())
        {
             pseqline->SetIsRare(true);
        }
        else
        {
            pseqline->SetIsRare(false);
        }

        if(itor.key() == str_firstname)//nuc文件第一行
        {
            for(int i=0;i<arry.size();i++)
            {
                if(arry[i] != '.')
                {
                   pseqline->AppendSeqArry(arry[i].toLatin1());
                }
                vec_set[i].insert(arry[i].toLatin1());
            }
        }
        else
        {
            int arry_size = arry.size();
            int count = f_size<arry_size ? f_size: arry_size;
            for(int i=0;i<count;i++)
            {
                if(arry[i] == '-')
                {
                    pseqline->AppendSeqArry(firstline[i].toLatin1());
                }
                else if (arry[i] == '*')
                {
                    pseqline->AppendSeqArry(arry[i].toLatin1());
                }
                else if(arry[i] == '.')
                {
                    if(firstline[i] != '.') //del
                    {
                        pos_del = GetRealPos(map_dotpos, i);
                        pseqline->InsertByType(3, pos_del, arry[i].toLatin1());
                        pseqline->AppendSeqArry(arry[i].toLatin1());
                        continue;
                    }
                }
                else if(temp.contains(arry[i].toLatin1()))
                {
                    if(firstline[i] == '.') //inl
                    {
                        pos_inl = GetRealPos(map_dotpos, i);
                        pseqline->InsertByType(2, pos_inl, arry[i].toLatin1());
                        continue;
                    }
                    else //change
                    {
                        pos_change = GetRealPos(map_dotpos, i);
                        pseqline->InsertByType(1, pos_change, arry[i].toLatin1());

                        pseqline->AppendSeqArry(arry[i].toLatin1());

                        vec_set[i].insert(arry[i].toLatin1());
                        continue;
                    }
                }
            }
        }

       int len = pseqline->CalcSeqInfo(vec_exonpos);
       map_len.insert(len,0);
    }
}

//map_len，入参
//vec_class，入参
//vec_seqline，出参
void SetSeqLineClassNum(QMap<int,int> &map_len, QVector<int> &vec_class,
                        QList<QSharedPointer<SeqLine>> &list_seqline)
{
    int i_a = map_len.count()/(vec_class.count()-1);

    QList<int> len_list = map_len.keys();
    for(int i=0;i<vec_class.count()-1;i++)
    {
        for(int j=i*i_a;j<(i+1)*i_a;j++)
        {
            map_len[len_list[j]] = vec_class[i];
        }
    }


    for(int i=0;i<list_seqline.size();i++)
    {
        int number = map_len[list_seqline[i]->GetSeqLineLenth()];
        list_seqline[i]->SetClassNumber(number);
    }
}

void GetConsensusSeq(QVector<QSet<char>> &vec_set, QByteArray &conseq)
{
    QByteArray arry;
    foreach(const QSet<char> &set, vec_set)
    {
        if(set.size() > 1)
        {
            auto itor = set.begin();
            unsigned int a = 0;
            for(;itor!=set.end();itor++)
            {
                a |= Core::GetInstance()->formatMerge(*itor);
            }
            arry.append(Core::GetInstance()->reFormatMerge(a));
        }
        else if(set.size() == 1)
        {
            arry.append(*set.begin());
        }
    }

    QRegularExpression re("[.\\|]+");
    conseq = QString(arry).remove(re).toLatin1();
}

void testwrite(QByteArray &conseq, QVector<SeqLine *> &vec_seqline)
{
    QFile file_out("E:/test/A_test.txt");
    if(file_out.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file_out);
        out<<conseq<<"\r\n";

        foreach(SeqLine *pseq, vec_seqline)
        {
            out<<pseq->GetSeqLineName()<<"\t"<<pseq->GetSeqArry()<<"\t"<<
                 pseq->GetClassNumber()<<"\t"<<pseq->GetStartindex()<<
                 pseq->GetEndindex()<<"\r\n";
        }

        file_out.close();
    }
}

void DeleteDB()
{
    SoapTypingDB::GetInstance()->deleteTable("geneTable");
    SoapTypingDB::GetInstance()->deleteTable("alleleTable");
    SoapTypingDB::GetInstance()->deleteTable("labAlignTable");
}

void InsertDataToGeneTable(const QString &genename, const QByteArray &seqarry, const QVector<int> &vec_pos,
                           const QVector<int> &vec_class,const QString &availble, const QString &version)
{
    GeneTable geneTable;
    geneTable.geneName = genename;
    geneTable.geneSequence = seqarry;
    geneTable.exonCount = vec_pos.count() -1;
    QStringList list_temp;
    foreach(int pos, vec_pos)
    {
        list_temp.push_back(QString::number(pos));
    }
    geneTable.exonPostionIndex = list_temp.join(':');

    list_temp.clear();
    auto itor = vec_class.crbegin();
    for(;itor!=vec_class.crend();itor++)
    {
        list_temp.push_back(QString::number(*itor));
    }
    geneTable.geneClasses = list_temp.join(':');
    geneTable.availableExon = availble;
    geneTable.version = version;

    //out<<geneTable.geneName<<'\t'<<geneTable.geneSequence<<'\t'<<geneTable.exonCount<<'\t'<<geneTable.exonPostionIndex<<'\r\n';
    SoapTypingDB::GetInstance()->insertGeneTable(geneTable);
}

int GetIndelInfo(QMap<int,char> &map_info, QString &str_seq)
{
    int pos = map_info.firstKey();
    str_seq.append(map_info.first());
    auto itor = map_info.begin()+1;
    for(int i=1;itor!=map_info.end();itor++,i++)
    {
        if(itor.key() == pos+i)
        {
            str_seq.append(itor.value());
        }
    }
    return pos;
}

void InsertDataToAlleleTable(QList<QSharedPointer<SeqLine>> &list_seqline, const QByteArray &seqarry, const QString &strgene)
{
    SoapTypingDB::GetInstance()->StartTransaction();
    foreach(QSharedPointer<SeqLine> pseq, list_seqline)
    {
        AlleleTable alleleTable;
        alleleTable.alleleName = pseq->GetSeqLineName();
        alleleTable.alleleSequence = pseq->GetSeqArry();
        alleleTable.geneName = strgene;
        alleleTable.isRare = pseq->GetIsRare();
        alleleTable.isIndel = pseq->GetIsIndel();
        if(pseq->GetIsIndel())
        {
            QMap<int,char> &map_inl = pseq->GetMapInfo(2);
            QMap<int,char> &map_del = pseq->GetMapInfo(3);
            if(!map_inl.empty())
            {
                QString str_seq;
                alleleTable.indelPosition = GetIndelInfo(map_inl, str_seq);
                alleleTable.indelInfo = QString("Insert '%1'").arg(str_seq);
            }
            else if (!map_del.empty())
            {
                auto itor = map_del.begin();
                for(;itor!=map_del.end();itor++)  //修正为简并序列碱基
                {
                    itor.value() = seqarry[itor.key()];
                }
                QString str_seq;
                alleleTable.indelPosition = GetIndelInfo(map_del, str_seq);
                alleleTable.indelInfo = QString("Delete '%1'").arg(str_seq);
            }
        }
        else
        {
            alleleTable.indelPosition = 0;
            alleleTable.indelInfo = "";
        }

        alleleTable.classesNumber = pseq->GetClassNumber();
        alleleTable.noStar = QString("%1%2").arg(pseq->GetStartindex()).arg(pseq->GetEndindex());

        SoapTypingDB::GetInstance()->insertAlleleTable(alleleTable);
    }
    SoapTypingDB::GetInstance()->EndTransaction();
}

void InsertDataToLabAlignTable(QList<QSharedPointer<SeqLine>> &list_seqline, const QString &strgene)
{
    SoapTypingDB::GetInstance()->StartTransaction();
    QByteArray temp("ATGC");
    foreach(QSharedPointer<SeqLine> pseq, list_seqline)
    {
        LabAlignTable labAlignTable;
        labAlignTable.alleleName = pseq->GetSeqLineName();
        QByteArray arry = pseq->GetSeqArry();
        for(int i=0; i<arry.size();i++)
        {
            if(temp.contains(arry[i]))
            {
                arry[i] = '-';
            }
        }

        labAlignTable.geneName = strgene;

        QMap<int,char> &map_change = pseq->GetMapInfo(1);
        QStringList list_temp;
        auto itor = map_change.begin();
        for(;itor!=map_change.end();itor++)
        {
            arry[itor.key()] = itor.value();
            list_temp.push_back(QString::number(itor.key()));
        }
        labAlignTable.alleleSequence = arry;
        labAlignTable.misPosition = list_temp.join(':');

        SoapTypingDB::GetInstance()->insertLabAlignTable(labAlignTable);
    }
    SoapTypingDB::GetInstance()->EndTransaction();
}


void UpdateDataDlg::UpdateDatabase()
{
    DeleteDB();

    QMap<QString,QVector<int>> map_class;
    ParseClassFile(map_class);

    QMap<QString,QString> map_availble;
    ParseAvailbleFile(map_availble);

    QSet<QString> set_rare;
    ParseRareList(set_rare);

    bool isDRB = false;
    auto itor = map_class.begin();
    for(;itor != map_class.end();itor++)
    {
//        QString str_path = QString("F:/test/%1_test.txt").arg(itor.key());
//        if(QFile::exists(str_path)) continue;


        QString str_version;
        QMap<QString, QHash<QString,QString>> map_hash_info;
        if(itor.key().contains("DRB"))
        {
            if(!isDRB) //DRB文件未解析
            {
                isDRB = true;
                ParseNucFile_DRB(str_version, m_str_dir, map_hash_info);
            }
            else
            {
                continue;
            }
        }
        else
        {
            ParseNucFile(itor.key(), str_version, m_str_dir, map_hash_info);
        }

        auto hash_itor=map_hash_info.begin();
        for(;hash_itor!=map_hash_info.end();hash_itor++)
        {
            QHash<QString,QString> &hash_info = *hash_itor;
            QString str_firstname = hash_itor.key();
            QString str_gene = str_firstname.split('*').at(0);

//            QFile file_out(QString("F:/test/%1_test.txt").arg(str_gene));
//            file_out.open(QIODevice::WriteOnly);
//            QTextStream out(&file_out);

            QVector<int> vec_pos;
            GetExonPos(hash_info[str_firstname], vec_pos);

            QMap<int,int> map_dotpos;//位置，长度
            GetDotPos(hash_info[str_firstname], map_dotpos);

            //QVector<SeqLine *> vec_seqline;
            //vec_seqline.reserve(hash_info.size());
            QMap<QString, QSharedPointer<SeqLine>> map_SeqLine;

            QVector<QSet<char>> vec_set;

            QMap<int,int> map_len;

            CreateSeqLine(hash_info, str_firstname, vec_pos, map_dotpos, set_rare, map_SeqLine, vec_set, map_len);

            hash_info.clear();
            map_dotpos.clear();
            set_rare.clear();

            QList<QSharedPointer<SeqLine>> list_seqline = map_SeqLine.values();

            SetSeqLineClassNum(map_len, map_class[str_gene], list_seqline);

            QByteArray conseq;
            GetConsensusSeq(vec_set, conseq);

            InsertDataToGeneTable(str_gene, conseq, vec_pos, map_class[str_gene], map_availble[str_gene], str_version);

//            foreach (QSharedPointer<SeqLine> pseq, list_seqline) {
//                out<<pseq->GetSeqLineName()<<' '<<pseq->GetClassNumber()<<' '<<pseq->GetSeqArry()<<"\r\n";
//            }
            InsertDataToAlleleTable(list_seqline, conseq, str_gene);

            InsertDataToLabAlignTable(list_seqline, str_gene);


            //file_out.close();
        }

        qDebug()<<itor.key()<<"finish";

    }


    //close();
}

SeqLine::SeqLine(const QString &name):
    m_strName(name)
{
    m_isRare = false;
    m_isIndel = false;
    m_classesNumber = 0;
    m_startindex = 0;
    m_endindex = 0;
    m_reallen = 0;
}

SeqLine::~SeqLine()
{

}

void SeqLine::AppendSeqArry(char ch)
{
    m_seqArry.append(ch);
}

QByteArray& SeqLine::GetSeqArry()
{
    return m_seqArry;
}

void SeqLine::InsertByType(int type,int pos,char ch)
{
    if(type == 1) //change
    {
        m_map_change.insert(pos,ch);
    }
    else if(type == 2)//inl
    {
        m_map_inl.insert(pos,ch);
    }
    else if (type == 3)//del
    {
        m_map_del.insert(pos,ch);
    }
}

QMap<int,char>& SeqLine::GetMapInfo(int type)
{
    switch (type)
    {
        case 1: return m_map_change;
        case 2: return m_map_inl;
        case 3: return m_map_del;
    }
}

int SeqLine::CalcSeqInfo(QVector<int> &vec_pos)
{
    m_reallen = m_seqArry.length() - m_seqArry.count('*');
    int startpos = 0;
    for(int i=0;i<m_seqArry.length();i++)
    {
        if(m_seqArry[i] == '*')
        {
            startpos++;
        }
        else
        {
            break;
        }
    }

    int endpos = startpos + m_reallen;

    m_startindex = CalcExonIndex(vec_pos,startpos+1);
    m_endindex = CalcExonIndex(vec_pos,endpos);

    return m_reallen;
}

void SeqLine::AppendSeq(const QByteArray &arry)
{
    m_seqArry.append(arry);
}

int SeqLine::GetSeqLineLenth()
{
    return m_reallen;
}

void SeqLine::SetClassNumber(int number)
{
    m_classesNumber = number;

    //qDebug()<<m_strName<<m_classesNumber<<m_startindex<<m_endindex;
}

int SeqLine::GetClassNumber()
{
    return m_classesNumber;
}

int SeqLine::CalcExonIndex(QVector<int> &vec_pos, int pos)
{
    for(int i=0;i<vec_pos.size()-1;i++)
    {
        if(pos >= vec_pos[i]+1 && pos <= vec_pos[i+1])
        {
            return  i+1;
        }
    }
    return vec_pos.size()-1;
}

QString& SeqLine::GetSeqLineName()
{
    return m_strName;
}

int SeqLine::GetStartindex()
{
    return m_startindex;
}

int SeqLine::GetEndindex()
{
    return m_endindex;
}

void SeqLine::SetIsRare(bool flag)
{
    m_isRare = flag;
}

bool SeqLine::GetIsRare()
{
    return m_isRare;
}

bool SeqLine::GetIsIndel()
{
    if(m_map_inl.empty() && m_map_del.empty())
    {
        return false;
    }
    return true;
}
