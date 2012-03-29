Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "Удаление программы $(^Name) было успешно завершено."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Вы уверены в том, что желаете удалить $(^Name) и все компоненты программы?" IDYES +2
  Abort
FunctionEnd

Section Uninstall

    SetShellVarContext all
    
    RMDir /r "$INSTDIR"
    RmDir /r "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME} ${PRODUCT_VERSION}"
    ;RmDir "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
    RmDir "$SMPROGRAMS\${PRODUCT_PUBLISHER}"

    DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
    DeleteRegKey /ifempty HKLM "Software\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
    DeleteRegKey /ifempty HKLM "Software\${PRODUCT_PUBLISHER}"

    DeleteRegKey HKCU "${PRODUCT_DIR_REGKEY}"
    DeleteRegKey /ifempty HKCU "Software\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
    DeleteRegKey /ifempty HKCU "Software\${PRODUCT_PUBLISHER}"
    ;SetAutoClose true

	SetShellVarContext all
	

	; Remove installer registry entries
	DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "DisplayName"
	DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "UninstallString"
	DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "DisplayIcon"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"

	; Remove the uninstaller itself
	Delete "$INSTDIR\${UNINSTALLER_EXECUTABLE}"

SectionEnd
