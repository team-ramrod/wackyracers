# Microsoft Developer Studio Project File - Name="LSY201" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LSY201 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LSY201.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LSY201.mak" CFG="LSY201 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LSY201 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LSY201 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LSY201 - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "LSY201 - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LSY201 - Win32 Release"
# Name "LSY201 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BmpStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\CqOctree.cpp
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\dibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\Huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\IP.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\LSY201.cpp
# End Source File
# Begin Source File

SOURCE=.\LSY201.rc
# End Source File
# Begin Source File

SOURCE=.\LSY201Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LSY_201.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BmpStatic.h
# End Source File
# Begin Source File

SOURCE=.\CDERROR.H
# End Source File
# Begin Source File

SOURCE=.\CDJPEG.H
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\Dib.h
# End Source File
# Begin Source File

SOURCE=.\dibapi.h
# End Source File
# Begin Source File

SOURCE=.\Huffman.h
# End Source File
# Begin Source File

SOURCE=.\ImageStatic.h
# End Source File
# Begin Source File

SOURCE=.\JCHUFF.H
# End Source File
# Begin Source File

SOURCE=.\JCONFIG.H
# End Source File
# Begin Source File

SOURCE=.\JDCT.H
# End Source File
# Begin Source File

SOURCE=.\JDHUFF.H
# End Source File
# Begin Source File

SOURCE=.\JERROR.H
# End Source File
# Begin Source File

SOURCE=.\JINCLUDE.H
# End Source File
# Begin Source File

SOURCE=.\JMEMSYS.H
# End Source File
# Begin Source File

SOURCE=.\Jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg.h
# End Source File
# Begin Source File

SOURCE=.\JPEGINT.H
# End Source File
# Begin Source File

SOURCE=.\JPEGLIB.H
# End Source File
# Begin Source File

SOURCE=.\JVERSION.H
# End Source File
# Begin Source File

SOURCE=.\LS_Y201.h
# End Source File
# Begin Source File

SOURCE=.\LSY201.h
# End Source File
# Begin Source File

SOURCE=.\LSY201Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TifHeader.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\backup\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bottom.bmp
# End Source File
# Begin Source File

SOURCE=..\..\backup\res\GxtY601.ico
# End Source File
# Begin Source File

SOURCE=.\res\GxtY601.ico
# End Source File
# Begin Source File

SOURCE=..\..\backup\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LSY201.ico
# End Source File
# Begin Source File

SOURCE=.\res\LSY201.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
