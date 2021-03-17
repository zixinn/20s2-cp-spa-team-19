@echo off
echo Start system tests...


if exist ..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe (
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe Source.txt Boolean_query.txt Boolean_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe Source.txt Calls_query.txt Calls_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe Source.txt Next_query.txt Next_out.xml
) else (
	set /p id=Enter path to your autotester:
	%id% Source.txt Boolean_query.txt Boolean_out.xml
	%id% Source.txt Calls_query.txt Calls_out.xml
	%id% Source.txt Next_query.txt Next_out.xml

) 

pause

