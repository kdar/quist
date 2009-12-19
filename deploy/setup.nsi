; Define your application name
!define APPNAME "Quist"
!define APPNAMEANDVERSION "${APPNAME} 1.0.5.1"
!define WNDTITLE "Ventrilo [${APPNAME}]"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES64\${APPNAME}"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
CRCCheck on
XPStyle on
ShowInstDetails show
ShowUninstDetails show
OutFile "quist-setup.exe"

; Modern interface settings
!include "MUI2.nsh"

; Contains a function that checks for the correct .NET framework
!define DOT_MAJOR "3"
!define DOT_MINOR "5"
!define DOT_MINOR_MINOR "0"
!include "dotnet.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\${APPNAME}.exe"
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_ICON "Resources\quist-install.ico"
!define MUI_UNICON "Resources\quist-uninstall.ico"
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_INSTFILESPAGE_COLORS "FFFFFF 000000"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

; Specifies the requested execution level for Windows Vista. 
; Necessary for correct uninstallation of Start menu shortcuts.
;RequestExecutionLevel admin

LangString DESC_MainApplication ${LANG_ENGLISH} "The main ${APPNAME} application."
Section "${APPNAME} (required)" MainApplication
	
	; Makes this section read-only/required
	SectionIn 1 RO	
	
	SetShellVarContext all

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"
	File /r "..\Distro\*"
	CreateDirectory "$SMPROGRAMS\${APPNAME}"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	
	SetShellVarContext current 

SectionEnd

Section -FinishSection
	
	SetShellVarContext all

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
	SetShellVarContext current

SectionEnd

; Desktop shortcut (optional section)
LangString DESC_DesktopShortcut ${LANG_ENGLISH} "Create a desktop shortcut to ${APPNAME}."
Section "Desktop shortcut" DesktopShortcut	
  CreateShortCut "$DESKTOP\${APPNAME}.lnk" '$INSTDIR\${APPNAME}.exe'
SectionEnd

; Quick launch shortcut (optional section)
LangString DESC_QuickLaunchShortcut ${LANG_ENGLISH} "Create a quick launch shortcut to ${APPNAME}."
Section "Quick Launch shortcut" QuickLaunchShortcut
  CreateShortCut "$QUICKLAUNCH\${APPNAME}.lnk" '$INSTDIR\${APPNAME}.exe'
SectionEnd

;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${MainApplication} $(DESC_MainApplication)
	!insertmacro MUI_DESCRIPTION_TEXT ${DesktopShortcut} $(DESC_DesktopShortcut)
	!insertmacro MUI_DESCRIPTION_TEXT ${QuickLaunchShortcut} $(DESC_QuickLaunchShortcut)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

  SetShellVarContext all

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\${APPNAME}.lnk"
	Delete "$QUICKLAUNCH\${APPNAME}.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"

	; Clean up Quist
	Delete "$INSTDIR\*.*"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\${APPNAME}"
	RMDir /r "$INSTDIR\"
	
	SetShellVarContext current 

SectionEnd


Function .onInit
	;Call CheckDotNETInstallation
	
	; Unselect these components by default
	;SectionSetFlags ${DesktopShortcut} SF_USELECTED
	SectionSetFlags ${QuickLaunchShortcut} SF_USELECTED
	
	FindWindow $0 "" "${WNDTITLE}"    
	StrCmp $0 0 continueInstall
	MessageBox MB_ICONSTOP|MB_OK "The application you are trying to install is running. Close it and try again."
	Abort
continueInstall:
    
FunctionEnd

Function un.onInit
	FindWindow $0 "" "${WNDTITLE}"
	StrCmp $0 0 continueUnInstall
	MessageBox MB_ICONSTOP|MB_OK "The application you are trying to remove is running. Close it and try again."
	Abort
continueUnInstall:

FunctionEnd

; eof