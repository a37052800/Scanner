#include "uselib.h"

class Symbol_Table
{
private:
    std::set<std::string> set;
public:
    Symbol_Table();
    ~Symbol_Table();
};

Symbol_Table::Symbol_Table()
{
}

Symbol_Table::~Symbol_Table()
{
}
