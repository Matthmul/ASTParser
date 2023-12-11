set(SOURCES
    AST.cpp
)

add_library(ASTLib STATIC ${SOURCES})

target_include_directories(ASTLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(parser_app main.cpp)
target_link_libraries(parser_app PRIVATE ASTLib)


set(GTEST_DIR "ut")

enable_testing()

add_executable(
    test_ast
    ut/TestAST.cpp
)
target_link_libraries(
    test_ast
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(test_ast)
