#ifndef _LOGINDLG_H_
#define _LOGINDLG_H_

#include <QtGui/QDialog>
class QLineEdit;
class CLoginDlg : public QDialog
{
    Q_OBJECT
public:
    CLoginDlg(QWidget* = 0);
    virtual ~CLoginDlg();
    void setOK(bool *ok);
public slots:
    virtual void accept();
private:
    void getDefaultUser();
    void setDefaultUser();
private:
    QLineEdit*	usrLineEdit;
    QLineEdit*	pwdLineEdit;
    bool *ok_;
};
#endif
