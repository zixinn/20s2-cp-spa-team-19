#!/bin/sh

echo Start system tests...

FILE=../../Code19/cmake-build-debug/src/autotester/autotester
if [ -f "$FILE" ]; then
	../../Code19/cmake-build-debug/src/autotester/autotester Source.txt Boolean_query.txt Boolean_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester Source.txt Calls_query.txt Calls_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester Source.txt CallsStar_query.txt CallsStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester Source.txt Next_query.txt Next_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester Source.txt NextStar_query.txt NextStar_out.xml
else 
	echo "Enter path to autotester: "
	read id
	$id Source.txt Boolean_query.txt Boolean_out.xml
	$id Source.txt Calls_query.txt Calls_out.xml
	$id Source.txt CallsStar_query.txt CallsStar_out.xml
	$id Source.txt Next_query.txt Next_out.xml
	$id Source.txt NextStar_query.txt NextStar_out.xml
fi
read -p "Press any key to resume ..."