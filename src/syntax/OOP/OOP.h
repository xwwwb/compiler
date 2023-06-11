#ifndef OOPHeader
#define OOPHeader
#include <map>
#include <stack>
#include <string>
#include <vector>
enum Relation {
    LessThan = 0,
    GreaterThan,
    Equal,
    Error
};

using namespace std;
class OOP {
public:
    OOP();
    // 以下两个值先使用手动传入 后来在开发程序计算FIRSTVT和LASTVT
    map<string, int> *terminator;   // 存放所有非终结符 也用于构建算符优先表
    vector<vector<Relation>> *table;// 算符优先表
    int parse(string token);
    bool isTerminator(string token) const;
    Relation getRelation(string a, string b) const;
};


#endif