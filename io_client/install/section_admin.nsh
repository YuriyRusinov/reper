Section $(SECTION_TITLE_ADMIN) SecAdmin

    ; Set the shell context
    SetShellVarContext all

    SetOverwrite ifnewer

    ;!include if_admin.nsh
    
    ;SetOutPath "$INSTDIR\bin"
    ; Create shortcuts for Admin Client
    CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Администратор.lnk" "$INSTDIR\${EXEC_SUBDIR}\${EXECUTABLE_PREFIX}_app.exe" "admin"

SectionEnd
