#!/bin/sh

FILE=../../Code19/build_win/x86-Debug/src/autotester/autotester.exe
if [ -f "$FILE" ]; then
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./AndWith_query.txt ./AndWith_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./Boolean_query.txt ./Boolean_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./Calls_query.txt ./Calls_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./Next_query.txt ./Next_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./NextStar_query.txt ./NextStar_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./Pattern_query.txt ./Pattern_out.xml
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./Source.txt ./Tuple_query.txt ./Tuple_out.xml
else
	echo "path to autotester not found"
fi
