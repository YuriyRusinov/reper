Section $(SECTION_TITLE_API) SecAPI

    ; Set the shell context
    SetShellVarContext all

    SetOverwrite ifnewer

    !include if_api.nsh

    ;ReadRegStr $0 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path"
    ;StrCpy $0 "$INSTDIR\bin;$0"
    ;WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path" $0

    ;ReadRegStr $0 HKCU "Environment" "LIB"
    ;StrCpy $0 "$INSTDIR\lib;$0"
    ;WriteRegStr HKCU "Environment" "LIB" $0

    ; Create shortcuts for XGIS Client


SectionEnd
