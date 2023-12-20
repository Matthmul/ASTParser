#include "AST.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage" << std::endl << std::endl;
        std::cout << "parser_app <expresion>" << std::endl << std::endl;
        return 0;
    }

    try {
        tree::AST ast{static_cast<std::string>(argv[1])};
        std::cout << "Equation: " << ast.getEquation();
        std::cout << " Result: " << ast.getResult() << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    } catch (const std::runtime_error e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
