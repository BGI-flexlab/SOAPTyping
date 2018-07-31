/*
 * =====================================================================================
 *
 *       Filename:  semigloble.c
 *
 *    Description:  semi globle align for hla
 *
 *        Version:  1.0
 *        Created:  2013-1-14 10:22:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equal.h"
#include "semigloble.h"
#include <QtCore>

char *reverse(char *s);

/*void semigloblalalign(char *s1, char *s2, align *sg)
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
    n = strlen(s2);
    m = strlen(s1);

    if(s2<=0 || s1<=0)
        return;

    //the DP and backtrace table are both stored column-wise.
    //int columns[n+1][m+1];
    //int backtrace[n+1][m+1];

    int **columns = (int **)malloc(sizeof(int*)*(n+1));
    int **backtrace = (int **)malloc(sizeof(int*)*(n+1));

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
            columns[j][i] = score;
            backtrace[j][i] = bt;
        }
    }

    // find position with highest score in last column or last row
    row_best = columns[0][m];
    col_best = columns[n][0];
    best_j = 0;
    best_i = 0;
    for(j = 0; j < n+1; j++)
    {
        if(row_best < columns[j][m])
        {
            row_best = columns[j][m];
            best_j = j;
        }
    }
    for(i = 0; i < m+1; i++)
    {
        if(col_best < columns[n][i])
        {
            col_best = columns[n][i];
            best_i = i;
        }
    }
    if(row_best > col_best)
    {
        best_i = m;
        best = row_best;
    }
    else
    {
        best_j = n;
        best = col_best;
    }

    // now trace back (from "lower right" corner)
    char *re1 = (char*) malloc(sizeof(char)*1024);
    char *re2 = (char*) malloc(sizeof(char)*1024);
    i = m;
    j = n;

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

    sg->start1 = i;
    sg->start2 = j;

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

    sg->stop1 = best_i-1;
    sg->stop2 = best_j-1;

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
}*/

void semigloblalalign_new(char *s1, char *s2, align *sg, int length)
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

//void semigloblalalign_new(char *s1, char *s2, align *sg, int length)
//{
//    int row_best, col_best;
//    int best_i, best_j;
//    int bt;
//    int score, tmp;
//    int i, j;
//    int q = 0, k = 0;
//    //    int direction;
//    int best;
//    int m, n;
//    int a;
//    sg->clear();
//    //n = strlen(s2);
//    n = length;
//    m = strlen(s1);

//    if(m<=0 || n<=0)
//        return;

//    //the DP and backtrace table are both stored column-wise.
//    //int columns[n+1][m+1];
//    //int backtrace[n+1][m+1];

//    int **columns = (int **)malloc(sizeof(int*)*(n+1));
//    int **backtrace = (int **)malloc(sizeof(int*)*(n+1));

//    for (a=0; a<n+1; a++) {
//        columns[a] = (int *)malloc(sizeof(int)*(m+1));
//        backtrace[a] = (int *)malloc(sizeof(int)*(m+1));
//    }

//    for(j = 0; j < n+1; j++)
//    {
//        for(i = 0; i < m+1; i++)
//        {
//            columns[j][i] = 0;
//            backtrace[j][i] = 0;
//        }
//    }
//    //    for(int j=0; j<n+1; j++)
//    //    {
//    //        columns[j][0]=SCORE_INSERTION*j;
//    //    }
//    // fill DP table (using unit costs)
//    // outer loop goes over columns
//    for(j = 1; j < n+1; j++)
//    {
//        for(i = 1; i < m+1; i++)
//        {
//            bt = DIAG;
//            score = columns[j-1][i-1] + (s1[i-1] == s2[j-1] ? NEW_SCORE_SAME : can_merge(s1[i-1], s2[j-1]) ? NEW_SCORE_MATCH : NEW_SCORE_MISMATCH);
//            tmp = columns[j][i-1] + NEW_SCORE_INSERTION;
//            if(tmp > score)
//            {
//                bt = LEFT;
//                score = tmp;
//            }
//            tmp = columns[j-1][i] + NEW_SCORE_DELETION;
//            if(tmp > score)
//            {
//                bt = TOP;
//                score = tmp;
//            }
//            columns[j][i] = score;
//            backtrace[j][i] = bt;
//        }
//    }

//    row_best = columns[0][m];
//    col_best = columns[n][0];
//    best_j = 0;
//    best_i = 0;
//    for(j = 0; j < n+1; j++)
//    {
//        if(row_best < columns[j][m])
//        {
//            row_best = columns[j][m];
//            best_j = j;
//        }
//    }
//    for(i = 0; i < m+1; i++)
//    {
//        if(col_best < columns[n][i])
//        {
//            col_best = columns[n][i];
//            best_i = i;
//        }
//    }
//    if(row_best > col_best)
//    {
//        best_i = m;
//        best = row_best;
//    }
//    else
//    {
//        best_j = n;
//        best = col_best;
//    }

//    // now trace back (from "lower right" corner)
//    char *re1 = (char*) malloc(sizeof(char)*(m+n));
//    char *re2 = (char*) malloc(sizeof(char)*(m+n));
//    i = m;
//    j = n;

//    // first, walk from the lower right corner to the
//    // position where we found the maximun score
//    if(i == best_i) // we are in the last row
//    {
//        while(j > best_j)
//        {
//            re1[k++] = '.';
//            re2[q++] = s2[j-1];
//            j -= 1;
//        }
//    }
//    else // we are in the last column
//    {
//        while(i > best_i)
//        {
//            re1[k++] = s1[i-1];
//            re2[q++] = '.';
//            i -= 1;
//        }
//    }


//    int score_left, score_top, score_diag, flag=DIAG;
//    bool is_match;
//    while(i>0 && j>0)
//    {
//        score_diag = columns[j][i]-columns[j-1][i-1];
//        score_left = columns[j][i]-columns[j][i-1];
//        score_top = columns[j][i]-columns[j-1][i];
//        is_match = s1[i-1] == s2[j-1] ? true : can_merge(s1[i-1], s2[j-1]) ? true : false;
//        if(score_diag==NEW_SCORE_SAME||score_diag==NEW_SCORE_MATCH)//||score_diag==SCORE_SAME||score_diag==SCORE_MISMATCH)
//        {
//            if(is_match)
//            {
//                if(score_left==NEW_SCORE_OPEN_GAP && flag==LEFT)
//                {
//                    re1[k++] = s1[i-1];
//                    re2[q++] = '.';
//                    i -= 1;
//                    flag=LEFT;
//                    continue;
//                }

//                if(score_top==NEW_SCORE_OPEN_GAP && flag==TOP)
//                {
//                    re1[k++] = '.';
//                    re2[q++] = s2[j-1];
//                    j -= 1;
//                    flag=TOP;
//                    continue;
//                }

//                re1[k++] = s1[i-1];
//                re2[q++] = s2[j-1];
//                i -= 1;
//                j -= 1;
//                flag =DIAG;
//                continue;
//            }
//            else
//            {
//                if(score_left==NEW_SCORE_OPEN_GAP)
//                {
//                    re1[k++] = s1[i-1];
//                    re2[q++] = '.';
//                    i -= 1;
//                    flag=LEFT;
//                    continue;
//                }

//                if(score_top==NEW_SCORE_OPEN_GAP)
//                {
//                    re1[k++] = '.';
//                    re2[q++] = s2[j-1];
//                    j -= 1;
//                    flag=TOP;
//                    continue;
//                }

//                re1[k++] = s1[i-1];
//                re2[q++] = s2[j-1];
//                i -= 1;
//                j -= 1;
//                flag =DIAG;
//                continue;
//            }
//        }
//        else if(score_diag==NEW_SCORE_MISMATCH)
//        {
//            if(score_top==NEW_SCORE_OPEN_GAP && !(flag==LEFT && score_left==NEW_SCORE_OPEN_GAP))
//            {
//                re1[k++] = '.';
//                re2[q++] = s2[j-1];
//                j -= 1;
//                flag=TOP;
//                continue;
//            }
//            else if(score_left ==NEW_SCORE_OPEN_GAP && !(flag==TOP &&score_top==NEW_SCORE_OPEN_GAP))
//            {
//                re1[k++] = s1[i-1];
//                re2[q++] = '.';
//                i -= 1;
//                flag=LEFT;
//                continue;
//            }

//            re1[k++] = s1[i-1];
//            re2[q++] = s2[j-1];
//            i -= 1;
//            j -= 1;
//            flag=DIAG;
//            continue;
//        }
//        else if(score_left==NEW_SCORE_OPEN_GAP)
//        {
//            if(score_top==NEW_SCORE_OPEN_GAP && flag==TOP)
//            {
//                re1[k++] = '.';
//                re2[q++] = s2[j-1];
//                j -= 1;
//                flag=TOP;
//                continue;
//            }
//            else
//            {
//                re1[k++] = s1[i-1];
//                re2[q++] = '.';
//                i -= 1;
//                flag=LEFT;
//                continue;
//            }
//        }
//        else
//        {
//            re1[k++] = '.';
//            re2[q++] = s2[j-1];
//            j -= 1;
//            flag=TOP;
//            continue;
//        }

//    }


//    // continue from where we are to the upper left corner
//    while(j > 0)
//    {
//        re1[k++] = '.';
//        re2[q++] = s2[j-1];
//        j -= 1;
//    }
//    while(i > 0)
//    {
//        re1[k++] = s1[i-1];
//        re2[q++] = '.';
//        i -= 1;
//    }
//    re1[k] = '\0';
//    re2[q] = '\0';


//    sg->r1 = reverse(re1);
//    sg->r2 = reverse(re2);

//    i=0;
//    while(sg->r1[i] == '.')
//        i++;
//    sg->start1 =i;
//    i=0;
//    while(sg->r2[i] == '.')
//        i++;
//    sg->start2 = i;
//    i=strlen(sg->r1);
//    while(sg->r1[i-1]=='.')
//        i--;
//    sg->stop1 = i;
//    i=strlen(sg->r2);
//    while(sg->r2[i-1]=='.')
//        i--;
//    sg->stop2 = i;

//    for (a = 0; a < n+1; a++)
//    {
//        free(columns[a]);
//        free(backtrace[a]);
//    }

//    qDebug()<<sg->r1;
//    qDebug()<<sg->r2;
//    qDebug()<<sg->start1<<sg->start2<<sg->stop1<<sg->stop2;

//    free(columns);
//    free(backtrace);
//    free(re1);
//    free(re2);

//    return;

//}


char *reverse(char *s)
{
    int m = strlen(s);
    int i;
    char *rs = (char*) malloc(sizeof(char)*(m+1));

    for(i = 0; i < m; i++)
        rs[i] = s[m-1-i];

    rs[m] = '\0';
    return rs;
}


bool can_merge(char A, char B)
{
    if(A=='.' || B=='.')
        return false;
    if(B == 'N')
        return true;
    //    return format(A) & format(B);
    return ((~format(A)) & format(B)) == 0;
}

