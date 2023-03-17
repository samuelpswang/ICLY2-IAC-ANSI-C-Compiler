#ifndef compiler_util_mem
#define compiler_util_mem

#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;


enum regtype { areg, sreg, treg, fareg, fsreg, ftreg };

class MemoryContext {
private:
    // Members
    // symbols & their assigned offset & dirty bit
    map<string, map<string, vector<int>>> symtable;
    // symbol & their types & sizes
    map<string, pair<string, int>> typetable;
    // current state of registers and their address
    map<int, vector<int>> regfile;
    // current offset of a specific function stack 
    map<string, int> curr_offset;
    // current function
    string curr_func;
    // current control flow start label
    vector<string> curr_cf_start;
    // current control flow start label
    vector<string> curr_cf_end;
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
};

#endif


// what is a symbol?
// In ast:
// string condsym = add_symbol("if_cond_reg");
// string condreg = asm_give_reg(condsym);
