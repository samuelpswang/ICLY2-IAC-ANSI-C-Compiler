#ifndef compiler_util_mem
#define compiler_util_mem

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

static int label_unique_num = 0;
static std::string make_label(const std::string& base) {
    return "." + base + std::to_string(label_unique_num++);
}

enum regtype { areg, sreg, treg, fareg, fsreg, ftreg };

class MemoryContext {
private:
    // Members
    // symbols & their assigned offset & dirty bit
    map<string, map<string, vector<int>>> symtable;
    // current state of registers and their address
    map<int, vector<int>> regfile;
    // symbol & their types & sizes
    map<string, pair<string, int>> typetable;
    // enum symbol & their values
    map<string, int> enumtable;
    // typedef symbol & their values
    map<string, string> typedeftable;
    // holds struct name and members
    map<string, vector<pair<string,string>>> struct_table;
    // current offset of a specific function stack 
    map<string, int> curr_offset;
    // current function
    string curr_func;
    // current control flow start label
    vector<string> curr_cf_start;
    // current control flow start label
    vector<string> curr_cf_end;
    // current enum number
    int curr_en_num;
    // unique number for creating names
    int curr_unique_num;

    // Utility Functions
    // returns if have register of specific type
    bool have(regtype);
    // returns the next available address of a specific type
    int next(regtype);
    // Operator
    // overload <<
    friend ostream& operator<<(ostream&, const MemoryContext&);

public:
    // Basics
    // constructor
    MemoryContext();

    // Accessors
    // returns symbol table name when added function to symtable and curr_offset
    string add_func(const string&);
    // returns symbol table name when switched to function space
    bool use_func(const string&);
    // returns true when added symbol to symbol table, variables are stored as is
    string add_symbol(const string&, bool);
    // returns true when successfully marked symbol as needed to store to mem
    bool use_symbol(const string&);
    // returns true when symbol added to typetable
    bool add_type(const string&, const string&, int);
    // returns true when symbol deleted from typetable
    bool delete_type(const string&);
    // returns true after control flow labels are pushed
    bool add_cf_label(const string&, const string&);
    // returns true after control flow labels are poped
    bool delete_cf_label();
    // returns true after an enum symbol is added to the enum table
    bool add_en_symbol(const string&, bool, int);
    // returns true after the curr enum count is reset
    bool delete_en_count();
    // returns true after the typedef symbol is added
    bool add_td_symbol(const string&, const string&);
    //adds struct into struct table
    bool add_struct_member(const std::string&, const std::string&, const std::string&);
    // adds struct declarations to symbol table
    bool add_struct_declaration (const std::string&, const std::string&);



    // Getters
    // returns the address offset of a symbol
    int get_symbol(const string&);
    // returns curr_offset for function
    int get_curr_offset(string);
    // returns current function
    string get_curr_function();
    // returns the type of a symbol
    string get_type(const string&);
    // returns the size of a symbol
    int get_size(const string&);
    // return the control flow labels
    pair<string, string> get_cf_label();
    // return true if the enum symbol is in enumtable
    bool get_en_symbol_check(const string&);
    // return the value of the enum symbol
    int get_en_symbol(const string&);
    // returns the value of a typedef symbol
    string get_td_symbol(const string&);

    // Assembly Generation
    // return initialized register after writing asm to set reg to 0 and linked to symtable
    string asm_give_reg(ostream&, const string&, regtype);
    // return loaded register after writing asm to load symbol into regfile and linked to symtable
    string asm_load_symbol(ostream&, const string&, regtype); 
    // return true after writing asm to store symbol to memory
    bool asm_store_symbol(ostream&, const string&);
    // return true after storing all symbols marked as dirty to memory
    bool asm_clean_up(ostream&);
    // return true after storing all registers to memory
    bool asm_spill_all(ostream&, regtype);
    // store current register file into memory
    bool asm_store_all(ostream&);
    // restore current register file from memory
    bool asm_restore_all_except_a(ostream&);
};

#endif


// what is a symbol?
// In ast:
// string condsym = add_symbol("if_cond_reg");
// string condreg = asm_give_reg(condsym);
