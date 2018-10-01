add_library(lightex2 STATIC
    parser/ast.cc
    parser/ast_adapted.cc
    parser/grammar.cc
    parser/string.cc
    printer/debug_printer.cc
)
target_link_libraries(lightex2 boost util)

# Unittest
add_executable(tester2
    tests/main.cc
    tests/math.cc
    tests/common.cc
    tests/olymp.cc
)
target_link_libraries(tester2 lightex2 boost_test)
