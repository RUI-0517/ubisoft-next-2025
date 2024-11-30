function(run_tests CURRENT_FOLDER_NAME TEST_EXECUTABLES)
    add_custom_target(run_tests)

    foreach(test_exec ${TEST_EXECUTABLES})
        add_dependencies(run_tests ${test_exec})

        add_custom_command(
            TARGET run_tests POST_BUILD
            COMMAND echo -e "\\033[1;33m------------------------------ \\033[0m"
            COMMAND echo -e "\\033[1;33mRunning ${test_exec}...\\033[0m"
            COMMAND ${CMAKE_BINARY_DIR}/${CURRENT_FOLDER_NAME}/${test_exec}
        )
    endforeach()
endfunction()