// SymbolTable.cpp

// This is a correct but inefficient implementation of
// the SymbolTable functionality.

#include "SymbolTable.h"
#include <string>
#include <list>
using namespace std;

#define NUM_BUCK   500

// This class does the real work of the implementation.

struct Node
{
    Node(const string& id, int lineNum, int scope)
    {
        m_id = id;
        m_line = lineNum;
        m_scope = scope;
    }
    string m_id;
    int m_line;
    int m_scope;
};

int hashFunc(const string &id)
{
    int total = 0;
    for(int i=0; i<id.length(); i++)
        total = id[i];
    
    int index = (total % NUM_BUCK);
    return(index);
}

class SymbolTableImpl
{
  public:
    SymbolTableImpl() {m_AtScope = 0;}
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id);
  private:
    list<Node> m_buckets[NUM_BUCK];
    int m_AtScope;
};

void SymbolTableImpl::enterScope()
{
    m_AtScope++;
}

bool SymbolTableImpl::exitScope()
{
    if (m_AtScope == 0)
        return false;
    
    for(int j=0; j<NUM_BUCK; j++)
    {
        if(!m_buckets[j].empty())
        for (list<Node>::iterator n = m_buckets[j].begin(); n != m_buckets[j].end();)
            {
                if(n->m_scope == m_AtScope)
                    n = m_buckets[j].erase(n);
                
                else
                    n++;
            }
    }
    m_AtScope--;
    return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
    int slot = hashFunc(id);
    
    list<Node>* b = &m_buckets[slot];
    
    Node* n = new Node(id, lineNum, m_AtScope);
    
    if(b->empty())
    {
        b->push_front(*n);
        return true;
    }
    
    
    for(list<Node>::iterator p = b->begin(); p != b->end(); p++)
    {
        if(n->m_id == p->m_id && p->m_scope == m_AtScope)
            return false;
    }
    
    b->push_front(*n);
    return true;
}

int SymbolTableImpl::find(const string& id)
{
    if (id.empty())
        return -1;
    
    int slot = hashFunc(id);
    
        list<Node>* b = &m_buckets[slot];
    
    for(list<Node>::iterator p = b->begin(); p != b->end(); p++)
    {
        if(p->m_id == id && p->m_scope <= m_AtScope)
            return p->m_line;
    }
    
    return -1;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}
