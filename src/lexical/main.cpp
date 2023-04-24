#include <iostream>
#include "lexical.h"
using namespace std;

// 需要扫描的代码文本
const string INPUT_FILENAME = "../src/lexical/code.txt";


int main(){
    auto *lexical = new Lexical(INPUT_FILENAME);
    lexical->reader();
    for (const auto& t : lexical->result) {
        cout << t;
    }
    return 0;
}