set(SOURCES
    AST.cpp
)

add_library(CalculatorLib STATIC ${SOURCES})

target_include_directories(CalculatorLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(calculator_app.out main.cpp)
target_link_libraries(calculator_app.out PRIVATE CalculatorLib)


set(GTEST_DIR "ut")

enable_testing()

add_executable(
    test_ast.out
    ut/TestAST.cpp
)
target_link_libraries(
    test_ast.out
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(test_ast.out)
