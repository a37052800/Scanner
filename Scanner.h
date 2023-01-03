#include "uselib.h"
#include "Token.h"
#include "Reader.h"
#include "Regex_Checker.h"
#include "Symbol_Table.h"

Symbol_Table symbol_table;

class Scanner;

class Scanner_State
{
public:
    virtual Token getToken(Scanner *) = 0;
};
class NORMAL_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class IDENTIFY_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class DEFINE_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class SLASH_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class CHARACTER_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class INCLUDE_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class PRINT_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class POINTER_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class SKIP_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class NUMBER_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};
class EOF_STATE : public Scanner_State
{
public:
    virtual Token getToken(Scanner *) override;
};

class Scanner
{
private:
    Scanner_State *state = new NORMAL_STATE();

public:
    Scanner(std::string);
    Scanner();
    ~Scanner();
    Reader reader;
    Token getToken();
    void setState(Scanner_State *);
};

Scanner::Scanner(std::string filename)
{
    this->reader.loadFile(filename);
}
Scanner::Scanner()
{
}
Scanner::~Scanner()
{
    delete this->state;
}
void Scanner::setState(Scanner_State *new_state)
{
    delete this->state;
    this->state = new_state;
}
Token Scanner::getToken()
{
    return state->getToken(this);
}
Token NORMAL_STATE::getToken(Scanner *scanner)
{
    Reader *reader = &scanner->reader;
    char temp_char;
    std::string temp_str;
    switch (Regex_Checker(reader->peek()).getType())
    {
    case regex_type::ALPHA:
        scanner->setState(new IDENTIFY_STATE());
        return scanner->getToken();
        break;
    case regex_type::NUMBER:
        scanner->setState(new NUMBER_STATE());
        return scanner->getToken();
        break;
    case regex_type::IGNORE:
        // std::cout<<"ignore:"<<(int)reader->peek()<<'\n'; //test
        if (reader->peek() == '$')
        {
            scanner->setState(new EOF_STATE());
            reader->getChar();
            return Token();
        }
        reader->getChar();
        return scanner->getToken();
        break;
    case regex_type::MARK:
        switch (reader->peek())
        {
        case '(':
            scanner->setState(new NUMBER_STATE());
            return scanner->getToken();
            break;
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
            return Token(reader->getChar(), token_type::Bracket);
            break;
        case '-':
            scanner->setState(new NUMBER_STATE());
            return scanner->getToken();
            break;
        case '+':
            reader->getChar();
            if (reader->peek() == '+')
            {
                reader->getChar();
                return Token("++", token_type::Operator);
            }
            else
                return Token("+", token_type::Operator);
            break;
        case '*':
        case '&':
            scanner->setState(new POINTER_STATE());
            return scanner->getToken();
            break;
        case '%':
            return Token(reader->getChar(), token_type::Operator);
            break;
        case '\\':
            scanner->setState(new SKIP_STATE);
            return Token(reader->getChar(), token_type::Undefined_token);
            break;
        case '/':
            scanner->setState(new SLASH_STATE());
            return scanner->getToken();
            break;
        case '^':
        case '|':
            return Token(reader->getChar(), token_type::Operator);
            break;
        case '=':
            reader->getChar();
            if (reader->peek() == '=')
            {
                reader->getChar();
                return Token("==", token_type::Comparator);
            }
            else
                return Token('=', token_type::Operator);
            break;
        case '>':
        case '<':
        case '!':
            temp_char = reader->getChar();
            if (reader->peek() == '=')
            {
                std::string str;
                str = temp_char;
                str += reader->getChar();
                return Token(str, token_type::Comparator);
            }
            else
                return Token(temp_char, token_type::Comparator);
            break;
        case '\'':
            scanner->setState(new CHARACTER_STATE());
            return scanner->getToken();
            break;
        case ',':
        case ';':
        case ':':
        case '"':
            return Token(reader->getChar(), token_type::Punctuation);
            break;
        case '#':
            scanner->setState(new INCLUDE_STATE());
            return scanner->getToken();
            break;
        default:
            std::cout << "regex error\n";
            exit(1);
        }
        break;
    case regex_type::UNKNOWN:
        // std::cout<<"unknow:"<<(int)reader->peek()<<'\n'; //test
        scanner->setState(new SKIP_STATE);
        return Token(reader->getChar(), token_type::Undefined_token);
        break;
    default:
        std::cout << "regex error\n";
        exit(1);
        break;
    }
}
Token IDENTIFY_STATE::getToken(Scanner *scanner)
{
    std::string str;
    while (Regex_Checker::regMatch(scanner->reader.peek(), "[A-Za-z0-9_]"))
        str += scanner->reader.getChar();
    switch (Regex_Checker(str).getType())
    {
    case regex_type::RESERVED_WORD:
        if (Regex_Checker::regMatch(str, "(char)|(int)|(float)", 1))
            scanner->setState(new DEFINE_STATE());
        else if (Regex_Checker::regMatch(str, "(printf)|(scanf)", 1))
        {
            while (Regex_Checker::regMatch(scanner->reader.peek(), "[ \r\n]"))
                scanner->reader.getChar();
            if (scanner->reader.peek() == '(')
                scanner->setState(new PRINT_STATE());
            else
                scanner->setState(new NORMAL_STATE());
        }
        else
            scanner->setState(new NORMAL_STATE());
        return Token(str, token_type::Reserved_word);
        break;
    case regex_type::ALPHA:
    case regex_type::UNKNOWN:
        if (symbol_table.exist(str))
        {
            scanner->setState(new NORMAL_STATE());
            return Token(str, token_type::Identifier);
            break;
        }
    default:
        scanner->setState(new SKIP_STATE());
        return Token(str, token_type::Undefined_token);
        break;
    }
}
Token DEFINE_STATE::getToken(Scanner *scanner)
{
    std::string str;
    switch (Regex_Checker(scanner->reader.peek()).getType())
    {
    case regex_type::IGNORE:
        scanner->reader.getChar();
        return scanner->getToken();
        break;
    case regex_type::ALPHA:
        str.clear();
        while (Regex_Checker::regMatch(scanner->reader.peek(), "[A-Za-z0-9_]"))
            str += scanner->reader.getChar();
        if (Regex_Checker(str).getType() == regex_type::RESERVED_WORD)
        {
            scanner->setState(new NORMAL_STATE());
            return Token(str, token_type::Reserved_word);
        }
        symbol_table.insert(str);
        return Token(str, token_type::Identifier);
        break;
    case regex_type::MARK:
        str.clear();
        switch (scanner->reader.peek())
        {
        case ',':
            return Token(scanner->reader.getChar(), token_type::Punctuation);
            break;
        case '*':
            str = scanner->reader.getChar();
            while (Regex_Checker::regMatch(scanner->reader.peek(), "[A-Za-z0-9_]"))
                str += scanner->reader.getChar();
            symbol_table.insert(str);
            return Token(str, token_type::Pointer);
        case ';':
            scanner->setState(new NORMAL_STATE());
            return Token(scanner->reader.getChar(), token_type::Punctuation);
            break;
        }
    default:
        scanner->setState(new NORMAL_STATE());
        return scanner->getToken();
    }
}
Token SLASH_STATE::getToken(Scanner *scanner)
{
    std::string str;
    str = scanner->reader.getChar();
    switch (scanner->reader.peek())
    {
    case '/':
        scanner->setState(new NORMAL_STATE());
        str += scanner->reader.getLine();
        return Token(str, token_type::Comment);
        break;
    case '*':
        scanner->setState(new NORMAL_STATE());
        while (!scanner->reader.eof())
        {
            if (scanner->reader.peek() == '*')
            {
                str += scanner->reader.getChar();
                if (scanner->reader.peek() == '/')
                {
                    str += scanner->reader.getChar();
                    break;
                }
            }
            str += scanner->reader.getChar();
        }
        return Token(str, token_type::Comment);
        break;
    default:
        scanner->setState(new NORMAL_STATE());
        return Token(str, token_type::Operator);
        break;
    }
}
Token CHARACTER_STATE::getToken(Scanner *scanner)
{
    scanner->setState(new NORMAL_STATE());
    std::string str;
    str = scanner->reader.getChar();
    str += scanner->reader.getChar();
    str += scanner->reader.getChar();
    if (Regex_Checker::regMatch(str, "'.'"))
        return Token(str, token_type::Character);
    else
    {
        scanner->reader.back();
        scanner->reader.back();
        return Token('\'', token_type::Punctuation);
    }
}
Token INCLUDE_STATE::getToken(Scanner *scanner)
{
    std::string str;
    switch (scanner->reader.peek())
    {
    case '#':
        return Token(scanner->reader.getChar(), token_type::Punctuation);
        break;
    case ' ':
        scanner->reader.getChar();
        return scanner->getToken();
        break;
    case 'i':
    case 'I':
        str.clear();
        while (Regex_Checker(scanner->reader.peek()).getType() == regex_type::ALPHA)
            str += scanner->reader.getChar();
        if (Regex_Checker::regMatch(str, "include", 1))
            return Token(str, token_type::Reserved_word);
        else
        {
            scanner->setState(new NORMAL_STATE());
            for (int i = 0; i < str.size(); i++)
                scanner->reader.back();
            return scanner->getToken();
        }
        break;
    case '<':
        str.clear();
        while (Regex_Checker(scanner->reader.peek()).getType() != regex_type::IGNORE)
            str += scanner->reader.getChar();
        if (Regex_Checker::regMatch(str, "<.+[.]h>"))
        {
            scanner->setState(new NORMAL_STATE());
            return Token(str, token_type::Library_name);
        }
        else
        {
            scanner->setState(new NORMAL_STATE());
            for (int i = 0; i < str.size(); i++)
                scanner->reader.back();
            return scanner->getToken();
        }
        break;
    default:
        scanner->setState(new NORMAL_STATE());
        return scanner->getToken();
        break;
    }
}
Token PRINT_STATE::getToken(Scanner *scanner)
{
    char temp_char;
    std::string temp_str;
    switch (Regex_Checker(scanner->reader.peek()).getType())
    {
    case regex_type::IGNORE:
        if (scanner->reader.peek() == ' ')
            scanner->reader.getChar();
        else
            scanner->setState(new NORMAL_STATE());
        return scanner->getToken();
    case regex_type::MARK:
        switch (scanner->reader.peek())
        {
        case '(':
            temp_char = scanner->reader.getChar();
            while (Regex_Checker::regMatch(scanner->reader.peek(), "[ \r\n]"))
                scanner->reader.getChar();
            if (scanner->reader.peek() == '"')
            {
                temp_str.clear();
                temp_str = scanner->reader.getChar();
                while (Regex_Checker::regMatch(scanner->reader.peek(), "[^\r\n\"]"))
                    temp_str += scanner->reader.getChar();
                if (scanner->reader.peek() != '"')
                    scanner->setState(new NORMAL_STATE());
                for (int i = 0; i < temp_str.size(); i++)
                    scanner->reader.back();
            }
            return Token(temp_char, token_type::Bracket);
            break;
        case '"':
            temp_char = scanner->reader.getChar();
            while (Regex_Checker::regMatch(scanner->reader.peek(), "[ \r\n]"))
                scanner->reader.getChar();
            if (Regex_Checker::regMatch(scanner->reader.peek(), "[,)]"))
                scanner->setState(new NORMAL_STATE());
            return Token(temp_char, token_type::Punctuation);
            break;
        case '%':
            temp_str.clear();
            temp_str = scanner->reader.getChar();
            switch (scanner->reader.peek())
            {
            case 'd':
            case 'f':
            case 'c':
                temp_str += scanner->reader.getChar();
                return Token(temp_str, token_type::Format_specifier);
                break;
            default:
                scanner->setState(new SKIP_STATE);
                return Token(temp_str, token_type::Undefined_token);
                break;
            }
        case '\\':
            temp_str.clear();
            temp_str = scanner->reader.getChar();
            if (Regex_Checker(scanner->reader.peek()).getType() == regex_type::ALPHA)
            {
                temp_str += scanner->reader.getChar();
                return Token(temp_str, token_type::Format_specifier);
            }
            scanner->setState(new SKIP_STATE);
            return Token(temp_str, token_type::Undefined_token);
            break;
        }
    default:
        temp_str.clear();
        while (Regex_Checker::regMatch(scanner->reader.peek(), "[^ \r\n\"%\\\\$]"))
            temp_str += scanner->reader.getChar();
        return Token(temp_str, token_type::Printed_token);
    }
}
Token POINTER_STATE::getToken(Scanner *scanner)
{
    char temp = scanner->reader.getChar();
    std::string str;
    if (temp == '*')
        str = temp;
    while (Regex_Checker::regMatch(scanner->reader.peek(), "[A-Za-z0-9_]"))
        str += scanner->reader.getChar();
    if (symbol_table.exist(str))
    {
        scanner->setState(new NORMAL_STATE());
        if (temp == '*')
            return Token(str, token_type::Pointer);
        else
        {
            std::string combine;
            combine = temp;
            combine += str;
            return Token(combine, token_type::Address);
        }
    }
    else
    {
        if (temp == '*')
        {
            scanner->setState(new NORMAL_STATE());
            for (int i = 0; i < str.size() - 1; i++)
                scanner->reader.back();
            return Token(temp, token_type::Operator);
        }
        else
        {
            scanner->setState(new NORMAL_STATE());
            for (int i = 0; i < str.size(); i++)
                scanner->reader.back();
            return Token(temp, token_type::Operator);
        }
    }
}
Token SKIP_STATE::getToken(Scanner *scanner)
{
    scanner->setState(new NORMAL_STATE());
    return Token(scanner->reader.getLine(), token_type::Skipped_token);
}
Token NUMBER_STATE::getToken(Scanner *scanner)
{
    std::string str;
    if (scanner->reader.peek() == '(')
    {
        str += scanner->reader.getChar();
        if (scanner->reader.peek() != '-')
        {
            scanner->setState(new NORMAL_STATE());
            return Token('(', token_type::Bracket);
        }
        str += scanner->reader.getChar();
        if (Regex_Checker::regMatch(scanner->reader.peek(), "[^0-9]"))
        {
            scanner->setState(new NORMAL_STATE());
            scanner->reader.back();
            return Token('(', token_type::Bracket);
        }
    }
    else if (scanner->reader.peek() == '-')
    {
        str += scanner->reader.getChar();
        if (Regex_Checker::regMatch(scanner->reader.peek(), "[^0-9]"))
        {
            scanner->setState(new NORMAL_STATE());
            if (scanner->reader.peek() == '-')
            {
                scanner->reader.getChar();
                return Token("--", token_type::Operator);
            }
            return Token('-', token_type::Operator);
        }
    }
    while (Regex_Checker::regMatch(scanner->reader.peek(), "[0-9.]"))
    {
        if (scanner->reader.peek() == '.')
        {
            str += scanner->reader.getChar();
            break;
        }
        str += scanner->reader.getChar();
    }
    while (Regex_Checker::regMatch(scanner->reader.peek(), "[0-9]"))
        str += scanner->reader.getChar();
    if (str[0] == '(')
    {
        if (scanner->reader.peek() == ')')
            str += scanner->reader.getChar();
        else
        {
            scanner->setState(new NORMAL_STATE());
            for (int i = 0; i < str.size() - 1; i++)
                scanner->reader.back();
            return Token('(', token_type::Bracket);
        }
    }
    switch (Regex_Checker(str).getType())
    {
    case regex_type::NUMBER:
        scanner->setState(new NORMAL_STATE());
        return Token(str, token_type::Number);
        break;
    case regex_type::MARK:
        scanner->setState(new NORMAL_STATE());
        switch (str[0])
        {
        case '-':
            return Token(str[0], token_type::Operator);
            break;
        case '(':
            return Token(str[0], token_type::Bracket);
            break;
        default:
            std::cout << "regex error\n";
            exit(1);
            break;
        }
        break;
    case regex_type::UNKNOWN:
        scanner->setState(new SKIP_STATE());
        return Token(str, token_type::Undefined_token);
        break;
    default:
        std::cout << "regex error\n";
        exit(1);
        break;
    }
    return Token(scanner->reader.getLine(), token_type::Skipped_token);
}
Token EOF_STATE::getToken(Scanner *scanner)
{
    return Token();
}