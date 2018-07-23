#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ab1.h"
#include "common.h"



/*
* Function:    isAB1File(fp)
*
* Descrip:     Test file type is AB1 format -
*              look for "ABIF" flag.
*
* Arg:         fp      [read]   [FILE*]
*
* Return       true if file type is AB1 format. [bool]
*
*/

bool AB1::isAB1File(FILE* fp)
{
    if(!fp)
    {
        return false;
    }
    char abi[5];
    memset(abi,'\0',5);
    if(fseek(fp,0,SEEK_SET) != 0)
    {
        return false;
    }
    if(fread((void*)abi,4,1,fp))
    {
        if(0 != strcmp(abi,"ABIF"))
        {
            return false;
        }
    }
    return true;
}

/*
* Function:   getTag_(fp,tagLabel,tagNumber,tag)
*
* Descrip:     read through an AB1 trace file until it reaches
*              a tag (tagLabel), then find the correct number
*              of this tag (tagNumber).
*              It saves the tag record into tag object.
*
* Arg:         fp      [read]  [FILE*]
* Arg:         tagLabel    [read]  [long]
* Arg:         tagNumber   [read]  [long]
* Arg:         tag     [write] [Tag]
*
* Return       true if successfully save that record.
*
*/

bool AB1::getTag_(FILE* fp, long tagLabel, long tagNumber, Tag tag)
{
    int tagNum = -1;
    long startOffset;
    int INDEX_ENTRY_LENGTH = 28;

    if(fseek(fp,26,SEEK_SET) || (!readInt4(fp, &startOffset)))
    {
        return false;
    }
    do
    {
        tagNum++;
        if(fseek(fp,startOffset+(tagNum*INDEX_ENTRY_LENGTH),SEEK_SET) != 0)
        {
            return false;
        }
        if(!readInt4(fp, &tag->name)) {return false;}
        if(!readInt4(fp, &tag->number)) {return false;}
        //if(!readInt2(fp))
    } while (!(tag->name == tagLabel && tag->number == tagNumber));

    readInt2(fp, &tag->elementtype);
    readInt2(fp, &tag->elementsize);
    readInt4(fp, &tag->numelements);
    readInt4(fp, &tag->datasize);
    readInt4(fp, &tag->dataoffset);

    return true;
}

/*
* Function:    getNumBase(fp)
*
* Descrip:     find the 'PBAS' tag in an AB1 trace file and
*              return 'the number of elements' of this record,
*              this will tell us the number of bases this AB1
*              file saved.
*
* Arg:         fp      [read]  [FILE*]
*
* return       number of elements  [int]
*
*/

int AB1::getNumBase(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long BASEtag = (long)(((((('P'<<8)+'B')<<8)+'A')<<8)+'S');
    if(getTag_(fp,BASEtag,2,tag))
    {
        long int tagtmp = tag->numelements;
        free(tag);
        return tagtmp;
    }
    free(tag);
    return 0;
}

/*
* Function:     getBaseOffset(fp)
*
*  Descrip:      Find the 'PBAS' tag in an AB1 trace file and
*               return 'data offset' of this tag, this will
*               tell us the position that stored the bases
*               in this AB1 trace file.
*
*  Arg:          fp      [read]  [FILE*]
*
*  return        data offset     [int]
*
*/

int AB1::getBaseOffset(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long BASEtag;
    BASEtag = ((long) ((((('P'<<8)+'B')<<8)+'A')<<8)+'S');
    if(!getTag_(fp,BASEtag,2,tag))
    {
        free(tag);
        return 0;
    }
    int tagtmp = tag->dataoffset;
    free(tag);
    return tagtmp;
}

/*
* Function:    getBasePosOffset(fp)
*
* Descrip:     find the 'PLOC' tag  in an AB1 trace file and
*              return 'data offset' of this tag. this will
*              tell us the position that stored each peak
*              position in this trace file.
*
* Arg:         fp      [read]  [FILE*]
*
* return       data offset     [int]
*
*/

int AB1::getBasePosOffset(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long BASEPOStag;

    BASEPOStag = ((long) ((((('P'<<8)+'L')<<8)+'O')<<8)+'C');
    if(!getTag_(fp,BASEPOStag,2,tag))
    {
        free(tag);
        return 0;
    }
    int tagtmp = tag->dataoffset;
    free (tag);
    return tagtmp;
}

/*
* Function:    getQualOffset(fp)
*
* Descrip:     find the 'PCON' tag in AB1 trace file and
*              return 'data offset' of this tag. this will
*              tell us the position that stored the quality
*              value of each base in this trace file.
*
* Arg:         fp      [read]  [FILE*]
*
* return       data offset     [int]
*
*/

int AB1::getQualOffset(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long BASEQUALtag;

    BASEQUALtag = ((long) ((((('P'<<8)+'C')<<8)+'O')<<8)+'N');
    if(!getTag_(fp,BASEQUALtag,2,tag))
    {
        free(tag);
        return 0;
    }
    int tagtmp = tag->dataoffset;
    free(tag);
    return tagtmp;
}

/*
* Function:    getNumTraceData(fp)
*
* Descrip:     find the 'DATA' tag in AB1 trace file and
*              return 'number of elements' of this tag.
*              this will tell us the number of trace data
*              stored in this trace file.
*
* Arg:         fp      [read]  [FILE*]
*
* return       number of elements  [int]
*
*/

int AB1::getNumTraceData(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long DATAtag;

    DATAtag = ((long) ((((('D'<<8)+'A')<<8)+'T')<<8)+'A');
    if(!getTag_(fp,DATAtag,9,tag))
    {
        free(tag);
        return 0;
    }
    int tagtmp = tag->numelements;
    free(tag);
    return tagtmp;
}

/*
* Function:    getTraceDataOffset(fp,Offset)
*
* Descrip:     find the 'DATA' tag in AB1 trace file.
*              'DATA9' correspond to 'G' trace data;
*              'DATA10' correspond to 'A' trace data;
*              'DATA11' correspond to 'T' trace data;
*              'DATA12' correspond to 'C' trace data;
*              save each 'data offset' to Offset[4].
*              this will tell us the position of
*              each trace data stored in this trace file.
*
* Arg:         fp      [read]  [FILE*]
* Arg:         Offset  [write] [long*]
*
* return       true if successfully saved.
*
*/

bool AB1::getTraceDataOffset(FILE* fp, long* Offset)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long dataxO[4];
    long fwo_;

    /* BYTE[i] is a byte mask for byte i */
    //const long BYTE[] = { 0x000000ff };
    short TRACE_INDEX;
    long DATAtag;

    TRACE_INDEX = 9;
    DATAtag     = ((long) ((((('D'<<8)+'A')<<8)+'T')<<8)+'A');

    /* Find FWO tag - Field order "GATC" */
    fwo_ = getFWO(fp);

    /* Get data trace offsets            */
    if (!getTag_(fp,DATAtag,TRACE_INDEX,tag))
    {
        free(tag);
        return false;
    }
    else
    {
        dataxO[baseIdx((char)(fwo_>>24&255))] = tag->dataoffset;
    }

    if (!getTag_(fp,DATAtag,TRACE_INDEX+1,tag))
    {
        free(tag);
        return false;
    }
    else
    {
        dataxO[baseIdx((char)(fwo_>>16&255))] = tag->dataoffset;
    }

    if (!getTag_(fp,DATAtag,TRACE_INDEX+2,tag))
    {
        free(tag);
        return false;
    }
    else
    {
        dataxO[baseIdx((char)(fwo_>>8&255))] = tag->dataoffset;
    }

    if (!getTag_(fp,DATAtag,TRACE_INDEX+3,tag))
    {
        free(tag);
        return false;
    }
    else
    {
        dataxO[baseIdx((char)(fwo_&255))] = tag->dataoffset;
    }

    Offset[0]=dataxO[baseIdx((char)(fwo_>>24&255))];
    Offset[1]=dataxO[baseIdx((char)(fwo_>>16&255))];
    Offset[2]=dataxO[baseIdx((char)(fwo_>>8&255))];
    Offset[3]=dataxO[baseIdx((char)(fwo_&255))];

    free(tag);
    return true;
}

/*
* Function:    getFWO(fp)
*
* Descrip:     find the 'FWO_' tag in AB1 trace file and
*              return 'data offset' of this tag. Because
*              this record size is '4-bytes', the actul
*              data is stored in 'data offset' field.The
*              data tells us 'sequencing field order', which
*              correspond to the 'DATA' tags. Normally, the
*              value = 'GATC', that means 'DATA9' corresponds
*              to 'G' trace data, 'DATA10' corresponds to 'A'
*              trace data, 'DATA11' corresponds to 'T' trace
*              data and 'DATA12' corresponds to 'c' trace data.
*
* Arg:         fp      [read]  [FILE*]
*
* return       data offset     [int]
*
*/

int AB1::getFWO(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long FWOtag;

    FWOtag = ((long) ((((('F'<<8)+'W')<<8)+'O')<<8)+'_');
    if(!getTag_(fp,FWOtag,1,tag))
    {
        free(tag);
        return 0;
    }
    int tagtmp = tag->dataoffset;
    free(tag);
    return(tagtmp);
}

/*
* Function:    getSampleName(fp)
*
* Descrip:     find the 'SMPL' tag in AB1 trace file and
*              return 'data offset' of this tag. Because
*              the size of this record is less than 4 bytes,
*              the actual data is stored in 'data offset'
*              field. This will tell us the sample name.
*
* Arg:         fp      [read]  [FILE*]
*
* return       data offset     [int]
*
*/

int AB1::getSampleName(FILE* fp)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long SMPLtag;

    SMPLtag = ((long) ((((('S'<<8)+'M')<<8)+'P')<<8)+'L');
    if(!getTag_(fp,SMPLtag,1,tag))
    {
        free(tag);
        return false;
    }
    int tagtmp = tag->dataoffset;
    free(tag);
    return tagtmp;
}

/*
* Function:    getSignalValue(fp,fwo_,sigC,sigA,sigG,sigT)
*
* Descrip:     find the 'S/N%' tag in AB1 trace file and
*              get the value of 'data offset', this will
*              tell us the position that actual signal
*              values were stored. Jump to that position,
*              get the signal value of 'GATC', the order
*              is decided by the 'fwo_', and saved the
*              corresponded signal value.
*
* Arg:         fp      [read]  [FILE*]
* Arg:         fwo_    [read]  [long]
* Arg:         sigC    [write] [short*]
* Arg:         sigA    [write] [short*]
* Arg:         sigG    [write] [short*]
* Arg:         sigT    [write] [short*]
*
* return       void , the actual values were saved into sigC, sigA, sigG, sigT;
*
*/

void AB1::getSignalValue(FILE* fp, long fwo_, short* sigC, short* sigA, short* sigG, short* sigT)
{
    Tag tag=(Tag)malloc(sizeof(Tago));
    long signalO;
    short* base[4];

    long SIGNALtag;

    SIGNALtag = ((long) ((((('S'<<8)+'/')<<8)+'N')<<8)+'%');

    if(getTag_(fp,SIGNALtag,1,tag))
    {
        base[0] = sigC;
        base[1] = sigA;
        base[2] = sigG;
        base[3] = sigT;
        signalO = tag->dataoffset;
        fseek(fp,signalO,SEEK_SET);
        readInt2(fp, base[baseIdx((char)(fwo_>>24&255))]);
        readInt2(fp, base[baseIdx((char)(fwo_>>16&255))]);
        readInt2(fp, base[baseIdx((char)(fwo_>>8&255))]);
        readInt2(fp, base[baseIdx((char)(fwo_&255))]);
    }
    free(tag);
}

/*
* Function:     getBaseQual(fp,pconO,numBases,qual)
*
* Descrip:      Jump to pconO, position that stored the actual
*               base quality for each base, pconO = getQualOffset(fp);
*               'numBases' tells us to read how many bytes
*               integer as the base quality, numBases = getNumBase(fp).
*               The base quality is saves into 'qual'.
*
* Arg:          fp      [read]  [FILE*]
* Arg:          pconO   [read]  [long]
* Arg:          numBases    [read]  [long]
* Arg:          qual    [write] [int*]
*
* return        true if successful!
*
*/

bool AB1::getBaseQual(FILE* fp, long pconO, long numBases, int qual[], int *maxQuality)
{
    int i;
    char qv;
    *maxQuality = 0;

    fseek(fp,pconO,SEEK_SET);

    for (i=0;i<(int)numBases;i++)
    {
        fread((void*)&qv,1,1,fp);
        qual[i] = (int)qv;
        if (qual[i] >= *maxQuality)
            *maxQuality = qual[i];
        //printf("%d",qual[i]);
    }

    return true;
}

/*
* Function:    getSeq(fp,baseO,numBases,seq)
*
* Descrip:     Jump to baseO, position that stored the actual
*              base sequence for each base, baseO = getBaseOffset(fp);
*              'numBases' tells us to read how many bytes
*              char as the base sequences, numBases = getNumBase(fp).
*              The base sequences are saved into 'seq'.
*
* Arg:         fp      [read]      [FILE*]
* Arg:         baseO   [read]      [long]
* Arg:         numBases[read]      [char*]
*
* return       true if successful!
*
*/

bool AB1::getSeq(FILE* fp, long baseO, long numBases, char* seq)
{
    int i;
    char pseq;

    fseek(fp,baseO,SEEK_SET);

    for (i=0;i<(int)numBases;i++)
    {
        fread((void*)&pseq,1,1,fp);
        seq[i] = (char)(int)pseq;
    }
    seq[i]='\0';

    return true;
}

/*
* Function:    getBasePosition(fp,basePositionO,numBases,basePositions)
*
* Descrip:     Jump to basePositionO, position that stored the actual
*              base position for each base, basePositionO = getBasePosOffset(fp);
*              'numBases' tells us how many bytes integers as base positions,
*              numBases = getNumBase(fp). The base positions are saved
*              into 'basePositions'.
*
* Arg:         fp              [read] [FILE*]
* Arg:         basePositionO   [read] [long]
* Arg:         numBases        [read] [long]
* Arg:         basePositions   [write] [int*]
*
* return       true if successful!
*
*/

bool AB1::getBasePosition(FILE* fp, long basePositionO, long numBases, int* basePositions)
{
    int i;
    short bP;

    fseek(fp,basePositionO,SEEK_SET);
    /* Read in base positions   */
    for (i=0;i<(int)numBases;i++)
    {
        if (!readInt2(fp,&bP))
        {
            return false;
        }
        else
        {
            basePositions[i] =(int)bP;
        }
    }
    return true;
}

/*
* Function:    getBaseSpace(fp)
*
* Descrip:     find the 'SPAC' tag in AB1 file and
*              return 'data offset' as float number.
*              This value tells us the average spacing
*              between two contiguous bases.
*
* Arg:         fp      [read]  [FILE*]
*
* return       data offset as float
*
*/

float AB1::getBaseSpace(FILE* fp)
{

    Tag tag=(Tag)malloc(sizeof(Tago));
    float spacing = 0.;
    long SPACINGtag;

    SPACINGtag = ((long) ((((('S'<<8)+'P')<<8)+'A')<<8)+'C');

    getTag_(fp,SPACINGtag,1,tag);
    spacing = IntToFloat(tag->dataoffset);
    free(tag);
    return spacing;
}

/*
* Function:    getTraceData(fp,baseG,baseA,baseT,baseC,numPoints,Offsets,fwo_)
*
* Descrip:     Jump to Offsets[0], Offsets[1], Offsets[2] and Offsets[3]
*              in order, where is the position that stored the actual
*              trace datas that correspond to the order of 'fwo_';
*              'numPoints' tells us to read how many integers as the
*              trace datas. 'numPoints' = getNumTraceData(fp),
*              'Offsets' is from getTraceDataOffset(fp,Offset), and
*              'fwo_' = getFWO(fp). baseG, baseA, baseT, baseC are
*              used to store coreposonding trace data.
*
* Arg:         fp          [read]  [FILE*]
* Arg:         baseG       [write] [Baseo*]
* Arg:         baseA       [write] [Baseo*]
* Arg:         baseT       [write] [Baseo*]
* Arg:         baseC       [write] [Baseo*]
* Arg:         numPoints   [read]  [long]
* Arg:         Offsets     [read]  [long*]
* Arg:         fwo_        [read]  [long]
*
* return       true if successful!
*
*/
bool AB1::getTraceData(FILE* fp, Baseo* baseG, Baseo* baseA, Baseo* baseT,
                       Baseo* baseC, long numPoints, long* Offsets, long fwo_, int *maxSignal)
{
    int i;
    int j;
    short traceValue;
    *maxSignal = 0;

    /* Read in data  */
    for (i=0;i<4;i++)
    {
        if (fseek(fp,Offsets[i],SEEK_SET))
        {
            return false;
        }
        for (j=0;j<(int)numPoints;j++)
        {
            if(readInt2(fp, &traceValue))
            {
                int a = 32-8*(i+1);
                if ((char)(fwo_>>a&255) == 'G')
                {
                    baseG[j].basetype = 'G';
                    baseG[j].pos = j + 1;
                    baseG[j].signal = traceValue;
                    baseG[j].qual = 0;

                    if (baseG[j].signal >= *maxSignal)
                        *maxSignal = baseG[j].signal;
                }
                else if ((char)(fwo_>>a&255) == 'A')
                {
                    baseA[j].basetype = 'A';
                    baseA[j].pos = j + 1;
                    baseA[j].signal = traceValue;
                    baseA[j].qual = 0;

                    if (baseA[j].signal >= *maxSignal)
                        *maxSignal = baseA[j].signal;
                }
                else if ((char)(fwo_>>a&255) == 'T')
                {
                    baseT[j].basetype = 'T';
                    baseT[j].pos = j + 1;
                    baseT[j].signal = traceValue;
                    baseT[j].qual = 0;

                    if (baseT[j].signal >= *maxSignal)
                        *maxSignal = baseT[j].signal;
                }
                else
                {
                    baseC[j].basetype = 'C';
                    baseC[j].pos = j + 1;
                    baseC[j].signal = traceValue;
                    baseC[j].qual = 0;

                    if (baseC[j].signal >= *maxSignal)
                        *maxSignal = baseC[j].signal;
                }
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

/*
* Function:    getBaseData(fp,base,numBases,qual,seq,basePositions,baseG,baseA,baseT,baseC)
*
* Descrip:     index 'i' is from 0 to 'numBases', base[i].basetype = seq[i];
*              base[i].pos = basePositions[i]; base[i].qual = qual[i].
*              The only values that we don't know is base[i].signal.
*              To get that we compare seq[i] to 'G' 'A' 'T' 'C',
*              if seq[i] == 'G', then base[i].signal = baseG[basePositions[i]].signal
*              and so on. Also we have to deal with seq[i] none of 'GATC'.
*
* Arg:         fp          [read]  [FILE*]
* Arg:         base        [write] [Baseo*]
* Arg:         numBases    [read]  [long]
* Arg:         qual        [read]  [int*]
* Arg:         seq         [read]  [char*]
* Arg:         basePositions[read] [int*]
* Arg:         baseG       [read]  [Baseo*]
* Arg:         baseA       [read]  [Baseo*]
* Arg:         baseT       [read]  [Baseo*]
* Arg:         baseC       [read]  [Baseo*]
*
* return       true if successful!
*
*/

bool AB1::getBaseData(FILE* fp, Baseo* base, long numBases, int* qual, char* seq,
                      int* basePositions, Baseo* baseG, Baseo* baseA,
                      Baseo* baseT, Baseo* baseC)
{
    int i;
    // short tracevalue;

    for(i=0; i<numBases; i++)
    {
        base[i].basetype = seq[i];
        base[i].pos = basePositions[i];
        base[i].qual = qual[i];

        if(seq[i] == 'G')
        {
            base[i].signal = baseG[basePositions[i]].signal;
        }
        else if(seq[i] == 'A')
        {
            base[i].signal = baseA[basePositions[i]].signal;
        }
        else if(seq[i] == 'T')
        {
            base[i].signal = baseT[basePositions[i]].signal;
        }
        else if(seq[i] == 'C')
        {
            base[i].signal = baseC[basePositions[i]].signal;
        }
        else
        {
            int g = baseG[basePositions[i]].signal;
            int a = baseA[basePositions[i]].signal;
            int t = baseT[basePositions[i]].signal;
            int c = baseC[basePositions[i]].signal;
            base[i].signal = MAX(MAX(MAX(g,a),t),c);
        }
    }

    return true;
}

/*
* Function:    hasPeak(base,low,high,pos)
*
* Descrip:     test whether 'base' has a local peak or not
*              from 'low' to 'high' region, if exist, save
*              the first local peak position into 'pos'.
*              we define a local peak position i if
*              base[i-1].signal <= base[i].signal &&
*              base[i+1].signal < base[i].signal. Although
*              this method is not that accurate, we only
*              saved the first peak position, if there are
*              more than one local peak regions, the others
*              are lost. But in our data, this situation
*              extremly low.
*
* Arg:         base [read] [Baseo*]
* Arg:         low [read]  [int]
* Arg:         high [read] [int]
* Arg:         pos  [write] [int*]
*
*
* return       true if exist local peak.
*
*/

bool AB1::hasPeak(Baseo* base, int low, int high, int* pos)
{
    int i;
    for (i = low; i < high; i++)
    {
        if (base[i-2].signal < base[i-1].signal && base[i-1].signal <= base[i].signal && base[i+1].signal < base[i].signal && base[i+2].signal < base[i+1].signal)
        {
            *pos = i;
            return true;
        }
    }
    return false;
}

/*
* Function:    getPeak(baesG,baseA,baseT,baseC,base,basePositions,numBases,threshold,peak)
*
* Descrip:     This function is used to get all potential double peak positions.
*              we define double peak position i as :
*              http://ars.els-cdn.com/content/image/1-s2.0-S1296207405000828-gr3.jpg
*              If base[i] is double peak, we know the first peak is basePositions[i],
*              then around basePositions[i], below the first peak, another
*              local peak must be found.
*
* Arg:         BaseG       [read]  [Baseo*]
* Arg:         BaseA       [read]  [Baseo*]
* Arg:         BaseT       [read]  [Baseo*]
* Arg:         BaseC       [read]  [Baseo*]
* Arg:         base        [read&write]    [Baseo*]
* Arg:         basePositions   [read]      [int*]
* Arg:         numBases    [read]  [int]
* Arg:         threshold   [read]  [float]
* Arg:         peak        [write] [int*]
*
* return       numbers of peak position
*
*/

int AB1::getPeak(Baseo* BaseG, Baseo* BaseA, Baseo* BaseT, Baseo* BaseC, Baseo* base, char* seq, int* basePositions, int numBase, float threshold)
{
    int i;
    int j = 0;
    //int peak[numBase];
    int low, high;
    for (i = 1; i < numBase - 1; i++)
    {
        if (base[i].basetype == 'N')
        {
            continue;
        }
        else if (base[i].basetype != 'G' && base[i].basetype != 'A' && base[i].basetype != 'T' && base[i].basetype != 'C')
        {
            //peak[j] = basePositions[i];
            //            peak[j] = i;
            j++;

        }
        else
        {
            int *pos = (int*)malloc(sizeof(int));
            low = basePositions[i] - int((basePositions[i] - basePositions[i-1])/2);
            high = basePositions[i] + int ((basePositions[i+1] - basePositions[i])/2);
            if (base[i].basetype == 'G' && base[i].qual <= 40)
            {
                if (hasPeak(BaseA, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseA[*pos].signal / base[i].signal >= threshold)
                {
                    //peak[j]= basePositions[i];
                    base[i].basetype = 'R';
                    //                      peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseT, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseT[*pos].signal / base[i].signal >= threshold)
                {
                    //printf("%d\n",i);
                    base[i].basetype = 'K';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseC, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseC[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'S';
                    //                    peak[j] = i;
                    j++;
                }
            }
            else if (base[i].basetype == 'A' && base[i].qual <= 40)
            {
                if (hasPeak(BaseG, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseG[*pos].signal / base[i].signal >= threshold)
                {
                    //peak[j] = basePositions[i];
                    base[i].basetype = 'R';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseT, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseT[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'W';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseC, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseC[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'M';
                    //                    peak[j] = i;
                    j++;
                }
            }
            else if (base[i].basetype == 'T' && base[i].qual <= 40)
            {
                if (hasPeak(BaseG, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseG[*pos].signal / base[i].signal >= threshold)
                {
                    //peak[j] = basePositions[i];
                    base[i].basetype = 'K';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseA, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseA[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'W';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseC, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseC[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'Y';
                    //                    peak[j] = i;
                    j++;
                }
            }
            else if (base[i].basetype == 'C' && base[i].qual <= 40)
            {
                if (hasPeak(BaseG, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseG[*pos].signal / base[i].signal >= threshold)
                {
                    //peak[j] = basePositions[i];
                    base[i].basetype = 'S';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseA, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseA[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'M';
                    //                    peak[j] = i;
                    j++;
                }
                else if (hasPeak(BaseT, low, high, pos) && abs(*pos - basePositions[i]) <= 4 && 1.0 * BaseT[*pos].signal / base[i].signal >= threshold)
                {
                    base[i].basetype = 'Y';
                    //                    peak[j] = i;
                    j++;
                }
            }
        }
    }

    /* int group = 0; //
    int num = 0; //
    int max = 0; //
    int total = 0;
    int left, right;
    int flag = 1;
    for (i = j; i > 0; i--)
    {
        if (peak[i]-1 == peak[i-1])
        {
            if (flag == 1)
            {
                num += 1;
                right = i;
                flag = 0;
            }
            else
            {
                 num += 1;
                 left = i - 1;
            }
        }
        else if (num >= 1)
        {
            if (num > max)
            {
                max = num;
            }
            group += 1;
            total += num;
            num = 0;
        }
        if (num == j)
        {
            group = 1;
            max = j;
        }
    }

    if (group >= 1 && max >= 5 && 1.0 * total / numBase >= 0.05)
    {
        for (i = left; i <= right; i++)
        {
            base[peak[i]].basetype = seq[peak[i]];
        }
    } */
    return j;
}

/*
 * Funciton:     reverseComplement(base,numBase)
 *
 * Descrip:      This both complements and reverses a sequence, and it also
 *               recongizes the "IUPAC-IUB codes" correctly.
 *
 * Arg:          base       [read]  [Baseo*]
 * Arg:          numBase    [read]  [int]
 *
 * return        reverse and complement bases
 *
*/
Baseo* AB1::reverseComplement(Baseo* base, int numBase, int numTrace, int flag)
{
    int i;
    int j;
    int end;
    Baseo *out;

    if (flag == 1)
    {
        out = (Baseo*) malloc(sizeof(Baseo)*numBase);
        end = numBase - 1;
    }
    else
    {
        out = (Baseo*) malloc (sizeof(Baseo)*numTrace);
        end = numTrace - 1;
    }


    for (j = 0, i = end; i >= 0; i--, j++)
    {

        switch(base[i].basetype)
        {
        case 'G':
            out[j].basetype = 'C';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'A':
            out[j].basetype = 'T';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'T':
            out[j].basetype = 'A';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'C':
            out[j].basetype = 'G';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'R':
            out[j].basetype = 'Y';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'Y':
            out[j].basetype = 'R';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'K':
            out[j].basetype = 'M';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'M':
            out[j].basetype = 'K';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'S':
            out[j].basetype = 'S';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'W':
            out[j].basetype = 'W';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'B':
            out[j].basetype = 'V';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'V':
            out[j].basetype = 'B';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'D':
            out[j].basetype = 'H';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'H':
            out[j].basetype = 'D';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        case 'N':
            out[j].basetype = 'N';
            out[j].pos = numTrace - base[i].pos - 1;
            out[j].qual = base[i].qual;
            out[j].signal = base[i].signal;
            break;
        }
    }

    free(base);

    return out;
}

bool AB1::isAb1Good(short sigG, short sigA, short sigT, short sigC)
{
    int sum;
    sum = sigG + sigA + sigT + sigC;
    if (sum < 800)
        return false;
    return true;
}

/**新增Start**/
double AB1::signalRatio(Baseo *baseG, Baseo *baseA, Baseo *baseT, Baseo *baseC, Baseo *base, double *ratios, int *basePositions, int numBases)
{
    double avgTemp ;
    double sum = 0.0;

    int i;
    for (i = 0; i < numBases; i++)
    {
        //printf("%d\t%d\t%d\t%d\t%d\n", base[i].signal, baseG[basePositions[i]-1].signal, baseA[basePositions[i]-1].signal, baseT[basePositions[i]-1].signal, baseC[basePositions[i]-1].signal);
        if (base[i].signal == baseG[basePositions[i]-1].signal)
        {
            int a = baseA[basePositions[i]-1].signal;
            int t = baseT[basePositions[i]-1].signal;
            int c = baseC[basePositions[i]-1].signal;
            int secondSignal = MAX(MAX(a,t),c);
            //		printf("%d\t%d\n",base[i].signal, secondSignal);
            ratios[i] = (base[i].signal == 0) ? 0.0 : secondSignal * 1.0 / base[i].signal;
            //		printf("%f\n", ratios[i]);
            //		printf("%lf\n", secondSignal * 1.0 / base[i].signal);
            sum += ratios[i];
        }
        else if (base[i].signal == baseA[basePositions[i]-1].signal)
        {
            int g = baseG[basePositions[i]-1].signal;
            int t = baseT[basePositions[i]-1].signal;
            int c = baseC[basePositions[i]-1].signal;
            int secondSignal = MAX(MAX(g,t),c);
            ratios[i] = (base[i].signal == 0) ? 0.0 : secondSignal * 1.0 / base[i].signal;
            sum += ratios[i];
        }
        else if (base[i].signal == baseT[basePositions[i]-1].signal)
        {
            int g = baseG[basePositions[i]-1].signal;
            int a = baseA[basePositions[i]-1].signal;
            int c = baseC[basePositions[i]-1].signal;
            int secondSignal = MAX(MAX(g,a),c);
            ratios[i] = (base[i].signal == 0) ? 0.0 : secondSignal * 1.0 / base[i].signal;
            sum += ratios[i];
        }
        else if (base[i].signal == baseC[basePositions[i]-1].signal)
        {
            int g = baseG[basePositions[i]-1].signal;
            int a = baseA[basePositions[i]-1].signal;
            int t = baseT[basePositions[i]-1].signal;
            int secondSignal = MAX(MAX(g,a),t);
            ratios[i] = (base[i].signal == 0) ? 0.0 : secondSignal * 1.0 / base[i].signal;
            sum += ratios[i];
        }
    }

    avgTemp = sum / numBases;

    return avgTemp;
}
/**新增End**/
