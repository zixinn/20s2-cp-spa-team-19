Static Program Analyser (SPA)

Solution chosen:
Cross-platform SPA Solution

Target platform:
Windows using Cmake

Visual Studio Version:
2019

Configuration:
Release

Instructions:
1) In Visual Studio, go to File->Open->Folder->(navigate to the folder with CMakeLists.txt)
2) Build the project in x86-Release mode
    i.  At the top of VS there is a dropdown list, select "x86-Release"
    ii. At Top Toolbar of VS -> Build -> BuildAll (Ctrl + Shift + B)
3) Autotester executable will be located in '../Team19/Code19/build_win/x86-Release/src/autotester'
4) After navigating to that folder, you can run Autotester with:
  ./autotester ..\..\..\..\..\Tests19\BasicSimple_source.txt ..\..\..\..\..\Tests19\BasicSimple_queries.txt ..\..\..\..\..\Tests19\out.xml
5) In addition to the tests in /Tests19 additional tests can be found in the following sub-directories
    i.   basic-system-test
    ii.  complex-system-test
    iii. keyword-system-test
    iv.  stress-system-test
5) To run each test, run the files
    i.   basic-system-test/runAllBasic.sh
    ii.  complex-system-test/runAll.bat
    iii. keyword-system-test/runAll.bat
    iv.  stress-system-test/runAll.bat
6) These generate .xml test output files for viewing the results categorized by type of queries

Extensions Tests:
5) The following test in /Tests19 specically tests for extension related relations
    v.   Tests19/test_extensions/extensions-system-test/runAll.bat
