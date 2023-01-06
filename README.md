# Scanner

>編譯程式期末專題
>developed in c++ by Tim

## 輸入輸出格式

+ 輸入一般 .txt 文字檔，於執行時輸入檔名
+ 輸出 .txt 文字檔，若輸入檔名為test.txt，輸出檔名為scanner_test.txt

## 使用說明

```Shell
g++ Scanner.cpp -o Scanner
./Scanner {filename}.txt
cat scanner_{filename}.txt | less
```

## 程式架構

#### Scanner.h

```c++
class Scanner_State;
class NORMAL_STATE : public Scanner_State;
class IDENTIFY_STATE : public Scanner_State;
class DEFINE_STATE : public Scanner_State;
class SLASH_STATE : public Scanner_State;
class CHARACTER_STATE : public Scanner_State;
class INCLUDE_STATE : public Scanner_State;
class PRINT_STATE : public Scanner_State;
class POINTER_STATE : public Scanner_State;
class SKIP_STATE : public Scanner_State;
class NUMBER_STATE : public Scanner_State;
class EOF_STATE : public Scanner_State;
```

#### Reader.h

...

#### Token.h

...

#### Regex_Checker.h

...

#### Symbol_Table.h

...

#### 單元測試檔

Test{classname}.cpp

## Token判斷方式

```c++
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
```
