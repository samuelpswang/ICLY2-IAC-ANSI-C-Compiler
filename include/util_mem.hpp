#ifndef compiler_mem
#define compiler_mem

#include <iostream>
#include <vector>
#include <string>
#include <map>

enum regtype { areg, sreg, treg, fareg, fsreg, ftreg };

class MemoryContext {
private:
    // Members
    // symbols & their assigned offset & dirty bit
    std::map<std::string, std::map<std::string, std::vector<int>>> symtable; 
    // current state of registers and their address
    std::map<int, std::vector<int>> regfile;
    // current offset of a specific function stack 
    std::map<std::string, int> curr_offset;
    // current function
    std::string curr_func;
    // unique number for creating names
    int curr_unique_num;

    // Utility Functions
    // returns if have register of specific type
    bool have(regtype);
    // returns the next available address of a specific type
    int next(regtype);
    // Operator
    // overload <<
    friend std::ostream& operator<<(std::ostream&, const MemoryContext&);

public:
    // Basics
    // constructor, should initialize four members
    MemoryContext();

    // Accessors
    // returns symbol table name when added function to symtable and curr_offset
    std::string add_func(const std::string&);
    // returns symbol table name when switched to function space
    bool use_func(const std::string&);
    // returns true when added symbol to symbol table, variables are stored as is
    std::string add_symbol(const std::string&, bool);
    // returns true when successfully marked symbol as needed to store to mem
    bool use_symbol(const std::string&);
    // returns the address offset of a symbol
    int get_symbol(const std::string&);

    // Assembly Generation
    // return initialized register after writing asm to set reg to 0 and linked to symtable
    std::string asm_give_reg(std::ostream&, const std::string&, regtype);
    // return loaded register after writing asm to load symbol into regfile and linked to symtable
    std::string asm_load_symbol(std::ostream&, const std::string&, regtype); 
    // return true after writing asm to store symbol to memory
    bool asm_store_symbol(std::ostream&, const std::string&);
    // return true after storing all symbols marked as dirty to memory
    bool asm_clean_up(std::ostream&);
    // return true after storing all registers to memory
    bool asm_spill_all(std::ostream&, regtype);
    //returns curr_offset for function
    int get_curr_offset(std::string);
    //returns current function
    std::string get_curr_function();
};

#endif


// what is a symbol?
// In ast:
// std::string condsym = add_symbol("if_cond_reg");
// std::string condreg = asm_give_reg(condsym);
