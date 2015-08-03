Section $(SECTION_TITLE_CORE) SecCore

    SectionIn RO
    ; Set the shell context
    SetShellVarContext all

    SetOverwrite ifnewer

    SetOutPath "$INSTDIR"

    !include if_core.nsh
    
    SetOutPath "$INSTDIR"
    ; Create shortcuts for KKS SITO Client
    CreateDirectory "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\${UNINSTALLER_EXECUTABLE}"

    ;CreateDirectory "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Doc"
    ;CreateShortcut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Doc\Инструкция по установке.lnk" "$INSTDIR\Installation Notes.rtf"

    ; Store install folder
    WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" $INSTDIR
    WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "LastOpenedDir" "."
    WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "LastSelectedFilter" "All files (*.*)"
    WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "Translators_path" "$INSTDIR\bin"

    WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}" "" $INSTDIR
    WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "LastOpenedDir" "."
    WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "LastSelectedFilter" "All files (*.*)"
    WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}\System settings\${EXECUTABLE_PREFIX}" "Translators_path" "$INSTDIR\bin"

    ; Store uninstall information
    WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "DisplayName" "$(^Name) (remove only)"
    WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "UninstallString" '"$INSTDIR\${UNINSTALLER_EXECUTABLE}"'
    WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "DisplayIcon" '"$INSTDIR\${UNINSTALLER_EXECUTABLE}"'

    ; Create uninstaller
    WriteUninstaller "$INSTDIR\${UNINSTALLER_EXECUTABLE}"

SectionEnd
