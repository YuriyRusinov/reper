Section $(SECTION_TITLE_OPERATOR) SecOperator

    ; Set the shell context
    SetShellVarContext all

    SetOverwrite ifnewer

    !include if_operator.nsh

    SetOutPath "$INSTDIR\bin"
    ; Create shortcuts for Operator Client
    CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}\Оператор.lnk" "$INSTDIR\${EXEC_SUBDIR}\${EXECUTABLE_PREFIX}_app.exe"

SectionEnd
