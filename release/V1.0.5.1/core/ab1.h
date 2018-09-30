#ifndef AB1_H_INCLUDED
#define AB1_H_INCLUDED

#include <stdio.h>

/* Object Baseo
 *
 * Descrip: This object is the basic data object that can hold two types
 *          of data. Since in an AB1 trace chromatogram, the base-caller
 *          sequences are at every peak positions and each one has a
 *          quality value to determine this called's confidence. If not
 *          a peak position, we set the quality value 0.
 *
 */
typedef struct
{
    char basetype;   /* A T G C (sequences or stored in trace data) */
    int  pos;        /* position */
    int  signal;     /* signal strength value */
    int  qual;       /* quality value if sequence or else 0 */
} Baseo;
//#define Base Baseo*

/* object Tag
 *
 * Descrip: This object is used to save each record that is retrived from
 *          an AB1 file.
 *
 *          Data in AB1 file is organized as "record", each record has the
 *          same data structure: tag name (4 bytes integers), tag number
 *          (4 bytes integers), element type code (2 bytes integers),
 *          element size (2 bytes integers), number of elements (4 bytes integers),
 *          data size (4 bytes integers), data offset (4 bytes integers) and
 *          handle (not used here).
 *
 */
typedef struct
{
    long int name;              /* tag name */
    long int number;            /* tag number */
    short int elementtype;      /* element type code */
    short int elementsize;      /* element size */
    long int numelements;       /* number of elements */
    long int dataoffset;        /* data offset */
    long int datasize;          /* data size */
} Tago;
#define Tag Tago*



/***************************************************/
/* Callable functions                              */
/* These are the functions you are expected to use */
/***************************************************/


class AB1
{
public:

    /* Function:    isAB1File(fp)
         *
         * Descrip:     Test file type is AB1 format -
         *              look for "ABIF" flag.
         *
         * Arg:         fp      [read]   [FILE*]
         *
         * Return       true if file type is AB1 format. [bool]
         *
         */
    bool isAB1File(FILE* fp);

    /* Function:   getTag_(fp,tagLabel,tagNumber,tag)
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
    bool getTag_(FILE* fp, long tagLabel, long tagNumber, Tag tag);

    /* Function:    getNumBase(fp)
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
    int getNumBase(FILE* fp);

    /* Function:     getBaseOffset(fp)
        *
        * Descrip:      Find the 'PBAS' tag in an AB1 trace file and
        *               return 'data offset' of this tag, this will
        *               tell us the position that stored the bases
        *               in this AB1 trace file.
        *
        * Arg:          fp      [read]  [FILE*]
        *
        * return        data offset     [int]
        *
        */
    int getBaseOffset(FILE* fp);

    /* Function:    getBasePosOffset(fp)
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
    int getBasePosOffset(FILE* fp);

    /* Function:    getQualOffset(fp)
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
    int getQualOffset(FILE* fp);

    /* Function:    getNumTraceData(fp)
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
    int getNumTraceData(FILE* fp);

    /* Function:    getTraceDataOffset(fp,Offset)
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
    bool getTraceDataOffset(FILE* fp, long* Offset);

    /* Function:    getFWO(fp)
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
    int getFWO(FILE* fp);

    /* Function:    getSampleName(fp)
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
    int getSampleName(FILE* fp);

    /* Function:    getSignalValue(fp,fwo_,sigC,sigA,sigG,sigT)
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
    void getSignalValue(FILE* fp, long fwo_, short* sigC, short* sigA, short* sigG, short* sigT);

    /* Function:     getBaseQual(fp,pconO,numBases,qual)
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
    bool getBaseQual(FILE* fp, long pconO, long numBases, int qual[], int *maxQuality);

    /* Function:    getSeq(fp,baseO,numBases,seq)
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
    bool getSeq(FILE* fp, long baseO, long numBases, char* seq);

    /* Function:    getBasePosition(fp,basePositionO,numBases,basePositions)
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
    bool getBasePosition(FILE* fp, long basePositionO, long numBases, int* basePositions);

    /* Function:    getBaseSpace(fp)
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
    float getBaseSpace(FILE* fp);

    /* Function:    getTraceData(fp,baseG,baseA,baseT,baseC,numPoints,Offsets,fwo_)
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
    bool getTraceData(FILE* fp, Baseo* baseG, Baseo* baseA, Baseo* baseT,
                      Baseo* baseC, long numPoints, long* Offsets, long fwo_, int *maxSignal);

    /* Function:    getBaseData(fp,base,numBases,qual,seq,basePositions,baseG,baseA,baseT,baseC)
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
    bool getBaseData(FILE* fp, Baseo* base, long numBases, int* qual, char* seq,
                     int* basePositions, Baseo* baseG, Baseo* baseA,
                     Baseo* baseT, Baseo* baseC);

    /* Function:    hasPeak(base,low,high,pos)
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
    bool hasPeak(Baseo* base, int low, int high, int* pos);

    /* Function:    getPeak(baesG,baseA,baseT,baseC,base,basePositions,numBases,threshold,peak)
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
    int getPeak(Baseo* BaseG, Baseo* BaseA, Baseo* BaseT, Baseo* BaseC, Baseo* base,
                char* seq, int* basePositions, int numBase, float threshold);

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
    Baseo* reverseComplement(Baseo* base, int numBase, int numTrace, int flag);

    bool isAb1Good(short sigG, short sigA, short sigT, short sigC);
    /**新增Start**/
    double signalRatio(Baseo *baseG, Baseo *baseA, Baseo *baseT, Baseo *baseC, Baseo *base, double *ratios, int *basePositions, int numBases);
    /**新增End**/
};
#endif // AB1_H_INCLUDED
