Section $(SECTION_TITLE_CONNECTOR) SecConnector

    ; Set the shell context
    SetShellVarContext all

    SetOverwrite ifnewer

    !include if_connector.nsh
    
    SetOutPath "$INSTDIR\bin"
    ; Create shortcuts for HTTP_CONNECTOR
    CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Синхронизатор данных.lnk" "$INSTDIR\${EXEC_SUBDIR}\http_connector.exe"

SectionEnd
