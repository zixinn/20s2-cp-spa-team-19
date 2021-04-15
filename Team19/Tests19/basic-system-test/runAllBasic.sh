#!/bin/sh

FILE=../../Code19/build_win/x86-Release/src/autotester/autotester.exe
if [ -f "$FILE" ]; then
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Affects_query.txt ./Affects_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./AffectsStar_query.txt ./AffectsStar_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./AndWith_query.txt ./AndWith_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Boolean_query.txt ./Boolean_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Calls_query.txt ./Calls_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Modifies_query.txt ./Modifies_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./MultiClause_query.txt ./MultiClause_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Next_query.txt ./Next_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./NextStar_query.txt ./NextStar_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./NoClause_query.txt ./NoClause_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Pattern_query.txt ./Pattern_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Tuple_query.txt ./Tuple_out.xml
	../../Code19/build_win/x86-Release/src/autotester/autotester.exe ./basicFullSimple_source.txt ./Uses_query.txt ./Uses_out.xml
else
	echo "path to autotester not found"
fi
