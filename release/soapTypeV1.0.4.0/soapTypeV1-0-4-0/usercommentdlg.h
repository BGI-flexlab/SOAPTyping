#ifndef USERCOMMENTDLG_H
#define USERCOMMENTDLG_H
#include <QtCore>
#include <QtGui>

class UserCommentDlg:public QDialog
{
    Q_OBJECT
public:
    UserCommentDlg(QWidget *parent=0);
    void setSampleName(const QString &sampleName);
private:
    void setDefault();
private slots:
    void slotClickSaveBt();
    void slotCommentChanged();
private:
    QLineEdit *sampleNameLine_;
    QTextEdit *commentEdit_;
    QPushButton *saveBt_;
    QPushButton *exitBt_;
    QString sampleName_;
};

#endif // USERCOMMENTDLG_H
