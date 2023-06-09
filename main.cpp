#include <iostream>
#include "Grammar.h"

using namespace std;


int main() {
    string fileName="C:\\Code\\C++\\LexicalDFA\\test.text";
    Lexical lexical(fileName);

    try {
        lexical.readFile();
    }catch (const char* error){
        string s=error;
        if(s=="error"){
            cout<<"´Ê·¨ÓÐÎó";
            return -1;
        }
    }

    Grammar grammar(lexical.storage);
    grammar.readTokens();

    return 0;
}