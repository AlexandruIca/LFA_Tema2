set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/fsm.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsm_builder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsm_builder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lnfa.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lnfa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nfa.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nfa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dfa.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dfa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/printer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/printer.cpp)

add_library(lfa_fsm STATIC ${SOURCE_FILES})
add_library(lfa::fsm ALIAS lfa_fsm)

target_include_directories(lfa_fsm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lfa_fsm PRIVATE project_options project_warnings)
