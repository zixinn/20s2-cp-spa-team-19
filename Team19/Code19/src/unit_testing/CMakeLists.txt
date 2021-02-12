file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} 
	src/SP/TestParseCallStmt.cpp
	src/SP/TestParseReadStmt.cpp
	src/SP/TestParseAssignStmt.cpp
	src/SP/TestParseExpr.cpp
	src/SP/TestParseCondExpr.cpp
	src/SP/TestLexerParser.cpp
	src/SP/TestParseWhile.cpp
	src/SP/TestParseIf.cpp
	src/SP/TestCondExprUtils.cpp
)

target_link_libraries(unit_testing spa)

