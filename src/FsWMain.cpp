/*
  Проект ""
  Автор
    Сибилев А.С.
  Описание

*/
#include "FsWMain.h"
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QDateTime>

FsWMain::FsWMain(QWidget *parent) :
  QMainWindow(parent)
  {
  QSettings s;

  mViewList = new QListWidget();
  }




FsWMain::~FsWMain()
  {
  }




void FsWMain::onFileChanged(const QString path)
  {
  watch(false);

  //Update files
  //We scan list of files, test if file is same name and test time of files
  // if appended file under then existing then it is updated and scan break.
  // if appended file older then existing then existing is updated and scan continues
  QFileInfo info( path );
  QDateTime time( info.fileTime(QFileDevice::FileModificationTime) );
  for( int i = 0; i < mViewList->count(); i++ ) {
    QFileInfo testInfo( mViewList->item(i)->text() );
    if( info.fileName() == testInfo.fileName() ) {
      if( time > testInfo.fileTime(QFileDevice::FileModificationTime) )
        fileCopy( path, testInfo.filePath(), time );
      }
    }

  watch(true);
  }




void FsWMain::configOpen(const QString path)
  {
  watch(false);
  //Try open config file
  QFile file(path);
  if( file.open( QIODevice::ReadOnly ) ) {
    mFileList = QJsonDocument::fromJson( file.readAll() ).array();
    //Config file successfully readed, fill visual list
    for( auto i : mFileList )
      //Retrive file path and append it to watcher and view
      fileAppend( i.toString() );
    }
  watch(true);
  }




void FsWMain::fileAppend(const QString path)
  {
  //Append file to watcher
  mWatcher->addPath( path );

  //Update files
  //We scan list of files, test if file is same name and test time of files
  // if appended file under then existing then it is updated and scan break.
  // if appended file older then existing then existing is updated and scan continues
  QFileInfo info( path );
  QDateTime time( info.fileTime(QFileDevice::FileModificationTime) );
  for( int i = 0; i < mViewList->count(); i++ ) {
    QFileInfo testInfo( mViewList->item(i)->text() );
    if( info.fileName() == testInfo.fileName() ) {
      QDateTime testTime( testInfo.fileTime(QFileDevice::FileModificationTime) );
      if( time != testTime ) {
        if( time < testTime ) {
          fileCopy( testInfo.filePath(), path, testTime );
          break;
          }
        else {
          fileCopy( path, testInfo.filePath(), time );
          }
        }
      }
    }


  //Append file to view
  mViewList->addItem( path );
  }





void FsWMain::fileCopy(const QString src, const QString dst, const QDateTime time)
  {
  QFile is(src);
  QFile os(dst);
  if( is.open(QIODevice::ReadOnly) && os.open(QIODevice::WriteOnly) ) {
    os.write( is.readAll() );
    os.setFileTime( time, QFileDevice::FileModificationTime );
    os.close();
    is.close();
    }
  }



void FsWMain::watch(bool enable)
  {
  if( enable )
    connect( mWatcher, &QFileSystemWatcher::fileChanged, this, &FsWMain::onFileChanged );
  else
    disconnect( mWatcher, &QFileSystemWatcher::fileChanged, this, &FsWMain::onFileChanged );
  }
