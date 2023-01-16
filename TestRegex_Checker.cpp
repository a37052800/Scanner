#include "uselib.h"
#include "Regex_Checker.h"
using namespace std;
int main()
{
    while (true)
    {
        cout<<">> ";
        string str;
        getline(cin, str);
        if (str == "exit")
            break;
        if (Regex_Checker::regMatch(str, "[- .0-9()]", 1))
            cout << "True\n";
        else
            cout << "False\n";
        switch (Regex_Checker(str).getType())
        {
        case regex_type::ALPHA:
            cout << "alpha\n";
            break;
        case regex_type::NUMBER:
            cout << "number\n";
            break;
        case regex_type::IGNORE:
            cout << "ignore\n";
            break;
        case regex_type::UNDERLINE:
            cout << "underline\n";
            break;
        case regex_type::MARK:
            cout << "mark\n";
            break;
        case regex_type::RESERVED_WORD:
            cout << "reserved word\n";
            break;
        case regex_type::UNKNOWN:
            cout << "unknown\n";
            break;
        }
    }
}