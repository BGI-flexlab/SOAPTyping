#include "config.h"
const QString configFile = "config.dat";
const QString configVersion = "1.0.0";
const qint32 configMagic = 1357924680;
void setDefaultConfig(QMap<QString, QString> &configMap)
{
    configMap.clear();
    configMap.insert("OpenThread", "2");
    configMap.insert("AnalysisThread", "2");
    configMap.insert("IgnoreIndel", "1");
    configMap.insert("UserID","");
    configMap.insert("Password", "");
}

void writeConfig(QMap<QString, QString> &configMap)
{
    QFile file(configFile);
    file.open(QFile::WriteOnly);
    QDataStream stream(&file);
    stream<<configMagic;
    stream<<configVersion;
    for(QMap<QString, QString>::iterator it=configMap.begin(); it!=configMap.end(); it++)
    {
        stream<<it.key()<<it.value();
    }
    file.close();
}
void readConfig(QMap<QString, QString> &configMap)
{
    QFile file(configFile);
    if(!file.exists())
    {
        setDefaultConfig(configMap);
        writeConfig(configMap);
    }
    else
    {
        file.open(QFile::ReadOnly);
        QDataStream stream(&file);
        qint32 m;
        QString v;
        stream>>m>>v;
        while(!stream.atEnd())
        {
            QString key, value;
            stream>>key>>value;
            configMap.insert(key, value);
        }
        file.close();
    }
}
int getPoolThreadNum()
{
    QMap<QString, QString> config;
    readConfig(config);
    return config.find("OpenThread").value().toInt();
}

bool getIgnoreIndel()
{
    QMap<QString, QString> config;
    readConfig(config);
    int ig = config.find("IgnoreIndel").value().toInt();
    if(ig==1)
        return true;
    return false;

}

int getAnalysisThreadNum()
{
    QMap<QString, QString> config;
    readConfig(config);
    return config.find("AnalysisThread").value().toInt();
}
