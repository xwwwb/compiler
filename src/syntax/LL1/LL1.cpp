#include "LL1.h"

LL1::LL1(GrammarList *list) {
    VnList = new vector<string>();
    VtList = new vector<string>();
    VnAndVt = new vector<string>();
    analyse = new stack<string>();
    PPT = new vector<GrammarList *>();
    grammars = list;
    // 计算出Vn和Vt
    CalcVnVt();
    // 执行预测分析表的构建
    // 一、计算每一个非终结符的FIRST和FOLLOW
    // 1. 对每一文法符号X属于Vn并Vt，计算FIRST(X)

    // 计算FIRST集合
    for (auto word: *VnAndVt) {
        vector<string> temp = CalcFirst(word);
        set<string> temp_(temp.begin(), temp.end());
        FIRST.push_back(temp_);
    }
    // 计算FOLLOW集合
    for (auto word: *VnList) {
        vector<string> temp = CalcFollow(word);
        set<string> temp_(temp.begin(), temp.end());
        FOLLOW.push_back(temp_);
    }
    // 构造预测分析表 全error
    for (auto i: *VnList) {
        auto temp = new GrammarList();
        for (auto j: *VtList) {
            auto temp_ = new Grammar(true);
            temp->push_back(temp_);
        }
        temp->push_back(new Grammar(true));// #
        PPT->push_back(temp);
    }
    for (auto item: *grammars) {
        // 拿到非终结符
        string vn = item->Vn;
        string generator = item->Generate;

        auto it = std::find(VnList->begin(), VnList->end(), vn);
        int index_vn = distance(VnList->begin(), it);
        if (item->isEmpty) {
            set<string> followList = FOLLOW[index_vn];
            for (auto word: followList) {
                if (word == "#") {
                    PPT->at(index_vn)->back() = item;
                } else {
                    it = std::find(VtList->begin(), VtList->end(), *followList.begin());
                    int index_ = distance(VtList->begin(), it);
                    PPT->at(index_vn)->at(index_) = item;
                }
            }
            continue;
        }
        // 对产生式 逐字分析
        for (int i = 0; i < generator.length(); i++) {
            string temp(1, generator[i]);
            auto it = std::find(VnAndVt->begin(), VnAndVt->end(), temp);
            int index = distance(VnAndVt->begin(), it);
            set<string> firstList = FIRST[index];

            if (firstList.size() == 1 && *firstList.begin() != "ε") {
                it = std::find(VtList->begin(), VtList->end(), *firstList.begin());
                int index_ = distance(VtList->begin(), it);
                PPT->at(index_vn)->at(index_) = item;
                break;
            }
            int countε = 0;
            for (auto word: firstList) {
                if (word != "ε") {
                    it = std::find(VtList->begin(), VtList->end(), word);
                    int index_ = distance(VtList->begin(), it);
                    PPT->at(index_vn)->at(index_) = item;
                } else {
                    countε++;
                    // 找Follow集
                    set<string> followList = FOLLOW[index_vn];
                    for (auto word: followList) {
                        if (word == "#") {
                            PPT->at(index_vn)->back() = item;
                        } else {
                            it = std::find(VtList->begin(), VtList->end(), *followList.begin());
                            int index_ = distance(VtList->begin(), it);
                            PPT->at(index_vn)->at(index_) = item;
                        }
                    }
                }
            }
            if (countε == 0) {
                break;
            }
        }
    }

    // 执行打印输出
    cout << "FIRST集合" << endl;
    for (int i = 0; i < VnList->size(); i++) {
        cout << VnList->at(i) << ":\t";
        for (auto item: FIRST.at(i)) {
            cout << item << " ";
        }
        cout << endl;
    }
    cout << "FOLLOW集合" << endl;
    for (int i = 0; i < VnList->size(); i++) {
        cout << VnList->at(i) << ":\t";
        for (auto item: FOLLOW.at(i)) {
            cout << item << " ";
        }
        cout << endl;
    }
    // 打印输出预测分析表
    cout << "预测分析表\nVn\t";
    for (auto i: *VtList) {
        cout << "|" << i << "\t";
    }
    cout << "|#" << endl;
    for (int i = 0; i < PPT->size(); i++) {
        cout << VnList->at(i) << "\t";
        for (int j = 0; j < PPT->at(i)->size(); j++) {
            cout << "|" << PPT->at(i)->at(j)->production << "\t";
        }
        cout << endl;
    }
}

LL1::~LL1() {
    // 清除堆内存
    for (auto item: *grammars) {
        delete item;
    }
    delete grammars;
    delete VnList;
    delete VtList;
    for (auto item: *PPT) {
        for (auto i: *item) {
            delete i;
        }
        delete item;
    }
    delete PPT;
}
void LL1::CalcVnVt() {
    // 计算所有Vn
    for (auto item: *grammars) {
        auto it = find(VnList->begin(), VnList->end(), item->Vn);
        if (it != VnList->end()) {
            // 已经存在了非终结符
            continue;
        } else {
            // 不存在 放入
            if (item->isStart) {
                // 开始符号放到最前面
                VnList->insert(VnList->begin(), item->Vn);
            } else {
                VnList->push_back(item->Vn);
            }
        }
    }
    // 计算所有Vt
    for (auto item: *grammars) {
        auto it = find(VnList->begin(), VnList->end(), item->Generate);
        if (it != VnList->end()) {
            // 已经存在了终结符
            continue;
        } else {
            // 不存在 放入
            // 将产生式分解 依次分析放入
            if (item->isEmpty) {
                // VtList->push_back("ε");
                continue;
            }
            for (auto word: item->Generate) {
                string temp(1, word);
                auto it = find(VnList->begin(), VnList->end(), temp);
                if (it == VnList->end()) {
                    VtList->push_back(temp);
                }
            }
        }
    }
    // 计算出VnAndVt
    VnAndVt->insert(VnAndVt->end(), VnList->begin(), VnList->end());
    VnAndVt->insert(VnAndVt->end(), VtList->begin(), VtList->end());
}

// 这里的FIRST计算 针对的是 X属于VnAndVt
vector<string> LL1::CalcFirst(string word) {
    vector<string> firstList;// word对应的first集
    auto it = std::find(VnList->begin(), VnList->end(), word);
    if (it != VnList->end()) {
        // 不在终结符中 即为非终结符
        bool haveε = false;// 主要是为了应对 S->ABC A->ε B->ε C->ε 时 S的First也要有一个空
        for (auto item: *grammars) {
            if (item->Vn == word) {
                // 如果item为空 则推入ε 对应课本c
                if (item->isEmpty) {
                    firstList.emplace_back("ε");
                    continue;// 分析下一个产生式
                }
                for (int index = 0; index < item->Generate.length(); index++) {
                    // 取到产生式第index个值
                    string a = string(1, item->Generate.at(index));
                    auto it = std::find(VtList->begin(), VtList->end(), a);
                    if (it != VtList->end()) {
                        // a为终结符 将a推入first集
                        // 对应课本b
                        firstList.push_back(a);
                        break;
                    } else {
                        // 如果进入到这个分支 说明a为非终结符
                        // 对这个新的非终结符求FIRST 如果列表只包含空 则继续扫描产生式下一个 如果只包含非终结符，则就求完了
                        // 如果既包括终结符又包括空，则也要继续扫描产生式下一个字符
                        // 对应课本d
                        vector<string> result = CalcFirst(a);
                        auto it = find(result.begin(), result.end(), "ε");
                        if (it == result.end()) {
                            // 没有空
                            firstList.insert(firstList.end(), result.begin(), result.end());// 将a的First放入word的first中
                            break;
                        } else {
                            // 有空
                            // 将非空的放入firstList
                            haveε = true;
                            for (auto r: result) {
                                if (r != "ε") {
                                    firstList.push_back(r);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (haveε) {
            auto it = find(firstList.begin(), firstList.end(), "ε");
            // 如果有X->ε 则
            if (it == firstList.end()) {
                firstList.emplace_back("ε");
            }
        }
    } else {
        // 终结符
        firstList.push_back(word);
    }
    return firstList;
}
vector<string> LL1::CalcFollow(string word) {
    vector<string> followList;
    if (VnList->at(0) == word) {
        // 如果是文法开始符号 将#加入follow中
        // 对应课本a
        followList.emplace_back("#");
    }
    // 遍历每一条文法 找到产生式含有word的
    for (auto item: *grammars) {
        if (item->isEmpty) continue;
        string generate = item->Generate;
        int pos = generate.find(word);
        int length = generate.length();
        if (pos == string::npos) continue;
        // 走到这里说明当前产生式含有非终结符
        if (pos != length - 1) {
            // 说明形如 A->αBβ 这里temp就是β
            string temp = generate.substr(pos + 1);
            vector<string> result = CalcFirst(temp);// 计算temp的First集
            int countε = 0;
            for (auto i: result) {
                if (i != "ε") {
                    followList.push_back(i);
                } else {
                    countε++;
                }
            }
            if (countε > 0) {
                // 将FollowA加入到FollowB中
                vector<string> followb = CalcFollow(item->Vn);// 当前产生式的非终结符
                followList.insert(followList.end(), followb.begin(), followb.end());
            }
        } else {
            // 说明形如 A->αB
            // 则FollowA=FollowB
            if (item->Vn == word) continue;
            followList = CalcFollow(item->Vn);// 当前产生式的非终结符
        }
    }
    return followList;
}
int LL1::Parser(string input) {
    cout << "开始对 " << input << " 进行预测分析" << endl;
    input += "#";
    analyse->push("#");
    analyse->push(VnList->front());
    int input_length = input.length();
    int input_pointer = 0;
    output_analyse(*analyse, input);
    while (true) {


        string X = analyse->top();
        analyse->pop();
        if (input_pointer >= input_length) return -1;
        string a = string(1, input[input_pointer]);
        auto it = std::find(VnList->begin(), VnList->end(), X);
        auto it_ = std::find(VtList->begin(), VtList->end(), a);

        if (it != VnList->end() || a == "#") {
            // top目前是非终结符
            if (X == "#") {
                if (X == a) {
                    return 0;
                } else {
                    return -1;
                }
            }
            int index = distance(VnList->begin(), it);
            int index_ = distance(VtList->begin(), it_);
            // 查找预测分析表
            auto g = PPT->at(index)->at(index_);
            if (g->error) {
                return -1;
            }
            // 将产生式逆序放入栈
            for (int i = g->Generate.size() - 1; i >= 0; i--) {
                analyse->push(string(1, g->Generate[i]));
            }
        } else {
            // top目前是终结符
            if (X == a) {
                input_pointer++;
            } else {
                return -1;
            }
        }
        output_analyse(*analyse, input.substr(input_pointer));
    }
}

void LL1::output_analyse(stack<string> analyse, string input) {
    string temp = "";
    while (!analyse.empty()) {
        temp = analyse.top()+temp;
        analyse.pop();
    }
    cout<<temp;
    cout << "\t";
    cout << input << endl;
}