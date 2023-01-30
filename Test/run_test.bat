@echo off
setlocal
set logFile=testlog.txt

echo. > %logFile%
for /R . %%i in (*.exe) do (
    echo Running test: %%i
    %%i >> %logFile%
)

FOR /F "delims=" %%i IN ('FIND /C "FAIL!" testlog.txt') DO SET count=%%i
FOR %%i IN ("%count:~-1%") DO SET ultimo=%%i
SET /A count=%ultimo%

if %count% == 0 (
    echo All tests passed
) else (
    echo Some tests failing
    echo -----------------------------------------
    echo.
    echo [COUNT]: %count%
    rem Search the log file for 'FAIL!' to find the failed test cases
    findstr "FAIL!" %logFile%
    echo.
    echo -----------------------------------------

    echo [COUNT]: %count% > gtest_resume.log
    echo. >> gtest_resume.log
    findstr "FAIL!" %logFile% >> gtest_resume.log
)
echo.

echo FINISHED