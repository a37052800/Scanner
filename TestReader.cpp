#include "uselib.h"
#include "Reader.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Reader reader("test.txt");
    while(!reader.eof())
    {
        cout<<reader.peek();
        cout<<reader.getChar()<<reader.currIndex()<<'\n';
        cout<<reader.getLine()<<reader.currLine()<<'\n';
    }
    return 0;
}
