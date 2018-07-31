#include "usercommentdlg.h"
#include "database/realtimedatabase.h"
UserCommentDlg::UserCommentDlg(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("User Comment"));
    setDefault();
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QVBoxLayout *vLayout1 = new QVBoxLayout;
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;

    QLabel *sampleLabel = new QLabel("Sample Name:", this);
    hLayout1->addWidget(sampleLabel);
    hLayout1->addWidget(sampleNameLine_);

    vLayout1->addWidget(saveBt_);
    vLayout1->addWidget(exitBt_);
    hLayout2->addWidget(commentEdit_);
    hLayout2->addLayout(vLayout1);

    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    resize(280,100);
    connect(commentEdit_, SIGNAL(textChanged()), this, SLOT(slotCommentChanged()));
    connect(saveBt_, SIGNAL(clicked()), this, SLOT(slotClickSaveBt()));
    connect(exitBt_, SIGNAL(clicked()), this, SLOT(close()));
}

void UserCommentDlg::setDefault()
{
    sampleNameLine_ = new QLineEdit(this);
    sampleNameLine_->setEnabled(false);

    commentEdit_ = new QTextEdit(this);
    commentEdit_->setWindowTitle("set user comments");

    saveBt_ = new QPushButton("save", this);
    saveBt_->setEnabled(false);
    exitBt_ = new QPushButton("exit", this);
}

void UserCommentDlg::setSampleName(const QString &sampleName)
{
    sampleName_ = sampleName;
    sampleNameLine_->setText(sampleName);
    QString info = getSetNoteBySampleName(sampleName_.toAscii());
    commentEdit_->setText(info);
}

void UserCommentDlg::slotCommentChanged()
{
    saveBt_->setEnabled(true);
}

void UserCommentDlg::slotClickSaveBt()
{
    updateSetNoteBySampleName(sampleName_.toAscii(), commentEdit_->toPlainText());
    close();
}
