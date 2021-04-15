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
3) Autotester executable will be located in '../Team19/Code19/build_win/x86-Release/extensions/autotester'
4) [SYSTEM TESTING]
    5) System tests for the extensions can be found in extensions-system-test
    6) To run the tests, run
        i.  extensions-system-test/runAll.bat
    7) It will generate an .xml test output file for viewing the results categorized by type of queries

