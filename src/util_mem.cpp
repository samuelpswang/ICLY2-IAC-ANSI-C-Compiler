#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "util_mem.hpp"

// Basics
// constructor, should initialize four members
MemoryContext::MemoryContext() {
    // iitialize typetable
    this->typetable["char"] = {"", 1};
    this->typetable["int"] = {"", 4};
    this->typetable["enum"] = {"", 4};
    this->typetable["unsigned"] = {"", 4};
    this->typetable["float"] = {"", 4};
    this->typetable["*"] = {"", 4};
    this->typetable["double"] = {"", 8};
    // initialize regfile
    for (int i = 0; i < 32; i++) {
        if (i < 10) this->regfile[i] = {1, -1};
        else this->regfile[i] = {0, -1};
    }
    // initialize other members
    this->curr_func = "";
    this->curr_en_num = -1;
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
        throw std::runtime_error("MemoryContextError: use_func() function name not initialized: "+func_name);
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
        throw std::runtime_error("MemoryContextError: use_symbol() symbol name not initialized: "+symbol_name);
    }
}

// returns true when symbol added to typetable
bool MemoryContext::add_type(const std::string& symbol_name, const std::string& type, int size) {
    for (const auto& entry: this->typetable) {
        if (entry.first == symbol_name) throw std::runtime_error("MemeoryContextError: add_type() duplicate symbol found: "+symbol_name);
    }
    this->typetable[symbol_name] = {type, size};
    return true;
}

// returns true when symbol deleted from typetable
bool MemoryContext::delete_type(const std::string& symbol_name) {
    bool found = false;
    for (const auto& entry: this->typetable) {
        if (entry.first == symbol_name) found = true;
    }
    if (found) {
        this->typetable.erase(symbol_name);
        return true;
    } else {
        throw std::runtime_error("MemeoryContextError: delete_type() symbol not found: "+symbol_name);
    }
}

// returns true after control flow labels are pushed
bool MemoryContext::add_cf_label(const std::string& start_label, const std::string& end_label) {
    this->curr_cf_start.push_back(start_label);
    this->curr_cf_end.push_back(end_label);
    return true;
}

// returns true after control flow labels are poped
bool MemoryContext::delete_cf_label() {
    this->curr_cf_start.pop_back();
    this->curr_cf_end.pop_back();
    return true;
}

// returns true after an enum symbol is added to the enum table
bool MemoryContext::add_en_symbol(const std::string& symbol_name, bool value_given, int value) {
    for (const auto& p: this->enumtable) {
        if (symbol_name == p.first) 
            throw std::runtime_error("MemoryContextError: add_en_symbol() duplicate symbol name: "+symbol_name);
    }
    if (value_given) {
        this->enumtable[symbol_name] = value;
        this->curr_en_num = value;
        return true;
    } else {
        this->enumtable[symbol_name] = ++this->curr_en_num;
        return true;
    }
}

// returns true after the curr enum count is reset
bool MemoryContext::delete_en_count() {
    this->curr_en_num = -1;
    return true;
}


// Getters
// returns the address offset of a symbol
int MemoryContext::get_symbol(const std::string& symbol_name) {
    auto it = this->symtable[curr_func].find(symbol_name);
    if (it != this->symtable[curr_func].end()) {
        return it->second[1];
    } else {
        throw std::runtime_error("MemoryContextError: get_symbol() symbol name not initialized: "+symbol_name);
    }
}

// returns the current offset
int MemoryContext::get_curr_offset(std::string function_name) {
    return this->curr_offset[function_name];
}

// returns the current function
std::string MemoryContext::get_curr_function() {
    return this->curr_func;
}

// returns the type of a symbol
std::string MemoryContext::get_type(const std::string& symbol_name) {
    for (const auto& entry: this->typetable) {
        if (entry.first == symbol_name) return entry.second.first;
    }
    throw std::runtime_error("MemeoryContextError: get_type() symbol not found: "+symbol_name);
}

// returns the size of a symbol
int MemoryContext::get_size(const std::string& symbol_name) {
    for (const auto& entry: this->typetable) {
        if (entry.first == symbol_name) return entry.second.second;
    }
    throw std::runtime_error("MemeoryContextError: get_size() symbol not found: "+symbol_name);
}


// Assembly Generation
// return initialized register after writing asm to set reg to 0 and link to symtable
std::string MemoryContext::asm_give_reg(std::ostream& os, const std::string& name, regtype t) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: asm_give_reg() symbol name not initialized: "+name);
    }
    
    std::string reg;
    int offset = this->symtable[this->curr_func][name][0];

    if( t == areg){
        if (have(t)) {
            std::string reg_number = std::to_string(next(t));
            reg = "x" + reg_number;
            this->symtable[curr_func][name] = {offset, next(t), 1};
            this->regfile[next(t)] = {1, offset};
        } 
        else {
            reg = "";
        }
    }
    else{
        if (have(t)) {
            std::string reg_number = std::to_string(next(t));
            reg = "x" + reg_number;
            os << "\tli " << reg << ", 0x00" << std::endl;
            this->symtable[curr_func][name] = {offset, next(t), 1};
            this->regfile[next(t)] = {1, offset};
        } 
        else {
            reg = "";
        }
    }

    return reg;
}

// return loaded register after writing asm to load symbol into regfile and linked to symtable
std::string MemoryContext::asm_load_symbol(std::ostream& os, const std::string& name, regtype t) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: asm_load_symbol() symbol name not initialized: "+name);
    }
    
    if (have(t)) {
        int offset = this->symtable[curr_func][name][0];
        std::string reg = asm_give_reg(os, name, t);
        os << "\tlw " << reg << ", " << offset << "(sp)" <<std::endl; 
        return reg;
    } else {
        asm_spill_all(os,t);
        int offset = this->symtable[curr_func][name][0];
        std::string reg = asm_give_reg(os, name, t);
        os << "\tlw " << reg << ", " << offset << "(sp)" <<std::endl; 
        return reg;
    }
    

}

// return true after writing asm to store symbol to memory
bool MemoryContext::asm_store_symbol(std::ostream& os, const std::string& name) {
    auto it = this->symtable[curr_func].find(name);
    if (it == this->symtable[curr_func].end()) {
        throw std::runtime_error("MemoryContextError: asm_store_symbol() symbol name not initialized: "+name);
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
bool MemoryContext::asm_spill_all(std::ostream& os, regtype t = treg) {
    for (auto it = this->symtable.begin(); it != this->symtable.end(); it++) {
        std::string function = it->first;
        for(auto it2 = this->symtable[function].begin(); it2 != this->symtable[function].end(); it2++){
            int reg = it2->second[1];
            std::string reg_file = "x" + std::to_string(reg);
            int offset = it2->second[0];
            switch (t) {
            case areg:
                if(0<=reg && reg<18){
                    os << "\tsw " << reg_file << ", " << offset << "(sp)" << std::endl;
                    it2->second[2] = 0;
                    this->regfile[reg] = {0 ,-1};
                    it2->second[1] = -1;
                }
                break;
            case sreg:
                if(18<=reg && reg<28){
                    os << "\tsw " << reg_file << ", " << offset << "(sp)" << std::endl;
                    it2->second[2] = 0;
                    this->regfile[reg] = {0 ,-1};
                    it2->second[1] = -1;
                }
                break;
            case treg:
                if(5<=reg && reg<8){
                    os << "\tsw " << reg_file << ", " << offset << "(sp)" << std::endl;
                    it2->second[2] = 0;
                    this->regfile[reg] = {0 ,-1};
                    it2->second[1] = -1;
                }
                if(28<=reg && reg<32){
                    os << "\tsw " << reg_file << ", " << offset << "(sp)" << std::endl;
                    it2->second[2] = 0;
                    this->regfile[reg] = {0 ,-1};
                    it2->second[1] = -1;
                }
                break;
            default:
                if(0<=reg && reg<32){
                    os << "\tsw " << reg_file << ", " << offset << "(sp)" << std::endl;
                    it2->second[2] = 0;
                    this->regfile[reg] = {0 ,-1};
                    it2->second[1] = -1;
                }
                break;
            }
                 
        }
    }
    return true;
}

// return the control flow labels
std::pair<std::string, std::string> MemoryContext::get_cf_label() {
    return { this->curr_cf_start.back(), this->curr_cf_end.back() };
}

// return true if the enum symbol is in enumtable
bool MemoryContext::get_en_symbol_check(const string& symbol_name) {
    bool found = false;
    for (const auto& p: this->enumtable) {
        if (symbol_name == p.first) found = true;
    }
    return found;
}

// return the value of the enum symbol
int MemoryContext::get_en_symbol(const std::string& symbol_name) {
    bool found = false;
    for (const auto& p: this->enumtable) {
        if (symbol_name == p.first) found = true;
    }
    if (!found) throw std::runtime_error("MemoryContextError: get_en_symbol() symbol name not initalized: "+symbol_name);
    else return this->enumtable[symbol_name];
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
