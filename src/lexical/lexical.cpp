#include "lexical.h"
#include <fstream>
#include <string>
#include <utility>

Lexical::Lexical(string INPUT_FILENAME)
    : INPUT_FILENAME(std::move(INPUT_FILENAME)) {
  categorize = new map<string, vector<int>>();
  pointer = 0;
  length = 0;
  init();
}
Lexical::~Lexical() { delete categorize; }

// 初始化
void Lexical::init() const {
  // 读取种别码文件 存入categorizes
  vector<string> categorizes = readFile("../src/lexical/categorize_code.csv");
  for (auto temp : categorizes) {
    vector<string> result_ = split(temp);
    categorize->insert(pair<string, vector<int>>(
        result_[0], {stoi(result_[1]), stoi(result_[2])}));
  }
  // 还有逗号是界符
  categorize->insert(pair<string, vector<int>>(",", {65, 1}));
}
// 基于逗号分割
vector<string> Lexical::split(string &str) {
  vector<string> result;
  while (str.find(',') != string::npos) {
    size_t index = str.find(',');
    result.push_back(str.substr(0, index));
    str = str.substr(index + 1);
  }
  result.push_back(str);
  return result;
}
// 按行读取文件
vector<string> Lexical::readFile(const string &filename) {
  ifstream fileReader;
  fileReader.open(filename);
  vector<string> line; // 存入每一行
  string temp;         // 每行的临时变量
  while (getline(fileReader, temp)) {
    line.push_back(temp);
  }
  return line;
}

// 初始化读取器
void Lexical::reader_init() {
  vector<string> code_lines = readFile(INPUT_FILENAME);
  for (const auto &line : code_lines) {
    code = code + line + "\n";
  }
  length = code.size();
}

void Lexical::reader() {
  reader_init();
  while (pointer < length) {
    int result_ = read_next();
    if (result_ == EOF_) {
      break;
    }
  }
}
int Lexical::judge(char ch) {
  if (isLetter(ch)) {
    current_code = ch;
    char next = get_next();

    while (isLetter(next) || isDigit(next)) {
      current_code += next;
      pointer++;
      next = get_next();
    }
    return isKeyword(current_code) ? KEYWORD_ : CHARACTER_;
  }
  if (isDelimiters(ch)) {
    current_code = ch;
    return DELIMITERS_;
  }
  if (ch == ' ' || ch == '\t' || ch == '\n') {
    return SPACE_;
  }
  if (ch == '/') {
    char next = get_next();
    if (next == '/') {
      pointer += 2; // 跳过双斜杠
      // 表明当前是注释，读到下一个换行符为止 并保存进current_code
      while (get_next() != '\n') {
        current_code += code[pointer];
        pointer++;
      }
      current_code += code[pointer];

      return COMMENTS_;
    }
  }
  return UNDEFINED_;
}

// 返回值有 0 正常 __EOF__ 文件结束 __UNDEFINED__ 未定义
int Lexical::read_next() {
  int type = judge(code[pointer]);
  if (pointer >= length) {
    return EOF_;
  }
  if (type == COMMENTS_) {
    result.emplace_back(current_code, current_code, COMMENTS_);
    current_code = "";
    pointer++;
    return 0;
  }
  if (type == SPACE_ && pointer < length) {
    pointer++;
    type = judge(code[pointer]);
  }
  if (type == KEYWORD_) {
    result.emplace_back(current_code,
                        to_string(categorize->at(current_code)[0]), KEYWORD_);
    current_code = "";
    pointer++;
    return 0;
  }
  if (type == CHARACTER_) {
    result.emplace_back(current_code, current_code, CHARACTER_);
    current_code = "";
    pointer++;
    return 0;
  }
  if (type == DELIMITERS_) {
    result.emplace_back(
        current_code, to_string(categorize->at(current_code)[0]), DELIMITERS_);
    current_code = "";
    pointer++;
    return 0;
  }
  if (type == UNDEFINED_) {
    current_code = "";
    pointer++;
    return 0;
  }
  return 0;
}

char Lexical::get_next() {
  if (pointer + 1 < length) {
    return code[pointer + 1];
  }
  return '\0';
}
bool Lexical::isLetter(char ch) {
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return true;
  }
  return false;
}

bool Lexical::isDigit(char ch) {
  if (ch >= '0' && ch <= '9') {
    return true;
  }
  return false;
}
bool Lexical::isKeyword(const string &str) const {
  // str在categorize中 且value[1]为0 则为关键字
  if (categorize->count(str) > 0) {
    return categorize->at(str)[1] == 0;
  }
  return false;
}
bool Lexical::isDelimiters(char ch) const {
  string str = string(1, ch);
  if (categorize->count(str) > 0) {
    return categorize->at(str)[1] == 1;
  }
  return false;
}
