function(build_test TEST_NAME)
  add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp)
  target_include_directories(
    ${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/helper/
                         ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
  target_link_libraries(${TEST_NAME} PRIVATE project_options project_warnings
                                             lfa::fsm)
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

build_test(fsm_test)
build_test(fsm_builder_test)
build_test(lnfa_test)
