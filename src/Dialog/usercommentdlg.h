#ifndef USERCOMMENTDLG_H
#define USERCOMMENTDLG_H

#include <QDialog>

namespace Ui {
class UserCommentDlg;
}

class UserCommentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserCommentDlg(QWidget *parent = nullptr);
    ~UserCommentDlg();
    void setSampleName(const QString &sampleName);

private slots:
    void slotSave();
private:
    Ui::UserCommentDlg *ui;
    QString m_str_SampleName;
};

#endif // USERCOMMENTDLG_H
