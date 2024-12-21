#include "scopes.cpp"

std::map<std::string, Token> variables = {
    {"_", Token("0", INT)},
    {"a", Token("0", INT)}
};

std::map<std::string, Token> functions = {
    {"function", Token("0", INT)},
};

std::map<std::string, Token> classes = {
    {"classA", Token("0", INT)},
};
//variables.insert();
//variables["b"] = Variable("b");

Token interpret(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
    //std::cout<<"()"<<depth<<","<<node->token.value<<","<<displayTokenType(node->token.type)<<")\n";
    if(node->token.type == IDENTIFIER){
        if(variables.count(node->token.value)){
            node->token = Token(node->token.value, VARIABLE);
        } else if(functions.count(node->token.value)){
            
        } else if(classes.count(node->token.value)){
            
        } else{
            node->token = Token(node->token.value, VARIABLE);
            variables.insert({node->token.value, Token("null", NUL)});
        }
        return node->token;
    } else if(node->token.type == INT || node->token.type == FLOAT){
        return node->token;
    } else{
        Token right, left;
        if (node->right) {
            right = interpret(node->right);
        } else{
            //return Token();
        }
        if (node->left) {
            left = interpret(node->left);
        } else{
            //return Token();
        }
        //std::cout<<" left: "<<left.value<<" right: "<< right.value << "\n";
        //std::cout<<" left: "<<displayTokenType(left.type)<<" right: "<< displayTokenType(right.type) <<"\n";
        
        if(node->token.type == ASSIGN){
            if(left.type == VARIABLE){
                if(right.type == VARIABLE){
                    variables[left.value] = variables[right.value];
                } else{
                    variables[left.value] = right;
                }
                return right;
            } else{
                std::cout<<"assigning error\n";
            }
        } else if(node->token.type == ARITMETIC_OPERATOR){
            if(left.type == VARIABLE){
                left = variables[left.value];
            }
            if(right.type == VARIABLE){
                right = variables[right.value];
            }
            if(node->token.value == "+"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) + std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "-"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) - std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "*"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) * std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "/"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), FLOAT);
                }
            } else if(node->token.value == "%"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) % std::stoi(right.value) : 0), INT);
                }
            }
        }
        std::cerr << "LEFT (" << left.value << "," << displayTokenType(left.type) << ")\n";
        std::cerr << "RIGHT (" << right.value << "," << displayTokenType(right.type) << ")\n";
    }
    std::cerr << "ERROR (" << node->token.value << "," << displayTokenType(node->token.type) << ")\n";
    return Token("null",NUL);
}

void interpreter(std::string& code){
    replace(code, "\n", "|n|");
    replace(code, "    ", "|t|");
    std::vector<std::string> scopes = {"{"};
    std::vector<std::string> scopeSymbols = {"{", "}"};
    splitString(code, scopeSymbols, scopes);
    scopes.push_back("~CURLYBRACKETCLOSE");
    for(std::string scope : scopes){
        std::cout << scope <<"\n";
    } std::cout << "\n";
    //Scope* rootScope = parseScopes(scopes);
    //printScopes(rootScope);
    // for (std::string& statement : scopes) {
    //     std::vector<Token> tokens;
    //     std::vector<std::string> strings;
    //     lexer(statement, strings, operators);
    //     tokenize(strings, tokens);
    //     //print(tokens);
    //     if(!tokens.empty())
    //         tokensArr.push_back(tokens);
    // }


    // std::vector<std::vector<Token>> statements;
    // for(std::vector<Token> tokens : statements){
    //     Node* root = parse(tokens);
    //     //std::cout << "\n";
    //     Token output = interpret(root);
    //     std::cout << "(" << output.value << "," << displayTokenType(output.type) << ")\n";
    // }
    // std::cout<<"Variables:\n";
    // for(auto variable : variables){
    //     std::cout<< "(" <<variable.first<<","<< variable.second.value << "," << displayTokenType(variable.second.type)<<")\n";
    // }
}