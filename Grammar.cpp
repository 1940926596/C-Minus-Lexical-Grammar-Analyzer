//
// Created by 19409 on 2023/5/10.
// Editor: CJR
// very angry tnnd
//
#include "Grammar.h"

using namespace std;

Grammar::Grammar() {
    location = 0;
}

Grammar::Grammar(vector<pair<string, Type>> storage) {
    storageCopy = storage;
    location = 0;
}

pair<string, Type> Grammar::getNextToken() {
    if (location == storageCopy.size()) {
        location++;
        return {"end", END};
    }
    string tokenValue = storageCopy[location].first;
    Type tokenType = storageCopy[location].second;
    location++;
    return {tokenValue, tokenType};
}

void Grammar::error() {
    //语法有误
    cout << "Syntax tree ERROR , please check out" << endl;
    exit(1);
}

void Grammar::printTree(TreeNode *node) {
    //打印语法树
    for (int i = 0; i < node->layer; ++i) {
        cout << "--";
    }
    cout << node->label << " " << node->value << endl;
    for (const auto &child: node->children) {
        if (!child->isEmpty)
            printTree(child);
    }
}

void Grammar::readTokens() {
    //程序开始的地方
    cout << "Syntax tree" << endl;
    TreeNode *programNode = program(0);
    if (programNode->error)
        error();
    else {
        printTree(programNode);
    }
}

TreeNode *Grammar::program(int layer) {
    TreeNode *node = new TreeNode("program", layer);
    TreeNode *pNode = declaration_list(layer + 1);
    node->children.push_back(pNode);
    const pair<string, Type> &token = getNextToken();
    if (token.second != END) {
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::declaration_list(int layer) {
    TreeNode *node = new TreeNode("declaration_list", layer);
    TreeNode *pNode = declaration(layer + 1);
    if (pNode->error) {
        node->isEmpty = true;
        return node;
    }
    TreeNode *pTreeNode = declaration_list(layer + 1);
    node->children.push_back(pNode);
    node->children.push_back(pTreeNode);
    return node;
}

TreeNode *Grammar::declaration(int layer) {
    TreeNode *node = new TreeNode("declaration", layer);
//    if(getNextToken().first!="int"){
//        node->error= true;
//    }
    TreeNode *pNode = var_declaration(layer + 1);
    if (pNode->error) {
        TreeNode *pTreeNode = fun_declaration(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    } else {
        node->children.push_back(pNode);
    }
    return node;
}

TreeNode *Grammar::var_declaration(int layer) {
    TreeNode *node = new TreeNode("var_declaration", layer);

    TreeNode *pNode = type_specifier(layer + 1);

    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        const pair<string, Type> &token = getNextToken();
        if (token.second == IDTYPE) {
            TreeNode *buffTreeNode = new TreeNode("ID", layer + 1);
            buffTreeNode->value = token.first;
            node->children.push_back(buffTreeNode);
        }
        const pair<string, Type> &nextToken = getNextToken();
        if (nextToken.first == "[") {
            const pair<string, Type> &pair1 = getNextToken();
            if (pair1.second == NUMTYPE) {
                TreeNode *buffTreeNode = new TreeNode("NUM", layer + 1);
                buffTreeNode->value = pair1.first;
                node->children.push_back(buffTreeNode);
            }
            const pair<string, Type> &pair2 = getNextToken();
            if (pair2.first == "]") {
                const pair<string, Type> &pair3 = getNextToken();
                if (pair3.first == ";");
            }
        } else {
            location--;
            const pair<string, Type> &pair1 = getNextToken();
            if (pair1.first == ";") { ;
            } else {
                location -= 3;
                node->error = true;
            }
        }
    }
    return node;
}

TreeNode *Grammar::fun_declaration(int layer) {
    TreeNode *node = new TreeNode("fun_declaration", layer);

    TreeNode *pNode = type_specifier(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        const pair<string, Type> &token = getNextToken();
        if (token.second == IDTYPE) {
            TreeNode *treeNode = new TreeNode("ID", layer + 1);
            treeNode->value = token.first;
            node->children.push_back(treeNode);
            const pair<string, Type> &nextToken = getNextToken();
            if (nextToken.first == "(") {
                TreeNode *pTreeNode = params(layer + 1);
                if (pTreeNode->error) {
                    location -= 2;
                    node->error = true;
                } else {
                    node->children.push_back(pTreeNode);
                    const pair<string, Type> &pair1 = getNextToken();
                    if (pair1.first == ")") {
                        TreeNode *stmt = compound_stmt(layer + 1);
                        if (stmt->error) {
                            node->error = true;
                            location -= 3;
                        } else {
                            node->children.push_back(stmt);
                        }
                    }
                }
            }
        } else {
            location--;
            node->error = true;
        }
    }

    return node;
}

TreeNode *Grammar::type_specifier(int layer) {
    TreeNode *node = new TreeNode("type_specifier", layer);

    const pair<string, Type> &token = getNextToken();

    if (token.first == "int" || token.first == "void") {
        node->value = token.first;
    } else {
        node->error = true;
        location--;
    }
    return node;
}

TreeNode *Grammar::params(int layer) {
    TreeNode *node = new TreeNode("params", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "void") {
        TreeNode *pNode = new TreeNode("void", layer + 1);
        node->children.push_back(pNode);
    } else {
        location--;
        TreeNode *pNode = param_list(layer + 1);
        node->children.push_back(pNode);
    }
    return node;
}

TreeNode *Grammar::param_list(int layer) {
    TreeNode *node = new TreeNode("param_list", layer);

    TreeNode *pNode = param(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = param_list_(layer + 1);
        node->children.push_back(pTreeNode);
    }
    return node;
}

TreeNode *Grammar::param_list_(int layer) {
    TreeNode *node = new TreeNode("param_list_", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == ",") {
        TreeNode *pNode = param(layer + 1);
        node->children.push_back(pNode);
        TreeNode *pTreeNode = param_list_(layer + 1);
        node->children.push_back(pTreeNode);
    } else {
        location--;
        node->isEmpty = true;
        return node;
    }

    return node;
}

TreeNode *Grammar::param(int layer) {
    TreeNode *node = new TreeNode("param", layer);
    TreeNode *pNode = type_specifier(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        const pair<string, Type> &token = getNextToken();
        if (token.second == IDTYPE) {
            TreeNode *pTreeNode = new TreeNode("ID", layer + 1);
            pTreeNode->value = token.first;
            node->children.push_back(pTreeNode);
            const pair<string, Type> &nextToken = getNextToken();
            if (nextToken.first == "[") {
                getNextToken();
            } else {
                location--;
            }
        } else {
            location--;
            node->error = true;
        }
    }
    return node;
};

TreeNode *Grammar::compound_stmt(int layer) {
    TreeNode *node = new TreeNode("compound_stmt", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "{") {
        TreeNode *pNode = local_declarations(layer + 1);
        if (pNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pNode);
            TreeNode *pTreeNode = statement_list(layer + 1);
            if (pTreeNode->error) {
                node->error = true;
                return node;
            } else {
                node->children.push_back(pTreeNode);
                const pair<string, Type> &nextToken = getNextToken();
                if (nextToken.first == "}") { ;
                } else {
                    location -= 2;
                }
            }
        }
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::local_declarations(int layer) {
    TreeNode *node = new TreeNode("local_declarations", layer);
    TreeNode *pNode = var_declaration(layer + 1);

    if (pNode->error) {
        node->isEmpty = true;
        return node;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = local_declarations(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    }

    return node;
}

TreeNode *Grammar::statement_list(int layer) {
    TreeNode *node = new TreeNode("statement_list", layer);

    TreeNode *pNode = statement(layer + 1);
    if (pNode->error) {
        node->isEmpty = true;
        return node;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = statement_list(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    }
    return node;
}

TreeNode *Grammar::statement(int layer) {
    TreeNode *node = new TreeNode("statement", layer);

    TreeNode *pNode = expression_stmt(layer + 1);

    if (pNode->error) {
        TreeNode *pTreeNode = compound_stmt(layer + 1);
        if (pTreeNode->error) {
            TreeNode *stmt = selection_stmt(layer + 1);
            if (stmt->error) {
                TreeNode *iterationStmt = iteration_stmt(layer + 1);
                if (iterationStmt->error) {
                    TreeNode *returnStmt = return_stmt(layer + 1);
                    if (returnStmt->error) {
                        node->error = true;
                    } else {
                        node->children.push_back(returnStmt);
                    }
                } else {
                    node->children.push_back(iterationStmt);
                }
            } else {
                node->children.push_back(stmt);
            }
        } else {
            node->children.push_back(pTreeNode);
        }
    } else {
        node->children.push_back(pNode);
    }

    return node;
}

TreeNode *Grammar::expression_stmt(int layer) {
    TreeNode *node = new TreeNode("expression_stmt", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == ";") {
        return node;
    } else {
        location--;
        TreeNode *pNode = expression(layer + 1);
        if (pNode->error) {
            node->error = true;
            return node;
        } else {
            node->children.push_back(pNode);
            const pair<string, Type> &nextToken = getNextToken();
            if (nextToken.first == ";") { ;
            } else {
                location--;
                node->error = true;
            }
        }
    }
    return node;
}

TreeNode *Grammar::selection_stmt(int layer) {
    TreeNode *node = new TreeNode("selection_stmt", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "if") {
        TreeNode *pTreeNode = new TreeNode("if", layer + 1);
        node->children.push_back(pTreeNode);
        const pair<string, Type> &nextToken = getNextToken();
        if (nextToken.first == "(") {
            TreeNode *pNode = expression(layer + 1);
            node->children.push_back(pNode);
            //")"
            const pair<string, Type> &pair1 = getNextToken();
            TreeNode *pNode2 = statement(layer + 1);
            node->children.push_back(pNode2);
            const pair<string, Type> &pair2 = getNextToken();
            if (pair2.first == "else") {
                TreeNode *pTreeNode1 = new TreeNode("else", layer + 1);
                TreeNode *pNode1 = statement(layer + 1);
                node->children.push_back(pTreeNode1);
                node->children.push_back(pNode1);
            } else {
                location--;
            }
        } else {
            location -= 2;
            node->error = true;
        }
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::iteration_stmt(int layer) {
    TreeNode *node = new TreeNode("iteration_stmt", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "while") {
        getNextToken();
        TreeNode *pNode = expression(layer + 1);
        node->children.push_back(pNode);
        getNextToken();
        TreeNode *pTreeNode = statement(layer + 1);
        node->children.push_back(pTreeNode);
    } else {
        node->error = true;
        location--;
    }
    return node;
}

TreeNode *Grammar::return_stmt(int layer) {
    TreeNode *node = new TreeNode("return_stmt", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "return") {
        node->value = "return";
        const pair<string, Type> &nextToken = getNextToken();
        if (nextToken.first == ";") { ;
        } else {
            location--;
            //const pair<string, Type> &token1 = getNextToken();
            TreeNode *pNode = expression(layer + 1);

            if (pNode->error) {
                node->error = true;
            } else {
                node->children.push_back(pNode);
                const pair<string, Type> &pair1 = getNextToken();
                if (pair1.first == ";");
                else {
                    location -= 2;
                    node->error = true;
                }
            }
        }
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::expression(int layer) {
    TreeNode *node = new TreeNode("expression", layer);

    TreeNode *pNode = var(layer + 1);
    if (pNode->error) {
        TreeNode *pTreeNode = simple_expression(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    } else {
        node->children.push_back(pNode);
        const pair<string, Type> &token = getNextToken();
        if (token.first == "=") {
            TreeNode *pTreeNode = expression(layer + 1);
            if (pTreeNode->error) {
                location--;
                node->error = true;
            } else {
                node->children.push_back(pTreeNode);
            }
        } else {
            location--;
            node->error = true;
        }
    }
    return node;
}

TreeNode *Grammar::var(int layer) {
    TreeNode *node = new TreeNode("var", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.second == IDTYPE) {
        const pair<string, Type> &pair1 = getNextToken();
        if (pair1.first != "=") {
            location -= 2;
            node->error = true;
            return node;
        } else {
            location--;
        }
        TreeNode *pNode = new TreeNode("ID", layer + 1);
        pNode->value = token.first;
        node->children.push_back(pNode);
        const pair<string, Type> &nextToken = getNextToken();
        if (nextToken.first == "[") {
            TreeNode *pTreeNode = expression(layer + 1);
            node->children.push_back(pTreeNode);
            getNextToken();
        } else {
            location--;
        }
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::simple_expression(int layer) {
    TreeNode *node = new TreeNode("simple_expression", layer);
    TreeNode *pNode = additive_expression(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = relop(layer + 1);
        if (pTreeNode->error) {
            return node;
        } else {
            node->children.push_back(pTreeNode);
            TreeNode *additiveExpression = additive_expression(layer + 1);
            node->children.push_back(additiveExpression);
        }
    }

    return node;
}


TreeNode *Grammar::relop(int layer) {
    TreeNode *node = new TreeNode("relop", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "<=" || token.first == "<" || token.first == ">" || token.first == ">=" || token.first == "==" ||
        token.first == "!=") {
        node->value = token.first;
    } else {
        location--;
        node->error = true;
    }

    return node;
}

TreeNode *Grammar::additive_expression(int layer) {
    TreeNode *node = new TreeNode("additive_expression", layer);

    TreeNode *pNode = term(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = additive_expression_(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    }
    return node;
}

TreeNode *Grammar::additive_expression_(int layer) {
    TreeNode *node = new TreeNode("additive_expression_", layer);

    TreeNode *pNode = addop(layer + 1);
    if (pNode->error) {
        node->isEmpty = true;
    } else {
        TreeNode *pTreeNode = term(layer + 1);
        TreeNode *additiveExpression = additive_expression_(layer + 1);
        if (pTreeNode->error || additiveExpression->error) {
            node->error = true;
        } else {
            node->children.push_back(pNode);
            node->children.push_back(pTreeNode);
            node->children.push_back(additiveExpression);
        }
        return node;
    }

    return node;
}

TreeNode *Grammar::addop(int layer) {
    TreeNode *node = new TreeNode("addop", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == "+" || token.first == "-") {
        node->value = token.first;
    } else {
        location--;
        node->error = true;
    }

    return node;
}

TreeNode *Grammar::term(int layer) {
    TreeNode *node = new TreeNode("term", layer);

    TreeNode *pNode = factor(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        node->children.push_back(pNode);
        TreeNode *pTreeNode = term_(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pTreeNode);
        }
    }

    return node;
}

TreeNode *Grammar::term_(int layer) {
    TreeNode *node = new TreeNode("term_", layer);

    TreeNode *pNode = mulop(layer + 1);
    if (pNode->error) {
        node->isEmpty = true;
        return node;
    } else {
        TreeNode *pNode1 = factor(layer + 1);
        if (pNode1->error) {
            node->error = true;
        } else {
            TreeNode *term1 = term_(layer + 1);
            if (pNode1->error || term1->error) {
                node->error = true;
            } else {
                node->children.push_back(pNode);
                node->children.push_back(pNode1);
                node->children.push_back(term1);
            }
        }
    }

    return node;
}

TreeNode *Grammar::mulop(int layer) {
    TreeNode *node = new TreeNode("mulop", layer);
    const pair<string, Type> &token = getNextToken();
    if (token.first == "*" || token.first == "/") {
        node->value = token.first;
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::factor(int layer) {
    TreeNode *node = new TreeNode("factor", layer);

    TreeNode *pNode = call(layer + 1);
    if (pNode->error) {
        const pair<string, Type> &nextToken = getNextToken();
        if (nextToken.second != IDTYPE) {
            location--;
            const pair<string, Type> &token = getNextToken();
            if (token.second == NUMTYPE) {
                TreeNode *pNode1 = new TreeNode("NUM", layer + 1);
                pNode1->value = token.first;
                node->children.push_back(pNode1);
            } else {
                if (token.first == "(") {
                    TreeNode *pNode1 = expression(layer + 1);
                    if (pNode1->error) {
                        location--;
                        node->error = true;
                    } else {
                        node->children.push_back(pNode1);
                        getNextToken();//")"
                    }
                } else {
                    location--;
                    node->error = true;
                }
            }
        } else {
            TreeNode *pTreeNode = new TreeNode("ID", layer + 1);
            pTreeNode->value = nextToken.first;
            node->children.push_back(pTreeNode);
        }
    } else {
        node->children.push_back(pNode);
    }

    return node;
}

TreeNode *Grammar::call(int layer) {
    TreeNode *node = new TreeNode("call", layer);
    const pair<string, Type> &token = getNextToken();
    if (token.second == IDTYPE) {
        const pair<string, Type> &nextToken = getNextToken();
        TreeNode *pTreeNode = new TreeNode("ID", layer + 1);
        pTreeNode->value = token.first;
        node->children.push_back(pTreeNode);
        if (nextToken.first == "(") {
            TreeNode *pNode = args(layer + 1);
            node->children.push_back(pNode);
            getNextToken();//")"
        } else {
            node->error = true;
            location -= 2;
        }
    } else {
        location--;
        node->error = true;
    }
    return node;
}

TreeNode *Grammar::args(int layer) {
    TreeNode *node = new TreeNode("args", layer);

    TreeNode *pNode = arg_list(layer + 1);
    if (pNode->error) {
        node->isEmpty = true;
    } else {
        node->children.push_back(pNode);
    }
    return node;
}

TreeNode *Grammar::arg_list(int layer) {
    TreeNode *node = new TreeNode("arg_list", layer);
    TreeNode *pNode = expression(layer + 1);
    if (pNode->error) {
        node->error = true;
    } else {
        TreeNode *pTreeNode = arg_list_(layer + 1);
        if (pTreeNode->error) {
            node->error = true;
        } else {
            node->children.push_back(pNode);
            node->children.push_back(pTreeNode);
        }
    }
    return node;
}

TreeNode *Grammar::arg_list_(int layer) {
    TreeNode *node = new TreeNode("arg_list_", layer);

    const pair<string, Type> &token = getNextToken();
    if (token.first == ",") {
        TreeNode *pNode = expression(layer + 1);
        if (pNode->error) {
            location--;
            node->error = true;
        } else {
            TreeNode *pTreeNode = arg_list_(layer + 1);
            node->children.push_back(pNode);
            node->children.push_back(pTreeNode);
        }
    } else {
        location--;
        node->isEmpty = true;
    }

    return node;
}
