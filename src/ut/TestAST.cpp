#include <gtest/gtest.h>
#include "../AST.cpp"

struct TestData {
    std::string input;
    int expectedOutput;
};

class ASTParameterizedTest : public ::testing::TestWithParam<TestData> {};

TEST_P(ASTParameterizedTest, CalculateExpression) {
    const TestData& data = GetParam();
    tree::AST tree(data.input);
    EXPECT_EQ(tree.getResult(), data.expectedOutput);
}

INSTANTIATE_TEST_SUITE_P(ASTParameterized, ASTParameterizedTest,
    ::testing::Values(
        TestData{"5", 5},
        TestData{"1 - 4", -3},
        TestData{"3 * 6", 18},
        TestData{"3 + 5 * 2", 13},
        TestData{"(4 + 5 * (7 - 3)) - 2", 22},
        TestData{"4+5+7/2", 12},
        TestData{"0-1+2/(5+4+1)+1", 0},
        TestData{"(2*5)/(5+1)", 1},
        TestData{"1-2/2*4", -3}
    )
);

struct ExceptionTestData {
    std::string input;
    std::string expectedExceptionMessage;
};

class ASTExceptionTest : public ::testing::TestWithParam<ExceptionTestData> {};

TEST_P(ASTExceptionTest, ParseInvalidExpression) {
    const ExceptionTestData& data = GetParam();
    EXPECT_THROW(tree::AST tree(data.input), std::exception);
    try {
        tree::AST tree(data.input);
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ(data.expectedExceptionMessage.c_str(), e.what());
    } catch (const std::logic_error e) {
        EXPECT_STREQ(data.expectedExceptionMessage.c_str(), e.what());
    } catch (const std::runtime_error e) {
        EXPECT_STREQ(data.expectedExceptionMessage.c_str(), e.what());
    }
}

INSTANTIATE_TEST_SUITE_P(ASTException, ASTExceptionTest,
    ::testing::Values(
        ExceptionTestData{"", "Missing expressions"},
        ExceptionTestData{"3 + * 5", "No literal between opretions"},
        ExceptionTestData{"10 + 0", "Literal too large"},
        ExceptionTestData{"1.1 + 2", "Fractional numbers are not allowed"},
        ExceptionTestData{"1,1 + 2", "Fractional numbers are not allowed"},
        ExceptionTestData{"1 + 1 +", "No literal at the end of expressions"},
        ExceptionTestData{"1 / 0", "Division by 0"},
        ExceptionTestData{"3 $ 5", "Unknow character"}
    )
);
