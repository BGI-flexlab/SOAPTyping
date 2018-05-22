#ifndef SEMIGLOBLE_H
#define SEMIGLOBLE_H

#define LEFT 1
#define TOP  2
#define DIAG 3

#define SCORE_SAME 2
#define SCORE_MATCH 1
#define SCORE_MISMATCH -1
#define SCORE_INSERTION -1
#define SCORE_DELETION -1

#define NEW_SCORE_SAME 4
#define NEW_SCORE_MATCH 2
#define NEW_SCORE_MISMATCH -1
#define NEW_SCORE_INSERTION -2
#define NEW_SCORE_DELETION -2
#define NEW_SCORE_OPEN_GAP -2
//#ifdef __cplusplus
//extern "C" {
//#endif

/* align struct contains the result of semiglobal alignment of string s1 and s2. 
 * r1 and r2 are sequences of the same length containing the alignment.
 * (an INDEL is marked by '-')
 * start1 is the position within r1 at which the part of s1, that is aligned, starts.
 * stop1 is the position within r1 at which the part of s1, that is aligned, ends.
 * the same holds for start2, stop2.
 *
 * It is always the case that at least one of start1 and start2 is zero.
 * It is always the case that either stop1==len(r1) or stop2==len(r2) or both
 *
 * errors is the number of errors in the alignment*/

typedef struct
{
	char *r1;
	char *r2;
	int errors;
	int start1;
	int start2;
	int stop1;
	int stop2;

    void clear()
    {
        if(r1!=NULL)
        {
            free(r1);
            r1=NULL;
        }
        if(r2!=NULL)
        {
            free(r2);
            r2=NULL;
        }
        errors = 0;
        start1 = 0;
        start2 = 0;
        stop1 = 0;
        stop2 = 0;
    }

} align;

void semigloblalalign(char *s1, char *s2, align *sg);
void semigloblalalign_new(char *s1, char *s2, align *sg, int length);
bool can_merge(char A, char B);
//#ifdef __cplusplus
//}
//#endif

#endif
