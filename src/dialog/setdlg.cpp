#include "setdlg.h"
#include "ui_setdlg.h"

SetDlg::SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDlg)
{
    ui->setupUi(this);
    ConnectSignalandSlot();
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

}

void SetDlg::slotClickSaveButton()
{
    close();
}
