file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})

    target_sources(${test_name} PRIVATE ${SOURCE_FILES})

    target_precompile_headers(${test_name} PRIVATE ${PCH_HEADER})
    target_include_directories(${test_name} PRIVATE "include" ${CMAKE_SOURCE_DIR})

    # Add each test executable to a list for later use
    list(APPEND TEST_EXECUTABLES ${test_name})
endforeach()

# Call the function to run all tests after the build finishes.
include(${CMAKE_SOURCE_DIR}/run_tests.cmake)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
run_tests(${CURRENT_FOLDER_NAME} ${TEST_EXECUTABLES})