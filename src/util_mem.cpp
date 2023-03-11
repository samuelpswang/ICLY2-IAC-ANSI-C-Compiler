#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../include/util_mem.hpp"

// Basics
// constructor, should initialize four members
MemoryContext::MemoryContext() {}

void MemoryContext::init_regfile(){
    
    for(int i = 0; i <32; i++){
        if(i < 10){
            this->regfile[i] = {1, -1};
        }
        else{
        this->regfile[i] = {0, -1};
        }
    }
}



// Accessors
// returns symbol table name when added function to symtable and curr_offset
std::string MemoryContext::add_func(const std::string& func_name) {
    std::string unique = func_name + std::to_string(++curr_unique_num);
    std::map<std::string, std::vector<int>> entry;
    this->symtable[unique] = entry;
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
std::string MemoryContext::add_symbol(const std::string& symbol_name, bool variable) {
    if(variable){
        this->symtable[this->curr_func][symbol_name] = {this->curr_offset[this->curr_func], -1, 0};
        this->curr_offset[this->curr_func] = this->curr_offset[this->curr_func]+4;
        return symbol_name;
    }
    else{
        std::string unique = symbol_name + std::to_string(++curr_unique_num);
        this->symtable[this->curr_func][unique] = {this->curr_offset[this->curr_func], -1, 0};   
        this->curr_offset[this->curr_func] = this->curr_offset[this->curr_func]+4;
        return unique;
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
std::string MemoryContext::asm_give_reg(std::ostream& os, const std::string& name, regtype reg_type) {
    std::string reg;
    int offset = this->symtable[this->curr_func][name][0];
    if(have(reg_type)){
        // std::cout<<"HEllo"<<std::endl;
        std::string reg_number = std::to_string(next(reg_type));
        reg = "x"+reg_number;
        os<<"li "<<reg<<", 0x00"<<std::endl;
        this->symtable[curr_func][name] = {offset, next(reg_type), 1};
        this->regfile[next(reg_type)] = {1,offset};
    }
    else{
        reg = "";
    }

    return reg;
}

// return loaded register after writing asm to load symbol into regfile and linked to symtable
std::string MemoryContext::asm_load_symbol(std::ostream& os, const std::string& name, regtype reg_type){
    int offset = this->symtable[curr_func][name][0];
    std::string reg = asm_give_reg(os,name,reg_type);
    os<<"lw "<<reg<<", "<<offset<<"(sp);"<<std::endl; 
    return reg;
}

// return true after writing asm to store symbol to memory
bool MemoryContext::asm_store_symbol(std::ostream& os, const std::string& name){
    int offset = this->symtable[curr_func][name][0];
    std::string reg = "x"+std::to_string(this->symtable[curr_func][name][1]);
    this->regfile[this->symtable[curr_func][name][1]] = {0 , -1};
    this->symtable[curr_func][name][1] = -1;
    os<<"sw "<<reg<<", "<<offset<<"(sp)"<<std::endl;
    return true;
}

// return true after storing all symbols marked as dirty to memory
bool MemoryContext::asm_clean_up(std::ostream& os){
    for(auto it = this->symtable[curr_func].begin(); it != this->symtable[curr_func].end() ; it++){
        std::string symbol = it->first;
        int offset = it->second[0];
        std::string reg = "x" + std::to_string(it->second[1]);
        int dirty_bit = it->second[2];
        if(dirty_bit == 1){
            os<<"sw "<<reg<<", "<<offset<<"(sp)"<<std::endl;
            it->second[2] = 0;
            this->regfile[it->second[1]] = {0 ,-1};
            it->second[1] = -1;
        }
    }
    return true;
}

// return true after storing all registers to memory
bool MemoryContext::asm_spill_all(std::ostream& os, regtype reg_type){
    for(auto it = this->symtable[curr_func].begin(); it != this->symtable[curr_func].end(); it++){
        int offset = it->second[0];
        if(it->second[1] != -1){
            std::string reg = "x" + std::to_string(it->second[1]);
            os<<"sw "<<reg<<", "<<offset<<"(sp)"<<std::endl; 
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
    m.init_regfile();
    m.add_func("main");
    m.use_func("main1");
    m.add_symbol("a", true);
    m.add_symbol("b",true);
    m.add_symbol("c",true);
    m.add_symbol("d",true);
    m.add_symbol("e",true);
    m.add_symbol("f",true);
    m.asm_give_reg(std::cout,"a",areg);
    m.asm_give_reg(std::cout,"b",areg);
    m.asm_store_symbol(std::cout,"a");
    m.asm_load_symbol(std::cout,"a",areg);
    m.asm_clean_up(std::cout);
    m.asm_load_symbol(std::cout,"a",areg);
    m.asm_load_symbol(std::cout,"b",areg);
    m.add_func("f");
    m.use_func("f2");
    m.add_symbol("g", true);
    m.add_symbol("h",true);
    m.add_symbol("i",true);
    m.add_symbol("j",true);
    m.add_symbol("k",true);
    m.add_symbol("l",true);
    m.asm_give_reg(std::cout,"g",areg);
    m.asm_give_reg(std::cout,"h",areg);
    m.asm_give_reg(std::cout,"i",areg);
    m.asm_give_reg(std::cout,"j",areg);
    m.asm_give_reg(std::cout,"k",areg);
    m.asm_give_reg(std::cout,"l",areg);
    // m.asm_store_symbol(std::cout,"g");
    m.asm_spill_all(std::cout,areg);
    m.use_func("main1");
    m.asm_spill_all(std::cout,areg);
    operator<<(std::cout,m);
}
