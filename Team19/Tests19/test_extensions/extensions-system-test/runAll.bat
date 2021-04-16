@echo off
echo Start system tests...


if exist ..\..\..\Code19\build_win\x86-Release\extensions\autotester\autotester_extensions.exe (
	..\..\..\Code19\build_win\x86-Release\extensions\autotester\autotester_extensions.exe complexFullSimple_source.txt NextBip_query.txt NextBip_out.xml
	..\..\..\Code19\build_win\x86-Release\extensions\autotester\autotester_extensions.exe complexFullSimple_source.txt NextBipStar_query.txt NextBipStar_out.xml
	..\..\..\Code19\build_win\x86-Release\extensions\autotester\autotester_extensions.exe complexFullSimple_source.txt AffectsBip_query.txt AffectsBip_out.xml
	..\..\..\Code19\build_win\x86-Release\extensions\autotester\autotester_extensions.exe complexFullSimple_source.txt AffectsBipStar_query.txt AffectsBipStar_out.xml
) else (
	set /p id=Enter path to your autotester:
	%id% complexFullSimple_source.txt NextBip_query.txt NextBip_out.xml
	%id% complexFullSimple_source.txt NextBipStar_query.txt NextBipStar_out.xml
	%id% complexFullSimple_source.txt AffectsBip_query.txt AffectsBip_out.xml
	%id% complexFullSimple_source.txt AffectsBipStar_query.txt AffectsBipStar_out.xml
)

pause
