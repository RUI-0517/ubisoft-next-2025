file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp")

set(TEST_EXECUTABLES "")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    message(STATUS "Creating test executable: ${test_name}")
    add_executable(${test_name} ${test_src})

    target_sources(${test_name} PRIVATE ${SOURCE_FILES})

    target_precompile_headers(${test_name} PRIVATE ${PCH_HEADER})
    target_include_directories(${test_name} PRIVATE "include" ${CMAKE_SOURCE_DIR})
    
    # Add each test executable to a list for later use
    list(APPEND TEST_EXECUTABLES ${test_name})
endforeach()

message(STATUS "Test executables: ${TEST_EXECUTABLES}")
add_custom_target(run_tests)
include(${CMAKE_SOURCE_DIR}/run_test.cmake)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach(test_exec ${TEST_EXECUTABLES})
    run_test(${CURRENT_FOLDER_NAME} ${test_exec})
    add_dependencies(run_tests run_test_${test_exec})
endforeach()