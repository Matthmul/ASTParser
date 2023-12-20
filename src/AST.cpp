#include "AST.hpp"

namespace tree {
AST::AST(std::string input) : equation{input} {
    if (input.size() == 0) {
        throw std::invalid_argument("Missing expressions");
    }

    root = expr();
    if (inputIndex < equation.length() || level != 0) {
        throw std::runtime_error("Invalid expression");
    }

    result = evaluate(root);
}

std::shared_ptr<AST::ASTNode> AST::expr() {    
    std::shared_ptr<AST::ASTNode> leftNode = term();
    skipWhiteSpace();

    while (inputIndex < equation.length() && (getType(equation[inputIndex]) == NodeType::ADDITION || getType(equation[inputIndex]) == NodeType::SUBTRACTION)) {
        auto op = getType(equation[inputIndex++]);
        std::shared_ptr<AST::ASTNode> rightNode = term();
        leftNode = std::make_shared<ASTNode>(ASTNode{op, leftNode, rightNode});
    }

    if (equation[inputIndex] == ')') {
        --level;
    }

    return leftNode;
}

std::shared_ptr<AST::ASTNode> AST::term() {    
    std::shared_ptr<AST::ASTNode> leftNode = factor();
    skipWhiteSpace();

    while (inputIndex < equation.length() && (getType(equation[inputIndex]) == NodeType::MULTIPLICATION || getType(equation[inputIndex]) == NodeType::DIVISION)) {
        auto op = getType(equation[inputIndex++]);
        std::shared_ptr<AST::ASTNode> rightNode = factor();
        leftNode = std::make_shared<ASTNode>(ASTNode{op, leftNode, rightNode});
    }
    return leftNode;
}

std::shared_ptr<AST::ASTNode> AST::factor() {
    skipWhiteSpace();
    if (inputIndex < equation.length()) {
        if (isdigit(equation[inputIndex])) {
            const int num = static_cast<int>(equation[inputIndex]) - 48;
            ++inputIndex;
            return std::make_shared<ASTNode>(ASTNode{num});
        } else if (equation[inputIndex] == '(') {
            ++level;
            ++inputIndex;
            std::shared_ptr<AST::ASTNode> newNode = expr();
            ++inputIndex;
            return newNode;
        }
    }
    throw std::runtime_error("Invalid expression");
}

void AST::skipWhiteSpace() {
    while (inputIndex < equation.length() && equation[inputIndex] == ' ')
    {
        ++inputIndex;
    }
}

AST::NodeType AST::getType(char type) {
    if (isdigit(type)) {
        return NodeType::VALUE;
    }

    switch (type)
    {
    case '+':
        return NodeType::ADDITION;
    case '-':
        return NodeType::SUBTRACTION;
    case '*':
        return NodeType::MULTIPLICATION;
    case '/':
        return NodeType::DIVISION;
    case '(':
    case ')':
        return NodeType::UNDEFINED;
    default:
        throw std::runtime_error("Unknow character");
    }
}

std::string AST::getEquation() const {
    return equation;
}

int AST::getResult() const {
    return result;
}

int AST::evaluate(const std::shared_ptr<ASTNode> node) {
    if (!node)
        return 0;

    if (!node->left_node && !node->right_node)
        return node->value;

    int leftVal = evaluate(node->left_node);
    int rightVal = evaluate(node->right_node);

    switch (node->type) {
        case NodeType::ADDITION:
        if ((rightVal > 0 && leftVal > std::numeric_limits<int>::max() - rightVal) ||
                (rightVal < 0 && leftVal < std::numeric_limits<int>::min() - rightVal)) {
                throw std::overflow_error("Overflow in addition");
            }
            return leftVal + rightVal;
        case NodeType::SUBTRACTION:
            if ((rightVal > 0 && leftVal < std::numeric_limits<int>::min() + rightVal) ||
                (rightVal < 0 && leftVal > std::numeric_limits<int>::max() + rightVal)) {
                throw std::overflow_error("Overflow in subtraction");
            }
            return leftVal - rightVal;
        case NodeType::MULTIPLICATION:
            if (leftVal > std::numeric_limits<int>::max() / rightVal ||
                leftVal < std::numeric_limits<int>::min() / rightVal) {
                throw std::overflow_error("Overflow in multiplication");
            }
            return leftVal * rightVal;
        case NodeType::DIVISION:
            if (rightVal == 0) {
                throw std::runtime_error("Division by 0");
            } else if (leftVal == std::numeric_limits<int>::min() && rightVal == -1) {
                throw std::overflow_error("Overflow in division");
            }
            return leftVal / rightVal;
        default:
            throw std::runtime_error("Unknow character");
    }
}
}
