#include "lexical.h"
#include <fstream>
#include <string>
//#include <utility>

Lexical::Lexical(string input_filename) {
    code_file = std::move(input_filename);
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
    for (auto temp: categorizes) {
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
    vector<string> line;// 存入每一行
    string temp;        // 每行的临时变量
    while (getline(fileReader, temp)) {
#ifdef __APPLE__
        // 去除\r
        if (temp[temp.size() - 1] == '\r') {
            temp = temp.substr(0, temp.size() - 1);
        }
        line.push_back(temp);
#else
        line.push_back(temp);
#endif
    }
    return line;
}

// 初始化读取器
void Lexical::reader_init() {
    vector<string> code_lines = readFile(code_file);
    for (const auto &line: code_lines) {
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
        tempCode = ch;
        char next = get_next();

        while (isLetter(next) || isDigit(next)) {
            tempCode += next;
            pointer++;
            next = get_next();
        }
        return isKeyword(tempCode) ? KEYWORD_ : CHARACTER_;
    }
    if (isDelimiters(ch)) {
        // 有可能是界符 有可能是运算符 例如 <<
        char next = get_next();
        if (isDelimiters(next) || isOperator(string(1, next))){
            // 运算符
            tempCode = string(1, ch) + string(1, next);
            if (isOperator(tempCode)) {
                pointer += 1;// 为什么不和下面一样加2
                return OPERATOR_;
            } else {
                tempCode = ch;
                return DELIMITERS_;
            }
        }
        // 可能是常量
        if (ch == '\'' || ch == '\"') {
            // 推入一个引号
            tempCode = ch;
            result.emplace_back(tempCode,
                                to_string(categorize->at(tempCode)[0]), DELIMITERS_);
            tempCode = "";
            while (get_next() != ch) {
                pointer++;
                tempCode += code[pointer];
            }
            // 推入后 再推入一个引号
            return TEXTS_;
        }

        // 界符
        tempCode = ch;
        return DELIMITERS_;
    }
    if (isDigit(ch)) {
        tempCode = ch;
        char next = get_next();
        while (isDigit(next) || next == '.') {
            tempCode += next;
            pointer++;
            next = get_next();
        }
        return NUMBERS_;
    }

    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        return SPACE_;
    }
    if (ch == '/') {
        char next = get_next();
        if (next == '/') {
            pointer += 2;// 跳过双斜杠
            // 表明当前是注释，读到下一个换行符为止 并保存进tempCode
            while (get_next() != '\n') {
                tempCode += code[pointer];
                pointer++;
            }
            tempCode += code[pointer];
            // 去除首空格
            while (tempCode[0] == ' ') {
                tempCode = tempCode.substr(1, tempCode.size());
            }

            return COMMENTS_;
        }
    }
    if (isOperator(string(1, ch))) {
        // 下一个字符也是运算符
        char next = get_next();
        if(isOperator(string(1, next))){
            tempCode = string(1, ch) + string(1, next);
            pointer++;
            return OPERATOR_;
        }
        tempCode = ch;
        return OPERATOR_;
    }
    return UNDEFINED_;
}

// 返回值有 0 正常 __EOF__ 文件结束 __UNDEFINED__ 未定义
int Lexical::read_next() {
    int type = judge(code[pointer]);
    if (type == SPACE_ && pointer < length) {
        pointer++;
        type = judge(code[pointer]);
    }

    if (pointer >= length) {
        return EOF_;
    }
    if (type == COMMENTS_ || type == CHARACTER_ || type == NUMBERS_ ) {
        result.emplace_back(tempCode, tempCode, type);
        tempCode = "";
        pointer++;
        return 0;
    }
    if(type == TEXTS_){
        // 当前是引号
        Token token = result.back();
        result.emplace_back(tempCode, tempCode, type);
        result.emplace_back(token.value, token.value, DELIMITERS_);

        tempCode = "";
        pointer += 2;
        return 0;
    }

    if (type == KEYWORD_ || type == DELIMITERS_ || type == OPERATOR_) {
        result.emplace_back(tempCode,
                            to_string(categorize->at(tempCode)[0]), type);
        tempCode = "";
        pointer++;
        return 0;
    }

    if (type == UNDEFINED_) {
        tempCode = "";
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
bool Lexical::isOperator(const string &str) const {
    if (categorize->count(str) > 0) {
        return categorize->at(str)[1] == 2;
    }
    return false;
}
void Lexical::show_result() const {
    for (const auto& t : result) {
        cout << t;
    }
    cout << "共计" << result.size() << "个单词" << endl;
}
