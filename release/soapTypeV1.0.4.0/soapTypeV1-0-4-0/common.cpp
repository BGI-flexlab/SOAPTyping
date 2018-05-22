#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
//#include <stdbool.h>
#include "common.h"

using namespace std;


/* @func readbinary ***********************************************************
**
** Binary read from an input file object using the C 'fread' function.
**
** @param file [FILE*] input file.
** @param count [int] number of elements to read.
** @param size [int] number of bytes per element.
** @param buffer [void*] buffer for output.
** @return [int] return value from 'fread'
**
** ***************************************************************************/
int readBinary(FILE* file, int count, int size, void* buffer)
{
    return fread(buffer, size, count, file);
}

/* @func fileSeek *************************************************************
**
** sets the current position in an open file.
**
** @param file [FILE*] input file.
** @param offset [long] Offset
** @param wherefrom [int] start of offset
** @return [int] result of 'fseek'.
**
** ***************************************************************************/
int fileSeek(FILE* fp, long offset, int wherefrom)
{
    return fseek(fp, offset, wherefrom);
}

/* @func readInt4 *************************************************************
**
** read 4 bytes from a file and return the integer.
**
** @param fp [FILE*] AB1 format file
** @param i4 [long*] long integer read in from AB1 file
** @retrun [bool] true if read successfully
**
** ***************************************************************************/
bool readInt4(FILE* fp,long* i4)
{
    unsigned char buf[sizeof(long)];

    if(fread((void*)buf,4,1,fp) != 1)
    return false;

    *i4 = (long)(((unsigned long)buf[3]) + ((unsigned long)buf[2]<<8) +
                 ((unsigned long)buf[1]<<16) + ((unsigned long)buf[0]<<24));

    return true;
}

/* @func readInt2 *************************************************************
**
** read 2 bytes from a file and return the short integer.
**
** @param fp [FILE*] AB1 format file
** @param i2 [short*] short integer read in from AB1 file
** @return [bool] true if read successfully
**
** ***************************************************************************/
bool readInt2(FILE* fp, short* i2)
{
    unsigned char buf[sizeof(short)];

    if(fread((void*)buf,2,1,fp) != 1)
    return false;

    *i2 = (short)(((unsigned short)buf[1]) + ((unsigned short)buf[0]<<8));

    return true;
}

/* @func readFloat4 ***********************************************************
**
** read 4 bytes from a file and return the float value.
**
** @param fp [FILE*] AB1 format file
** @param f4 [float*] float value read in from AB1 file
** @return [bool] true if read successfully
**
** ***************************************************************************/
bool readFloat4(FILE* fp, float* f4)
{
    unsigned char buf[sizeof(long)];
    unsigned long res;

    if(fread((void*)buf,4,1,fp) != 1)
    return false;

    res = (unsigned long)(((unsigned long)buf[3]) + ((unsigned long)buf[2]<<8) +
                  ((unsigned long)buf[1]<<16) + ((unsigned long)buf[0]<<24));

    *f4 = (float)res;

    return true;
}

/* @func baseIdx **************************************************************
**
** Returns: 0 if C, 1 if A, 2 if G, 3 if T.
**
** @param B [char] base (A,T,C,G)
** ***************************************************************************/
short baseIdx(char B)
{
    return ((B) == 'C' ? 0 : (B) == 'A' ? 1 : (B) == 'G' ? 2 : 3);
}

/* @func IntToStr**************************************************************
**
** Returns: string
**
** it is convert by ascii
** 
** ***************************************************************************/
string IntToStr(int num) 
{

    string str1="";

    int num1 = ((num>>24) & 255);
    int num2 = ((num>>16) & 255);
    int num3 = ((num>>8) & 255);
    int num4 = (num & 255);

    if(((num1 >= 0)&&( num1 <=31 ))||(num1 >= 127))
    {
        str1+="";
    }
    else
    {
        str1+=(char)(num1);
    }
    
    if(((num2 >= 0)&&( num2 <=31 ))||(num2 >= 127))
    {
        str1+="";
    }
    else
    {
        str1+=(char)(num2);
    }

    if(((num3 >= 0)&&( num3 <=31 ))||(num3 >= 127))
    {
        str1+="";
    }
    else
    {
        str1+=(char)(num3);
    }

    if(((num4 >= 0)&&( num4 <=31 ))||(num4 >= 127))
    {
        str1+="";
    }
    else
    {
        str1+=(char)(num4);
    }
    
    return str1;
}

/* @func StrToint**************************************************************
**
** Returns: int
**
** it is convert by ascii
**
** ***************************************************************************/
int StrToint(string str)
{
    int num=0;
    for(int i=0;i<str.length();i++)
    {
        num = num << 8 | (int)(str[i]);
    }
    return num;
}

/* @func IntToStr2**************************************************************
**
** Returns: string
**
** it is convert directly without by ascii
**
** ***************************************************************************/
string IntToStr2(int num)
{
    char* test_tmp2 = (char*)malloc(sizeof(char)*100);
    sprintf(test_tmp2, "%d", num);
    string test_tmp3 = test_tmp2;
    return test_tmp3;
}

/* @func IntToFloat**************************************************************
**
** Returns: float
**
** it is convert int to float.
**
** ***************************************************************************/
float IntToFloat(int num)
{
    float fValue;
    memcpy(&fValue, &num, sizeof(int));
    return fValue;
}
