@echo off

set VERSION=0.11.0

set OUTPUT=./error.log

if "%STDOUT_REDIRECTED%" == "" (
    set STDOUT_REDIRECTED=yes
    cmd.exe /c %0 %* >%OUTPUT% 2>&1
    exit /b %ERRORLEVEL%
)


rem ������ ��������� �� ����� settings.txt, ������� ������ ������������� �
rem ��� �� ��������, ��� � bat-����. ���� �� ������� ���������� ��������� -
rem ������� � ��������� ����� ��������.

call :read_settings ..\user.config || exit /b 1

%LINTER_ROOT%\bin\dropdb.exe -h %HOST% -p %LINTER_PORT% -U %USER% %BASE%

%LINTER_ROOT%\bin\createdb.exe -h %HOST% -p %LINTER_PORT% -E %ENCODING% -U %USER% %BASE% || exit 1

cd ../internals 

copy /Y functions\files\readd_files_c_lin.sql functions\files\readd_files_c.sql

%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -f ./initdb.sql %USER%

%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -c "select setkkssitoversion('DynamicDocs Server version %VERSION%', '%VERSION%')" -t %USER%
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -c "select setfilearchivepath('%PGDATA_DIR%/%FILE_ARCH%')" -t %USER%
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -c "select usetlocaladdress('%LOCAL_ADDRESS%')" -t %USER% 
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -c "select startsync()" -t %USER%
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -c "select setorgasmain(%IS_MAIN_ORG%)" -t %USER%

%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -f ./initdata.sql %USER%
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -f ./initiotriggers.sql %USER%
%LINTER_ROOT%\bin\psql.exe -h %HOST% -p %LINTER_PORT% -d %BASE% -f ./initprivs.sql %USER%


rem ����� �� ��������. ������ - ������ �������.
exit 0




rem
rem ������� ��� ������ �������� �� �����.
rem ����:
rem %1           - ��� ����� � �����������

:read_settings

set SETTINGSFILE=%1

rem �������� ������������� �����
if not exist %SETTINGSFILE% (
    echo FAIL: ���� � ����������� �����������
    exit /b 1
)

rem ��������� ����� c �����������
rem �����:
rem     eol=# ��������� �� ��, ��� ���������� ������ ������� � ������� #
rem     � �� �� ����� ����� ���� ��������� ��� �����������.
rem
rem     delims== ���������, ��� ������������ �������� �������� ������ =
rem 
rem     tokens=1,2 �������� � ����, ��� � ���������� %%i ����� ������� ������
rem     �����, � � %%j - ������.
rem 

for /f "eol=# delims== tokens=1,2" %%i in (%SETTINGSFILE%) do (
    rem � ���������� i - ����
    rem � ���������� j - ��������
    rem �� ����������� ��� � ���������� ���������
    set %%i=%%j
)

exit /b 0
