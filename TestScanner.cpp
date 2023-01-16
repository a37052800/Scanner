#include "uselib.h"
#include "Scanner.h"

using namespace std;

ostream &operator<<(ostream &os, const token_type &type)
{
    os << token_type_toString(type);
    return os;
}

int main(int argc, char const *argv[])
{
    Scanner scanner = Scanner("test.txt");
    /* // delete state test
    while (true)
    {
        scanner.setState(new NORMAL_STATE());
        cout<<'.';
    }
    */
    int i = 1;
    while (!scanner.reader.eof())
    {
        Token token = scanner.getToken();
        if (token.getType() != token_type::NUL)
            cout << i++ << ". " << token.getText() << " | " << token.getType() << '\n';
    }
    return 0;
}
