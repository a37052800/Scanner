#include "uselib.h"

class Symbol_Table
{
private:
    std::set<std::string> set;

public:
    Symbol_Table() { this->set.clear(); };
    ~Symbol_Table(){};
    void insert(std::string);
    bool exist(std::string);
};
void Symbol_Table::insert(std::string str)
{
    this->set.insert(str);
}
bool Symbol_Table::exist(std::string str)
{
    if (this->set.count(str) == 1)
        return true;
    else
        return false;
}