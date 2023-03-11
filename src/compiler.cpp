#include <fstream>
#include <iostream>
#include <unistd.h>

#include "cli.hpp"
#include "ast.hpp"

void compile(std::ostream &w)
{
    w << ".text" << std::endl;
    w << ".globl f" << std::endl;
    w << std::endl;

    w << "f:" << std::endl;
    w << "addi  t0, zero, 0" << std::endl;
    w << "addi  t0, t0,   5" << std::endl;
    w << "add   a0, zero, t0" << std::endl;
    w << "ret" << std::endl;
}

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
    ast->compile(output, "rst","    ");
    std::cout << "[compiler.cpp] Compiled to: " << outputPath << std::endl;

    output.close();
    return 0;
}
