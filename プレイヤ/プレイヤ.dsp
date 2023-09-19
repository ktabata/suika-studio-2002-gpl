# Microsoft Developer Studio Project File - Name="プレイヤ" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=プレイヤ - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "プレイヤ.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "プレイヤ.mak" CFG="プレイヤ - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "プレイヤ - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "プレイヤ - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "プレイヤ - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /WX /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "プレイヤ - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "プレイヤ - Win32 Release"
# Name "プレイヤ - Win32 Debug"
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\EventType.cpp
# End Source File
# Begin Source File

SOURCE=..\VNBinFileReader.cpp
# End Source File
# Begin Source File

SOURCE=..\VNBinFileReader.h
# End Source File
# Begin Source File

SOURCE=..\VNBinFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\VNBinFileWriter.h
# End Source File
# Begin Source File

SOURCE=..\VNDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\VNDialog.h
# End Source File
# Begin Source File

SOURCE=..\VNEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\VNEvent.h
# End Source File
# Begin Source File

SOURCE=..\VNEventFile.cpp
# End Source File
# Begin Source File

SOURCE=..\VNEventFile.h
# End Source File
# Begin Source File

SOURCE=..\VNException.cpp
# End Source File
# Begin Source File

SOURCE=..\VNException.h
# End Source File
# Begin Source File

SOURCE=..\VNFlagFile.cpp
# End Source File
# Begin Source File

SOURCE=..\VNFlagFile.h
# End Source File
# Begin Source File

SOURCE=..\VNString.cpp
# End Source File
# Begin Source File

SOURCE=..\VNString.h
# End Source File
# Begin Source File

SOURCE=..\VNWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\VNWnd.h
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Group "画像処理"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AlphaTable.c
# End Source File
# Begin Source File

SOURCE=.\VNImage.cpp
# End Source File
# Begin Source File

SOURCE=.\VNImage.h
# End Source File
# Begin Source File

SOURCE=.\VNImage_blt.cpp
# End Source File
# Begin Source File

SOURCE=.\VNImage_buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNImage_load.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPromptDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPromptDrawer.h
# End Source File
# Begin Source File

SOURCE=.\VNTextDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNTextDrawer.h
# End Source File
# End Group
# Begin Group "イベント"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNPControl.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPSound.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPVisual.cpp
# End Source File
# End Group
# Begin Group "ウインドウ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNPWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPWindow.h
# End Source File
# End Group
# Begin Group "セーブ＆ロード"

# PROP Default_Filter ""
# Begin Group "セーブ＆ロード ダイアログ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNDLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDLoad.h
# End Source File
# Begin Source File

SOURCE=.\VNDSave.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDSave.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\VNPSave.cpp
# End Source File
# Begin Source File

SOURCE=.\VNSaveFile.cpp
# End Source File
# Begin Source File

SOURCE=.\VNSaveFile.h
# End Source File
# End Group
# Begin Group "バックログ"

# PROP Default_Filter ""
# Begin Group "バックログ ダイアログ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNDBackLog.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDBackLog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\VNBackLog.cpp
# End Source File
# Begin Source File

SOURCE=.\VNBackLog.h
# End Source File
# Begin Source File

SOURCE=.\VNPBackLog.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\VNPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNPlayer.h
# End Source File
# Begin Source File

SOURCE=.\VNPlayerProfile.h
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mouse.cur
# End Source File
# Begin Source File

SOURCE=.\player.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\Script.rc
# End Source File
# Begin Source File

SOURCE=.\ship.ico
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNCDPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNCDPlayer.h
# End Source File
# Begin Source File

SOURCE=.\VNMidiPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\VNMidiPlayer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Target
# End Project
