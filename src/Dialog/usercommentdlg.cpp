#include "usercommentdlg.h"
#include "ui_usercommentdlg.h"
#include "DataBase/soaptypingdb.h"

UserCommentDlg::UserCommentDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserCommentDlg)
{
    ui->setupUi(this);

    connect(ui->btnSave, &QPushButton::clicked, this, &UserCommentDlg::slotSave);
    connect(ui->btnExit, &QPushButton::clicked, this, &UserCommentDlg::close);
}

UserCommentDlg::~UserCommentDlg()
{
    delete ui;
}

void UserCommentDlg::setSampleName(const QString &sampleName)
{
    m_str_SampleName = sampleName;
    ui->lineEdit->setText(m_str_SampleName);
    QString str_noteinfo;
    SoapTypingDB::GetInstance()->getSetNoteFromSampleTable(sampleName, str_noteinfo);
    if(!str_noteinfo.isEmpty())
    {
        ui->textEdit->setText(str_noteinfo);
    }
}

void UserCommentDlg::slotSave()
{
    SoapTypingDB::GetInstance()->updateSetNoteBySampleName(m_str_SampleName, ui->textEdit->toPlainText());
    close();
}

