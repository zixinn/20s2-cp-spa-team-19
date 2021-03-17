@echo off
echo Start system tests...

set /p id=Enter path to autotester: 
%id% Source.txt Boolean_query.txt Boolean_out.xml
%id% Source.txt Calls_query.txt Calls_out.xml
%id% Source.txt Next_query.txt Next_out.xml

pause

