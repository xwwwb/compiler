#include "lexical.h"
#include<iostream>
#include<fstream>
#include<string>

void Lexical::init(){
    // 读取种别码文件 存入categorize
    ifstream fileReader;
    fileReader.open("categorize.csv");
    vector<string> line; // 存入每一行
    string temp; // 每行的临时变量
    while(getline(fileReader, temp)){
        line.push_back(temp);
    }
}