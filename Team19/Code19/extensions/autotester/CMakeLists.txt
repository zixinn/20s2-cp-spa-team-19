file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(autotester_extensions ${srcs})
target_link_libraries(autotester_extensions spa_extensions ${AUTOTESTER_LIBRARIES})

if (NOT WIN32)
    target_link_libraries(autotester_extensions pthread)
endif()
