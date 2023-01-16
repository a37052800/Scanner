#include "uselib.h"

enum class regex_type
{
    ALPHA,
    NUMBER,
    MARK,
    UNDERLINE,
    IGNORE,
    UNKNOWN,
    RESERVED_WORD,
};

class Regex_Checker
{
private:
    std::string text;

public:
    Regex_Checker(std::string str) { this->text = str; };
    Regex_Checker(char c) { this->text = c; };
    regex_type getType();
    static bool regMatch(char c, std::string regex)
    {
        std::string ch;
        ch = c;
        return std::regex_match(ch, std::regex(regex));
    }
    static bool regMatch(char c, std::string regex, int flag)
    {
        std::string ch;
        ch = c;
        switch (flag)
        {
        case 0:
            return std::regex_match(ch, std::regex(regex));
            break;
        case 1:
            return std::regex_match(ch, std::regex(regex, std::regex::icase));
            break;
        default:
            std::cout << "undefined flag\n";
            exit(1);
            break;
        }
    }
    static bool regMatch(std::string str, std::string regex)
    {
        return std::regex_match(str, std::regex(regex));
    }
    static bool regMatch(std::string str, std::string regex, int flag)
    {
        switch (flag)
        {
        case 0:
            return std::regex_match(str, std::regex(regex));
            break;
        case 1:
            return std::regex_match(str, std::regex(regex, std::regex::icase));
            break;
        default:
            std::cout << "undefined flag\n";
            exit(1);
            break;
        }
    }
};

regex_type Regex_Checker::getType()
{
    if (this->text.size() == 1)
    {
        if (std::regex_match(this->text, std::regex("[A-Za-z]")))
            return regex_type::ALPHA;
        else if (std::regex_match(this->text, std::regex("[0-9]")))
            return regex_type::NUMBER;
        else if (std::regex_match(this->text, std::regex("_")))
            return regex_type::UNDERLINE;
        else if (std::regex_match(this->text, std::regex("[ \r\n$]")))
            return regex_type::IGNORE;
        else if (std::regex_match(this->text, std::regex("[ -#%--/:-\?[-^{-}]")))
            return regex_type::MARK;
        else
            return regex_type::UNKNOWN;
    }
    else
    {
        if ((std::regex_match(this->text, std::regex("(include)|(main)|(char)|(int)|(float)|(if)|(else)|(elseif)|(for)|(while)|(do)|(return)|(switch)|(case)|(printf)|(scanf)", std::regex::icase))))
            return regex_type::RESERVED_WORD;
        else if (std::regex_match(this->text, std::regex("(-?[0-9]+([.][0-9]+)?)|([(][ ]*-[0-9]+([.][0-9]+)?[ ]*[)])")))
            return regex_type::NUMBER;
        else
            return regex_type::UNKNOWN;
    }
}