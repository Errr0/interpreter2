#include "parser.cpp"

class Scope{
    public: 
    Node* root;
    Scope* parent;
    std::string type;
    std::vector<Token> tokens;

    std::vector<std::vector<Token>> statementsTokens;
    std::vector<Scope> scopes;
    int debug_trash;
    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    std::map<std::string, Token>  ScopeNamespace;
    
    Scope(std::string type = "{}", Scope* parent = nullptr, std::map<std::string, Token> ScopeNamespace = {}){
        this->type = type;
        this->ScopeNamespace = ScopeNamespace;
        this->parent = parent;
    }

    ~Scope(){
        delete root;
        delete parent;
    }

    void appendToken(Token token){
        tokens.push_back(token);
    }

    void makeStatement(std::string type){
        statementsTokens.push_back(tokens);
        tokens.clear();        
    }

    Scope appendScope(const std::string& scopeType, std::map<std::string, Token> ns = {}) {
        scopes.push_back(Scope(scopeType, this, ns));
        tokens.push_back(Token(scopeType, SCOPE, scopes.size()-1));
        return scopes.back();
    }

    void printNamespaces(int depth = 0){
        for(auto pair : ScopeNamespace){
            std::cout<<std::string(depth * 5, ' ')<<"("<<pair.first<<","<<"("<<pair.second.value<<","<<displayTokenType(pair.second.type)<<","<<pair.second.weight<<")"<<")\n";
        }
        if(parent){
            parent->printNamespaces();
        }
    }

    void print(int depth = 0){
        std::cout << std::string(depth * 5, ' ') << "Type: "<<type<<" Tokens: ";
        if(tokens.empty()){
            std::cout<<"EMPTY";
        } else {
            for(Token token : tokens){
                std::cout<<token.value<<" ";//displayTokenType(token.type)<<((token.type == SCOPE) ? std::to_string(token.weight) : "")<<" ";
            }
        }
        std::cout <<"\n"<< std::string((depth) * 5, ' ')<<"Namespaces:\n";
        printNamespaces(depth);
        if(!statementsTokens.empty()){
            for(std::vector<Token> statement : statementsTokens){
                std::cout << std::string(depth * 5, ' ') <<"Tokens: ";
                for(Token token : statement){
                    std::cout<<token.value<<" ";//displayTokenType(token.type)<<((token.type == SCOPE) ? std::to_string(token.weight) : "")<<" ";
                }
                std::cout << "\n";
            }
        }
        std::cout<<"\n";
        for (auto scope : scopes) {
            scope.print(depth + 1);
        }
    }
    
    void processOperator() { 
        if (operatorStack.empty() || exprStack.size() < 2) {
            std::cerr << "PARSER ERROR: Invalid operator/operand stack state!\n";
            std::cerr << "  operStackSize: "<<operatorStack.size()<<"\n";
            displayToken(operatorStack.top());
            std::cerr <<"\n";
            std::cerr << "  exprStackSize: "<<exprStack.size()<<"\n";
            displayToken(exprStack.top()->token);
            std::cerr <<"\n";
            // std::cin >> debug_trash;
            return;
        }
        Token op = operatorStack.top();
        operatorStack.pop();
        Node* right = exprStack.top();
        exprStack.pop();
        Node* left = exprStack.top();
        exprStack.pop();

        exprStack.push(new Node(op, left, right));
    }
    
    Node* parse(std::vector<Token> statement){
        statement.push_back(Token());
        int skip = 0;
        for (Token& token : statement) {
            if(token.type==END){
                break;
            }
            if(skip>0){
                skip--;
                continue;
            }
            // std::cout <<"parsing ";displayToken(token);
            // std::cout <<"next ";displayToken(*(&token+1));
                if (token.type == SCOPE) {
                    Token scope = scopes[token.weight].interpret();
                    exprStack.push(new Node(scope));
                } else if (token.type == IDENTIFIER) {
                    if((&token+1)->type == SCOPE && (&token+1)->value == "()"){
                        
                    }
                    exprStack.push(new Node(token));
                    // if((&token+1)->type == SCOPE && (&token+1)->value == "()"){ 
                    //     if(externalFunctions.find(token.value) != externalFunctions.end()){
                    //         callExternalFunction(token.value);
                    //         skip=1;
                    //     } else 
                    //     if((&token+2)->type == SCOPE && (&token+2)->value == "{}"){
                    //         scopes[(&token+1)->weight].ScopeNamespace["~~arguments_names~~"] = scopes[(&token+1)->weight]->interpret();
                    //         scopes[(&token+1)->weight].copy(scopes[(&token+2)->weight]);
                    //         scopes[(&token+1)->weight].type = "function";
                    //         functions[token.value] = *scopes[(&token+1)->weight];
                    //         skip = 2;
                    //     } else{
                    //         if(functions.count(token.value)){
                    //             functions[token.value].updateArguments(scopes[(&token+1)->weight]->interpret(), scopes[(&token+1)->weight]->namespaces);
                    //             exprStack.push(new Node(functions[token.value].interpret()));
                    //         } else{
                    //             std::cerr << "FUNCTION ERROR: function doesent exist"<<displayTokenType(token.type)<<"\n";
                    //         }
                    //         skip = 1;
                    //     }
                    // }
                // } else if(token.type == KEYWORD){
                //     if(token.value == "if"){
                //         if((&token+1)->type == SCOPE && (&token+1)->value == "()"){ 
                //             if((&token+2)->type == SCOPE && (&token+2)->value == "{}"){
                //                 skip = 2;
                //                 Token condition = scopes[(&token+1)->weight]->interpret();
                //                 //bool isTrue = false;
                //                 if(condition.type == INT && stoi(condition.value) > 0){//maybe to combine to one BIG if with ||
                //                     exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                //                 } else if(condition.type == FLOAT && stof(condition.value) > 0){
                //                     exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                //                 } else if(condition.type == CHAR && condition.value != ""){
                //                     exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                //                 } else if((condition.type == ARRAY || condition.type == STRING) && Arrays[condition.weight].size() > 0){
                //                     exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                //                 } else{
                //                 }
                //                 //scopes[(&token+1)->weight];
                //                 //exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                
                //             } else{
                //                 std::cerr << "IF ERROR: body doesent exist"<<displayTokenType(token.type)<<"\n";
                //                 skip = 1;
                //                 std::cin >> debug_trash;
                //             }
                //         } else{
                //                 std::cerr << "IF ERROR: condition doesent exist"<<displayTokenType(token.type)<<"\n";
                //                 std::cin >> debug_trash;
                //                 //skip = 1;
                //             }
                //     } else if((&token+1)->type == SCOPE && (&token+1)->value == "{}"){
                //         scopes[(&token+1)->weight]->type = "class";
                //         classes.insert({token.value, *scopes[(&token+1)->weight]});
                //         skip=1;
                //     } else{
                //         exprStack.push(new Node(token));
                //     }
                } else if (token.type == ARRAY || token.type == STRING){
                    exprStack.push(new Node(token));
                } else if (token.type == INT || token.type == FLOAT || token.type == CHAR) {
                    exprStack.push(new Node(token));
                } else if (token.type == OPERATOR || token.type == ASSIGN || token.type == AMPERSAND) {
                    while (!operatorStack.empty() && 
                        operatorStack.top().type != BRACKET_OPEN &&
                        operatorStack.top().weight >= token.weight) {
                        processOperator();
                    }
                    operatorStack.push(token);
                } else {
                    std::cerr << "PARSER ERROR: Unknown token type! "<<displayTokenType(token.type)<<"\n";
                    //std::cin >> debug_trash;
                    return nullptr;
                }
            }
        while (!operatorStack.empty()) {
            processOperator();
        }
        if (exprStack.size() != 1) {
            std::cerr << "PARSER ERROR: Malformed expression!\n";
            while(!exprStack.empty()){
                displayToken(exprStack.top()->token);
                exprStack.pop();
            }
            return nullptr;
        }
        Node* return_value = exprStack.top();
        exprStack.pop();
        return return_value;
    }
    
    Token findInNamespace(std::string key){
        if(ScopeNamespace.count(key)){
            return ScopeNamespace[key];
        }
        if(parent){
            return parent->findInNamespace(key);
        }
        return Token("null", NUL);
    }

    bool UpdateInNamespace(std::string key, Token newValue = Token("null", NUL)){
        if(ScopeNamespace.count(key)){
            ScopeNamespace[key] = newValue;
            return true;
        }
        if(parent){
            return parent->UpdateInNamespace(key, newValue);
        }
        return false;
    }

    Token interpretTree(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
        //std::cout<<"interpreting: ";displayToken(node->token.value);
        if(node->token.type == IDENTIFIER){
            ScopeNamespace.insert({node->token.value, Token("null", NUL)});
            return node->token;
        } else if(node->token.type == INT || node->token.type == FLOAT || node->token.type == CHAR){// || node->token.type == ARRAY || node->token.type == STRING
            return node->token;
        } else{
            Token right, left;
            if (node->right) {
                right = interpretTree(node->right);
                //std::cout<<"right: ";displayToken(right);
            } else{
                std::cout<<"ERROR interpreting no righr\n";
                return Token("null", NUL);
            }
            if (node->left) {
                left = interpretTree(node->left);
                //std::cout<<"left: ";displayToken(left);
            } else{
                std::cout<<"ERROR interpreting no left\n";
                return Token("null", NUL);
            }
            if(node->token.type == ASSIGN){
                if(left.type == IDENTIFIER){
                    UpdateInNamespace(left.value, (right.type == IDENTIFIER) ? findInNamespace(right.value) : right);// = (right.type == IDENTIFIER) ? findInNamespace(right.value) : right;
                    return left;
                } else{
                    std::cout<<"assigning error\n";
                }
            } else if(node->token.type == AMPERSAND){
                return findInNamespace(right.value);
            } else if(node->token.type == OPERATOR){
                if(left.type == IDENTIFIER){
                    left = findInNamespace(left.value);
                    //std::cout<<"left value: ";displayToken(left);
                }
                if(right.type == IDENTIFIER){
                    right = findInNamespace(right.value);
                    //std::cout<<"right value: ";displayToken(right);
                }
                if(node->token.value == "+"){
                    if(left.type == INT && right.type == INT){
                        return Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), INT);
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        return Token(std::to_string(std::stof(left.value) + std::stof(right.value)), FLOAT);
                    } else if(left.type == NUL){
                        return right;
                    } else if(right.type == NUL){
                        return left;
                    }
                } else if(node->token.value == "-"){
                    if(left.type == INT && right.type == INT){
                        return Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), INT);
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        return Token(std::to_string(std::stof(left.value) - std::stof(right.value)), FLOAT);
                    } else if(left.type == NUL){
                        return right;
                    } else if(right.type == NUL){
                        return left;
                    }
                } else if(node->token.value == "*"){
                    if(left.type == INT && right.type == INT){
                        return Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), INT);
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        return Token(std::to_string(std::stof(left.value) * std::stof(right.value)), FLOAT);
                    } else if(left.type == NUL){
                        return right;
                    } else if(right.type == NUL){
                        return left;
                    }
                } else if(node->token.value == "/"){
                    if(left.type == INT && right.type == INT){
                        return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), INT);
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        return Token(std::to_string(std::stof(right.value) != 0 ? std::stof(left.value) / std::stof(right.value) : 0), FLOAT);
                    } else if(left.type == NUL){
                        return right;
                    } else if(right.type == NUL){
                        return left;
                    }
                } else if(node->token.value == "%"){
                    if(left.type == INT && right.type == INT){
                        return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) % std::stoi(right.value) : 0), INT);
                    } else if(left.type == NUL){
                        return right;
                    } else if(right.type == NUL){
                        return left;
                    }
                } else if(node->token.value == "=="){
                    if(left.value == right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == "!="){
                    if(left.value != right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == ">"){
                    if(left.value > right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == ">="){
                    if(left.value >= right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == "<"){
                    if(left.value < right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == "<="){
                    if(left.value <= right.value){
                        return Token("1", INT);
                    } else{
                        return Token("0", INT);
                    }
                } else if(node->token.value == "&&"){
                    if(left.type == INT && right.type == INT){
                        if(stoi(left.value) > 0 && stoi(right.value) > 0){
                            return Token("1", INT);
                        } else{
                            return Token("0", INT);
                        }
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        if(stof(left.value) > 0 && stof(right.value) > 0){
                            return Token("1", INT);
                        } else{
                            return Token("0", INT);
                        }
                    } else if(left.type == NUL){
                        return Token("0", INT);
                    } else if(right.type == NUL){
                        return Token("0", INT);
                    }
                } else if(node->token.value == "||"){
                    if(left.type == INT || right.type == INT){
                        if(stoi(left.value) > 0 && stoi(right.value) > 0){
                            return Token("1", INT);
                        } else{
                            return Token("0", INT);
                        }
                    } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT) || (left.type == INT && right.type == FLOAT)){
                        if(stof(left.value) > 0 || stof(right.value) > 0){
                            return Token("1", INT);
                        } else{
                            return Token("0", INT);
                        }
                    } else if(left.type == NUL){
                        if(right.type == INT || right.type == FLOAT){
                            if(stoi(right.value) > 0){
                                return Token("1", INT);
                            } else{
                                return Token("0", INT);
                            }
                        }
                    } else if(right.type == NUL){
                        if(left.type == INT || left.type == FLOAT){
                            if(stoi(left.value) > 0){
                                return Token("1", INT);
                            } else{
                                return Token("0", INT);
                            }
                        }
                    }
                } else if(node->token.value == "!"){
                    if(left.type == INT){
                        return Token(std::to_string(-stoi(right.value)), INT);
                    } else if(left.type == FLOAT){
                        return Token(std::to_string(-stof(right.value)), FLOAT);
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
        std::vector<Token> output;
        if(!statementsTokens.empty()){
            for(std::vector<Token>& statement : statementsTokens){
                output.push_back(interpretTree(parse(statement)));
            }
        }

        if(!tokens.empty()){
            output.push_back(interpretTree(parse(tokens)));
        }

        if(output.size() == 1){
            return output.back();
        } else if(output.size() > 1){
            Arrays.push_back(output);
            return Token(type, ARRAY, Arrays.size()-1);
        } else {
            std::cerr<<"INTERPRETING ERROR no outputs\n";
            return Token("null",NUL);
        }
    }
};

