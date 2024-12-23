#include "parser.cpp"

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

class Scope{
    public:
    Node* root;
    std::string type;
    std::vector<Token> tokens;
    std::vector<std::unique_ptr<Scope>> scopes;

    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    std::map<std::string, Token>  ScopeNamespace;
    std::vector<std::shared_ptr<std::map<std::string, Token>>> namespaces;//make to pointers
    
    Scope(std::string type = "{}", std::vector<std::shared_ptr<std::map<std::string, Token>>> namespaces = {}){
        this->type = type;
        this->namespaces = namespaces;
        this->namespaces.insert(this->namespaces.begin(), std::make_shared<std::map<std::string, Token>>(ScopeNamespace));
    }

    void appendToken(Token token){
        tokens.push_back(token);
    }

    Scope* appendScope(const std::string& scopeType) {
        scopes.emplace_back(std::make_unique<Scope>(scopeType));
        tokens.push_back(Token(scopeType, SCOPE, scopes.size()-1));
        return scopes.back().get();
    }

    void print(int depth = 0){
        std::cout << std::string(depth * 5, ' ') << "Type: "<<type<<" Tokens: ";
        if(tokens.empty()){
            std::cout<<"EMPTY";
        } else {
            for(Token token : tokens){
            std::cout<<token.value<<displayType(token.type)<<" ";
            }
        }
        std::cout<<"\n";
        for (const auto& scope : scopes) {
            scope->print(depth + 1);
        }
    }

    void processOperator() {
            if (operatorStack.empty() || exprStack.size() < 2) {
                std::cerr << "PARSER ERROR: Invalid operator/operand stack state!\n";
                return;
            }
            Token op = operatorStack.top();
            operatorStack.pop();

            Node* right = exprStack.top();
            exprStack.pop();

            Node* left = exprStack.top();
            exprStack.pop();

            exprStack.push(new Node(op, left, right));
        };

    Node* parse(std::vector<Token> expected = {}){
        
        for (const Token& token : tokens) {
            if(!expected.empty()){
                std::cout <<"clearing expected";
                expected = {};
            } else if (token.type == SCOPE) {
                exprStack.push(new Node(scopes[token.weight] -> interpret()));
            } else if (token.type == FUNCTION_DECLARATION) {
                expected.push_back(Token("()", SCOPE));
                expected.push_back(Token("{}", SCOPE));
                exprStack.push(new Node(token));
            } else if (token.type == CLASS) {
                expected.push_back(Token("()", SCOPE));
                expected.push_back(Token("{}", SCOPE));
                exprStack.push(new Node(token));
            } else if (token.type == INT || token.type == FLOAT || token.type == IDENTIFIER) {
                exprStack.push(new Node(token));
            } else if (token.type == ARITMETIC_OPERATOR || token.type == ASSIGN) {
                while (!operatorStack.empty() && 
                    operatorStack.top().type != BRACKET_OPEN &&
                    operatorStack.top().weight >= token.weight) {
                    processOperator();
                }
                operatorStack.push(token);
            } else {
                std::cerr << "PARSER ERROR: Unknown token type! "<<displayTokenType(token.type)<<"\n";
                return nullptr;
            }
        }
        // Process remaining operators
        while (!operatorStack.empty()) {
            if (operatorStack.top().type == BRACKET_OPEN) {
                std::cerr << "PARSER ERROR: Mismatched brackets!\n";
                return nullptr;
            }
            processOperator();
        }
        if (exprStack.size() != 1) {
            std::cerr << "PARSER ERROR: Malformed expression!\n";
            return nullptr;
        }
        return exprStack.top();
    }

    Token* findInNamespace(std::string key){
        for(std::shared_ptr<std::map<std::string, Token>> ns : namespaces){
            if((*ns).count(key)){
                return &(*ns)[key];
            }
        }
        return &nullvalue;
    }

    Token interpretTree(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
        //std::cout<<"()"<<depth<<","<<node->token.value<<","<<displayTokenType(node->token.type)<<")\n";
        if(1==0){
            
        } else if(node->token.type == INT || node->token.type == FLOAT || node->token.type == IDENTIFIER){
            return node->token;
        } else{
            Token right, left;
            if (node->right) {
                right = interpretTree(node->right);
            } else{
                //return Token();
            }
            if (node->left) {
                left = interpretTree(node->left);
            } else{
                //return Token();
            }
            if(node->token.type == ASSIGN){
                if(left.type == IDENTIFIER){
                    ScopeNamespace[left.value] = (right.type == IDENTIFIER) ? *findInNamespace(right.value) : right;
                    return ScopeNamespace[left.value];
                } else{
                    std::cout<<"assigning error\n";
                }
            } else if(node->token.type == ARITMETIC_OPERATOR){
                if(left.type == VARIABLE){
                    left = *findInNamespace(left.value);
                }
                if(right.type == VARIABLE){
                    right = *findInNamespace(right.value);
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

    Token interpret(int depth = 0){
        root = this->parse();
        return interpretTree(root);
    }
};


Scope makeScopeTree(std::vector<Token>& tokens) {
    Scope root("{}");
    Scope* currentScope = &root;
    std::stack<Scope*> scopeStack;
    for (const Token& token : tokens) {
        if (token.type != BRACKET_OPEN && token.type != BRACKET_CLOSE) {
            currentScope->appendToken(token);
        } else if (token.type == BRACKET_OPEN) {
            std::string type;
            if (token.value == "{") type = "{}";
            else if (token.value == "(") type = "()";
            else if (token.value == "[") type = "[]";

            Scope* newScope = currentScope->appendScope(type);
            scopeStack.push(currentScope);
            currentScope = newScope;
        } else if (token.type == BRACKET_CLOSE) {
            char expectedClose = currentScope->type[1];
            if (token.value[0] != expectedClose) {
                std::cerr << "ERROR: Mismatched brackets " << token.value << " and " << currentScope->type << "\n";
                return root;
            }
            currentScope = scopeStack.top();
            scopeStack.pop();
        }
    }
    return root;
}

// Scope makeScopeTree(std::vector<Token>& tokens){
//     std::string type = "{}";
//     Scope root = Scope(type);
//     Scope* scope = &root;
//     std::stack<Scope*> history;
//     for(Token token : tokens){
//         if(token.type != BRACKET_OPEN && token.type != BRACKET_CLOSE){
//             scope->appendToken(token);
//         } else{
//             if(token.type == BRACKET_OPEN){
//                 if(token.value == "{"){
//                     type = "{}";
//                 } else if(token.value == "("){
//                     type = "()";
//                 } else if(token.value == "["){
//                     type = "[]";
//                 }
//                 int index = scope->appendScope(Scope(type));
//                 history.push(scope);
//                 scope = &(scope->scopes[index]);
//             } else if(token.value == std::to_string(scope->type[1])){
//                 scope = history.top();
//                 history.pop();
//             } else{
//                 std::cerr << "ERROR not matching brackets "<<token.value<<" and "<<type<<"\n";
//             }
//         }
//     }
//     return root;
// }

// void printScopes(Scope* node, int depth = 0, bool isLeft = true) {
//     if (!node) return;
//     if (node->right) {
//         printScopes(node->right, depth + 1, false);
//     }
//     std::cout << std::string(std::max(depth-1, 0) * 10, ' ');
//     if (depth > 0) {
//         std::cout << (!isLeft ? "/" : "\\");
//     }
//     std::cout << std::string(depth * 10, '-') << node->scope << "\n";
//     if (node->left) {
//         printScopes(node->left, depth + 1, true);
//     }
// }

// Scope* parseScopes(std::vector<std::string>& scopes) {
//     std::stack<std::string> operatorStack;
//     std::stack<Scope*> scopeStack;

//     auto processOperator = [&]() {
//         if (operatorStack.empty() || scopeStack.size() < 2) {
//             std::cerr << "PARSER ERROR: Invalid operator/operand stack state!\n";
//             return;
//         }
//         std::string scope = operatorStack.top();
//         operatorStack.pop();

//         Scope* right = scopeStack.top(); // Right operand
//         scopeStack.pop();

//         Scope* left = scopeStack.top(); // Left operand
//         scopeStack.pop();

//         scopeStack.push(new Scope(scope, left, right));
//     };

//     for (const std::string& scope : scopes) {
//         // if (scope == "{") {
//         //     operatorStack.push(scope);
//         // } else 
//         if (scope == "}") {
//             while (!operatorStack.empty() && operatorStack.top() != "{") {
//                 processOperator();
//             }
//             if (operatorStack.empty() || operatorStack.top() != "{") {
//                 std::cerr << "PARSER ERROR: Mismatched brackets!\n";
//                 return nullptr;
//             }
//             operatorStack.pop(); // Pop the opening bracket
//         } else {
//             operatorStack.push(scope);
//             // std::cerr << "PARSER ERROR: Unknown token type!\n";
//             // return nullptr;
//         }
//     }

//     // Process remaining operators
//     while (!operatorStack.empty()) {
//         if (operatorStack.top() == "{") {
//             std::cerr << "PARSER ERROR: Mismatched brackets!\n";
//             return nullptr;
//         }
//         processOperator();
//     }

//     if (scopeStack.size() != 1) {
//         std::cerr << "PARSER ERROR: Malformed expression!\n";
//         return nullptr;
//     }

//     return scopeStack.top(); // Root of the expression tree
// }