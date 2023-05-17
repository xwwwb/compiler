#include "LL1.h"
#include <iostream>

// LL1的流程
// 我在构造完LL1类时，传入所有文法
// 构造预测分析表
// 调用LL1的分析方法 传入待分析语句
// 进行分析

// 要求传入的文法必须是LL1文法 且不重复传入文法
int main() {
    auto *G1 = new GrammarList();
    G1->push_back(new Grammar("S", "a", false, true));
    G1->push_back(new Grammar("S", "^", false, true));
    G1->push_back(new Grammar("S", "(T)", false, true));
    G1->push_back(new Grammar("T", "SF"));
    G1->push_back(new Grammar("F", ",SF"));
    G1->push_back(new Grammar("F", "", true, false));

    LL1 *G1Parser = new LL1(G1);
    int result = G1Parser->Parser("(a,a)");
    if(result == 0){
        cout << "解析成功"<<endl;
    }
    return 0;
}