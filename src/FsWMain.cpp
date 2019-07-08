/*
  Проект ""
  Автор
    Сибилев А.С.
  Описание

*/
#include "FsConfig.h"
#include "FsWMain.h"

#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>

FsWMain::FsWMain(QWidget *parent) :
  QMainWindow(parent),
  mIndex(0)
  {
  QSettings s;

  QMenuBar *menu = menuBar();
  QMenu *workspace = menu->addMenu( tr("Workspace") );
  workspace->addAction( tr("New"), this, &FsWMain::cmWorkspaceNew );
  workspace->addAction( tr("Open..."), this, &FsWMain::cmWorkspaceOpen );
  workspace->addAction( tr("Save"), this, &FsWMain::cmWorkspaceSave );
  workspace->addAction( tr("Save as..."), this, &FsWMain::cmWorkspaceSaveAs );
  workspace->addSeparator();
  workspace->addAction( tr("Exit"), this, &FsWMain::cmWorkspaceExit );

  QMenu *file = menu->addMenu( tr("Files") );
  file->addAction( tr("Append..."), this, &FsWMain::cmFileAppend );
  file->addAction( tr("Remove"), this, &FsWMain::cmFileRemove );


  mViewList = new QListWidget();
  setCentralWidget( mViewList );

  workspaceOpen( s.value(SDK_LAST_FILE, QString("default" FS_CONFIG_EXTENSION ) ).toString() );

  mTimer.start( 20 );
  connect( &mTimer, &QTimer::timeout, this, &FsWMain::onTimer );
  }




FsWMain::~FsWMain()
  {
  }





void FsWMain::cmWorkspaceNew()
  {
  //Clear workspace file name
  mWorkspace.clear();
  //Clear config contents
  mViewList->clear();
  }




void FsWMain::cmWorkspaceOpen()
  {
  if( !testDirty() ) {
    QString path = QFileDialog::getOpenFileName( this, tr("Open workspace file name") );
    if( !path.isEmpty() )
      workspaceOpen( path );
    }
  }






void FsWMain::cmWorkspaceSave()
  {
  if( mWorkspace.isEmpty() || mWorkspace.startsWith("default") )
    cmWorkspaceSaveAs();
  else
    workspaceSave( mWorkspace );
  }





void FsWMain::cmWorkspaceSaveAs()
  {
  QString file = QFileDialog::getSaveFileName( this, tr("Enter name to save workspace"), mWorkspace );
  if( !file.isEmpty() ) {
    if( !file.endsWith( QString(FS_CONFIG_EXTENSION) ) )
      file.append( QString(FS_CONFIG_EXTENSION) );
    mWorkspace = file;
    workspaceSave( file );
    }
  }

void FsWMain::cmWorkspaceExit()
  {

  }





void FsWMain::cmFileAppend()
  {
  QString file = QFileDialog::getOpenFileName( this, tr("Select file to add to sync list") );
  if( !file.isEmpty() )
    fileAppend( file );
  }




void FsWMain::cmFileRemove()
  {
  int i = mViewList->currentRow();
  if( i >= 0 && i < mViewList->count() ) {
    mViewList->model()->removeRow( i );
    mDirty = true;
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("No selected row to delete. Select and try again.") );
  }




void FsWMain::onTimer()
  {
  if( mIndex < mViewList->count() )
    testFileChanged( mViewList->item(mIndex++)->text() );
  else mIndex = 0;
  }





bool FsWMain::testDirty()
  {
  if( mDirty ) {
    if( QMessageBox::question( this, tr("Warning!"), tr("Workspace '%1' is changed. Save changes to disk?").arg(mWorkspace) ) == QMessageBox::Yes )
      cmWorkspaceSave();
    else return false;
    }
  return mDirty;
  }




void FsWMain::fileAppend(const QString path)
  {
  //Find place to insert path
  QString fileName( QFileInfo(path).fileName() );
  for( int i = 0; i < mViewList->count(); i++ ) {
    QString testPath( mViewList->item(i)->text() );
    if( testPath == path )
      //Skeep same name file
      return;
    if( QFileInfo(testPath).fileName() > fileName ) {
      mViewList->insertItem( i, path );
      mDirty = true;
      return;
      }
    }
  mViewList->addItem( path );
  mDirty = true;
  }






void FsWMain::testFileChanged(const QString path)
  {
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
  }








void FsWMain::workspaceOpen(const QString path)
  {
  qDebug() << "try open" << path;
  mWorkspace.clear();
  mViewList->clear();
  mDirty = false;
  //Try open config file
  QFile file(path);
  if( file.open( QIODevice::ReadOnly ) ) {
    mWorkspace = path;
    QSettings s;
    s.setValue( SDK_LAST_FILE, path );

    QJsonArray fileList = QJsonDocument::fromJson( file.readAll() ).array();
    //Config file successfully readed, fill visual list
    for( auto i : fileList )
      //Retrive file path and append it to view
      mViewList->addItem( i.toString() );
    }
  }





void FsWMain::workspaceSave(const QString path)
  {
  QSettings s;
  s.setValue( SDK_LAST_FILE, path );

  //Prepare json array of files to save
  QJsonArray ar;
  for( int i = 0; i < mViewList->count(); i++ )
    ar.append( mViewList->item(i)->text() );

  //Open file and save array to it
  QFile file( path );
  if( file.open(QIODevice::WriteOnly) ) {
    file.write( QJsonDocument(ar).toJson() );
    mDirty = false;
    }
  }









void FsWMain::fileCopy(const QString src, const QString dst, const QDateTime time)
  {
  qDebug() << "upgraded" << dst;
  QFile is(src);
  QFile os(dst);
  if( is.open(QIODevice::ReadOnly) && os.open(QIODevice::WriteOnly) ) {
    os.write( is.readAll() );
    os.flush();
    os.setFileTime( time, QFileDevice::FileModificationTime ); // << time << src << os.fileTime(QFileDevice::FileModificationTime);
    os.close();
    is.close();
    }
  //qDebug() << "closed time" << os.fileTime(QFileDevice::FileModificationTime);
  }



