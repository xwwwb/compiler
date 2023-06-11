#include "OOP.h"
OOP::OOP() {
    // 构造函数先置空
    terminator = new map<string, int>();
    table = new vector<vector<Relation>>();
}
int OOP::parse(string token) {
    // 在这里进行语法分析
    // 按照课本79页构造算法
    int K, j;
    string a, Q;
    token = token + "#";           // 初始化短语
    auto *S = new vector<string>();// 初始化分析栈
    S->push_back("#");
    K = 1;// 这里的K为以1为下标的写法
    bool flag = true;
    while (true) {
        if (flag) {// 当前符号读入a
            a = token.front();
            token = token.substr(1);
            if (isTerminator(S->at(K - 1))) {
                j = K;
            } else {
                j = K - 1;
            }
        }
        flag = true;
        // 开始判断当前栈顶符号和当前输入符号的关系 注意j - 1
        Relation rel = getRelation(S->at(j - 1), a);
        if (rel != GreaterThan) {
            if (rel == LessThan || rel == Equal) {
                S->push_back(a);
                K++;
                continue;
            } else {
                // 出错
                return -1;
            }
        }
        // 开始规约
        Relation rel2;
        do {
            Q.push_back(S->at(j - 1)[0]);
            j = j - 1;
            if (!isTerminator(S->at(j - 1))) {
                j = j - 1;
            }
            rel2 = getRelation(S->at(j - 1), Q);
        } while (rel2 != LessThan);
        // 输出规约产生式
        K = j + 1;
        S->at(K - 1) = "N";
        // 判断是否结束
        if (K == 2 && a == "#") {
            return 0;
        } else {
            flag = false;
        }
    }
}
bool OOP::isTerminator(string token) const {
    if (token == "#") {
        return true;
    }
    if (terminator->find(token) != terminator->end()) {
        return true;
    }
    return false;
}
Relation OOP::getRelation(string a, string b) const {
    if (a == "#") {
        return LessThan;
    }
    if(b == "#"){
        return GreaterThan;
    }
    return table->at((*terminator)[a])[(*terminator)[b]];
}
