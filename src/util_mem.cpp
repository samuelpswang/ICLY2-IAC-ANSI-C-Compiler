#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "util_mem.hpp"

// Basics
// constructor, should initialize four members
MemoryContext::MemoryContext() {
    for (int i = 0; i < 32; i++) {
        if (i < 10) {
            this->regfile[i] = {1, -1};
        }
        else {
            this->regfile[i] = {0, -1};
        }
    }
    this->curr_func = "";
    this->curr_unique_num = 0;
}


// Accessors
// returns symbol table name when added function to symtable and curr_offset
std::string MemoryContext::add_func(const std::string& func_name) {
    std::string unique_func_name = func_name + std::to_string(++this->curr_unique_num);
    std::map<std::string, std::vector<int>> entry;
    this->symtable[unique_func_name] = entry;
    return unique_func_name;
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
std::string MemoryContext::add_symbol(const std::string& symbol_name, bool variable) {
    if (variable) {
        this->symtable[this->curr_func][symbol_name] = {this->curr_offset[this->curr_func], -1, 0};
        this->curr_offset[this->curr_func] = this->curr_offset[this->curr_func]+4;
        return symbol_name;
    } else {
        std::string unique_symbol_name = symbol_name + std::to_string(++this->curr_unique_num);
        this->symtable[this->curr_func][unique_symbol_name] = {this->curr_offset[this->curr_func], -1, 0};
        this->curr_offset[this->curr_func] = this->curr_offset[this->curr_func]+4;
        return unique_symbol_name;
    }
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

// return initialized register after writing asm to set reg to 0 and link to symtable
std::string MemoryContext::asm_give_reg(std::ostream& os, const std::string& name, regtype t) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: symbol name not initialized");
    }
    
    std::string reg;
    int offset = this->symtable[this->curr_func][name][0];
    if (have(t)) {
        std::string reg_number = std::to_string(next(t));
        reg = "x" + reg_number;
        os << "\tli " << reg << ", 0x00" << std::endl;
        this->symtable[curr_func][name] = {offset, next(t), 1};
        this->regfile[next(t)] = {1, offset};
    } else {
        reg = "";
    }
    return reg;
}

// return loaded register after writing asm to load symbol into regfile and linked to symtable
std::string MemoryContext::asm_load_symbol(std::ostream& os, const std::string& name, regtype t) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: symbol name not initialized");
    }
    
    if (have(t)) {
        int offset = this->symtable[curr_func][name][0];
        std::string reg = asm_give_reg(os, name, t);
        os << "\tlw " << reg << ", " << offset << "(sp)" <<std::endl; 
        return reg;
    } else {
        return "";
    }

}

// return true after writing asm to store symbol to memory
bool MemoryContext::asm_store_symbol(std::ostream& os, const std::string& name) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: symbol name not initialized");
    }

    int offset = this->symtable[curr_func][name][0];
    std::string reg = "x" + std::to_string(this->symtable[curr_func][name][1]);
    this->regfile[this->symtable[curr_func][name][1]] = {0 , -1};
    this->symtable[curr_func][name][1] = -1;
    os << "\tsw " << reg << ", " << offset << "(sp)" << std::endl;
    return true;
}

// return true after storing all symbols marked as dirty to memory
bool MemoryContext::asm_clean_up(std::ostream& os) {
    for (auto it = this->symtable[curr_func].begin(); it != this->symtable[curr_func].end(); it++) {
        std::string symbol = it->first;
        int offset = it->second[0];
        std::string reg = "x" + std::to_string(it->second[1]);
        int dirty_bit = it->second[2];
        if (dirty_bit == 1) {
            os << "\tsw " << reg << ", " << offset << "(sp)" << std::endl;
            it->second[2] = 0;
            this->regfile[it->second[1]] = {0 ,-1};
            it->second[1] = -1;
        }
    }
    return true;
}

// return true after storing all registers to memory
bool MemoryContext::asm_spill_all(std::ostream& os, regtype t) {
    for (auto it = this->symtable[curr_func].begin(); it != this->symtable[curr_func].end(); it++) {
        int offset = it->second[0];
        if(it->second[1] != -1) {
            std::string reg = "x" + std::to_string(it->second[1]);
            os << "\tsw " << reg << ", " << offset << "(sp)" << std::endl; 
            it->second[2] = 0;
            this->regfile[it->second[1]] = {0 ,-1};
            it->second[1] = -1;
        }
    }
    return true;
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
    os << "===========================\n";
    os << "Symbol Table:\n";
    for (auto it = mc.symtable.begin(); it != mc.symtable.end(); it++) {
        os << it->first << ":\n";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            os << it2->first;
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
                os << "\t" << *it3;
            }
            os << "\n";
        }
    }
    os << "Register File:\n";
    for (auto it = mc.regfile.begin(); it != mc.regfile.end(); it++) {
        os << it->first;
        for (int num : it->second) {
            os << "\t" << num;
        }
        os << "\n";
    }
    os << "Current Offset:\n";
    for (auto it = mc.curr_offset.begin(); it != mc.curr_offset.end(); it++) {
        os << it->first << "\t" << it->second << "\n";
    }
    os << "Current Function:\n" << mc.curr_func << "\n";
    os << "Unique Number:\n" << mc.curr_unique_num << "\n";
    return os;
}
