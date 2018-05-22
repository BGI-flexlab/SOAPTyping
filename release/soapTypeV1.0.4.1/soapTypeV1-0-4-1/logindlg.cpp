#include <QtGui/QtGui>
#include "logindlg.h"
#include "config.h"

CLoginDlg::CLoginDlg(QWidget* parent)
 :	QDialog(parent)
{
    QLabel* usrLabel = new QLabel(tr("User ID:"));
    QLabel* pwdLabel = new QLabel(tr("Password:"));
	usrLineEdit = new QLineEdit;
	pwdLineEdit = new QLineEdit;
	pwdLineEdit->setEchoMode(QLineEdit::Password);
	QGridLayout* gridLayout = new QGridLayout;
	gridLayout->addWidget(usrLabel, 0, 0, 1, 1);
	gridLayout->addWidget(usrLineEdit, 0, 1, 1, 3);
	gridLayout->addWidget(pwdLabel, 1, 0, 1, 1);
	gridLayout->addWidget(pwdLineEdit, 1, 1, 1, 3);
	
    QPushButton* okBtn = new QPushButton(tr("Login"));
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"));
	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->setSpacing(60);
	btnLayout->addWidget(okBtn);
	btnLayout->addWidget(cancelBtn);	
	
	QVBoxLayout* dlgLayout = new QVBoxLayout;
	dlgLayout->setMargin(40);
	dlgLayout->addLayout(gridLayout);
	dlgLayout->addStretch(40);
	dlgLayout->addLayout(btnLayout);
	setLayout(dlgLayout);

    getDefaultUser();
	
	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	
    setWindowTitle(tr("Login"));
	resize(300, 200);
}

CLoginDlg::~CLoginDlg()
{
	delete usrLineEdit;
	delete pwdLineEdit;
}

void CLoginDlg::accept()
{
    if(usrLineEdit->text().trimmed() == tr("bgi-sss")
        && pwdLineEdit->text() == tr("sss-2013"))
	{
        *ok_ = true;
        setDefaultUser();
		QDialog::accept();
	}
	else
	{
		QMessageBox::warning(this, 
        tr("Warning"),
        tr("User ID or password is not correct!"),
		QMessageBox::Yes);
		usrLineEdit->setFocus();
	}

}

void CLoginDlg::setOK(bool *ok)
{
    ok_ = ok;
}

void CLoginDlg::getDefaultUser()
{
    QMap<QString, QString> config;
    readConfig(config);
    usrLineEdit->setText(config.find("UserID").value());
    pwdLineEdit->setText("");
}

void CLoginDlg::setDefaultUser()
{
    QMap<QString, QString> config;
    readConfig(config);
    config.insert("UserID", usrLineEdit->text());
    config.insert("Password", pwdLineEdit->text());
    writeConfig(config);
}
