#ifndef compiler_mem
#define compiler_mem

#include <iostream>
#include <vector>
#include <string>
#include <map>

enum regtype {
    rreg,
    sreg,
    treg
};

class MemoryContext {
public:
    // Basics
    MemoryContext();
    ~MemoryContext();

    // Accessors
    bool add_func(const std::string& func_name);
    bool use_func(const std::string& func_name);
    bool add_symbol(const std::string& symbol_name);
    bool use_symbol(const std::string& symbol_name);
    int get_symbol(const std::string& symbol_name);

    // Assembly Generation
    const std::string& asm_load_symbol(std::ostream& os, const std::string& name, regtype); // write asm to load symbol into regfile
    bool asm_store_symbol(std::ostream& os, const std::string& name); // write asm to store symbol valute to memory
    bool asm_clean_up(std::ostream& os); // called after every statement
    bool asm_spill_all(std::ostream& os, regtype); // spill everything

    // Utility Functions
    bool have(regtype);

private:
    // Members
    std::map<std::string, std::vector<int>> symtable; // symbols & their assigned offset & dirty bit
    std::map<int, std::vector<int>> regfile; // current state of registers and their address
    std::map<std::string, int> curr_offset; // current offset of a specific function stack
    std::string curr_func;
};

#endif