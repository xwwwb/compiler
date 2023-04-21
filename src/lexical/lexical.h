// 定义词法分析器的类
#include<vector>
#include<string>
#include<map>
using namespace std;

// 分类
enum {
    __KEYWORD__, // 关键字
    __DELIMITERS__, // 界符
    __OPERATOR__, // 操作符
    __CHARACTER__, // 标识符
    __COMMENTS__, // 注释
    __NUMBERS__, // 数字
    __TEXTS__, // 字符常量
    __ERROR__, // 错误
    };

class Lexical {
public:
    Lexical();
    ~Lexical();
    void init(); // 初始化 读入文件和存入种别码

private:
    // 保存代码中的每一个字符
    vector<string> code;
    // 保存种别码和分类
    map<string,vector<int>[2]> categorize;

    // 需要读取的文件名字
    string INPUT_FILENAME;
};