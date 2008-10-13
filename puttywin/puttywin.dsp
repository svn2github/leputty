# Microsoft Developer Studio Project File - Name="putty" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=putty - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "puttywin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "puttywin.mak" CFG="putty - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "putty - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "putty - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "putty - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 advapi32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib shell32.lib winmm.lib imm32.lib winspool.lib ole32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Release/putty.exe"

!ELSEIF  "$(CFG)" == "putty - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DEBUG" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 advapi32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib shell32.lib winmm.lib imm32.lib winspool.lib ole32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/putty.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "putty - Win32 Release"
# Name "putty - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\putty.ico
# End Source File
# Begin Source File

SOURCE=..\puttycfg.ico
# End Source File
# Begin Source File

SOURCE=..\win_res.rc
# End Source File
# End Group
# Begin Group "NonSSH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ldisc.c
# End Source File
# Begin Source File

SOURCE=..\pzmodem.c
# End Source File
# Begin Source File

SOURCE=..\raw.c
# End Source File
# Begin Source File

SOURCE=..\rlogin.c
# End Source File
# Begin Source File

SOURCE=..\telnet.c
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\be_all.c
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cmdline.c
# End Source File
# Begin Source File

SOURCE=..\misc.c
# End Source File
# Begin Source File

SOURCE=..\pproxy.c
# End Source File
# Begin Source File

SOURCE=..\proxy.c
# End Source File
# Begin Source File

SOURCE=..\settings.c
# End Source File
# Begin Source File

SOURCE=..\tree234.c
# End Source File
# Begin Source File

SOURCE=..\version.c
# End Source File
# Begin Source File

SOURCE=..\windefs.c
# End Source File
# Begin Source File

SOURCE=..\winmisc.c
# End Source File
# Begin Source File

SOURCE=..\winnet.c
# End Source File
# Begin Source File

SOURCE=..\winstore.c
# End Source File
# End Group
# Begin Group "SSH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\noise.c
# End Source File
# Begin Source File

SOURCE=..\pageantc.c
# End Source File
# Begin Source File

SOURCE=..\portfwd.c
# End Source File
# Begin Source File

SOURCE=..\ssh.c
# End Source File
# Begin Source File

SOURCE=..\sshaes.c
# End Source File
# Begin Source File

SOURCE=..\sshblowf.c
# End Source File
# Begin Source File

SOURCE=..\sshbn.c
# End Source File
# Begin Source File

SOURCE=..\sshcrc.c
# End Source File
# Begin Source File

SOURCE=..\sshcrcda.c
# End Source File
# Begin Source File

SOURCE=..\sshdes.c
# End Source File
# Begin Source File

SOURCE=..\sshdh.c
# End Source File
# Begin Source File

SOURCE=..\sshdss.c
# End Source File
# Begin Source File

SOURCE=..\sshmd5.c
# End Source File
# Begin Source File

SOURCE=..\sshpubk.c
# End Source File
# Begin Source File

SOURCE=..\sshrand.c
# End Source File
# Begin Source File

SOURCE=..\sshrsa.c
# End Source File
# Begin Source File

SOURCE=..\sshsh512.c
# End Source File
# Begin Source File

SOURCE=..\sshsha.c
# End Source File
# Begin Source File

SOURCE=..\sshzlib.c
# End Source File
# Begin Source File

SOURCE=..\wildcard.c
# End Source File
# Begin Source File

SOURCE=..\x11fwd.c
# End Source File
# End Group
# Begin Group "GUITERM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config.c
# End Source File
# Begin Source File

SOURCE=..\dialog.c
# End Source File
# Begin Source File

SOURCE=..\ldiscucs.c
# End Source File
# Begin Source File

SOURCE=..\logging.c
# End Source File
# Begin Source File

SOURCE=..\printing.c
# End Source File
# Begin Source File

SOURCE=..\sizetip.c
# End Source File
# Begin Source File

SOURCE=..\terminal.c
# End Source File
# Begin Source File

SOURCE=..\unicode.c
# End Source File
# Begin Source File

SOURCE=..\wcwidth.c
# End Source File
# Begin Source File

SOURCE=..\wincfg.c
# End Source File
# Begin Source File

SOURCE=..\winctrls.c
# End Source File
# Begin Source File

SOURCE=..\windlg.c
# End Source File
# Begin Source File

SOURCE=..\window.c
# End Source File
# Begin Source File

SOURCE=..\winutils.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\putty.mft
# End Source File
# End Target
# End Project
