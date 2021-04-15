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
1) Edit Team19/Code19/CMakeLists.txt
    At the bottom of the file, change the following 4 lines from this:
        add_subdirectory(src/spa)
        add_subdirectory(src/autotester)
        add_subdirectory(src/unit_testing)
        add_subdirectory(src/integration_testing)

        to this:
        add_subdirectory(extensions/spa)
        add_subdirectory(extensions/autotester)
        add_subdirectory(extensions/unit_testing)
        add_subdirectory(extensions/integration_testing)

2) In Visual Studio, go to File->Open->Folder->(navigate to the folder with CMakeLists.txt)
3) Build the project in x86-Release mode
4) Autotester executable will be located in '../Team19/Code19/build_win/x86-Release/src/autotester'
5) After navigating to that folder, you can run Autotester with:
  ./autotester ..\..\..\..\..\Tests19\BasicSimple_source.txt ..\..\..\..\..\Tests19\BasicSimple_queries.txt ..\..\..\..\..\Tests19\out.xml
6) In addition to the tests in /Tests19 additional tests can be found in the following sub-directories
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
