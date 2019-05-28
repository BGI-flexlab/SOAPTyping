#ifndef CORE_H
#define CORE_H

#include "all_base_struct.h"
#include <QIcon>
#include <QSettings>

typedef struct _tagMyalign
{
    std::vector<char> ref_back;
    std::vector<char> seq_back;
    int ref_start;
    int ref_stop;
    int seq_start;
    int seq_stop;
}Myalign;

class Core
{
public:
    static Core *GetInstance()
    {
        static Core instance;
        return &instance;
    }
    ~Core();
    void AnalysisAb1();
    void GetFileAlignResult(FileAlignNew &file_align_new, FileAlignResult &result, bool auto_cut);
    void Align_LCS(char *s1, char *s2, align *sg, int length);
    bool Optimize_boundary(align *nw, FileAlignResultNew *result, bool auto_cut);
//    unsigned int format(char a);
//    char reformat(unsigned int a);
    int equal(char a, char b);
    char *reverse(char *s);
    bool can_merge(char A, char B);
    unsigned int formatMerge(char A);
    char reFormatMerge(unsigned int a);
    char mergeBases(char A, char B);
    QIcon getIcon(int analysisType, int markType);
    QString getAnalysisType(int type);
    QString getMarkType(int type);
    void SetConfig(const QString &key, const QString &value);
    void GetConfig(const QString &key, QString &value);
    bool isEqualPC(char A, char B);
    void zzk(const char *ref, const char *seq, Myalign &zlign);
    bool zzk_boundary(const char *ref, const char *seq, Myalign &zlign, FileAlignResultNew *result);
    void Align_LCS_new(const char *ref, const char *seq, FileAlignResultNew *result);
private: //禁用构造函数
    Core();
    Core(const Core &){}
    Core& operator=(const Core&){}

private:
    QSettings *m_pConfigSet;                    //读写ini文件
};

#endif // CORE_H
