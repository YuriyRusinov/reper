!define PRODUCT_NAME1 "DynamicDocs"
!define PRODUCT_NAME_ENG1 "DynamicDocs"
!define PRODUCT_NAME "${PRODUCT_NAME1} (клиентская часть)"
!define PRODUCT_NAME_ENG "${PRODUCT_NAME_ENG1} Client"
!define PRODUCT_VERSION "0.14.3"
!define PRODUCT_PUBLISHER "DynamicSoft"
!define EXECUTABLE_PREFIX "ddocclient"
!define PRODUCT_WEB_SITE "http://www.dynamicdocs.ru"

!define UNINSTALLER_EXECUTABLE "${EXECUTABLE_PREFIX}-${PRODUCT_VERSION}-uninstaller.exe"
!define SETUP_EXECUTABLE "${EXECUTABLE_PREFIX}-${PRODUCT_VERSION}-setup.exe"

!define PRODUCT_DIR_REGKEY "Software\${PRODUCT_PUBLISHER}\${PRODUCT_NAME_ENG} ${PRODUCT_VERSION}\"

!define CORE_DIR ".\core"
!define API_DIR ".\dev"
!define SRC_DIR "..\"
!define OPERATOR_DIR ".\operator"
!define ADMIN_DIR ".\admin"
!define CONNECTOR_DIR ".\connector"

; Place all temporary files used by the installer in their own subdirectory
; under $TEMP (makes the files easier to find)
!define TEMPDIR "$TEMP\${EXECUTABLE_PREFIX}_installer"

!define EXEC_SUBDIR "bin"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile ${SETUP_EXECUTABLE}
InstallDir "$PROGRAMFILES\${PRODUCT_PUBLISHER}\${PRODUCT_NAME_ENG} ${PRODUCT_VERSION}"
