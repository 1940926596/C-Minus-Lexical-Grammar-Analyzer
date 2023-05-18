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
            cout<<"������������������";
            return -1;
        }
    }

    Grammar grammar(lexical.storage);


    return 0;
}
