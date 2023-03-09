#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "util_mem.hpp"


// Basics
// constructor, should initialize four members
MemoryContext::MemoryContext() {}


// Accessors
// returns symbol table name when added function to symtable and curr_offset
std::string& MemoryContext::add_func(const std::string& func_name) {
    
}

// returns symbol table name when switched to function space
bool MemoryContext::use_func(const std::string& func_name) {
    
}

// returns true when added symbol to symbol table, variables are stored as is
std::string& MemoryContext::add_symbol(const std::string& symbol_name, bool variable) {
    
}

// returns true when successfully marked symbol as needed to store to mem
bool MemoryContext::use_symbol(const std::string& symbol_name) {
    
}

// returns the address offset of a symbol
int MemoryContext::get_symbol(const std::string& symbol_name) {
    
}

std::string& MemoryContext::asm_give_reg(std::ostream& os, const std::string& name, regtype) {

}

// return loaded register after writing asm to load symbol into regfile and linked to symtable
std::string& MemoryContext::asm_load_symbol(std::ostream& os, const std::string& name, regtype){

}

// return true after writing asm to store symbol to memory
bool MemoryContext::asm_store_symbol(std::ostream& os, const std::string& name){

}

// return true after storing all symbols marked as dirty to memory
bool MemoryContext::asm_clean_up(std::ostream& os){

}

// return true after storing all registers to memory
bool MemoryContext::asm_spill_all(std::ostream& os, regtype){

}


// Utility Functions
// returns if have register of specific type
bool MemoryContext::have(regtype) {

}

// returns the next available address of a specific type
int MemoryContext::next(regtype) {

}


// Operator
std::ostream& operator<<(std::ostream& os, const MemoryContext& mc) {
    
}
