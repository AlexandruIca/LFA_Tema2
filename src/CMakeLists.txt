set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/fsm.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/fsm.cpp)

add_library(lfa_fsm STATIC ${SOURCE_FILES})
add_library(lfa::fsm ALIAS lfa_fsm)

target_include_directories(lfa_fsm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lfa_fsm PRIVATE project_options project_warnings)
