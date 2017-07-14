REM Do all this stuff to only overwrite version.h on a real git commit change
REM otherwise executable is rebuild on every debug start
@ECHO Determining last Git commit hash...
FOR /F %%v IN (version.txt) DO set dummy=%%v
FOR /F %%w IN ('"C:\Progra~2\Git\bin\git log --format=%%h -1"') DO SET dummy2=%%w
@ECHO Local version.txt says %dummy%
@ECHO git log says %dummy2%
IF NOT %dummy%==%dummy2% (@ECHO Hash not equal - doing rebuild due to remote changes) ELSE (@ECHO Hash equal - doing rebuild due to local changes only)
REM The 'set /p' stuff is to suppress the carriage return after 'echo'
IF NOT %dummy%==%dummy2% ECHO | SET /P dummy=#define LASTGITCOMMIT > version.h
IF NOT %dummy%==%dummy2% ECHO %dummy2% >> version.h
IF NOT %dummy%==%dummy2% ECHO %dummy2% > version.txt