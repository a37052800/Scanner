#include "uselib.h"
enum class token_type
{
    Reserved_word,
    Library_name,
    Identifier,
    Character,
    Number,
    Pointer,
    Bracket,
    Operator,
    Comparator,
    Address,
    Punctuation,
    Format_specifier,
    Printed_token,
    Comment,
    Undefined_token,
    Skipped_token
};

class Token
{
private:
    token_type type;
    std::string text;

public:
    Token();
    ~Token();
};

Token::Token()
{

}

Token::~Token()
{
    
}