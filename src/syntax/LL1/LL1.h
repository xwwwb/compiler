#ifndef COMPILER_LL1_H
#define COMPILER_LL1_H
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// 文法
class Grammar {
public:
    string Vn;        // 非终结符
    string Generate;  // 终结符
    bool isEmpty;     // ε
    bool isStart;     // 非终结符是不是开始符号
    string production;// 产生式 在构造函数中计算 方便打印
    bool error;       // 实际上是表格中的空白 也就是error
    Grammar(string Vn, string Generate, bool empty = false, bool start = false) {
        this->Vn = Vn;
        this->Generate = Generate;
        this->isEmpty = empty;
        this->isStart = start;
        if (empty) {
            this->production = Vn + "->" + "ε";
        } else {
            this->production = Vn + "->" + Generate;
        }
        error = false;
    };

    Grammar(bool error) {
        this->error = error;
    }
};
typedef vector<Grammar *> GrammarList;
class LL1 {
public:
    explicit LL1(GrammarList *list);// 构造函数中要做构造预测分析表
    ~LL1();                         // 清除堆内存
    int Parser(string input);
    stack<string>* analyse;     // 分析栈
    void output_analyse(stack<string>  analyse,string input );
    GrammarList *grammars;     // 储存导入的文法
    vector<string> *VnList;    // 储存非终结符集，第一个为开始符号
    vector<string> *VtList;    // 储存终结符集
    vector<string> *VnAndVt;   // 所有符号
    vector<GrammarList *> *PPT;// predictive parsing table 预测分析表
private:
    void CalcVnVt();
    vector<string> CalcFirst(string word);
    vector<string> CalcFollow(string word);
    vector<set<string>> FIRST;
    vector<set<string>> FOLLOW;
};


#endif
