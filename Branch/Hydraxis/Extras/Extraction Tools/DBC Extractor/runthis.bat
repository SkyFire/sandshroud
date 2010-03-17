:TOP
@CLS
@ECHO OFF

:Begin

CLS
ECHO.
ECHO *******************************************************************
ECHO ** Welcome to the Universal DBC extractor of Thetruecrow         **
ECHO ** This extractor will extract your dbc files for you            **
ECHO ** Make sure you have placed the files in the correct directory  **
ECHO ** See Help.txt for an explanation on where to place these files **
ECHO *******************************************************************
ECHO.
ECHO 	1 - Extract the deDE DBC Files
ECHO 	2 - Extract the enGB DBC Files
ECHO 	3 - Extract the enUS DBC Files
ECHO 	4 - Extract the enES DBC Files
ECHO 	5 - Extract the frFR DBC Files
ECHO 	6 - Extract the koKR DBC Files
ECHO 	7 - Extract the zhCN DBC Files
ECHO 	8 - Extract the zhTW DBC Files
ECHO 	9 - Exit this extractor
ECHO.
SET /p v= 		Enter a number:
IF %v%==1 GOTO DE
IF %v%==2 GOTO GB
IF %v%==3 GOTO US
IF %v%==4 GOTO ES
IF %v%==5 GOTO FR
IF %v%==6 GOTO KR
IF %v%==7 GOTO CN
IF %v%==8 GOTO TW
IF %v%==9 GOTO exit
IF %v%==* GOTO error
GOTO error

:DE
mpqe /p locale-deDE.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:GB
mpqe /p locale-enGB.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:US
mpqe /p locale-enUS.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:ES
mpqe /p locale-esES.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:FR
mpqe /p locale-frFR.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:KR
mpqe /p locale-koKR.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:CN
mpqe /p locale-zhCN.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:TW
mpqe /p locale-zhTW.MPQ DBFilesClient\*.dbc
ECHO Done.
PAUSE
GOTO begin

:error
ECHO 	Please enter a correct number.
PAUSE
GOTO begin

:exit