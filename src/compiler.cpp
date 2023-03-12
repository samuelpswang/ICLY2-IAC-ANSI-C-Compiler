#include <fstream>
#include <iostream>
#include <unistd.h>

#include "cli.hpp"
#include "ast.hpp"
#include "util_mem.hpp"

extern FILE *yyin;

int main(int argc, char** argv) {
    std::string sourcePath = "";
    std::string outputPath = "";
    if (parse_command_line_args(argc, argv, sourcePath, outputPath))
    {
        return 1;
    }

    yyin = fopen(sourcePath.c_str(), "r");
    if (yyin == NULL)
    {
        perror("Could not open source file");
        return 1;
    }

    std::ofstream output;
    output.open(outputPath, std::ios::trunc);

    std::cout << "[compiler.cpp] Parsing from: " << sourcePath << std::endl;
    const Node* ast = parse_ast();
    MemoryContext m;
    ast->compile(output, "a0", m);
    std::cout << "[compiler.cpp] Compiled to: " << outputPath << std::endl;

    output.close();
    return 0;
}
