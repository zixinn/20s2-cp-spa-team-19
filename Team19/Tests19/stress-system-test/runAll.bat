@echo off
echo Start system tests...


if exist ..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe (
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Boolean_query.txt Boolean_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Calls_query.txt Calls_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Next_query.txt Next_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt NextStar_query.txt NextStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Modifies_query.txt Modifies_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Uses_query.txt Uses_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Pattern_query.txt Pattern_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt Tuples_query.txt Tuples_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt With_query.txt With_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe fullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
) else (
	set /p id=Enter path to your autotester:
	%id% fullSimple_source.txt Boolean_query.txt Boolean_out.xml
	%id% fullSimple_source.txt Calls_query.txt Calls_out.xml
	%id% fullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	%id% fullSimple_source.txt Next_query.txt Next_out.xml	
	%id% fullSimple_source.txt NextStar_query.txt NextStar_out.xml
	%id% fullSimple_source.txt Modifies_query.txt Modifies_out.xml
	%id% fullSimple_source.txt Uses_query.txt Uses_out.xml
	%id% fullSimple_source.txt Pattern_query.txt Pattern_out.xml
	%id% fullSimple_source.txt Tuples_query.txt Tuples_out.xml
	%id% fullSimple_source.txt With_query.txt With_out.xml
	%id% fullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
) 

pause