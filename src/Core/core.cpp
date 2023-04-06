#include "core.h"
#include <QDebug>

#define LEFT 1
#define TOP  2
#define DIAG 3

#define SCORE_SAME 2
#define SCORE_MATCH 1
#define SCORE_MISMATCH -1
#define SCORE_INSERTION -1
#define SCORE_DELETION -1

#define MIN_MIS 15
#define MIN_CUT_MIS 10

Core::Core()
{
    m_pConfigSet = new QSettings("./config.ini", QSettings::IniFormat);
}

Core::~Core()
{
    delete m_pConfigSet;
    m_pConfigSet = nullptr;
}

void Core::AnalysisAb1()
{

}

int Core::equal(char a, char b)//简并碱基有相同的碱基，认为是相同（H=Y H=W）
{
    int fa = formatMerge(a);
    int fb = formatMerge(b);

    return fa & fb;
}

char *Core::reverse(char *s)
{
    int m = strlen(s);
    int i;
    char *rs = (char*) malloc(sizeof(char)*(m+1));

    for(i = 0; i < m; i++)
        rs[i] = s[m-1-i];

    rs[m] = '\0';
    return rs;
}


bool Core::can_merge(char A, char B)
{
    if(A=='.' || B=='.')
        return false;
    if(B == 'N')
        return true;
    //return ((~formatMerge(A)) & formatMerge(B)) == 0;
    return equal(A, B);
}

void Core::GetFileAlignResult(FileAlignNew &file_align_new, FileAlignResult &result, bool auto_cut)
{
    if(strlen(file_align_new.consensus)==0 ||
            file_align_new.exclude_left_num+file_align_new.exclude_right_num >= strlen(file_align_new.raw_seq))
    {
        result.isUnDefined =1;
        return;
    }


    FileAlignResultNew tmp_result;
    tmp_result.left_cut = file_align_new.exclude_left_num;
    tmp_result.right_cut = file_align_new.exclude_right_num;

    //寻找consensus和raw_seq的最大公共子序列，并调整边界，把结果序列保存到tmp_result中


//    qDebug()<<"---start------";
//    qDebug()<<file_align_new.consensus<<"\r\n";
//    qDebug()<<file_align_new.raw_seq<<"\r\n";

    Align_LCS_new(file_align_new.consensus, file_align_new.raw_seq, &tmp_result);

    //qDebug()<<tmp_result.consensus_alignment<<"\r\n";
    //qDebug()<<tmp_result.sample_alignment<<"\r\n";

    int ref_len = strlen(file_align_new.consensus);
    int seq_len = strlen(file_align_new.raw_seq);
    result.sampleAlign = (char*) malloc(sizeof(char) * (ref_len+1));
    result.baseMatchConsensusPos = (int*) malloc(sizeof(int) * (seq_len));
    if(tmp_result.is_match)
    {
        result.isUnDefined = 0;
        result.leftLimit = tmp_result.left_cut;
        result.rightLimit = tmp_result.right_cut;//seq_len -tmp_result.right_cut;


        for(int i=0;i<seq_len;i++)
        {
            result.baseMatchConsensusPos[i] = -1;
        }

        int cmp_len = tmp_result.consensus_alignment.size(); //比对后的参考序列，其大小大于ref_len
        int index = 0;
        int pos = tmp_result.left_cut;
        for(int i=0;i<cmp_len;i++)
        {
            if(tmp_result.consensus_alignment[i] !='-') //参考序列经过比对处理，可能存在缺失情况，需要过滤掉
            {
                if(tmp_result.sample_alignment[i]=='-')
                {
                    result.sampleAlign[index]='-';
                   // qDebug()<<pos;
                }
                else
                {
                    result.sampleAlign[index]= tmp_result.sample_alignment[i];
                    if(pos<seq_len)
                    {
                       // qDebug()<<pos;
                      //  qDebug()<<index+file_align_new.consensus_start ;
                        //qDebug()<<file_align_new.consensus_start;
                      //  qDebug()<<"====";
                        result.baseMatchConsensusPos[pos++] = index+file_align_new.consensus_start;


                    }
                }
                index++;
            }
            else
            {
                pos++;
                cmp_len++;
                //result.rightLimit--;
            }
        }
        result.sampleAlign[ref_len] ='\0';
    }
    else
    {
        result.isUnDefined = 1;
    }
    //al.clear();

    //qDebug()<<result.sampleAlign;
}


unsigned int Core::formatMerge(char A)
{
    switch(A)
    {
    case '-': return 0x0;
    case 'A': return 0x1;
    case 'G': return 0x2;
    case 'T': return 0x4;
    case 'C': return 0x8;
    case 'R': return 0x3;
    case 'Y': return 0xc;
    case 'W': return 0x5;
    case 'S': return 0xa;
    case 'M': return 0x9;
    case 'K': return 0x6;
    case 'B': return 0xe;
    case 'D': return 0x7;
    case 'H': return 0xd;
    case 'V': return 0xb;
    case 'N': return 0xf;
    case '.':
    {
        //qDebug()<<__func__<<".";
        return 0x0;
    }
    case '*':
    {
        //qDebug()<<__func__<<"*";
        return 0x2f;
    }
    }
    return 0x0;
}

char Core::reFormatMerge(unsigned int a)
{
    switch(a)
    {
    case 0x0: return '-';
    case 0x1: return 'A';
    case 0x2: return 'G';
    case 0x4: return 'T';
    case 0x8: return 'C';
    case 0x3: return 'R';
    case 0xc: return 'Y';
    case 0x5: return 'W';
    case 0xa: return 'S';
    case 0x9: return 'M';
    case 0x6: return 'K';
    case 0xe: return 'B';
    case 0x7: return 'D';
    case 0xd: return 'H';
    case 0xb: return 'V';
    case 0xf: return 'N';
    case 0x1f: return '.';
    default:
        return '-';
    }
}

char Core::mergeBases(char A, char B)
{
    unsigned int a = formatMerge(A);
    unsigned int b = formatMerge(B);
    return reFormatMerge(a|b);
}


QIcon Core::getIcon(int analysisType, int markType)
{
    return QIcon(QString(":/png/images/filetree%1%2.png").arg(markType).arg(analysisType));
}

QString Core::getAnalysisType(int type)
{
    switch(type)
    {
    case 0:
        return "Match(common)";
    case 1:
        return "Match(rare)";
    case 2:
        return "Match(bad quality)";
    case 3:
        return "Mismatch";
    default:
        return "Undefined";
    }
    return "Undefined";
}

QString Core::getMarkType(int type)
{
    switch(type)
    {
    case 0:
        return "OWNED";
    case 1:
        return "PENDING";
    case 2:
        return "REVIEWED";
    case 3:
        return "APPROVED";
    default:
        return "OWNED";
    }
    return "OWNED";
}

void Core::SetConfig(const QString &key, const QString &value)
{
    m_pConfigSet->setValue(key, value);
}

void Core::GetConfig(const QString &key,QString &value)
{
    value = m_pConfigSet->value(key).toString();
}

bool Core::isEqualPC(char A, char B)
{
    unsigned int a = formatMerge(A);
    unsigned int b = formatMerge(B);
    return (a|b)==a;
}


void Core::Align_LCS_new(const char *ref, const char *seq, FileAlignResultNew *result)
{
    int m = strlen(ref);
    qDebug() << m;
    int n = strlen(seq);
    if (n>1100){
        n = 1100;
    }
    int maxscore = 0, x = 0, y = 0;
    std::vector<std::vector<int>> vec_matrix(m, std::vector<int>(n+1, 0));
    //空矩阵设为mx(n+1)，保证最右一个列全是0
    std::vector<char> ref_back;
    std::vector<char> seq_back;

    for (int i = 0; i < m ; i++)
    {

        for (int j = 0; j < n ; j++)
        {

            if (equal(ref[i], seq[j]))
            {
                if (i == 0 || j == 0)
                {
                    vec_matrix[i][j] = 1;
                }
                else
                {
                    vec_matrix[i][j] = vec_matrix[i - 1][j - 1] + 1;
                }
            }

            if (maxscore < vec_matrix[i][j])
            {
                maxscore = vec_matrix[i][j];
                x = i;
                y = j;
            }
        }
    }

    int ref_stop = x+1;
    int seq_stop = y+1;

 /*   for (int x = 0; x<m;x++){

        for (int y = 0; y <n; y++){
            qDebug()<<vec_matrix[x][y];
        }
        qDebug()<<"============";
    }
    */
    qDebug()<<maxscore;

    ref_back.clear();
    seq_back.clear();
    while (maxscore>0)
    {
        //小于10的片段不考虑
        if (vec_matrix[x][y]<(vec_matrix[x][y+1]-10)){
            ref_back.push_back(ref[x--]);
            seq_back.push_back('-');
           // qDebug()<<maxscore<<" <  "<<vec_matrix[x+1][y+1];
            maxscore = vec_matrix[x+1][y+1]-1;

        }else{
            ref_back.push_back(ref[x--]);
            seq_back.push_back(seq[y--]);
            maxscore--;
        }
        //qDebug()<<maxscore;
    }

//    while (x >= 0 && y >= 0)
//    {
//        if (equal(ref[x], seq[y]))
//        {
//            ref_back.push_back(ref[x]);
//            seq_back.push_back(seq[y]);
//            x--;
//            y--;
//        }
//        else
//        {
//            ref_back.push_back('-');
//            seq_back.push_back(seq[y]);
//            y--;
//        }

//    }

    while (x >= 0)
    {
        ref_back.push_back(ref[x--]);
        seq_back.push_back('-');
    }

    std::reverse(ref_back.begin(), ref_back.end());
    std::reverse(seq_back.begin(), seq_back.end());

    int dif_num = 0;
    std::vector<char> ref_back_tmp;
    std::vector<char> seq_back_tmp;
    int ref_stop_tmp = ref_stop;
    int seq_stop_tmp = seq_stop;
    while (ref_stop < m && seq_stop< n)
    {
        if (equal(ref[ref_stop], seq[seq_stop]))
        {
            ref_back_tmp.push_back(ref[ref_stop]);
            seq_back_tmp.push_back(seq[seq_stop]);
            ref_stop++;
            seq_stop++;
        }
        else
        {
            ref_back_tmp.push_back('-');
            seq_back_tmp.push_back(seq[seq_stop]);
            seq_stop++;
            dif_num++;
        }
    }

    if(dif_num > 5) //不一致的过多
    {
        ref_stop = ref_stop_tmp;
        seq_stop = seq_stop_tmp;
//        seq_stop = seq_stop_tmp+(m-ref_stop);
//        if(seq_stop>n)
//        {
//            seq_stop = n;
//        }
    }
    else
    {
        for(auto it=ref_back_tmp.begin();it!=ref_back_tmp.end();it++)
        {
            ref_back.push_back(*it);
        }

        for(auto it=seq_back_tmp.begin();it!=seq_back_tmp.end();it++)
        {
            seq_back.push_back(*it);
        }
    }
    while (ref_stop < m)
    {
        ref_back.push_back(ref[ref_stop]);
        seq_back.push_back('-');
        ref_stop++;
    }

    result->consensus_alignment.append(ref_back.data(), ref_back.size());
    result->sample_alignment.append(seq_back.data(), seq_back.size());
    result->left_cut = y+1;
    result->right_cut = seq_stop;
    result->is_match = true;
}
