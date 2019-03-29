#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QEvent>
#include <QSet>
#include "all_base_struct.h"
#include <QListWidget>
#include <QTableWidgetItem>

namespace Ui {
class OpenFileDialog;
}

//class QComboBoxNew : public QComboBox
//{
//    Q_OBJECT
//public:
//    QComboBoxNew(QWidget *parent=0)
//        :QComboBox(parent)
//    {}
//protected:
//    bool event(QEvent *event)
//    {
//        if(event->type() == QEvent::Wheel) //屏蔽鼠标滚轮消息
//        {
//            event->ignore();
//            return true;
//        }
//        else
//        {
//            return QComboBox::event(event);
//        }
//    }
//};

class OpenFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenFileDialog(QWidget *parent = 0);
    ~OpenFileDialog();
private:
    void InitUi();                          //初始化界面
    void InitData();                        //从数据库获取初始数据（Gene GSSP）
    void ConnectSignalandSlot();            //连接信号与槽函数
    void SlotOpenFile();                    //响应弹出文件对话框
    void SlotAnalysisFile();                //响应解析文件，使用线程池
    void SlotCheckAll(int state);           //响应全选勾选框
    void SlotDelSelect();                   //响应删除选中项
//    void FilePathListProcess(const QStringList &filePathList);      //处理选中的文件列表
//    void FilePathProcess(const QString &filePath);                  //处理选中的文件
    bool AnalysisFileName(const QString &filePath, OpenFileTable &openFileTable);       //从文件名中提取信息
    //void AddRowToTableWidget(const OpenFileTable &openFileTable);                   //添加到对话框的文件列表中
    //void SetComboBoxData(QComboBox *box, const QString &text);
    bool AnalysisExonInfo(const QString &exonString, OpenFileTable &openFileTable);     //从文件名提取外显子信息
    void SetProcessbarValue();
    void slotprocessone(const OpenFileTable &info);
    void AddRowToTableWidget_s(const OpenFileTable &openFileTable);
    void showPopupList(int row, int column);
    void changetableitem(QListWidgetItem *item);
    void FilePathProcess_s(const QString &filePath, OpenFileTable &info);
private:
    Ui::OpenFileDialog *ui;

    QStringList m_geneNames_List;               //基因名称列表
    QStringList m_gsspNames_List;               //gssp名称列表
    QStringList m_exonNames_List;               //外显子名称列表
    QStringList m_ROrFNames_List;               //正序，反序名称列表
    QSet<QString> m_set_File;                  //列表文件集合
    QSet<QString> m_set_sample;                 //样品set
    //QMap<QString, ExonAndRF> m_map_ExonAndRF;  //外显子和正反序map
    int m_iPrgvalue;                            //进度条数值
    int m_iOpenfile;                            //选中导入的文件数目

    QListWidget *m_geneNames_ptr;
    QListWidget *m_gene_panel_ptr;
    QListWidget *m_gsspNames_ptr;
    QListWidget *m_exonNames_ptr;
    QListWidget *m_ROrFNames_ptr;

    QTableWidgetItem *m_tableitem_ptr;
};

#endif // OPENFILEDIALOG_H
