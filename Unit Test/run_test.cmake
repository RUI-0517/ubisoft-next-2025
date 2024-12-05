function(run_test CURRENT_FOLDER_NAME TEST_EXECUTABLE)
    add_custom_target(run_test_${TEST_EXECUTABLE}
        COMMAND ${CMAKE_BINARY_DIR}/${CURRENT_FOLDER_NAME}/Release/${TEST_EXECUTABLE}
        COMMENT "Running ${TEST_EXECUTABLE}"
    )
endfunction()