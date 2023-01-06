#include "uselib.h"
#include "Scanner.h"
#include <map>
#include <cctype>

using namespace std;

ostream &operator<<(ostream &os, const token_type &type)
{
    os << token_type_toString(type);
    return os;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        cout << "Please enter the correct argument\n";
        return 1;
    }
    string filename(argv[1]);
    Scanner scanner = Scanner(filename);
    ofstream fout("scanner_" + filename);
    if (fout.fail())
    {
        cout << "Create output file fail\n";
        return 1;
    }
    map<string, int> token_list[16];
    int count = 0;
    while (!scanner.reader.eof())
    {
        Token token = scanner.getToken();
        if (token.getType() != token_type::NUL)
        {
            fout << ++count << ".\ttoken " << token.getText() << " belongs to " << token.getType() << '\n';
            if (token_list[(int)token.getType()].find(token.getText()) == token_list[(int)token.getType()].end())
                token_list[(int)token.getType()][token.getText()] = 1;
            else
                token_list[(int)token.getType()][token.getText()] += 1;
        }
    }
    fout << "\nTotal: " << count << " tokens\n";
    for (int i = 0; i < 16; i++)
    {
        int size = token_list[i].size();
        if (size == 0)
            continue;
        string type_name = token_type_toString((token_type)i);
        type_name[0] = toupper(type_name[0]);
        fout << '\n'
             << type_name << ": " << size << '\n';
        for(const auto& element :token_list[i])
        {
            fout<<element.first;
            if(element.second>1)
                fout<<" (x"<<element.second<<")";
            fout<<'\n';
        }
    }
    fout.close();
    return 0;
}
