# Microsoft Developer Studio Project File - Name="�t���O�G�f�B�^" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=�t���O�G�f�B�^ - Win32 Debug
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "�t���O�G�f�B�^.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "�t���O�G�f�B�^.mak" CFG="�t���O�G�f�B�^ - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "�t���O�G�f�B�^ - Win32 Release" ("Win32 (x86) Application" �p)
!MESSAGE "�t���O�G�f�B�^ - Win32 Debug" ("Win32 (x86) Application" �p)
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "�t���O�G�f�B�^ - Win32 Release"

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
# ADD CPP /nologo /G6 /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "�t���O�G�f�B�^ - Win32 Debug"

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
# ADD CPP /nologo /G6 /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
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

# Name "�t���O�G�f�B�^ - Win32 Release"
# Name "�t���O�G�f�B�^ - Win32 Debug"
# Begin Group "lib"

# PROP Default_Filter ""
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

SOURCE=..\VNException.cpp
# End Source File
# Begin Source File

SOURCE=..\VNFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\VNFileDialog.h
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
# End Group
# Begin Group "header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNDFlagEdit.h
# End Source File
# Begin Source File

SOURCE=.\VNDFlagEdit_Add.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VNDFlagEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\VNDFlagEdit_Add.cpp
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\flageditor.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\map.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Script.rc

!IF  "$(CFG)" == "�t���O�G�f�B�^ - Win32 Release"

!ELSEIF  "$(CFG)" == "�t���O�G�f�B�^ - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Target
# End Project
