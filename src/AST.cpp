#include "AST.hpp"

namespace tree {
AST::AST(std::string input) : equation{input} {
    root = std::make_shared<ASTNode>();
    parse(input);
    calculate();
}

void AST::parse(std::string input) {
    std::shared_ptr<ASTNode> actualNode = root;

    for(const auto& c : input) {
        switch (c)
        {
        case ' ':
            continue;
        case '(':
            winded = true;
            continue;
        case ')':
            actualNode = unwind(actualNode);
            unwinded = true;
            continue;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if(actualNode->type == NodeType::VALUE) {
                throw "Literal too large";
            }
            actualNode = addValue(static_cast<int>(c) - 48, actualNode);
            break;
        case '/':
        case '-':
        case '*':
        case '+':
            if(actualNode->type != NodeType::VALUE && !unwinded) {
                throw "No literal between opretions";
            }
            actualNode = addOperation(c, actualNode);
            winded = false;
            unwinded = false;
            break;
        case ',':
        case '.':
            throw "Fractional numbers are not allowed";
        default:
            std::cout << "Unknow character: " << c << std::endl;
            break;
        }
    }

    if(actualNode->type != NodeType::VALUE && !unwinded) {
        throw "No literal at the end of expressions";
    }
}

std::shared_ptr<AST::ASTNode> AST::unwind(std::shared_ptr<ASTNode> node) const {
    const auto parent = node->parent.lock();
    if (parent == nullptr) {
        return node;
    }

    return parent;
}

std::shared_ptr<AST::ASTNode> AST::addValue(const int val, std::shared_ptr<ASTNode> node) {
    std::shared_ptr<ASTNode> newNode = std::make_shared<ASTNode>(ASTNode{node, NodeType::VALUE, val});
    node->nodes.push_back(newNode);

    return newNode;
}

std::shared_ptr<AST::ASTNode> AST::addOperation(const char oper, std::shared_ptr<ASTNode> node) {
    const auto type = getType(oper);
    auto parent = node->parent.lock();
    if (parent == nullptr) {
        std::shared_ptr<ASTNode> newNode = std::make_shared<ASTNode>(ASTNode{parent, type});
        newNode->nodes.push_back(node);
        node->parent = newNode;
        root = newNode;
        return newNode;
    }
    if (parent->type == NodeType::UNDEFINED) {
        parent->type = type;
        if (type == NodeType::MULTIPLICATION) {
            parent->value = 1;
        }
        return parent;
    }

    std::shared_ptr<ASTNode> newNode = std::make_shared<ASTNode>(ASTNode{parent, type});
    if (type == NodeType::MULTIPLICATION) {
        newNode->value = 1;
    }

    if ((getWeight(type) > getWeight(parent->type) && !unwinded) || winded) {
        newNode->nodes.push_back(node);
        node->parent = newNode;
        parent->nodes.pop_back();
        parent->nodes.push_back(newNode);
    }
    else {
        if (getWeight(type) == getWeight(parent->type) && isTransitional(type)) {
            return parent;
        }

        const auto grandParent = parent->parent.lock();
        if (grandParent) {
            if (grandParent->type == type) {
                return grandParent;
            }
            else {
                grandParent->nodes.back() = newNode;
            }
        }
        else {
            root = newNode;
            newNode->parent.reset();
        }

        newNode->nodes.push_back(parent);
        parent->parent = newNode;
    }

    return newNode;
}

bool AST::isTransitional(AST::NodeType type) {
    switch (type)
    {
    case NodeType::ADDITION:
    case NodeType::MULTIPLICATION:
        return true;
    case NodeType::UNDEFINED:
    case NodeType::SUBTRACTION:
    case NodeType::DIVISION:
    case NodeType::VALUE:
        return false;
    default:
        throw std::runtime_error("Unknow type");
    }
}

unsigned int AST::getWeight(AST::NodeType type) {
    switch (type)
    {
    case NodeType::UNDEFINED:
        return 0;
    case NodeType::ADDITION:
        return 1;
    case NodeType::SUBTRACTION:
        return 2;
    case NodeType::MULTIPLICATION:
        return 3;
    case NodeType::DIVISION:
        return 4;
    default:
        throw std::runtime_error("Unknow type");
    }
}

AST::NodeType AST::getType(char type) {
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
    case '\0':
        return NodeType::UNDEFINED;
    default:
        throw "Unknow type: " + type;
    }
}

void AST::printTree() const {}

std::string AST::getEquation() const {
    return equation;
}

int AST::getResult() const {
    return root->value;
}

void AST::calculate() {
    inorderTraversal(root);
}

void AST::inorderTraversal(const std::shared_ptr<ASTNode> node) {
    if (node == nullptr) {
        return;
    }

    for (const auto& n : node->nodes) {
        inorderTraversal(n);
    }

    if (isTransitional(node->type)) {
        for (const auto& n : node->nodes) {
            doCalc(n, node);
        }
    } else {
        if (node->nodes.size() == 2) {
            doCalc(node->nodes[0], node->nodes[1], node);
        }
    }
}

void AST::doCalc(const std::shared_ptr<ASTNode> node, const std::shared_ptr<ASTNode> parent) {
    if (node == nullptr) {
        return;
    }
    
    switch (parent->type)
    {
    case NodeType::ADDITION:
        parent->value += node->value;
        break;
    case NodeType::MULTIPLICATION:
        parent->value *= node->value;
        break;
    default:
        throw std::runtime_error("Unknow type");
    }
}

void AST::doCalc(const std::shared_ptr<ASTNode> node1, const std::shared_ptr<ASTNode> node2, const std::shared_ptr<ASTNode> parent) {
    if (node1 == nullptr || node2 == nullptr) {
        return;
    }
    
    switch (parent->type)
    {
    case NodeType::DIVISION:
        if (node2->value == 0) {
            throw std::runtime_error("Division by 0");
        }
        parent->value = node1->value / node2->value;
        break;
    case NodeType::SUBTRACTION:
        parent->value = node1->value - node2->value;
        break;
    default:
        throw std::runtime_error("Unknow type");
    }
}
}