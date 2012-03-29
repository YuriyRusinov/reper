; Use better compression
SetCompressor lzma

!include "Sections.nsh"
!include "MUI.nsh"

!include defines.nsh

ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"

ShowInstDetails show
ShowUnInstDetails show

; --------------------------------
; Interface Settings

!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\colorful.bmp"
!define MUI_ICON "ico\kkssito32.ico"
!define MUI_UNICON "ico\kkssito32.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "ico\kkssito-large.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "ico\kkssito-large.bmp"

; --------------------------------
; Install Pages
!include install_pages.nsh

!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

!include lang_strings.nsh

;--------------------------------
; Installer Sections
!include section_core.nsh
!include section_api.nsh
!include section_src.nsh
!include section_operator.nsh
!include section_admin.nsh
!include section_connector.nsh
!include section_uninstall.nsh

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecAPI} $(DESC_SecAPI)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSrc} $(DESC_SecSrc)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecOperator} $(DESC_SecOperator)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecAdmin} $(DESC_SecAdmin)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecConnector} $(DESC_SecConnector)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit

    ;IntOp $0 0 | ${SF_RO}
    ;SectionSetFlags ${SecGrid} $0
    ;SectionSetFlags ${SecQualifier} $0
    ;SectionSetFlags ${SecSearch} $0
    ;SectionSetFlags ${SecRaster} $0
    ;SectionSetFlags ${SecSRS} $0
    

    !insertmacro MUI_LANGDLL_DISPLAY
	; Initialise the plugins dir
	InitPluginsDir
FunctionEnd

Function .onGUIEnd
    SetOutPath "$TEMP"
    
    RMDir /r "${TEMPDIR}"
    ;MessageBox MB_OK $(PROMPT_TO_REBOOT)
    
   NoAbort:

FunctionEnd

Function .onSelChange
    SectionGetFlags ${SecAPI} $0
    IntOp $0 $0 & ${SF_SELECTED}
    IntCmp $0 0 unset_src
    Return
    unset_src:
        SectionGetFlags ${SecSrc} $0
        IntOp $0 $0 & 126
        SectionSetFlags ${SecSrc} $0
FunctionEnd
