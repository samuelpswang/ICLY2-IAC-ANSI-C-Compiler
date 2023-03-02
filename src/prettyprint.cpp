#include <fstream>
#include <iostream>
#include <unistd.h>
#include <iomanip>

#include "cli.hpp"
#include "ast.hpp"

extern FILE *yyin;

int main(int argc, char** argv)
{
    std::string sourcePath = "";
    std::string outputPath = "";
    if (parse_command_line_args(argc, argv, sourcePath, outputPath)) {
        return 1;
    }

    yyin = fopen(sourcePath.c_str(), "r");
    if (yyin == NULL) {
        perror("Could not open source file");
        return 1;
    }

    std::ofstream output;
    output.open(outputPath, std::ios::trunc);

    std::cout << "[prettyprint.cpp] Parsing: " << sourcePath << std::endl;
    const Node* ast = parse_ast();
    ast->print(output, "");
    std::cout << "[prettyprint.cpp] Pretty-printed to: " << outputPath << std::endl;

    output.close();
    return 0;
}
