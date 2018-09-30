/*
 * =====================================================================================
 *
 *       Filename:  equal.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-12-13 16:05:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include "equal.h"

//typedef enum {false, true} bool;

int equal(char a, char b)
{
    int fa = 0, fb = 0;
    fa = format(a);
    fb = format(b);

    return fa & fb;
}

unsigned int format(char a)
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

char reformat(unsigned int a)
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
