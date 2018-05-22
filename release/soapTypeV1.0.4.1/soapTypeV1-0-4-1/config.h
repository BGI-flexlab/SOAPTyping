#ifndef CONFIG_H
#define CONFIG_H
#include <QtCore>
void setDefaultConfig(QMap<QString, QString> &configMap);
void readConfig(QMap<QString, QString> &configMap);
void writeConfig(QMap<QString, QString> &configMap);
int getPoolThreadNum();
bool getIgnoreIndel();
int getAnalysisThreadNum();
#endif // CONFIG_H
