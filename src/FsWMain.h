/*
Project "File synchronization utility"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Main window widget. It contains menu and watching file list visual representation.

*/
#ifndef FSWMAIN_H
#define FSWMAIN_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>

class FsWMain : public QMainWindow
  {
    Q_OBJECT

    QString             mWorkspace;  //Current workspace file
    bool                mDirty;
    QListWidget        *mViewList;   //Visual representation of file list
    QTimer              mTimer;      //Timer to scan files
    int                 mIndex;      //Index of scaning file
  public:
    explicit FsWMain(QWidget *parent = nullptr);
    ~FsWMain();

  public slots:
    //Commands

    void cmWorkspaceNew();
    void cmWorkspaceOpen();
    void cmWorkspaceSave();
    void cmWorkspaceSaveAs();
    void cmWorkspaceExit();

    void cmFileAppend();
    void cmFileRemove();

    void onTimer();

  private:
    bool testDirty();
    void fileAppend( const QString path );
    void testFileChanged( const QString path );
    void workspaceOpen( const QString path );
    void workspaceSave( const QString path );
    void fileCopy( const QString src, const QString dst, const QDateTime time );
  };

#endif // FSWMAIN_H
