#!/bin/sh

echo Start system tests...

FILE=../../Code19/cmake-build-debug/src/autotester/autotester
if [ -f "$FILE" ]; then
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Affects_query.txt Affects_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt AffectsStar_query.txt AffectsStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Calls_query.txt Calls_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Follows_query.txt Follows_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt FollowsStar_query.txt FollowsStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Next_query.txt Next_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt NextStar_query.txt NextStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Modifies_query.txt Modifies_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Uses_query.txt Uses_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Pattern_query.txt Pattern_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt Parent_query.txt Parent_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt ParentStar_query.txt ParentStar_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt With_query.txt With_out.xml
	../../Code19/cmake-build-debug/src/autotester/autotester keywordSimple_source.txt MultiClause_query.txt MultiClause_out.xml
else 
	echo "Enter path to autotester: "
	read id
	$id keywordSimple_source.txt Affects_query.txt Affects_out.xml
	$id keywordSimple_source.txt AffectsStar_query.txt AffectsStar_out.xml
	$id keywordSimple_source.txt Calls_query.txt Calls_out.xml
	$id keywordSimple_source.txt CallsStar_query.txt CallsStar_out.xml
	$id keywordSimple_source.txt Follows_query.txt Follows_out.xml
	$id keywordSimple_source.txt FollowsStar_query.txt FollowsStar_out.xml
	$id keywordSimple_source.txt Next_query.txt Next_out.xml
	$id keywordSimple_source.txt NextStar_query.txt NextStar_out.xml
	$id keywordSimple_source.txt Modifies_query.txt Modifies_out.xml
	$id keywordSimple_source.txt Uses_query.txt Uses_out.xml
	$id keywordSimple_source.txt Pattern_query.txt Pattern_out.xml
	$id keywordSimple_source.txt Parent_query.txt Parent_out.xml
	$id keywordSimple_source.txt ParentStar_query.txt ParentStar_out.xml
	$id keywordSimple_source.txt With_query.txt With_out.xml
	$id keywordSimple_source.txt MultiClause_query.txt MultiClause_out.xml
fi
read -p "Press any key to resume ..."