#include <QtCore>
#include <iostream>
#include <stdlib.h>
#include "core.h"
#include "ab1.h"
#include "semigloble.h"
#include "interface.h"
#include <string>

bool get_info_from_ab1(FileTable filetable)
{
    int base_position_offset, base_offset, quality_offset, fwo;
    long trace_offsets[4];
    short sigG, sigA, sigT, sigC;
    char *orignal_seq;
    int *quality, *base_positions;
    Baseo *base, *baseG, *baseA, *baseT, *baseC;
    float threshold = 0.3;

    QByteArray f = filetable.filePath.toLocal8Bit();

    FILE *fp = fopen(f.data(), "rb");
    AB1 ab1;

    if(!ab1.isAB1File(fp))
    {
        fprintf(stderr, "%s\t is not a valid trace file, please check it!\n", f.data());
        return false;
        //exit(-1);
    }

    filetable.baseNumber = ab1.getNumBase(fp);
    base_position_offset = ab1.getBasePosOffset(fp);
    base_offset = ab1.getBaseOffset(fp);
    quality_offset = ab1.getQualOffset(fp);
    filetable.signalNumber = ab1.getNumTraceData(fp);
    fwo = ab1.getFWO(fp);
    //sample_name = ab1.getSampleName(fp);
    filetable.averageBaseWidth = ab1.getBaseSpace(fp);
    ab1.getTraceDataOffset(fp, trace_offsets);
    ab1.getSignalValue(fp, fwo, &sigC, &sigA, &sigG, &sigT);

    orignal_seq = (char*)malloc(sizeof(char)*filetable.baseNumber+1);
    quality = (int*)malloc(sizeof(int)*filetable.baseNumber);
    base_positions = (int*)malloc(sizeof(int)*filetable.baseNumber);
    base = (Baseo*)malloc(sizeof(Baseo)*filetable.baseNumber);
    baseG = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseA = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseT = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseC = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);

    ab1.getSeq(fp, base_offset, filetable.baseNumber, orignal_seq);
    ab1.getBaseQual(fp, quality_offset, filetable.baseNumber, quality, &filetable.maxQuality);
    ab1.getTraceData(fp, baseG, baseA, baseT, baseC, filetable.signalNumber, trace_offsets, fwo, &filetable.maxSignal);
    ab1.getBasePosition(fp, base_position_offset, filetable.baseNumber, base_positions);
    ab1.getBaseData(fp, base, filetable.baseNumber, quality, orignal_seq, base_positions, baseG, baseA, baseT, baseC);
    ab1.getPeak(baseG, baseA, baseT, baseC, base, orignal_seq, base_positions, filetable.baseNumber, threshold);
    filetable.isGood = ab1.isAb1Good(sigG, sigA, sigT, sigC);

    if (filetable.rOrF.contains("R"))
    {
        base = ab1.reverseComplement(base, filetable.baseNumber, filetable.signalNumber, 1);
        baseG = ab1.reverseComplement(baseG, filetable.baseNumber, filetable.signalNumber, 0);
        baseA = ab1.reverseComplement(baseA, filetable.baseNumber, filetable.signalNumber, 0);
        baseT = ab1.reverseComplement(baseT, filetable.baseNumber, filetable.signalNumber, 0);
        baseC = ab1.reverseComplement(baseC, filetable.baseNumber, filetable.signalNumber, 0);
    }

    QStringList basePostion_t, baseQuality_t;
    for (int i = 0; i < filetable.baseNumber; i++)
    {
         basePostion_t << QString::number(base[i].pos);
         baseQuality_t << QString::number(quality[i]);
    }
    filetable.basePostion = basePostion_t.join(":");
    filetable.baseQuality = baseQuality_t.join(":");

    QStringList baseASignal_t, baseGSignal_t, baseTSignal_t, baseCSignal_t;

    if(filetable.rOrF.contains("F"))
    {
        for (int i = 0; i < filetable.signalNumber; i++)
        {
            baseGSignal_t << QString::number(baseG[i].signal);
            baseASignal_t << QString::number(baseA[i].signal);
            baseTSignal_t << QString::number(baseT[i].signal);
            baseCSignal_t << QString::number(baseC[i].signal);
        }
    }
    else
    {
        for (int i = 0; i < filetable.signalNumber; i++)
        {
            baseCSignal_t << QString::number(baseG[i].signal);
            baseTSignal_t << QString::number(baseA[i].signal);
            baseASignal_t << QString::number(baseT[i].signal);
            baseGSignal_t << QString::number(baseC[i].signal);
        }
    }

    filetable.baseASignal = baseASignal_t.join(":");
    filetable.baseCSignal = baseCSignal_t.join(":");
    filetable.baseGSignal = baseGSignal_t.join(":");
    filetable.baseTSignal = baseTSignal_t.join(":");

    char *modified_seq = (char*) malloc(sizeof(char)*(filetable.baseNumber+1));
    for (int i = 0; i < filetable.baseNumber; i++)
        modified_seq[i] = base[i].basetype;
    modified_seq[filetable.baseNumber] = '\0';
    filetable.baseSequence = QByteArray(modified_seq);

    ExonInfoS exonInfos;
    extract_exon_info(filetable.geneName, filetable.exonIndex, exonInfos);
    filetable.exonStartPos = exonInfos.exonStartPos;
    filetable.exonEndPos = exonInfos.exonEndPos;

    FileAlignResult fileAlignResult;
    FileAlignNew file_align_new;
    file_align_new.consensus = exonInfos.consensusSeq.data();
    file_align_new.raw_seq = filetable.baseSequence.data();
    file_align_new.consensus_start = exonInfos.exonStartPos;
    file_align_new.consensus_end = exonInfos.exonEndPos;
    file_align_new.exclude_left_num =0;
    file_align_new.exclude_right_num =0;

    if(filetable.baseNumber >=20) // 一些ab1文件里面碱基序列为空或很短
    {
        new_align_function(file_align_new, fileAlignResult, true);
        filetable.usefulSequence = QByteArray(fileAlignResult.sampleAlign);
        filetable.alignResult = fileAlignResult.isUnDefined;
    }
    else
        filetable.alignResult = 1;



//    FileAlign fileAlign;
//    FileAlignResult fileAlignResult;
//    fileAlign.rawSequence = modified_seq;
//    fileAlign.consensusSequence = exonInfos.consensusSeq.data();
//    fileAlign.consensusStart = filetable.exonStartPos;
//    fileAlign.consensusEnd = filetable.exonEndPos;

//    if(filetable.baseNumber >=20) // 一些ab1文件里面碱基序列为空或很短
//    {
//        regionShowAlign(&fileAlign, &fileAlignResult, 0);
//        filetable.usefulSequence = QByteArray(fileAlignResult.sampleAlign);
//        filetable.alignResult = fileAlignResult.isUnDefined;
//        if(filetable.alignResult)
//        {
//            FileAlignNew file_align_new;
//            file_align_new.consensus = exonInfos.consensusSeq.data();
//            file_align_new.raw_seq = filetable.baseSequence.data();
//            file_align_new.consensus_start = exonInfos.exonStartPos;
//            file_align_new.consensus_end = exonInfos.exonEndPos;
//            file_align_new.exclude_left_num =0;
//            file_align_new.exclude_right_num =0;
//            new_align_function(file_align_new, fileAlignResult, true);
//            if(fileAlignResult.isUnDefined == 0)
//            {
//                filetable.usefulSequence = QByteArray(fileAlignResult.sampleAlign);
//                filetable.alignResult = fileAlignResult.isUnDefined;
//            }
//        }
//    }
//    else
//        filetable.alignResult = 1;

    QStringList alignInfo_t;
    if(filetable.alignResult)
    {
        filetable.alignStartPos = filetable.baseNumber - 1 ;
        filetable.alignEndPos = filetable.baseNumber - 1;
        for (int i = 0; i < filetable.baseNumber; i++)
            alignInfo_t << QString::number(-1);
    }
    else
    {
        filetable.alignStartPos = fileAlignResult.leftLimit;
        filetable.alignEndPos = fileAlignResult.rightLimit;
        for (int i = 0; i < filetable.baseNumber; i++)
            alignInfo_t << QString::number(fileAlignResult.baseMatchConsensusPos[i]);

        filetable.excludeLeft = getExcludeLeftPosition(QString("%1_%2_%3").arg(QString(filetable.geneName))
                                                       .arg(filetable.exonIndex)
                                                       .arg(QString(filetable.rOrF)));
        filetable.excludeRight = getExcludeRightPostion(QString("%1_%2_%3").arg(QString(filetable.geneName))
                                                        .arg(filetable.exonIndex)
                                                        .arg(QString(filetable.rOrF)));
    }

    filetable.alignInfo = alignInfo_t.join(":");
    insertFileToRealTimeDatabase(filetable);

    free(orignal_seq);
    free(quality);
    free(base_positions);
    free(base);
    free(baseG);
    free(baseA);
    free(baseT);
    free(baseC);
    free(modified_seq);

    if(filetable.baseNumber >=20)
    {
        free(fileAlignResult.baseMatchConsensusPos);
        free(fileAlignResult.sampleAlign);
    }


    fclose(fp);
    return true;
}

bool get_info_from_gssp(GsspFileTable filetable)
{
    int base_position_offset, base_offset, quality_offset, fwo;
    long trace_offsets[4];
    short sigG, sigA, sigT, sigC;
    char *orignal_seq;
    int *quality, *base_positions;
    Baseo *base, *baseG, *baseA, *baseT, *baseC;
//    float threshold = 0.2;
//    int peak[50];

    QByteArray f = filetable.filePath.toLocal8Bit();
    FILE *fp = fopen(f.data(), "rb");

    AB1 ab1;

    if(!ab1.isAB1File(fp))
    {
        fprintf(stderr, "%s\t is not a valid trace file, please check it!\n", f.data());
        exit(-1);
    }

    filetable.baseNumber = ab1.getNumBase(fp);
    base_position_offset = ab1.getBasePosOffset(fp);
    base_offset = ab1.getBaseOffset(fp);
    quality_offset = ab1.getQualOffset(fp);
    filetable.signalNumber = ab1.getNumTraceData(fp);
    fwo = ab1.getFWO(fp);
    //sample_name = ab1.getSampleName(fp);
    filetable.averageBaseWidth = ab1.getBaseSpace(fp);
    ab1.getTraceDataOffset(fp, trace_offsets);
    ab1.getSignalValue(fp, fwo, &sigC, &sigA, &sigG, &sigT);

    orignal_seq = (char*)malloc(sizeof(char)*filetable.baseNumber+1);
    quality = (int*)malloc(sizeof(int)*filetable.baseNumber);
    base_positions = (int*)malloc(sizeof(int)*filetable.baseNumber);
    base = (Baseo*)malloc(sizeof(Baseo)*filetable.baseNumber);
    baseG = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseA = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseT = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);
    baseC = (Baseo*)malloc(sizeof(Baseo)*filetable.signalNumber);

    ab1.getSeq(fp, base_offset, filetable.baseNumber, orignal_seq);
    ab1.getBaseQual(fp, quality_offset, filetable.baseNumber, quality, &filetable.maxQuality);
    ab1.getTraceData(fp, baseG, baseA, baseT, baseC, filetable.signalNumber, trace_offsets, fwo, &filetable.maxSignal);
    ab1.getBasePosition(fp, base_position_offset, filetable.baseNumber, base_positions);
    ab1.getBaseData(fp, base, filetable.baseNumber, quality, orignal_seq, base_positions, baseG, baseA, baseT, baseC);
//    ab1.getPeak(baseG, baseA, baseT, baseC, base, orignal_seq, base_positions, filetable.baseNumber, threshold, peak);
    filetable.isGood = ab1.isAb1Good(sigG, sigA, sigT, sigC);

    if (filetable.rOrF.contains("R"))
    {
        base = ab1.reverseComplement(base, filetable.baseNumber, filetable.signalNumber, 1);
        baseG = ab1.reverseComplement(baseG, filetable.baseNumber, filetable.signalNumber, 0);
        baseA = ab1.reverseComplement(baseA, filetable.baseNumber, filetable.signalNumber, 0);
        baseT = ab1.reverseComplement(baseT, filetable.baseNumber, filetable.signalNumber, 0);
        baseC = ab1.reverseComplement(baseC, filetable.baseNumber, filetable.signalNumber, 0);
    }

    QStringList basePostion_t, baseQuality_t;
    for (int i = 0; i < filetable.baseNumber; i++)
    {
         basePostion_t << QString::number(base[i].pos);
         baseQuality_t << QString::number(quality[i]);
    }
    filetable.basePostion = basePostion_t.join(":");
    filetable.baseQuality = baseQuality_t.join(":");



    QStringList baseASignal_t, baseGSignal_t, baseTSignal_t, baseCSignal_t;

    if(filetable.rOrF.contains("F"))
    {
        for (int i = 0; i < filetable.signalNumber; i++)
        {
            baseGSignal_t << QString::number(baseG[i].signal);
            baseASignal_t << QString::number(baseA[i].signal);
            baseTSignal_t << QString::number(baseT[i].signal);
            baseCSignal_t << QString::number(baseC[i].signal);
        }
    }
    else
    {
        for (int i = 0; i < filetable.signalNumber; i++)
        {
            baseCSignal_t << QString::number(baseG[i].signal);
            baseTSignal_t << QString::number(baseA[i].signal);
            baseASignal_t << QString::number(baseT[i].signal);
            baseGSignal_t << QString::number(baseC[i].signal);
        }
    }
    filetable.baseASignal = baseASignal_t.join(":");
    filetable.baseCSignal = baseCSignal_t.join(":");
    filetable.baseGSignal = baseGSignal_t.join(":");
    filetable.baseTSignal = baseTSignal_t.join(":");

    char *modified_seq = (char*) malloc(sizeof(char)*(filetable.baseNumber+1));
    for (int i = 0; i < filetable.baseNumber; i++)
        modified_seq[i] = base[i].basetype;
    modified_seq[filetable.baseNumber] = '\0';
   // filetable.baseSequence = QByteArray(orignal_seq);
    filetable.baseSequence = QByteArray(modified_seq);

    ExonInfoS exonInfos;
    extract_exon_info(filetable.geneName, filetable.exonIndex, exonInfos);
    filetable.exonStartPos = exonInfos.exonStartPos;
    filetable.exonEndPos = exonInfos.exonEndPos;

    FileAlignResult fileAlignResult;
    FileAlignNew file_align_new;
    file_align_new.consensus = exonInfos.consensusSeq.data();
    file_align_new.raw_seq = filetable.baseSequence.data();
    file_align_new.consensus_start = exonInfos.exonStartPos;
    file_align_new.consensus_end = exonInfos.exonEndPos;
    file_align_new.exclude_left_num =0;
    file_align_new.exclude_right_num =0;

    if(filetable.baseNumber >= 20)
    {
        new_align_function(file_align_new, fileAlignResult, true);
        filetable.usefulSequence = QByteArray(fileAlignResult.sampleAlign);
        if(fileAlignResult.isUnDefined)
            filetable.alignResult = 4;
        else
            filetable.alignResult = 0;
    }
    else
        filetable.alignResult = 4;

    QStringList alignInfo_t;
    if(filetable.alignResult==4)
    {
        filetable.alignStartPos = filetable.baseNumber-1 ;
        filetable.alignEndPos = filetable.baseNumber-1;
        for (int i = 0; i < filetable.baseNumber; i++)
            alignInfo_t << QString::number(-1);
    }
    else
    {
        filetable.alignStartPos = fileAlignResult.leftLimit;
        filetable.alignEndPos = fileAlignResult.rightLimit;
        for (int i = 0; i < filetable.baseNumber; i++)
        {
            alignInfo_t << QString::number(fileAlignResult.baseMatchConsensusPos[i]);
        }
    }

    filetable.alignInfo = alignInfo_t.join(":").toAscii();

    insertGsspFileToRealTimeDatabase(filetable);

    free(orignal_seq);
    free(quality);
    free(base_positions);
    free(base);
    free(baseG);
    free(baseA);
    free(baseT);
    free(baseC);
    free(modified_seq);
    if(filetable.baseNumber >= 20)
    {
        free(fileAlignResult.baseMatchConsensusPos);
        free(fileAlignResult.sampleAlign);
    }
    fclose(fp);

    return true;
}

