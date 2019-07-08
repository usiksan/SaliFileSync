/*
Project "File synchronization utility"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SaliFileSync configuration and history

History
  08.07.2019 v0.1  created
*/
#ifndef FSCONFIG_H
#define FSCONFIG_H

//Do'nt change this name
#define FS_AUTHOR                    "Alexander Sibilev"
#define FS_NAME                      "SaliFileSync"

//Version definition
#define FS_VERSION_MAJOR             0
#define FS_VERSION_MINOR             1

//Some defaults
#define FS_DEFAULT_WEB               "www.SaliLAB.com"

//SaliFileSync configuration file extension
#define FS_CONFIG_EXTENSION          ".saliFileSync"


//Previous file count in menu
#define PREVIOUS_FILES_COUNT         10


//Settings key names
#define SDK_WMAIN_MAX                "WMainMax"
#define SDK_LANGUAGE                 "Language"
#define SDK_LAST_FILE                "LastFile"
#define SDK_PREVIOUS_FILES           "PreviousFiles"
#define SDK_HELP_PATH                "HelpPath"


#endif // FSCONFIG_H
