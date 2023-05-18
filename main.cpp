#include <iostream>
#include "Lexical.h"
#include "Grammar.h"

int main() {
    string fileName="C:\\Code\\C++\\LexicalDFA\\test.text";
    Lexical lexical(fileName);

    try {
        lexical.readFile();
    }catch (const char* error){
        string s=error;
        if(s=="error"){
            cout<<"输入有误，请重新输入";
            return -1;
        }
    }

    Grammar grammar(lexical.storage);


    return 0;
}
