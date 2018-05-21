#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED
#include <string>
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;
int fileSeek(FILE* fp, long offset, int wherefrom);
bool readInt4(FILE* fp, long* i4);
bool readInt2(FILE* fp, short* i2);
bool readFloat4(FILE* fp, float* f4);
int  readBinary(FILE* fp, int count, int element_size, void* ptr);
short baseIdx(char B);
string IntToStr(int num);
int StrToint(string str);
string IntToStr2(int num);
float IntToFloat(int num);

#endif // TOOL_H_INCLUDED
