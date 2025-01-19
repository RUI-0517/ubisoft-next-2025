file(GLOB TEST_SOURCES "./*.cpp")

get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
set(INCLUDE_PATH "${PARENT_DIR}/Game/${CURRENT_FOLDER_NAME}")
set(LIBRARY_PATH "${PARENT_DIR}/Game/Libraries/${ARCH_DIR}")

set(TEST_EXECUTABLES "")
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    message(STATUS "Creating test executable: ${test_name}")
    add_executable(${test_name} ${test_src})

    target_include_directories(${test_name} PRIVATE ${INCLUDE_PATH})

    # Link libraries explicitly
    file(GLOB LIB_FILES "${LIBRARY_PATH}/*.lib")
    foreach(lib ${LIB_FILES})
    	message(STATUS "Linking library: ${lib}")
        target_link_libraries(${test_name} PRIVATE ${lib})
    endforeach()

    # Add each test executable to a list for later use
    list(APPEND TEST_EXECUTABLES ${test_name})
endforeach()

add_custom_target(run_tests)
include(${CMAKE_SOURCE_DIR}/run_test.cmake)
foreach(test_exec ${TEST_EXECUTABLES})
    run_test(${CURRENT_FOLDER_NAME} ${test_exec})
    add_dependencies(run_tests run_test_${test_exec})
endforeach()
