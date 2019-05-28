#include "fileprocessthreadtask.h"
#include "DataBase/soaptypingdb.h"
#include <QtDebug>
#include <QTableWidgetItem>


fileprocessthreadtask::fileprocessthreadtask(const QStringList &filePathList)
{
    m_strlist = filePathList;
    getFileInfoData();
}

void fileprocessthreadtask::getFileInfoData()
{
    SoapTypingDB::GetInstance()->GetGsspMapToExonAndFR(m_map_ExonAndRF);

    SoapTypingDB::GetInstance()->GetCommonGsspMapToExonAndFR(m_map_CommonGssp);
}


fileprocessthreadtask::~fileprocessthreadtask()
{

}

void fileprocessthreadtask::FilePathProcess(const QString &filePath, OpenFileTable &info)
{
    info.right = true;
    info.filePath  = filePath;
    int pos = filePath.lastIndexOf('/');
    info.fileName = filePath.mid(pos+1); //eg:18B0139284-A-8551-D01_A_2F_D01.ab1

    QVector<QStringRef> part = info.fileName.splitRef("_");
    if(part.size() != 4)
    {
        info.right = false;
    }
    else
    {
        info.sampleName = part.at(0).toString().toUpper();
        info.geneName = part.at(1).toString();
        QString exonString = part.at(2).toString();
        if(exonString.size()==2 && (exonString.endsWith('F') || exonString.endsWith('R')))
        {
            info.exonIndex = exonString.at(0);
            info.rOrF = exonString.at(1);
            info.gsspName = "";
        }
        else
        {
            QMap<QString, ExonAndRF>::iterator it = m_map_ExonAndRF.find(exonString);
            if(it != m_map_ExonAndRF.end())
            {
                info.gsspName = exonString;
                info.exonIndex = it.value().exonIndex;
                info.rOrF = it.value().rOrF;
            }
            else {
                QMap<QString, ExonAndRF>::iterator it_common = m_map_CommonGssp.find(exonString);
                if(it_common != m_map_CommonGssp.end())//虽然是引物文件，但是当作正常的测序文件进行处理
                {
                    info.exonIndex = it_common.value().exonIndex;
                    info.rOrF = it_common.value().rOrF;
                    info.gsspName = "";
                }
                else
                {
                     info.right = false;
                }
            }
        }
    }
}

void fileprocessthreadtask::run()
{
    for(int i=0; i<m_strlist.size(); i++)
    {
        OpenFileTable info;
        FilePathProcess(m_strlist.at(i), info);
        emit processone(info);
    }
}
