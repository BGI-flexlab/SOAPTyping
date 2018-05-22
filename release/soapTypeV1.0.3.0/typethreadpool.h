#ifndef TYPETHREADPOOL_H
#define TYPETHREADPOOL_H
#include "ghthreadengine.h"
#define COUNT 4
class TypeThreadPool:public GHThreadTaskItem
{

public:
  TypeThreadPool();
  void mapped(QList<FileTable>,bool  (* task_fun)(FileTable ));
  void mapped(QList<GsspFileTable>,bool  (* task_fun)(GsspFileTable ));
  void mapped(QList<QByteArray>,bool  (* task_fun)(const QByteArray  &));
  virtual void run( FileTable &fileTableList);
  virtual void run( GsspFileTable &fileTableList);
  virtual void run( QByteArray &fileTableList);


private:
     //static const int Maxcount=8;


  bool (*task_source_File)(FileTable );
  bool (*task_source_Gssp)(GsspFileTable );
  bool (*task_source_Byte)(const QByteArray &);
      int current;
      int total;
      GHThreadEngine * pool;
      int count;

public slots:
     virtual  void finishedbyOne();

};

#endif // TYPETHREADPOOL_H
