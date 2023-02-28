# ANSI C to RISC-V Compiler: TeamBS

## File Structure

```txt
root/
├─ bin/
├─ include/
│  ├─ ast.hpp
│  └─ ...
├─ src/
│  ├─ c_lexer.flex
│  ├─ c_parser.y
│  └─ ...
├─ tests/
├─ ref/
├─ .gitignore
├─ Makefile
└─ README.md
```

1. `bin/`: contains compiled binary files, not committed.
2. `include/`: contains user-defined header files, e.g. `ast.hpp`.
3. `src/`: contains lexer, parser, compiler, and command line programs.
4. `test/`: contains c program tests.
5. `ref/`: contains reference files, e.g. official c grammer.
6. `Makefile`: build script.
