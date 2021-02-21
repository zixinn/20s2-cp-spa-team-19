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
3) Autotester executable will be located in '../Team19/Code19/build_win/x86-Release/src/autotester'
4) After navigating to that folder, you can run Autotester with:
  ./autotester ..\..\..\..\..\Tests19\BasicSimple_source.txt ..\..\..\..\..\Tests19\BasicSimple_queries.txt ..\..\..\..\..\Tests19\out.xml
