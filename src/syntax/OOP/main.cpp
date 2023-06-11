#include "OOP.h"
#include <iostream>
using namespace std;
int main() {
    OOP *oop = new OOP();
    // 暂时手动填入 算法运行成功后使用计算
    oop->terminator->insert(pair<string, int>("+", 0));
    oop->terminator->insert(pair<string, int>("*", 1));
    oop->terminator->insert(pair<string, int>("i", 2));
    oop->terminator->insert(pair<string, int>("(", 3));
    oop->terminator->insert(pair<string, int>(")", 4));
    // 填入表格
    vector<Relation> line1 = {GreaterThan, LessThan, LessThan, LessThan, GreaterThan};
    vector<Relation> line2 = {GreaterThan, GreaterThan, LessThan, LessThan, GreaterThan};
    vector<Relation> line3 = {GreaterThan, GreaterThan, Error, Error, GreaterThan};
    vector<Relation> line4 = {LessThan, LessThan, LessThan, LessThan, Equal};
    vector<Relation> line5 = {GreaterThan, GreaterThan, Error, Error, GreaterThan};
    oop->table->push_back(line1);
    oop->table->push_back(line2);
    oop->table->push_back(line3);
    oop->table->push_back(line4);
    oop->table->push_back(line5);
    // 到此为止 完成预测分析表的构建
    // 分析 i + i * i
    cout << oop->parse("i+i*i") << endl;
    cout << oop->parse("i*i+i") << endl;
    cout << oop->parse("i+i") << endl;

    return 0;
}