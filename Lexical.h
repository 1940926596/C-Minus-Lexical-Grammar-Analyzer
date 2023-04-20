//
// Created by 19409 on 2023/4/20.
//

#ifndef LEXICALDFA_LEXICAL_H
#define LEXICALDFA_LEXICAL_H
#include<string>
#include<unordered_map>
#include<fstream>
using namespace std;

enum State{
    START,INNUM,INID,INEQ,INNEQ,INLESS,INLGREAT,
    LCOMMENT,RCOMMENT,INCOMMENT,DONE
};

class Lexical {
public:
    //一词一码
    unordered_map<string,string> reserveWords;    //保留字
    unordered_map<string,string> identifierWords;    //特殊符号

    //多词一码，对应的属性值是自身的值
    string ID;  //  <ID,value=xxx>
    string NUM;  //  <NUM,value=xxx>
    State state;  //  当前词法分析器所处的状态
    string input;  //  当前词法分析器输入的输入缓冲区
    ifstream f1;   //读取文件的指针
    ifstream f2;   //读取文件行的指针
    int row;  //读到文件的第几行了

    Lexical(string fileName); //构造函数
    void changeState(char c);
    void readFile();

};


#endif //LEXICALDFA_LEXICAL_H
