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

struct TreeNode {
    string label;
    string value;
    bool error;
    bool isEmpty;
    int layer;

    vector<TreeNode *> children;

    TreeNode(string label, int layer) : label(label), value(""), isEmpty(false), layer(layer),
                                                                error(false) {}
};

class Grammar {
public:
    vector<pair<string, Type>> storageCopy;
    int location;

    Grammar();

    Grammar(vector<pair<string, Type>> storage);

    pair<string, Type> getNextToken();

    void error();
    void readTokens();
    void printTree(TreeNode* node);

    TreeNode* program(int layer);
    TreeNode* declaration_list(int layer);
    TreeNode* declaration(int layer);
    TreeNode* var_declaration(int layer);
    TreeNode* type_specifier(int layer);
    TreeNode* fun_declaration(int layer);
    TreeNode* params(int layer);
    TreeNode* param_list(int layer);
    TreeNode* param_list_(int layer);
    TreeNode* param(int layer);
    TreeNode* compound_stmt(int layer);
    TreeNode* local_declarations(int layer);
    TreeNode* statement_list(int layer);
    TreeNode* statement(int layer);
    TreeNode* expression_stmt(int layer);
    TreeNode* selection_stmt(int layer);
    TreeNode* iteration_stmt(int layer);
    TreeNode* return_stmt(int layer);
    TreeNode* expression(int layer);
    TreeNode* var(int layer);
    TreeNode* simple_expression(int layer);
    TreeNode* relop(int layer);
    TreeNode* additive_expression(int layer);
    TreeNode* additive_expression_(int layer);
    TreeNode* addop(int layer);
    TreeNode* term(int layer);
    TreeNode* term_(int layer);
    TreeNode* mulop(int layer);
    TreeNode* factor(int layer);
    TreeNode* call(int layer);
    TreeNode* args(int layer);
    TreeNode* arg_list(int layer);
    TreeNode* arg_list_(int layer);

};


#endif //LEXICALDFA_GRAMMAR_H
