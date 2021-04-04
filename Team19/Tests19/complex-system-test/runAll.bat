@echo off
echo Start system tests...


if exist ..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe (
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Boolean_query.txt Boolean_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Calls_query.txt Calls_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Next_query.txt Next_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt NextStar_query.txt NextStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Modifies_query.txt Modifies_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Uses_query.txt Uses_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Pattern_query.txt Pattern_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Tuples_query.txt Tuples_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt With_query.txt With_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Affects_query.txt Affects_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt AffectsStar_query.txt AffectsStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt SelectAttribute_query.txt SelectAttribute_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Follows_query.txt Follows_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt FollowsStar_query.txt FollowsStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt Parent_query.txt Parent_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt ParentStar_query.txt ParentStar_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt NoClause_query.txt NoClause_out.xml
	..\..\Code19\build_win\x86-Debug\src\autotester\autotester.exe complexFullSimple_source.txt NextBip_query.txt NextBip_out.xml
) else (
	set /p id=Enter path to your autotester:
	%id% complexFullSimple_source.txt Boolean_query.txt Boolean_out.xml
	%id% complexFullSimple_source.txt Calls_query.txt Calls_out.xml
	%id% complexFullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	%id% complexFullSimple_source.txt Next_query.txt Next_out.xml
	%id% complexFullSimple_source.txt NextStar_query.txt NextStar_out.xml
	%id% complexFullSimple_source.txt Modifies_query.txt Modifies_out.xml
	%id% complexFullSimple_source.txt Uses_query.txt Uses_out.xml
	%id% complexFullSimple_source.txt Pattern_query.txt Pattern_out.xml
	%id% complexFullSimple_source.txt Tuples_query.txt Tuples_out.xml
	%id% complexFullSimple_source.txt With_query.txt With_out.xml
	%id% complexFullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
	%id% complexFullSimple_source.txt Affects_query.txt Affects_out.xml
	%id% complexFullSimple_source.txt AffectsStar_query.txt AffectsStar_out.xml
	%id% complexFullSimple_source.txt SelectAttribute_query.txt SelectAttribute_out.xml
	%id% complexFullSimple_source.txt Follows_query.txt Follows_out.xml
	%id% complexFullSimple_source.txt FollowsStar_query.txt FollowsStar_out.xml
	%id% complexFullSimple_source.txt Parent_query.txt Parent_out.xml
	%id% complexFullSimple_source.txt ParentStar_query.txt ParentStar_out.xml
	%id% complexFullSimple_source.txt NoClause_query.txt NoClause_out.xml
	%id% complexFullSimple_source.txt NextBip_query.txt NextBip_out.xml
)

pause
