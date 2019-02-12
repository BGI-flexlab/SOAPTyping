#include "core.h"


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

int Core::equal(char a, char b)
{
    int fa = 0, fb = 0;
    fa = format(a);
    fb = format(b);

    return fa & fb;
}

unsigned int Core::format(char a)
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
    return ((~format(A)) & format(B)) == 0;
}

void Core::GetFileAlignResult(FileAlignNew &file_align_new, FileAlignResult &result, bool auto_cut)
{
    if(strlen(file_align_new.consensus)==0 ||
            file_align_new.exclude_left_num+file_align_new.exclude_right_num >= strlen(file_align_new.raw_seq))
    {
        result.isUnDefined =1;
        return;
    }

    align al;
    al.r1 = NULL;
    al.r2 = NULL;
    FileAlignResultNew tmp_result;
    tmp_result.left_cut = file_align_new.exclude_left_num;
    tmp_result.right_cut = file_align_new.exclude_right_num;
    int orange_length = strlen(file_align_new.raw_seq);
    int align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;

    //寻找consensus和raw_seq的最大公共子序列，并调整边界，把结果序列保存到tmp_result中
    Align_LCS(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
    while(!Optimize_boundary(&al, &tmp_result, auto_cut)){
        align_length = orange_length - tmp_result.left_cut-tmp_result.right_cut;
        Align_LCS(file_align_new.consensus, file_align_new.raw_seq+tmp_result.left_cut, &al, align_length);
    }

    result.sampleAlign = (char*) malloc(sizeof(char) * (strlen(file_align_new.consensus)+1));
    result.baseMatchConsensusPos = (int*) malloc(sizeof(int) * (strlen(file_align_new.raw_seq)));
    if(tmp_result.is_match)
    {
        result.isUnDefined = 0;
        result.leftLimit = tmp_result.left_cut;
        result.rightLimit = strlen(file_align_new.raw_seq) -tmp_result.right_cut;
        int start=0, end=tmp_result.sample_alignment.size();
        for(int i=0; i<end; i++){
            if(tmp_result.sample_alignment[i]=='.'){
                result.sampleAlign[i]='-';
                start++;
            }else{
                break;
            }
        }

        int c_index = strlen(file_align_new.consensus);
        for(int i=end-1; i>=start; i--)
        {
            if(tmp_result.sample_alignment[i]=='.'){
                result.sampleAlign[--c_index]='-';
                end--;
            }else{
                break;
            }
        }
        int index=0;
        for(int i=0; i<tmp_result.left_cut; i++){
            result.baseMatchConsensusPos[index]=-1;
            index++;
        }
        c_index = start;
        for(int i=start; i<end; i++){
            if(tmp_result.consensus_alignment[i] != '.'){
                result.sampleAlign[c_index]=tmp_result.sample_alignment[i];
                if(tmp_result.sample_alignment[i]!='.'){
                    result.baseMatchConsensusPos[index]=c_index+file_align_new.consensus_start;
                    index++;
                }
                c_index++;
            }else{
                result.baseMatchConsensusPos[index]=-1;
                index++;
            }
        }
        for(int i=0; i<tmp_result.right_cut;i++){
            result.baseMatchConsensusPos[index]=-1;
            index++;
        }
        result.sampleAlign[strlen(file_align_new.consensus)]='\0';
    }
    else
    {
        result.isUnDefined = 1;
    }
    al.clear();
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
            if(!(equal(s1[i-1], s2[j-1]))||format(s2[j-1]) > format(s1[i-1]))
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
    if(nw->r1==NULL || nw->r2==NULL){
        result->is_match = false;
        return true;
    }

    char *ref = nw->r1;
    char *seq = nw->r2;
    int start = nw->start2, end=nw->stop2;
    int length = strlen(seq);
    if(nw->stop1<length)
    {
        result->right_cut+=length-nw->stop1;
        nw->r1[nw->stop1]='\0';
        nw->r2[nw->stop1]='\0';
        end = nw->stop1;
    }

    if(nw->start1>0)
    {
        result->left_cut+=nw->start1;
        ref = ref+nw->start1;
        seq = seq+nw->start1;
        end -= nw->start1;
    }

    length = end - start;

    int *mis= new int[length];
    mis[0]=0;
    int mis_index=0,max_leng=0;

    int left_mis=0, right_mis=0, half_pos = (start+end)/2;
    for(int i=start; i<end; i++)
    {
        if(seq[i]==ref[i] || can_merge(ref[i], seq[i]))
        {
            mis[mis_index]++;
        }
        else
        {
            if(max_leng<mis[mis_index])
                max_leng = mis[mis_index];
            mis_index++;
            mis[mis_index]=1;

            if(i<=half_pos)
                left_mis++;
            else
                right_mis++;
        }
    }

    int total_mis = left_mis+right_mis;
    if(max_leng<mis[mis_index]){
        max_leng = mis[mis_index];
    }

    if(total_mis > MIN_MIS)
    {
        if(left_mis<=right_mis)
            result->right_cut++;
        else
            result->left_cut++;
        delete []mis;
        return false;
    }


    // >= 50
    if(!auto_cut)
    {
        result->consensus_alignment = QByteArray(ref);
        result->sample_alignment = QByteArray(seq);
        result->is_match = true;
        delete[] mis;
        return true;
    }

    int left_cut=0,right_cut=0;
    for(int i=0; i<=mis_index; i++)
    {
        if(mis[i]>=MIN_CUT_MIS)
        {
            if(!can_merge(ref[start+left_cut], seq[start+left_cut]))
                left_cut++;
            break;
        }
        left_cut+=mis[i];
    }

    for(int i=mis_index; i>=0; i--)
    {
        if(mis[i]>=MIN_CUT_MIS)
            break;
        right_cut+=mis[i];
    }

    delete[] mis;
    if(left_cut==0 && right_cut==0)
    {
        result->consensus_alignment = QByteArray(ref);
        result->sample_alignment = QByteArray(seq);
        result->is_match = true;
        return true;
    }


    result->left_cut += left_cut;
    for(int i=start; i<start+left_cut; i++)
    {
        if(seq[i]=='.')
            result->left_cut--;
    }

    result->right_cut += right_cut;
    for(int i=end-right_cut; i<end; i++)
    {
        if(seq[i]=='.')
            result->right_cut--;
    }
    return false;
}

unsigned int Core::formatMerge(char A)
{
    switch(A)
    {
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
    case '-': return 0x0;
    case '.': return 0x1f;
    }
    return 0xf;
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
    case 0x1f: return '.';
    default:
        return 'n';
    }
    return 'n';
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
