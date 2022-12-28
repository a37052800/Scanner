#include "uselib.h"
#include "Token.h"

enum class scanner_state{
    NORMAL_STATE,
    IDENTIFY_STATE,
    SLASH_STATE,
    INCLUDE_STATE,
    PRINT_STATE,
    POINTER_STATE,
    ADDRESS_STATE
};
class Scanner
{
private:
    scanner_state state;
    int current_line;
    Token getToken();
public:
    Scanner();
    ~Scanner();
};

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

Token Scanner::getToken()
{
    Token token;
}