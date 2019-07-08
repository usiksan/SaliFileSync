/*
Project "File synchronization utility"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Entry point for programm

*/
#include "FsConfig.h"
#include "FsWMain.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
  {
  //Setups for settings
  QCoreApplication::setOrganizationName("SaliLAB");
  QCoreApplication::setOrganizationDomain("http://salilab.com/");
  QCoreApplication::setApplicationName(FS_NAME);


  QApplication a(argc, argv);
  FsWMain w;
  w.show();

  return a.exec();
  }
