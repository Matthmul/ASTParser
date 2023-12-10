#pragma once

#include <vector>
#include <map>
#include <queue>
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

    void printTree() const;
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
        std::weak_ptr<ASTNode> parent;
        NodeType type{NodeType::UNDEFINED};
        int value{0};
        std::vector<std::shared_ptr<ASTNode>> nodes{};
    };

    std::shared_ptr<ASTNode> addValue(const int val, std::shared_ptr<ASTNode> node);
    std::shared_ptr<ASTNode> addOperation(const char oper, std::shared_ptr<ASTNode> node);
    void parse(std::string input);
    NodeType getType(char type);
    unsigned int getWeight(NodeType type);
    bool isTransitional(NodeType type);
    std::shared_ptr<AST::ASTNode> unwind(std::shared_ptr<ASTNode> node) const;
    
    void calculate();
    void inorderTraversal(const std::shared_ptr<ASTNode> node);
    void doCalc(const std::shared_ptr<ASTNode> node, const std::shared_ptr<ASTNode> parent);
    void doCalc(const std::shared_ptr<ASTNode> node1, const std::shared_ptr<ASTNode> node2, const std::shared_ptr<ASTNode> parent);

    std::shared_ptr<ASTNode> root;
    bool winded{false};
    bool unwinded{false};
    std::string equation;
};
}