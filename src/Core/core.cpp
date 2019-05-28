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

/*unsigned int Core::format(char a)
{
    switch(a)
    {
    case '-': return 0xf;
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
    case '*': return 0xf;
    case '.': return 0xf;
    }
    return 0x0;
}

char Core::reformat(unsigned int a)
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
    }
    return '-';
}*/

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

//    align al;
//    al.r1 = NULL;
//    al.r2 = NULL;
    FileAlignResultNew tmp_result;
    tmp_result.left_cut = file_align_new.exclude_left_num;
    tmp_result.right_cut = file_align_new.exclude_right_num;
    //int orange_length = strlen(file_align_new.raw_seq);
   // int align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;

    //寻找consensus和raw_seq的最大公共子序列，并调整边界，把结果序列保存到tmp_result中
//    Align_LCS(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
//    while(!Optimize_boundary(&al, &tmp_result, auto_cut)){
//        align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;
//        Align_LCS(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
//    }

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
                }
                else
                {
                    result.sampleAlign[index]= tmp_result.sample_alignment[i];
                    if(pos<seq_len)
                    {
                        result.baseMatchConsensusPos[pos++] = index+file_align_new.consensus_start;
                    }
                }
                index++;
            }
            else
            {
                pos++;
                result.rightLimit--;
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

void Core::Align_LCS(char *s1, char *s2, align *sg, int length)
{
    int row_best, col_best;
    int best_i, best_j;
    int bt;
    int score, tmp;
    int i, j;
    int q = 0, k = 0;
    int direction;
    int best;
    int m, n;
    int a;
    sg->clear();
    n = length;  //入参，想要比较的长度,小于等于s2的长度
    m = strlen(s1); //字符串S1的长度

    if(n<=0 || m<=0)
        return;

    //the DP and backtrace table are both stored column-wise.
    //int columns[n+1][m+1];
    //int backtrace[n+1][m+1];
    //申请一块内存，构造一个（n+1）*（m+1）的二维数组，并初始化

    int **columns = (int **)malloc(sizeof(int*)*(n+1));  //长度矩阵
    int **backtrace = (int **)malloc(sizeof(int*)*(n+1));//状态矩阵

    for (a=0; a<n+1; a++) {
        columns[a] = (int *)malloc(sizeof(int)*(m+1));
        backtrace[a] = (int *)malloc(sizeof(int)*(m+1));
    }

    for(j = 0; j < n+1; j++)
    {
        for(i = 0; i < m+1; i++)
        {
            columns[j][i] = 0;
            backtrace[j][i] = 0;
        }
    }

    // fill DP table (using unit costs)
    // outer loop goes over columns
    for(j = 1; j < n+1; j++)
    {
        for(i = 1; i < m+1; i++)
        {
            bt = DIAG;
            score = columns[j-1][i-1] + (s1[i-1] == s2[j-1] ? SCORE_SAME : equal(s1[i-1], s2[j-1]) ? SCORE_MATCH : SCORE_MISMATCH);
            tmp = columns[j][i-1] + SCORE_INSERTION;
            if(tmp > score)
            {
                bt = LEFT;
                score = tmp;
            }
            tmp = columns[j-1][i] + SCORE_DELETION;
            if(tmp > score)
            {
                bt = TOP;
                score = tmp;
            }
            columns[j][i] = score;//把左，上，（左上角）三个值的最大值赋值给右下角，所以右边的值总是最大的
            backtrace[j][i] = bt;
        }
    }

    // find position with highest score in last column or last row
    row_best = columns[0][m]; //第0行最后一个
    col_best = columns[n][0]; //第n行第一个
    best_j = 0;
    best_i = 0;
    for(j = 0; j < n+1; j++)
    {
        if(row_best < columns[j][m])
        {
            row_best = columns[j][m];
            best_j = j;
        }
    }//寻找最后一列的最大值，记录位置
    for(i = 0; i < m+1; i++)
    {
        if(col_best < columns[n][i])
        {
            col_best = columns[n][i];
            best_i = i;
        }
    }//寻找最后一行的最大值，记录位置
    if(row_best > col_best) //如果最后一列最大值比最后一行的最大值大，不用问，best_i直接赋值为m(最后一列)
    {
        best_i = m;
        best = row_best;
    }
    else
    {
        best_j = n;
        best = col_best;
    }//比较上述两个最大值，赋值给best

    // now trace back (from "lower right" corner)
    char *re1 = (char*) malloc(sizeof(char)*(m+n));
    char *re2 = (char*) malloc(sizeof(char)*(m+n));
    i = m;//列
    j = n;//行

    // first, walk from the lower right corner to the
    // position where we found the maximun score
    if(i == best_i) // we are in the last row
    {
        while(j > best_j)
        {
            re1[k++] = '.';
            re2[q++] = s2[j-1];
            j -= 1;
        }
    }
    else // we are in the last column
    {
        while(i > best_i)
        {
            re1[k++] = s1[i-1];
            re2[q++] = '.';
            i -= 1;
        }
    }

    // now, the actual backtracing.
    // we build reverse sequences while backtracing and
    // reverse them afterwards.
    while(i > 0 && j > 0) // columns[j][i] > 0
    {
        score = columns[j][i];
        direction = backtrace[j][i];
        if (direction == DIAG)
        {
            if(!(equal(s1[i-1], s2[j-1]))||formatMerge(s2[j-1]) > formatMerge(s1[i-1]))
                sg->errors += 1;
            re1[k++] = s1[i-1];
            re2[q++] = s2[j-1];
            i -= 1;
            j -= 1;
        }
        else if(direction == TOP)
        {
            re1[k++] = '.';
            re2[q++] = s2[j-1];
            sg->errors += 1;
            j -= 1;
        }
        else if(direction == LEFT)
        {
            re1[k++] = s1[i-1];
            re2[q++] = '.';
            sg->errors += 1;
            i -= 1;
        }
    }

    //    sg->start1 = i;
    //    sg->start2 = j;

    // continue from where we are to the upper left corner
    while(j > 0)
    {
        re1[k++] = '.';
        re2[q++] = s2[j-1];
        j -= 1;
    }
    while(i > 0)
    {
        re1[k++] = s1[i-1];
        re2[q++] = '.';
        i -= 1;
    }
    re1[k] = '\0';
    re2[q] = '\0';


    sg->r1 = reverse(re1);
    sg->r2 = reverse(re2);

    i=0;
    while(sg->r1[i] == '.')
        i++;
    sg->start1 =i;
    i=0;
    while(sg->r2[i] == '.')
        i++;
    sg->start2 = i;
    i=strlen(sg->r1);
    while(sg->r1[i-1]=='.')
        i--;
    sg->stop1 = i;
    i=strlen(sg->r2);
    while(sg->r2[i-1]=='.')
        i--;
    sg->stop2 = i;

    for (a = 0; a < n+1; a++)
    {
        free(columns[a]);
        free(backtrace[a]);
    }
    free(columns);
    free(backtrace);
    free(re1);
    free(re2);

    return;
}

bool Core::Optimize_boundary(align *nw, FileAlignResultNew *result, bool auto_cut)
{
    if(nw->r1==nullptr || nw->r2==nullptr){
        result->is_match = false;
        return true;
    }

    char *ref = nw->r1;
    char *seq = nw->r2;

    int start = nw->start1 > nw->start2 ? nw->start1 : nw->start2;
    int end = nw->stop1 < nw->stop2 ? nw->stop1 : nw->stop2;
    int length = end - start;

    if (nw->start1 > 0)//参照ref，掐头
    {
        result->left_cut += nw->start1;
    }

    int *mis = new int[length];
    mis[0] = 0;
    int mis_index = 0;
    int total_mis = 0;

    for (int i = start; i < end; i++)
    {
        //if (seq[i] == ref[i] || can_merge(ref[i], seq[i]))
        if(equal(ref[i], seq[i]))
        {
            mis[mis_index]++;
        }
        else
        {
            mis_index++;
            mis[mis_index] = 1;

            total_mis++;
        }
    }


    if (total_mis > MIN_MIS)
    {
        if (mis[0] > MIN_CUT_MIS || mis[0] == 0)
        {
            result->left_cut++;
        }
        else
        {
            result->left_cut += mis[0];
        }

        delete[]mis;
        return false;
    }
    else
    {

        if (nw->stop1 < strlen(seq))//参照ref，去尾
        {
            result->right_cut += strlen(seq) - nw->stop1;
            nw->r1[nw->stop1]='\0';
            nw->r2[nw->stop1]='\0';
        }

        if (total_mis != 0)
        {
            int len_start = mis[0];
            if (len_start< MIN_CUT_MIS)
            {
                result->left_cut += len_start;
                for (int i=0;i< len_start;i++)
                {
                    nw->r2[start + i] = '.';
                }

            }
            int len_end = mis[total_mis];
            if (len_end < MIN_CUT_MIS)
            {
                result->right_cut += len_end;
                for (int i = len_end; i > 0; i--)
                {
                    nw->r2[end - i] = '.';
                }
            }
        }

        result->consensus_alignment = ref+nw->start1;
        result->sample_alignment = seq + nw->start1;
        result->is_match = true;
        delete[]mis;
        return true;
    }
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


//Smith-Waterman
typedef struct _tagLCS
{
    int score;
    int trace;
}LCS;

void Core::zzk(const char *ref, const char *seq, Myalign &zlign)
{
    int m = strlen(ref);
    int n = strlen(seq);
    int maxscore=0, x=0, y=0;
    std::vector<std::vector<LCS>> vec_matrix(m + 1);

    int bt = 0;
    int score = 0, tmp = 0;
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            LCS lcs_zzk;
            lcs_zzk.score = 0;
            lcs_zzk.trace = 0;
            vec_matrix[i].push_back(lcs_zzk);
            if (i == 0 || j == 0)
            {
                continue;
            }

            bt = DIAG;
            tmp = ref[i - 1] == seq[j - 1] ? SCORE_SAME : equal(ref[i - 1], seq[j - 1]) ? SCORE_MATCH : SCORE_MISMATCH;
            score = vec_matrix[i - 1][j - 1].score + tmp;
            tmp = vec_matrix[i][j - 1].score - 2;
            if (tmp > score)
            {
                bt = LEFT;
                score = tmp;
            }
            tmp = vec_matrix[i - 1][j].score - 2;
            if (tmp > score)
            {
                bt = TOP;
                score = tmp;
            }
            vec_matrix[i][j].score = score < 0? 0: score;//把左，上，（左上角）三个值的最大值赋值给右下角，所以右边的值总是最大的
            vec_matrix[i][j].trace = bt;

            if (maxscore < vec_matrix[i][j].score)
            {
                maxscore = vec_matrix[i][j].score;
                x = i;
                y = j;
            }
        }
    }

    zlign.ref_back.clear();
    zlign.seq_back.clear();
    zlign.ref_stop = x;
    zlign.seq_stop = y;

    while (x>0 && y>0)
    {
        if (vec_matrix[x][y].trace == DIAG)
        {
            zlign.ref_back.push_back(ref[x-1]);
            zlign.seq_back.push_back(seq[y-1]);
            x--;
            y--;
        }
        else if (vec_matrix[x][y].trace == TOP)
        {
            zlign.ref_back.push_back(ref[x - 1]);
            zlign.seq_back.push_back('-');
            x--;
        }
        else if (vec_matrix[x][y].trace == LEFT)
        {
            zlign.ref_back.push_back('-');
            zlign.seq_back.push_back(seq[y-1]);
            y--;
        }
    }

    zlign.ref_start = x;
    zlign.seq_start = y;


    std::reverse(zlign.ref_back.begin(), zlign.ref_back.end());
    std::reverse(zlign.seq_back.begin(), zlign.seq_back.end());
}



typedef struct _tagcomp
{
    bool ismatch;
    int len;
    int start_pos;
    int end_pos;
}comp;


bool Core::zzk_boundary(const char *ref, const char *seq, Myalign &zlign, FileAlignResultNew *result)
{
    std::vector<comp> vec_mis_match;
    int i_mis = 0;
    int i_match = 0;
    int start_pos = 0;
    int end_pos = zlign.ref_back.size();

    for (int i = 0; i < end_pos; i++)
    {
        if (equal(zlign.ref_back[i], zlign.seq_back[i]))
        {
            if (i_mis)
            {
                comp c_com;
                c_com.ismatch = false;
                c_com.start_pos = start_pos;
                c_com.end_pos = i;
                c_com.len = i_mis;
                vec_mis_match.push_back(c_com);
                i_mis = 0;
                start_pos = i;
            }
            i_match++;
        }
        else
        {
            if (i_match)
            {
                comp c_com;
                c_com.ismatch = true;
                c_com.start_pos = start_pos;
                c_com.end_pos = i;
                c_com.len = i_match;
                vec_mis_match.push_back(c_com);
                i_match = 0;
                start_pos = i;
            }
            i_mis++;
        }
    }

    if (i_mis)
    {
        comp c_com;
        c_com.ismatch = false;
        c_com.start_pos = start_pos;
        c_com.end_pos = end_pos;
        c_com.len = i_mis;
        vec_mis_match.push_back(c_com);
    }
    if (i_match)
    {
        comp c_com;
        c_com.ismatch = true;
        c_com.start_pos = start_pos;
        c_com.end_pos = end_pos;
        c_com.len = i_match;
        vec_mis_match.push_back(c_com);
    }

    result->right_cut = zlign.seq_stop + result->left_cut;
    if (zlign.ref_start == 0)
    {
        result->left_cut += zlign.seq_start;
    }

    int total_mis = vec_mis_match.size();
    if (total_mis > MIN_MIS)
    {
        if (vec_mis_match[0].len > MIN_CUT_MIS || vec_mis_match[0].len == 0)
        {
            result->left_cut++;
        }
        else
        {
            result->left_cut += vec_mis_match[0].len;
        }

        return false;
    }
    else
    {

        std::sort(vec_mis_match.begin(), vec_mis_match.end(),
            [](comp &a, comp &b) {return a.len > b.len; }); //取最大的公共子串

        int ref_adjust = 1;
        int seq_adjust = 1;
        for (int p= 0;p< vec_mis_match[0].start_pos;p++)
        {
            if (zlign.ref_back[p]=='-')
            {
                ref_adjust++;
            }
            if (zlign.seq_back[p]=='-')
            {
                seq_adjust++;
            }
        }

        char *sz_ref = new char[512]; memset(sz_ref, 0, 512);
        char *sz_seq = new char[512]; memset(sz_seq, 0, 512);

        std::vector<char> ref_tmp;
        std::vector<char> seq_tmp;
        int i, j;
        i = vec_mis_match[0].start_pos + zlign.ref_start- ref_adjust;
        j = vec_mis_match[0].start_pos + result->left_cut- seq_adjust;
        while (i>0)//向前延伸
        {
            if (equal(ref[i], seq[j]))
            {
                ref_tmp.push_back(ref[i]);
                seq_tmp.push_back(seq[j]);
                i--;
                j--;
            }
            else
            {
                ref_tmp.push_back('-');
                seq_tmp.push_back(seq[j]);
                j--;
            }
            if (j < 0)
            {
                break;
            }
        }

        for (;i>=0;i--)
        {
            ref_tmp.push_back(ref[i]);
            seq_tmp.push_back('-');
        }

        int len_tmp = ref_tmp.size();
        for (int k=0;k< len_tmp;k++)
        {
            sz_ref[k] = ref_tmp[len_tmp - 1 - k];
            sz_seq[k] = seq_tmp[len_tmp - 1 - k];
        }

        int index=0;
        for (int k = vec_mis_match[0].start_pos; k < vec_mis_match[0].end_pos; k++,index++)
        {
            sz_ref[index+ len_tmp] = zlign.ref_back[k];
            sz_seq[index + len_tmp] = zlign.seq_back[k];
        }


        ref_tmp.clear();
        seq_tmp.clear();
        i = vec_mis_match[0].end_pos + zlign.ref_start;
        j = vec_mis_match[0].end_pos + result->left_cut;
        while (i < strlen(ref) && j< strlen(seq))//向后延伸
        {
            if (equal(ref[i], seq[j]))
            {
                ref_tmp.push_back(ref[i]);
                seq_tmp.push_back(seq[j]);
                i++;
                j++;
            }
            else
            {
                ref_tmp.push_back('-');
                seq_tmp.push_back(seq[j]);
                j++;
            }
        }

        for (;i< strlen(ref);i++)
        {
            ref_tmp.push_back(ref[i]);
            seq_tmp.push_back('-');
        }

        int len_tmp_1 = ref_tmp.size();
        for (int k = 0; k < len_tmp_1; k++)
        {
            sz_ref[k + len_tmp + vec_mis_match[0].len] = ref_tmp[k];
            sz_seq[k + len_tmp + vec_mis_match[0].len] = seq_tmp[k];
        }

        result->consensus_alignment.push_back(sz_ref);
        result->sample_alignment.push_back(sz_seq);
        result->is_match = true;

        delete [] sz_ref;
        delete [] sz_seq;
        return true;
    }

}


void Core::Align_LCS_new(const char *ref, const char *seq, FileAlignResultNew *result)
{
    int m = strlen(ref);
    int n = strlen(seq);
    int maxscore = 0, x = 0, y = 0;
    std::vector<std::vector<int>> vec_matrix(m, std::vector<int>(n, 0));
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

    ref_back.clear();
    seq_back.clear();
    while (maxscore>0)
    {
        ref_back.push_back(ref[x--]);
        seq_back.push_back(seq[y--]);
        maxscore--;
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
