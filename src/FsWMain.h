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
#include <QJsonArray>
#include <QListWidget>
#include <QTimer>

class FsWMain : public QMainWindow
  {
    Q_OBJECT

    QString             mConfigPath; //Current config file
    QJsonArray          mFileList;   //Watching file list
    QListWidget        *mViewList;   //Visual representation of file list
    QTimer              mTimer;      //Timer to scan files
    int                 mIndex;
  public:
    explicit FsWMain(QWidget *parent = nullptr);
    ~FsWMain();

  public slots:
    //Commands

    void cmConfigNew();
    void cmConfigOpen();
    void cmConfigSave();
    void cmConfigSaveAs();
    void cmConfigExit();

    void cmFileAppend();
    void cmFileRemove();

    void onFileChanged( const QString path );

  private:
    void testFileChanged( const Q)
    void configOpen( const QString path );
    void fileAppend( const QString path );
    void fileCopy( const QString src, const QString dst, const QDateTime time );
    void watch( bool enable );
  };

#endif // FSWMAIN_H
