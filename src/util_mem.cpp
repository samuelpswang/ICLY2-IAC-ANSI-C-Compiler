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
const std::string& MemoryContext::add_func(const std::string& func_name) {
    std::string unique = func_name + std::to_string(++curr_unique_num);
    std::map<std::string, std::vector<int>> entry;
    symtable[unique] = entry;
    return unique;
}

// returns symbol table name when switched to function space
bool MemoryContext::use_func(const std::string& func_name) {
    if (this->symtable.find(func_name) != this->symtable.end()) {
        this->curr_func = func_name;
        return true;
    } else {
        throw std::runtime_error("MemoryContextError: function name not initialized");
    }
}

// returns true when added symbol to symbol table, variables are stored as is
const std::string& MemoryContext::add_symbol(const std::string& symbol_name, bool variable) {
    std::string unique = symbol_name + std::to_string(++curr_unique_num);
    this->symtable[this->curr_func][unique] = {++this->curr_offset[this->curr_func], -1, 0};
    return unique;
}

// returns true when successfully marked symbol as needed to store to mem
bool MemoryContext::use_symbol(const std::string& symbol_name) {
    auto it = this->symtable[curr_func].find(symbol_name);
    if (it != this->symtable[curr_func].end()) {
        it->second[2] = 1;
        return true;
    } else {
        throw std::runtime_error("MemoryContextError: symbol name not initialized");
    }
}

// returns the address offset of a symbol
int MemoryContext::get_symbol(const std::string& symbol_name) {
    auto it = this->symtable[curr_func].find(symbol_name);
    if (it != this->symtable[curr_func].end()) {
        return it->second[1];
    } else {
        throw std::runtime_error("MemoryContextError: symbol name not initialized");
    }
}

const std::string& MemoryContext::asm_give_reg(std::ostream& os, const std::string& name, regtype) {

}

// return loaded register after writing asm to load symbol into regfile and linked to symtable
const std::string& MemoryContext::asm_load_symbol(std::ostream& os, const std::string& name, regtype){

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
bool MemoryContext::have(regtype t) {
    switch (t) {
        case areg:
            for (size_t reg = 10; reg < 18; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return true;
                }
            }
            return false;
        case sreg:
            for (size_t reg = 18; reg < 28; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return true;
                }
            }
            return false;
        case treg:
            for (size_t reg = 5; reg < 8; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return true;
                }
            }
            for (size_t reg = 28; reg < 32; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return true;
                }
            }
            return false;
        default:
            for (size_t reg = 0; reg < 32; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return true;
                }
            }
            return false;
    }
}

// returns the next available address of a specific type
int MemoryContext::next(regtype t) {
    switch (t) {
        case areg:
            for (size_t reg = 10; reg < 18; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return reg;
                }
            }
            return -1;
        case sreg:
            for (size_t reg = 18; reg < 28; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return reg;
                }
            }
            return -1;
        case treg:
            for (size_t reg = 5; reg < 8; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return reg;
                }
            }
            for (size_t reg = 28; reg < 32; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return reg;
                }
            }
            return -1;
        default:
            for (size_t reg = 0; reg < 32; reg++) {
                if (this->regfile[reg][0] == 0) {
                    return reg;
                }
            }
            return -1;
    }
}


// Operator
std::ostream& operator<<(std::ostream& os, const MemoryContext& mc) {
    os << "MemoryContext @ " << &mc << "\n";
    os << "Unique Number:\n" << mc.curr_unique_num << "\n";
    os << "Current Function:\n" << mc.curr_func << "\n";
    os << "Current Offset:\n";
    for (auto it = mc.curr_offset.begin(); it != mc.curr_offset.end(); it++) {
        os << it->first << "\t" << it->second << "\n";
    }
    os << "Register File:\n";
    for (auto it = mc.regfile.begin(); it != mc.regfile.end(); it++) {
        os << it->first;
        for (int num : it->second) {
            os << "\t" << num;
        }
        os << "\n";
    }
    os << "Symbol Table:\n";
    for (auto it = mc.symtable.begin(); it != mc.symtable.end(); it++) {
        os << "--- " << it->first << " ---\n";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            os << it2->first;
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
                os << "\t" << *it3;
            }
            os << "\n";
        }
    }
}


int main() {
    MemoryContext m;
    m.add_func("main");
    m.use_func("main");
    m.add_symbol("a", true);
    std::cout << m << std::endl;
}
