#!/bin/sh

echo Start system tests...

echo "Enter path to autotester: "
read id
$id Source.txt Boolean_query.txt Boolean_out.xml
$id Source.txt Calls_query.txt Calls_out.xml

read -p "Press any key to resume ..."