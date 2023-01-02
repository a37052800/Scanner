#include "uselib.h"

class Reader
{
private:
    bool is_eof;
    int current_index;
    int current_line;
    std::string file_content;

public:
    Reader(std::string filename) {this->loadFile(filename);};
    Reader();
    ~Reader(){};
    void loadFile(std::string filename);
    bool eof() { return this->is_eof; };
    char peek();
    void back();
    char getChar();
    std::string getLine();
    int currLine() { return this->current_line; };
    int currIndex() { return this->current_index; };
};

Reader::Reader()
{
    this->is_eof = true;
    this->file_content.clear();
}
void Reader::loadFile(std::string filename)
{
    std::ifstream in(filename);
    std::stringstream ss;
    if (in.fail())
    {
        std::cout << "load file fail\n";
        exit(1);
    }
    else
    {
        ss << in.rdbuf();
        this->file_content = ss.str();
        this->current_index = 0;
        this->current_line = 1;
        this->is_eof = false;
    }
    in.close();
}
char Reader::peek()
{
    if (this->is_eof)
        return EOF;
    if (this->file_content[this->current_index] == '\0')
        return '$';
    return this->file_content[this->current_index];
}
void Reader::back()
{
    if (this->file_content[--this->current_index]=='\n')
        this->current_line--;
}
char Reader::getChar()
{
    if (!this->is_eof)
    {
        char c = this->file_content[this->current_index++];
        if (c == '\n')
            this->current_line++;
        if (c == '\0')
        {
            this->is_eof = true;
            return '$';
        }
        return c;
    }
    return '$';
}
std::string Reader::getLine()
{
    std::string str;
    while (!this->is_eof)
    {
        if ((this->peek() == '\n') || (this->peek() == '$'))
        {
            this->getChar();
            break;
        }
        str += this->getChar();
    }
    return str;
}
