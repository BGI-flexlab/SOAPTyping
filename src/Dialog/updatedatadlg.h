#ifndef UPDATEDATADLG_H
#define UPDATEDATADLG_H

#include <QDialog>
#include <QMessageBox>
#include <QMap>

namespace Ui {
class UpdateDataDlg;
}

class UpdateDataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDataDlg(QWidget *parent = nullptr);
    ~UpdateDataDlg();
private:
    void ConnectSignalandSlot();
    bool isFileExists(const QString &file);
    void UpdateDatabase();
private slots:
    void slotOpenFileFlg();
    void slotApply();
    void slotClear();
    void slotInsert();
    void slotOpenDir();
private:
    Ui::UpdateDataDlg *ui;
    QString m_str_dir;
};

class SeqLine
{
public:
    SeqLine(const QString &name);
    ~SeqLine();
    void AppendSeqArry(char ch);
    void AppendSeq(const QByteArray &arry);
    QByteArray& GetSeqArry();
    void InsertByType(int type, int pos, char ch);
    int CalcSeqInfo(QVector<int> &vec_index);
    int GetSeqLineLenth();
    void SetClassNumber(int number);
    int GetClassNumber();
    int CalcExonIndex(QVector<int> &vec_pos, int pos);
    QString& GetSeqLineName();
    int GetStartindex();
    int GetEndindex();
    void SetIsRare(bool flag);
    bool GetIsRare();
    bool GetIsIndel();
    QMap<int,char>& GetMapInfo(int type);
private:
    bool m_isRare;
    bool m_isIndel;
    int m_classesNumber;

    int m_startindex;
    int m_endindex;
    int m_reallen;

    QByteArray m_seqArry;
    QString m_strName;
    QMap<int,char> m_map_change;
    QMap<int,char> m_map_inl;
    QMap<int,char> m_map_del;
};

#endif // UPDATEDATADLG_H
