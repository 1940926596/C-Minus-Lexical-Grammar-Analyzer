//
// Created by 19409 on 2023/5/10.
//
#include "Grammar.h"

using namespace std;

Grammar::Grammar() {}

Grammar::Grammar(vector<pair<string,Type>> storage) {
    for (int i = 0; i < storage.size(); ++i) {
        cout<<storage[i].first<<' ';
    }
}

string Grammar::getNextToken() {

    return "hello";
}

void Grammar::procedurePROGRAM(string token) {

}