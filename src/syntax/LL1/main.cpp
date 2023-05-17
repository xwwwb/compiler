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
    if (G1Parser->Parser("(a,a)") == 0) {
        cout << "解析成功" << endl;
    }
    if (G1Parser->Parser("(a,^,a)") == 0) {
        cout << "解析成功" << endl;
    }
    if (G1Parser->Parser("a") == 0) {
        cout << "解析成功" << endl;
    }

    auto *G2 = new GrammarList();
    G2->push_back(new Grammar("S", "aAB", false, true));
    G2->push_back(new Grammar("A", "bB", false, false));
    G2->push_back(new Grammar("A", "dA", false, false));
    G2->push_back(new Grammar("A", "", true, false));
    G2->push_back(new Grammar("B", "a", false, false));
    G2->push_back(new Grammar("B", "e", false, false));
    LL1 *G2Parser = new LL1(G2);
    if (G2Parser->Parser("abae") == 0) {
        cout << "解析成功" << endl;
    }
    if (G2Parser->Parser("aa") == 0) {
        cout << "解析成功" << endl;
    }

    return 0;
}