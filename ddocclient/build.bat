del /F /Q mgrlibs/kkscoreapplication\kksclient_name.h
del /F /Q corelibs/kksutils\configEndians.h

C:\Qt\4.4.0\bin\qmake -recursive ddocclient_desktop.pro

nmake

nmake install
