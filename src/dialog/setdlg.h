#ifndef SETDLG_H
#define SETDLG_H

#include <QDialog>

namespace Ui {
class SetDlg;
}

class SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetDlg(QWidget *parent = nullptr);
    ~SetDlg();
private:
    void ConnectSignalandSlot();
private slots:
    void slotClickResetButton();
    void slotClickSaveButton();
private:
    Ui::SetDlg *ui;
};

#endif // SETDLG_H
