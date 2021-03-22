
FILE=../../Code19/build_win/x86-Debug/src/autotester/autotester.exe

if [ -f "$FILE" ]; then
	echo "Enter query eg: Next for Next_query.txt"
	read query
	../../Code19/build_win/x86-Debug/src/autotester/autotester.exe ./basicFullSimple_source.txt ./${query}_query.txt ./${query}_out.xml

else
	echo "path to autotester not found"
fi
