#include "setdlg.h"
#include "ui_setdlg.h"
#include "Core/core.h"

SetDlg::SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDlg)
{
    ui->setupUi(this);
    ConnectSignalandSlot();
    QString strig;
    Core::GetInstance()->GetConfig("Set/Ignore", strig);
    if(strig == "1")
    {
        ui->checkBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox->setCheckState(Qt::Unchecked);
    }
}

SetDlg::~SetDlg()
{
    delete ui;
}

void SetDlg::ConnectSignalandSlot()
{
    connect(ui->btnReset, &QPushButton::clicked, this, &SetDlg::slotClickResetButton);
    connect(ui->btnSave, &QPushButton::clicked, this, &SetDlg::slotClickSaveButton);
    connect(ui->btnExit, &QPushButton::clicked, this, &SetDlg::close);
}

void SetDlg::slotClickResetButton()
{
    ui->checkBox->setCheckState(Qt::Checked);
    Core::GetInstance()->SetConfig("Set/Ignore", "1");
}

void SetDlg::slotClickSaveButton()
{
    if(ui->checkBox->isChecked())
    {
        Core::GetInstance()->SetConfig("Set/Ignore", "1");
    }
    else
    {
        Core::GetInstance()->SetConfig("Set/Ignore", "0");
    }

    close();
}
