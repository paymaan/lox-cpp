#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "error_handler/error_handler.hpp"
#include "scanner/scanner.hpp"

namespace lox {
    static void run(const std::string& source, ErrorHandler& errorHandler) {
        Scanner scanner(source, errorHandler);
        const auto tokens = scanner.scanAndGetTokens();
        // just print all the tokens for now
        for (const auto token : tokens) {
            std::cout << token.toString() << std::endl;
        }
        // if found error during scanning, report
        if (errorHandler.foundError) {
            errorHandler.report();
        }
    }

    static void runFile(const std::string& path, ErrorHandler& errorHandler) {
        std::ifstream file(path);
        std::ostringstream stream;
        stream << file.rdbuf();
        file.close();
        run(stream.str(), errorHandler);
    }

    static void runPrompt(ErrorHandler& errorHandler) {
        while (true) {
            std::cout << "> ";
            std::string line;
            getline(std::cin, line);
            run(line, errorHandler);
            if (errorHandler.foundError) {
                errorHandler.clear();
            }
        }
    }
}

int main(int argc, char** argv) {
    lox::ErrorHandler errorHandler;
    if (argc > 2) {
        std::cout << "Usage: lox [filename]" << std::endl;
    } else if (argc == 2) {
        lox::runFile(argv[1], errorHandler);
    } else {
        lox::runPrompt(errorHandler);
    }
    return 0;
}
