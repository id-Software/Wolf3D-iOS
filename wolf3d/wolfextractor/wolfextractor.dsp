# Microsoft Developer Studio Project File - Name="wolfextractor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=wolfextractor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wolfextractor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wolfextractor.mak" CFG="wolfextractor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wolfextractor - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "wolfextractor - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wolfextractor - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D vsnprintf=_vsnprintf /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib vorbisenc_static.lib vorbis_static.lib ogg_static.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcmt.lib"

!ELSEIF  "$(CFG)" == "wolfextractor - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D vsnprintf=_vsnprintf /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib vorbisenc_static.lib vorbis_static.lib ogg_static.lib /nologo /subsystem:console /profile /debug /machine:I386 /nodefaultlib:"libcmt.lib"

!ENDIF 

# Begin Target

# Name "wolfextractor - Win32 Release"
# Name "wolfextractor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\adlib\adlib.c
# End Source File
# Begin Source File

SOURCE=..\..\common\arch.c
# End Source File
# Begin Source File

SOURCE=.\string\com_string.c
# End Source File
# Begin Source File

SOURCE=..\..\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\filesys\file.c
# End Source File
# Begin Source File

SOURCE=.\adlib\fmopl.c
# End Source File
# Begin Source File

SOURCE=.\hq2x.c
# End Source File
# Begin Source File

SOURCE=.\mac\mac.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\memory\memory.c
# End Source File
# Begin Source File

SOURCE=.\pak.c
# End Source File
# Begin Source File

SOURCE=.\loaders\tga.c
# End Source File
# Begin Source File

SOURCE=..\..\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\vorbisenc_inter.c
# End Source File
# Begin Source File

SOURCE=.\loaders\wav.c
# End Source File
# Begin Source File

SOURCE=.\filesys\win32\win_file.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wl6_name.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_aud.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_gfx.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_map.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_pal.c
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_pm.c
# End Source File
# Begin Source File

SOURCE=.\zip\zipfile.c
# End Source File
# Begin Source File

SOURCE=..\..\zlib\zutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\adlib\adlib.h
# End Source File
# Begin Source File

SOURCE=..\..\common\arch.h
# End Source File
# Begin Source File

SOURCE=.\string\com_string.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common_utils.h
# End Source File
# Begin Source File

SOURCE=.\filesys\file.h
# End Source File
# Begin Source File

SOURCE=.\adlib\fmopl.h
# End Source File
# Begin Source File

SOURCE=.\hq2x.h
# End Source File
# Begin Source File

SOURCE=.\mac\mac.h
# End Source File
# Begin Source File

SOURCE=.\memory\memory.h
# End Source File
# Begin Source File

SOURCE=.\loaders\tga.h
# End Source File
# Begin Source File

SOURCE=.\loaders\wav.h
# End Source File
# Begin Source File

SOURCE=.\wolf\wolf_def.h
# End Source File
# Begin Source File

SOURCE=.\zip\zip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
