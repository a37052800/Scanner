# Scanner

編譯程式期末專題
developed in c++ by Tim

---

## 輸入輸出格式

+ 輸入一般 .txt 文字檔，於執行時輸入檔名
+ 輸出 .txt 文字檔，若輸入檔名為test.txt，輸出檔名為scanner_test.txt

---

## 使用說明

+ In Windows

```Shell
g++ Scanner.cpp -o Scanner.exe
Scanner.exe [{Path}//]{filename}.txt
cat [{Path}//]scanner_{filename}.txt | less
```

+ In Linux

```Shell
g++ Scanner.cpp -o Scanner
./Scanner [{Path}/]{filename}.txt
type [{Path}/]scanner_{filename}.txt
```

---

## Token類別

| 類別 |格式 | 說明 |
|-|-|-|
| Reserved_word | include main char int float if else elseif for while do return switch case printf scanf | 允許各式大小寫組合 |
| Library_name | <xxx.h> | 前面必須存在#include，否則視為undefined token |
| Identifier | char/int/float 有定義的變數 | 由英文字母、底線及數字組成，限定英文字母開頭，否則視為undefined token |
| Character | ‘*’ | ‘*’ 算1個token |
| Number | 整數/浮點數/負數 |
| 負數可以表示成-3或 (-3) | 算1個token |
| Pointer | *identifier |*identifier 算1個token |
| Bracket | ( ) [ ] { } | |
| Operator | + - */ % ^ ++ -- & |  = | |
| Comparator | = = < > <= >= != | |
| Address | &identifier (identifier必須已定義) | &identifier 算1個token |
| Punctuation | , ; : # “ ‘ | |
| Format_specifier | %d %f %c \* | |
| Printed_token | printf或scanf裡直接印出的token | 以空格區隔 |
| Comment | /*…*/ 或 // … | 不管內容，都算1個token |
| Undefined_token | | |
| Skipped_token | | 不管內容，都算1個token |
| NUL | | 空的token |

---

## 程式架構

#### Scanner.h

Scanner以FSM的概念做設計，分為11種狀態，Scanner處於不同狀態時皆由不同的token判別方式。

#### Reader.h

負責檔案的讀取。

#### Regex_Checker.h

負責Regex判斷。

#### Symbol_Table.h

提供Symbol_Table物件。

#### Token.h

提供Token.h物件。

#### 單元測試檔

Test{classname}.cpp

---

## Token判斷方式

#### 1. NORMAL_STATE

若下個字元為`英文字母`切換狀態至**IDENTIFY_STATE**
若下個字元為`數字`切換狀態至**NUMBER_STATE**
若下個字元為`空格`、`換行`則讀取下一個字元
若下個字元為`$`切換狀態至**EOF_STATE**  `Note.待改進`
若下個字元為`([{`切換狀態至**NUMBER_STATE**
若下個字元為`)]}`返回*Bracket*
若下個字元為`*%^|`切換狀態至**NUMBER_STATE**並返回*Operator*
若下個字元為`+`則切換狀態至**NUMBER_STATE**
並讀取下個字元是否為`+`是則返回*Operator(++)* 否則返回*Operator(+)*
若下個字元為`-`則切換狀態至**NUMBER_STATE**
並讀取下個字元是否為`-`是則返回*Operator(--)* 否則返回*Operator(-)*
若下個字元為`=><!`則切換狀態至**NUMBER_STATE**
並讀取下個字元是否為`=`返回*Operator*
若下個字元為`/`切換狀態至**SLASH_STATE**
若下個字元為`*&`切換狀態至**POINTER_STATE**
若下個字元為`’`切換狀態至**CHARACTER_STATE**
若下個字元為`,:`切換狀態至**NUMBER_STATE**並返回*Punctuation*
若下個字元為`;”`返回*Punctuation*
若下個字元為`#`切換狀態至**INCLUDE_STATE**
若讀取到未定義之字元則切換狀態至**SKIP_STATE**並返回*Undefined_token*

#### 2. IDENTIFY_STATE

讀取符合`[A-Za-z0-9_]`之字元進入字串
若該字串屬於`Reserved_word`返回*Reserved_word*並切換狀態至**NORMAL_STATE**
判斷是否為`int` `float` `char`是則切換狀態至**DEFINE_STATE**
判斷是否為`printf` `scanf`是則切換狀態至**PRINT_STATE**
若該字串存在於[Symbol_Table]返回*Identifier*並切換狀態至**NORMAL_STATE**
切換狀態至**SKIP_STATE**並返回*Undefined_token*

#### 3. DEFINE_STATE

若遇到`Reserved_word` `；`切換狀態至**NORMAL_STATE**
若遇到`,`返回*Punctuation*
將符合規則的字串存入[Symbol_Table]並返回*Identifier*

#### 4. SLASH_STATE

切換狀態至**NORMAL_STATE**
判斷是否為`//` `/*`
是則返回*Comment*
否則切換狀態至**NUMBER_STATE**並返回*Operator*

#### 5. CHARACTER_STATE

切換狀態至**NORMAL_STATE**
判斷是否符合`'.'`
是則返回*Character*
否則返回*Punctuation*

#### 6. INCLUDE_STATE

切換狀態至**NORMAL_STATE**
若符合規則，分別返回*Punctuation* 、*Reserved_word* 、*Library_name*

#### 7. PRINT_STATE

若符合規則則讀取下個字元
若下個字元為`(`返回*Bracket*
若下個字元為`"`返回*Punctuation*
若下個字串為`%[dfc]`、`\[A-Za-z0-9\\\\]`返回*Format_specifier*
若下個字串為其他返回*Printed_token*
若不符合規則則切換狀態至**NORMAL_STATE**

#### 8. POINTER_STATE

切換狀態至**NORMAL_STATE**
若`*identifier`存在於[Symbol_Table]返回*Pointer*
若&identifie中的`identifie`存在於[Symbol_Table]返回*Address*
切換狀態至**NUMBER_STATE**並返回*Operator*

#### 9. SKIP_STATE

切換狀態至**NORMAL_STATE**
讀取整行並返回*Skipped_token*

#### 10. NUMBER_STATE

切換狀態至**NORMAL_STATE**
符合格式則返回*Number*

#### 11. EOF_STATE

返回*NUL*
