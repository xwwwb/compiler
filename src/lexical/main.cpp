#include <iostream>
#include "lexical.h"
using namespace std;

// 需要扫描的代码文本
//const string INPUT_FILENAME = "../src/lexical/lexical.cpp";
const string INPUT_FILENAME = "../src/lexical/code.cpp";

int main(){
    auto *lexical = new Lexical(INPUT_FILENAME);
    lexical->reader();
    lexical->show_result();
    return 0;
}