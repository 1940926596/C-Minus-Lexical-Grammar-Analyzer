//
// Created by 19409 on 2023/5/9.
//

#ifndef LEXICALDFA_GRAMMAR_H
#define LEXICALDFA_GRAMMAR_H

#include<string>
#include<iostream>
#include<vector>
#include"Lexical.h"

using namespace std;

enum NonTerminator{
    PROGRAM, //源程序，开始符号 （终结符，非终结符，产生式，开始符号）
    DECLIST,
    TYPE,
    STLIST,
    DECLISTN,
    STLISTN
};


class Grammar{
public:
    Grammar();
    Grammar(vector<pair<string,Type>> storage);
    void procedurePROGRAM(string token);

    string getNextToken();
};


#endif //LEXICALDFA_GRAMMAR_H
