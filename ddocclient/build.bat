del /F /Q kkscore\kksclient_name.h
del /F /Q kksutils\configEndians.h

C:\Qt\4.4.0\bin\qmake -recursive

nmake

nmake install
