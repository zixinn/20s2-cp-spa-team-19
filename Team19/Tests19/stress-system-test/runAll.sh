#!/bin/sh

echo Start system tests...

FILE=../../Code19/cmake-build-debug/src/autotester/autotester
if [ -f "$FILE" ]; then
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Boolean_query.txt Boolean_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Calls_query.txt Calls_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Next_query.txt Next_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt NextStar_query.txt NextStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Modifies_query.txt Modifies_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Uses_query.txt Uses_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Pattern_query.txt Pattern_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt Tuples_query.txt Tuples_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt With_query.txt With_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester fullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
else 
	echo "Enter path to autotester: "
	read id
	$id fullSimple_source.txt Boolean_query.txt Boolean_out.xml
	$id fullSimple_source.txt Calls_query.txt Calls_out.xml
	$id fullSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	$id fullSimple_source.txt Next_query.txt Next_out.xml
	$id fullSimple_source.txt NextStar_query.txt NextStar_out.xml
	$id fullSimple_source.txt Modifies_query.txt Modifies_out.xml
	$id fullSimple_source.txt Uses_query.txt Uses_out.xml
	$id fullSimple_source.txt Pattern_query.txt Pattern_out.xml
	$id fullSimple_source.txt Tuples_query.txt Tuples_out.xml
	$id fullSimple_source.txt With_query.txt With_out.xml
	$id fullSimple_source.txt MultiClause_query.txt MultiClause_out.xml
fi
read -p "Press any key to resume ..."