#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>

namespace tree {
class AST
{
public:
    explicit AST(std::string input);
    AST(const AST& other) = delete;
    AST(const AST&& other) = delete;
    AST& operator=(const AST& other) = delete;
    AST& operator=(const AST&& other) = delete;

    int getResult() const;
    std::string getEquation() const;

private:
    enum class NodeType {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        VALUE,

        UNDEFINED
    };

    friend std::ostream& operator<<(std::ostream& os, const NodeType& nodeType) {
        switch (nodeType) {
            case NodeType::ADDITION:
                return os << "ADDITION";
            case NodeType::SUBTRACTION:
                return os << "SUBTRACTION";
            case NodeType::MULTIPLICATION:
                return os << "MULTIPLICATION";
            case NodeType::DIVISION:
                return os << "DIVISION";
            case NodeType::VALUE:
                return os << "VALUE";
            default:
                return os << "UNDEFINED";
        }
    }

    struct ASTNode {
        NodeType type{NodeType::UNDEFINED};
        int value{0};
        std::shared_ptr<ASTNode> left_node{nullptr};
        std::shared_ptr<ASTNode> right_node{nullptr};

        ASTNode(NodeType type, std::shared_ptr<ASTNode> left = nullptr, std::shared_ptr<ASTNode> right = nullptr)
            : type{type}, value{0}, left_node{left}, right_node{right} {}
        ASTNode(int value) : type{NodeType::VALUE}, value{value}, left_node{nullptr}, right_node{nullptr} {}
    };

    NodeType getType(char type);
    void skipWhiteSpace();
    std::shared_ptr<ASTNode> parse();
    std::shared_ptr<ASTNode> expr();
    std::shared_ptr<ASTNode> term();
    std::shared_ptr<ASTNode> factor();
    
    int evaluate(const std::shared_ptr<ASTNode> node);

    std::shared_ptr<ASTNode> root;
    std::string equation;
    size_t inputIndex{0};
    int result{0};
    uint16_t level{0};
};
}