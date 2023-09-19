# Microsoft Developer Studio Project File - Name="イベントエディタ" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=イベントエディタ - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "イベントエディタ.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "イベントエディタ.mak" CFG="イベントエディタ - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "イベントエディタ - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "イベントエディタ - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "イベントエディタ - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "イベントエディタ - Win32 Debug"

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
# ADD CPP /nologo /G6 /W3 /Gm /GX /Zi /Od /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "イベントエディタ - Win32 Release"
# Name "イベントエディタ - Win32 Debug"
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

SOURCE=..\VNFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\VNFileDialog.h
# End Source File
# Begin Source File

SOURCE=..\VNString.cpp
# End Source File
# Begin Source File

SOURCE=..\VNString.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter ""
# Begin Group "event source"

# PROP Default_Filter "VND*.cpp"
# Begin Source File

SOURCE=.\VNDBackground.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDCdda.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDComment.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDJump.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDMidi.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDNarration.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDPcm.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDSerif.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDWait.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\VNDEventEdit.cpp
# End Source File
# End Group
# Begin Group "header"

# PROP Default_Filter ""
# Begin Group "event header"

# PROP Default_Filter "VND*.h"
# Begin Source File

SOURCE=.\VNDBackground.h
# End Source File
# Begin Source File

SOURCE=.\VNDCdda.h
# End Source File
# Begin Source File

SOURCE=.\VNDCharacter.h
# End Source File
# Begin Source File

SOURCE=.\VNDComment.h
# End Source File
# Begin Source File

SOURCE=.\VNDFlag.h
# End Source File
# Begin Source File

SOURCE=.\VNDJump.h
# End Source File
# Begin Source File

SOURCE=.\VNDLabel.h
# End Source File
# Begin Source File

SOURCE=.\VNDMidi.h
# End Source File
# Begin Source File

SOURCE=.\VNDNarration.h
# End Source File
# Begin Source File

SOURCE=.\VNDPcm.h
# End Source File
# Begin Source File

SOURCE=.\VNDSelect.h
# End Source File
# Begin Source File

SOURCE=.\VNDSerif.h
# End Source File
# Begin Source File

SOURCE=.\VNDTitle.h
# End Source File
# Begin Source File

SOURCE=.\VNDWait.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\VNDEventEdit.h
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\eventeditor.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\Script.rc

!IF  "$(CFG)" == "イベントエディタ - Win32 Release"

!ELSEIF  "$(CFG)" == "イベントエディタ - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\voyage.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Target
# End Project
