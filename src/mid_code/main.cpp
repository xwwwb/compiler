#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 定义操作符枚举类型
enum Operator {
    ADD,// 加法运算
    SUB,// 减法运算
    MUL,// 乘法运算
    DIV // 除法运算
};

// 定义四元组结构体
struct Quadruple {
    Operator op;  // 操作符
    string arg1;  // 第一个操作数
    string arg2;  // 第二个操作数
    string result;// 结果变量
};

class IntermediateCodeGenerator {
public:
    // 构造函数
    IntermediateCodeGenerator() : nextTemp(0), nextLabel(0) {}

    // 生成四元式
    vector<Quadruple> generate(const string &expression) {
        // 将表达式分解为一系列记号
        vector<string> tokens = tokenize(expression);

        // 初始化语法树节点栈和运算符栈
        vector<string> nodeStack;
        vector<Operator> operatorStack;

        // 遍历记号序列
        for (size_t i = 0; i < tokens.size(); ++i) {
            const string &token = tokens[i];

            if (isOperator(token)) {// 如果当前记号是运算符
                Operator op = getOperator(token);
                while (!operatorStack.empty() && comparePrecedence(op, operatorStack.back()) <= 0) {
                    // 弹出运算符栈顶的元素，生成一个新的语法树节点，并将其压入语法树节点栈
                    Operator opTop = operatorStack.back();
                    operatorStack.pop_back();

                    string arg2 = nodeStack.back();
                    nodeStack.pop_back();

                    string arg1 = nodeStack.back();
                    nodeStack.pop_back();

                    string result = newTemp();
                    nodeStack.push_back(result);

                    Quadruple quadruple;
                    quadruple.op = opTop;
                    quadruple.arg1 = arg1;
                    quadruple.arg2 = arg2;
                    quadruple.result = result;

                    quads.push_back(quadruple);
                }
                // 将当前运算符压入运算符栈
                operatorStack.push_back(op);
            } else {// 如果当前记号是操作数
                nodeStack.push_back(token);
            }
        }
        // 处理剩余的运算符
        while (!operatorStack.empty()) {
            Operator op = operatorStack.back();
            operatorStack.pop_back();

            string arg2 = nodeStack.back();
            nodeStack.pop_back();

            string arg1 = nodeStack.back();
            nodeStack.pop_back();

            string result = newTemp();
            nodeStack.push_back(result);

            Quadruple quadruple;
            quadruple.op = op;
            quadruple.arg1 = arg1;
            quadruple.arg2 = arg2;
            quadruple.result = result;

            quads.push_back(quadruple);
        }

        return quads;
    }

private:
    int nextTemp;           // 下一个临时变量
    int nextLabel;          // 下一个标签
    vector<Quadruple> quads;// 生成的四元式序列

    // 将表达式分解为一系列记号
    vector<string> tokenize(const string &expression) {
        vector<string> tokens;
        size_t pos = 0;

        while (pos < expression.size()) {
            if (isspace(expression[pos])) {
                // 跳过空格字符
                ++pos;
            } else if (isdigit(expression[pos])) {
                // 处理数字
                size_t endPos = pos + 1;
                while (endPos < expression.size() && isdigit(expression[endPos])) {
                    ++endPos;
                }
                string token = expression.substr(pos, endPos - pos);
                tokens.push_back(token);
                pos = endPos;
            } else {
                // 处理运算符
                tokens.push_back(string(1, expression[pos]));
                ++pos;
            }
        }

        return tokens;
    }

    // 判断一个记号是否是运算符
    bool isOperator(const string &token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }
    // 获取一个运算符的优先级
    int getPrecedence(Operator op) {
        if (op == ADD || op == SUB) {
            return 1;
        } else {
            return 2;
        }
    }

    // 比较两个运算符的优先级
    int comparePrecedence(Operator op1, Operator op2) {
        return getPrecedence(op1) - getPrecedence(op2);
    }

    // 获取一个运算符对应的枚举值
    Operator getOperator(const string &token) {
        if (token == "+") {
            return ADD;
        } else if (token == "-") {
            return SUB;
        } else if (token == "*") {
            return MUL;
        } else {
            return DIV;
        }
    }

    // 生成一个新的临时变量
    string newTemp() {
        string temp = "t" + to_string(nextTemp++);
        return temp;
    }
};
string operatorToString(Operator op) {
    switch (op) {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case MUL:
            return "*";
        case DIV:
            return "/";
        default:
            throw runtime_error("Invalid operator");
    }
}

int main() {
    IntermediateCodeGenerator generator;
    vector<Quadruple> quads = generator.generate("3 + 4 * 5 - 6 / 2");
    // 输出生成的四元式序列
    for (const auto &quad: quads) {
        cout << "(" << operatorToString(quad.op) << ", " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")" << endl;
    }

    return 0;
}