#ifndef SYMBOLTABLE_INCLUDED
#define SYMBOLTABLE_INCLUDED

#include <string>

class SymbolTableImpl;

class SymbolTable
{
  public:
    SymbolTable();
    ~SymbolTable();
    void enterScope();
    bool exitScope();
    bool declare(const std::string& id, int lineNum);
    int find(const std::string& id) const;
  private:
    SymbolTableImpl* m_impl;
      // SymbolTables can not be copied or assigned.  We enforce this
      // by declaring the copy constructor and assignment operator private and
      // not implementing them.
    SymbolTable(const SymbolTable&);
    SymbolTable& operator=(const SymbolTable&);
};

#endif // SYMBOLTABLE_INCLUDED
