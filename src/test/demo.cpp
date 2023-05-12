#include <iostream>
#include <string>
using namespace std;

//string testCode = "a";
//string testCode = "(a,a)";
string testCode = "(a,^,a)";
int pointer = 0;
string sym;
void scaner() {
    if (pointer >= testCode.length()) {
        sym = "$";// 结束符
        return;
    }
    sym = testCode[pointer];
    pointer++;
}

void S();
void T();
void T_();
void error();
int main() {
    scaner();
    S();
    if (sym == "$") {
        cout << "success" << endl;
    } else {
        cout << "error" << endl;
    }
}

void S() {
    if (sym == "a" || sym == "^") {
        scaner();
    } else if (sym == "(") {
        scaner();
        T();
        if (sym == ")") {
            scaner();
        } else {
            error();
        }
    } else {
        error();
    }
}

void T() {
    S();
    T_();
}

void T_() {
    if (sym == ",") {
        scaner();
        S();
        T_();
    } else if (sym != ")") {
        error();
    }
}
void error(){
    cout << "paser_error" << endl;
    exit(0);
}