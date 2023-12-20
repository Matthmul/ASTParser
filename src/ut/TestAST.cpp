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
        TestData{"5", 5}, // 0
        TestData{"1 - 4", -3}, // 1
        TestData{"3 * 6", 18}, // 2
        TestData{"3 + 5 * 2", 13}, // 3
        TestData{"(4 + 5 * (7 - 3)) - 2", 22}, // 4
        TestData{"4+5+7/2", 12}, // 5
        TestData{"0-1+2/(5+4+1)+1", 0}, // 6
        TestData{"(2*5)/(5+1)", 1}, // 7
        TestData{"1-2/2*4", -3}, // 8
        TestData{"1 + 1 - 9 -9-9", -25}, // 9
        TestData{"5-(5-5-5)", 10} // 10
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
        ExceptionTestData{"", "Missing expressions"}, // 12
        ExceptionTestData{"3 + * 5", "Invalid expression"}, // 13
        ExceptionTestData{"10 + 0", "Invalid expression"}, // 14
        ExceptionTestData{"1.1 + 2", "Unknow character"}, // 15
        ExceptionTestData{"1,1 + 2", "Unknow character"}, // 16
        ExceptionTestData{"1 + 1 +", "Invalid expression"}, // 17
        ExceptionTestData{"()", "Invalid expression"}, // 18
        ExceptionTestData{")", "Invalid expression"}, // 19
        ExceptionTestData{"1+(2*2+)", "Invalid expression"}, // 20
        ExceptionTestData{"(1+(1+(1", "Invalid expression"}, // 21
        ExceptionTestData{"1 / 0", "Division by 0"}, // 22
        ExceptionTestData{"3 $ 5", "Unknow character"} // 23
    )
);
