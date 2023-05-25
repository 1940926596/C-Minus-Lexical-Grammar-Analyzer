//
// Created by 19409 on 2023/4/20.
//

#include <iostream>
#include "Lexical.h"

void operateReserve(Lexical* lexical){
    //处理 保留字 关键字
    string reserve[]={"if","then","else","end","repeat","until"
            ,"read","white","int","double","float","void","return"};

    for(auto i:reserve){
        string buff;
        for (auto j:i){
            buff+=j+'A'-'a';
        }
        lexical->reserveWords.insert({i,buff});
    }
}

void operateIdentifier(Lexical* lexical){
    //运算符
    string identifier[]={"+","-","=",">=","<=","==",
                         "*","/","!=","{","}",
                         "[","]","(",")",";",","};

    for(auto i:identifier)
        lexical->identifierWords.insert({i,i});
}


Lexical::Lexical(string fileName){
    operateReserve(this);
    operateIdentifier(this);
    ID="ID";  //标识符
    NUM="NUM";   //数
    state=START;
    f1=ifstream(fileName,ios::in|ios::binary);
    f2=ifstream(fileName,ios::in|ios::binary);
    row=1;
}

void Lexical::changeState(const char c) {
    input+=c;
    switch (state) {
        case START:{
            if(c=='\t'||c==' '||c=='\n'){
                state=DONE;
                if(c=='\n'){
                    row++;
                }

            }
            else if((c<='z'&&c>='a')||(c<='Z'&&c>='A')){
                state=INID;
            }
            else if(c>='0'&&c<='9'){
                state=INNUM;
            }
            else if(c=='='){
                state=INEQ;
            }
            else if(c=='!'){
                state=INNEQ;
            }
            else if(c=='<'){
                state=INLESS;
            }
            else if(c=='>'){
                state=INLGREAT;
            }
            else if(c=='/'){
                state=LCOMMENT;
            }
            else{
                state=DONE;
            }
            break;
        }
        case RCOMMENT:{
            if(c=='/'){
                state=DONE;
            }
            else{
                state=RCOMMENT;
            }
            break;
        }
        case INCOMMENT:{
            if(c=='*'){
                state=RCOMMENT;
            }
            else{
                state=INCOMMENT;
            }
            break;
        }
        case INNUM:{
            if(c>='0'&&c<='9'){
                state=INNUM;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case INID:{
            if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
                state=INID;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case INEQ:{
            if(c=='='){
                state=DONE;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case INNEQ:{
            if(c=='='){
                state=DONE;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case INLESS:{
            if(c=='='){
                state=DONE;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case INLGREAT:{
            if(c=='='){
                state=DONE;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case LCOMMENT:{
            if(c=='*'){
                state=INCOMMENT;
            }
            else{
                state=DONE;
                f1.seekg(-1,ios::cur);
                input.erase(input.end()-1);
            }
            break;
        }
        case DONE:{
            break;
        }
    }

    //DONE有点区别，注意
    if(state==DONE){

        if(input[0]!=' '&&input[0]!='\t'&&input[0]!='\n'&&input[0]!='\r'){
            if(reserveWords.count(input)!=0){
                storage.push_back(pair<string,Type>(input,reserveWord));
                cout<<'\t'<<row<<':'<<' '<<"reserved word: "<<input<<endl;
            }
            else if(identifierWords.count(input)!=0){
                storage.push_back(pair<string,Type>(input,identifierWord));
                cout<<'\t'<<row<<':'<<' '<<"identifier word: "<<input<<endl;
            }
            else if(input[0]>='0'&&input[0]<='9'){
                storage.push_back(pair<string,Type>(input,NUMTYPE));
                cout<<'\t'<<row<<':'<<' '<<"NUM: "<<input<<endl;
            }
            else if((input[0]>='a'&&input[0]<='z')||(input[0]>='A'&&input[0]<='Z')){
                storage.push_back(pair<string,Type>(input,IDTYPE));
                cout<<'\t'<<row<<':'<<' '<<"ID: "<<input<<endl;
            }
            else{
                cout<<'\t'<<row<<':'<<' '<<input<<endl;
            }
        }

        input="";
        state=START;
    }

}

void Lexical::readFile() {
    int row1=row;
    string s;
    getline(f2,s);
    cout<<row<<": "<<s<<endl;
    char inChar;
    while(1){
        inChar=(char)f1.get();
        if(f1.tellg()==EOF){
            return;
        }
        changeState(inChar);
        if(row1!=row){
            row1=row;
            getline(f2,s);
            cout<<row<<": "<<s<<endl;
        }
    }


}
