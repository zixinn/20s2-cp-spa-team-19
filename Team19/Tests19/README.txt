[SYSTEM TESTING - SPA]
AutoTester can be run manually with this command:
  ./autotester ..\..\..\..\..\Tests19\BasicSimple_source.txt ..\..\..\..\..\Tests19\BasicSimple_queries.txt ..\..\..\..\..\Tests19\out.xml

1) In addition to the tests in /Tests19, additional tests can be found in the following sub-directories
    i.   basic-system-test
    ii.  complex-system-test
    iii. keyword-system-test
    iv.  stress-system-test
2) To run each set of tests, run the files
    i.   basic-system-test/runAllBasic.sh
    ii.  complex-system-test/runAll.bat
    iii. keyword-system-test/runAll.bat
    iv.  stress-system-test/runAll.bat
3) These generate .xml test output files for viewing the results categorized by type of queries