// 定义词法分析器的类
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// 为了开发阶段方便区分
enum {
  KEYWORD_,    // 关键字
  DELIMITERS_, // 界符
  OPERATOR_,   // 操作符
  CHARACTER_,  // 标识符
  COMMENTS_,   // 注释 种别码 -1
  NUMBERS_,    // 数字
  TEXTS_,      // 字符常量
  ERROR_,      // 错误
  SPACE_,      // 空格
};

static string name_[9] = {"关键字", "界符", "操作符", "标识符", "注释", "数字", "字符常量", "错误", "空格"};

const int EOF_ = -1;
const int UNDEFINED_ = -2;

class Token {
public:
  string value; // 值
  string code;  // 种别码
  int category; // 对应ENUM的位置
  Token(string value, string code, int category)
      : value(std::move(value)), code(std::move(code)), category(category) {}
  friend ostream &operator<<(ostream &os, const Token &token) {
    os << "值：" << token.value << " 种别码：" << token.code << " 分类："
       <<  name_[token.category]  << endl;
    return os;
  }
};

class Lexical {
public:
  explicit Lexical(string INPUT_FILENAME);
  ~Lexical();
  void init() const; // 初始化 读入文件和存入种别码
  static vector<string>
  readFile(const string &filename); // 按行读取文件返回字符串列表
  static vector<string> split(string &str);
  void reader();      // 读取代码文件
  void reader_init(); // 初始化读取器
  int read_next();    // 往后推进
  int judge(char ch); // 判断器
  char get_next();    // 获取下一个字符
  // 保存完整的code
  string code;
  // 保存当前没有读到尾的code
  string current_code;
  // 代码长度
  size_t length;
  // 当前位置
  size_t pointer;
  // 保存种别码和分类
  map<string, vector<int>> *categorize;
  // 需要读取的文件名字
  string INPUT_FILENAME;
  // 保存最终结果
  vector<Token> result;

  // 判断函数
  static bool isLetter(char ch);
  static bool isDigit(char ch);
  bool isKeyword(const string &str) const;
  bool isDelimiters(char ch) const;
};