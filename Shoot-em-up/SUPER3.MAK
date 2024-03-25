# Microsoft Visual C++ Generated NMAKE File, Format Version 20051
# MSVCPRJ: version 2.0.4135
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "SUPER3.MAK" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
OUTDIR=.
INTDIR=.

ALL : $(OUTDIR)/SUPER3.exe $(OUTDIR)/SUPER3.bsc $(MTL_TLBS)

# ADD BASE CPP /nologo /ML /W3 /GX /YX /O2 /D "N32" /D "NDEBUG" /D "_CONSOLE" /FR /c
# ADD CPP /nologo /ML /W3 /GX /YX /O2 /D "N32" /D "NDEBUG" /D "_CONSOLE" /FR /c
CPP_PROJ=/nologo /ML /W3 /GX /YX /O2 /D "N32" /D "NDEBUG" /D "_CONSOLE"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"SUPER3.pch" /Fo$(INTDIR)/ /c 
# ADD BASE RSC /d "DEBUG"
# SUBTRACT BASE RSC /l 0x0
# ADD RSC /d "NDEBUG"
# SUBTRACT RSC /l 0x0
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/SUPER3.SBR
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"SUPER3.bsc" 

$(OUTDIR)/SUPER3.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/SUPER3.OBJ
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO /SUBSYSTEM:console /MACHINE:I386
# ADD LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO /SUBSYSTEM:console /MACHINE:I386
LINK32_FLAGS=user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO\
 /SUBSYSTEM:console /INCREMENTAL:no /PDB:$(OUTDIR)/"SUPER3.pdb" /MACHINE:I386\
 /OUT:$(OUTDIR)/"SUPER3.exe" 

$(OUTDIR)/SUPER3.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
OUTDIR=.
INTDIR=.

ALL : $(OUTDIR)/SUPER3.exe $(OUTDIR)/SUPER3.bsc $(MTL_TLBS)

# ADD BASE CPP /nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /c
# ADD CPP /nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /c
CPP_PROJ=/nologo /ML /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"SUPER3.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"SUPER3.pdb"\
 /c 
# ADD BASE RSC /d "_DEBUG"
# SUBTRACT BASE RSC /l 0x0
# ADD RSC /d "_DEBUG"
# SUBTRACT RSC /l 0x0
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/SUPER3.SBR
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"SUPER3.bsc" 

$(OUTDIR)/SUPER3.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/SUPER3.OBJ
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO /SUBSYSTEM:console /DEBUG /MACHINE:I386
# ADD LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO /SUBSYSTEM:console /DEBUG /MACHINE:I386
LINK32_FLAGS=user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib /NOLOGO\
 /SUBSYSTEM:console /INCREMENTAL:yes /PDB:$(OUTDIR)/"SUPER3.pdb" /DEBUG\
 /MACHINE:I386 /OUT:$(OUTDIR)/"SUPER3.exe" 

$(OUTDIR)/SUPER3.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\SUPER3.CPP

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/SUPER3.OBJ :  $(SOURCE)  $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/SUPER3.OBJ :  $(SOURCE)  $(INTDIR)

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
