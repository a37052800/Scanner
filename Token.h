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
    Skipped_token,
    NUL
};
std::string token_type_toString(token_type type)
{
    switch (type)
    {
    case token_type::Reserved_word:
        return "reserved word";
        break;
    case token_type::Library_name:
        return "library name";
        break;
    case token_type::Identifier:
        return "identifier";
        break;
    case token_type::Character:
        return "character";
        break;
    case token_type::Number:
        return "number";
        break;
    case token_type::Pointer:
        return "pointer";
        break;
    case token_type::Bracket:
        return "bracket";
        break;
    case token_type::Operator:
        return "operator";
        break;
    case token_type::Comparator:
        return "comparator";
        break;
    case token_type::Address:
        return "address";
        break;
    case token_type::Punctuation:
        return "punctuation";
        break;
    case token_type::Format_specifier:
        return "format specifier";
        break;
    case token_type::Printed_token:
        return "printed token";
        break;
    case token_type::Comment:
        return "comment";
        break;
    case token_type::Undefined_token:
        return "undefined token";
        break;
    case token_type::Skipped_token:
        return "skipped token";
        break;
    case token_type::NUL:
        return "NUL";
        break;
    default:
        return "error";
        break;
    }
}

class Token
{
private:
    token_type type;
    std::string text;

public:
    Token();
    Token(char, token_type);
    Token(std::string, token_type);
    std::string getText();
    token_type getType();
    ~Token(){};
};

Token::Token()
{
    this->type = token_type::NUL;
    this->text = '$';
}
Token::Token(char c, token_type type)
{
    this->type = type;
    this->text = c;
}
Token::Token(std::string str, token_type type)
{
    this->type = type;
    this->text = str;
}
std::string Token::getText()
{
    return this->text;
}
token_type Token::getType()
{
    return this->type;
}