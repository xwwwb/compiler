// 递归下降分析法

#include <iostream>
#include <string>
using namespace std;

string testCode1 = "a";
string testCode2 = "(a,a)";
string testCode3 = "(a,^,a)";
string testCode4 = "abae";
string testCode5 = "aa";
string testCode6 = "addde";

void error();
void scaner();

void Lex_1_S();
void Lex_1_T();
void Lex_1_T_();

void Lex_2_S();
void Lex_2_A();
void Lex_2_B();

string testCode = testCode6;
int pointer = 0;
string sym;

int main() {
    scaner();
    //    Lex_1_S(); // 文法1
    Lex_2_S();// 文法2
    if (sym == "$") {
        cout << "success" << endl;
    } else {
        cout << "error" << endl;
    }
}

// 文法1 开始
void Lex_1_S() {
    if (sym == "a" || sym == "^") {
        scaner();
    } else if (sym == "(") {
        scaner();
        Lex_1_T();
        if (sym == ")") {
            scaner();
        } else {
            error();
        }
    } else {
        error();
    }
}
void Lex_1_T() {
    Lex_1_S();
    Lex_1_T_();
}
void Lex_1_T_() {
    if (sym == ",") {
        scaner();
        Lex_1_S();
        Lex_1_T_();
    } else if (sym != ")") {
        error();
    }
}
// 文法1 结束

// 文法2 开始
void Lex_2_S() {
    if (sym == "a") {
        scaner();
        Lex_2_A();
        Lex_2_B();
    } else {
        error();
    }
}
void Lex_2_A() {
    if (sym == "b") {
        scaner();
        Lex_2_B();
    } else if (sym == "d") {
        scaner();
        Lex_2_A();
    } else if (sym != "a" && sym != "e") {
        error();
    }
}

void Lex_2_B() {
    if (sym == "a") {
        scaner();
    } else if (sym == "e") {
        scaner();
    } else {
        error();
    }
}

// 工具函数
void error() {
    cout << "paser_error" << endl;
    exit(0);
}
void scaner() {
    if (pointer >= testCode.length()) {
        sym = "$";// 结束符
        return;
    }
    sym = testCode[pointer];
    pointer++;
}