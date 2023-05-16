#ifndef COMPILER_LL1_H
#define COMPILER_LL1_H
#include <string>
using namespace std;
class Grammar{
public:
    string Vn; // 非终结符
    string Vt; // 终结符
    bool isEmpty; // ε
    bool isStart; // 非终结符是不是开始符号
    string production; // 产生式 在构造函数中计算 方便打印
    Grammar(string Vn, string Vt){
        this->Vn = Vn;
        this->Vt = Vt;
        this->isEmpty = false;
        this->isStart = false;
        this->production = Vn + "->" + Vt;
    }
};


class LL1 {
};


#endif
