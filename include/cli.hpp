#ifndef LANGPROC_COMPILER_CLI
#define LANGPROC_COMPILER_CLI

#include <iostream>
#include <unistd.h>

int parse_command_line_args(int argc, char **argv, std::string &sourcePath, \
    std::string &outputPath);

#endif
