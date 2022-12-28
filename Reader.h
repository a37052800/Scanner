#include "uselib.h"

class Reader
{
private:
    bool is_fail;
    bool is_eof;
    std::string file_content;

public:
    Reader(std::string filename);
    ~Reader();
    void loadFile(std::string filename);
    bool fail();
    bool eof();
    char peek();
    char getChar();
    std::string getLine();
};

Reader::Reader(std::string filename)
{
    this->loadFile(filename);
}

Reader::~Reader()
{
}

void Reader::loadFile(std::string filename)
{
    std::ifstream in;
    in.open(filename);
    if (in.fail())
        this->is_fail = true;
    else
    {
        while (in.eof())
        {
            char c;
            in.get(c);
            this->file_content+=c;
        }
    }
}

char Reader::peek()
{
}

char Reader::getChar()
{
}

std::string Reader::getLine()
{
}

bool Reader::eof()
{
}

bool Reader::fail()
{
}
