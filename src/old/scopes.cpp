#include "parser.cpp"

void displayToken(Token token, int depth = 0){
    std::cout << std::string(depth * 5, ' ') << "(" << token.value << "," << displayTokenType(token.type) << "," << token.weight <<")\n";
     if(token.type == ARRAY){
        for(Token token : Arrays[token.weight]){
            displayToken(token, depth+1);
        }
    } else if(token.type == STRING){
        for(Token token : Strings[token.weight]){
            displayToken(token, depth+1);
        }
    }
}

std::map<std::string, Token> GlobalNamespace = {
    //{"", Token("0", INT)}
    //{"a", Token("2", INT)}
};

std::map<std::string, Token> BuildInNamespace = {
    {"PI", Token("3.14", FLOAT)}
};

std::vector<std::map<std::string, Token>*> BuildInNamespaces = {
    &BuildInNamespace
};

class Scope{
    public:
    Node* root;
    std::string type;
    std::vector<Token> tokens;
    std::vector<Token> statements;//to delete
    std::vector<std::vector<Token>> statementsTokens;
    std::vector<Scope*> scopes;

    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    std::map<std::string, Token>  ScopeNamespace;
    std::vector<std::map<std::string, Token>*> namespaces;
    
    Scope(std::string type = "{}", std::vector<std::map<std::string, Token>*> namespaces = {}){
        this->type = type;
        this->namespaces = namespaces;
        this->namespaces.insert(this->namespaces.begin(), &ScopeNamespace);
    }//to do working destructor

    void appendToken(Token token){
        tokens.push_back(token);
    }

    void makeStatement(std::string type){
        statementsTokens.push_back(tokens);
        tokens.clear();
        statements.push_back(Token(type, STATEMENT, statementsTokens.size()-1));
        
    }

    Scope* appendScope(const std::string& scopeType) {
        scopes.push_back(new Scope(scopeType, namespaces));
        tokens.push_back(Token(scopeType, SCOPE, scopes.size()-1));
        return scopes.back();
    }

    void copy(Scope* second){
        this->tokens = second->tokens;
        this->statements = second->statements;
        this->statementsTokens = second->statementsTokens;
        this->scopes = second->scopes;
    }

    void printNamespaces(int depth = 0){
        for(std::map<std::string, Token>* ns : namespaces){
            //std::cout<<std::string(depth * 5, ' ')<<"namespace:\n";
            for(auto pair : *ns){
                std::cout<<std::string(depth * 5, ' ')<<"("<<pair.first<<","<<"("<<pair.second.value<<","<<displayTokenType(pair.second.type)<<","<<pair.second.weight<<")"<<")\n";
            }
        }
    }

    void print(int depth = 0){
        std::cout << std::string(depth * 5, ' ') << "Type: "<<type<<" Tokens: ";
        if(tokens.empty()){
            std::cout<<"EMPTY";
        } else {
            for(Token token : tokens){
                std::cout<<token.value<<displayType(token.type)<<((token.type == SCOPE) ? std::to_string(token.weight) : "")<<" ";
            }
        }
        std::cout <<"\n"<< std::string((depth) * 5, ' ')<<"Namespaces:\n";
        printNamespaces(depth);
        if(!statements.empty()){
            for(std::vector<Token> statement : statementsTokens){
                std::cout << std::string(depth * 5, ' ') <<"Tokens: ";
                for(Token token : statement){
                    std::cout<<token.value<<displayType(token.type)<<((token.type == SCOPE) ? std::to_string(token.weight) : "")<<" ";
                }
                std::cout << "\n";
            }
        }
        std::cout<<"\n";
        for (const auto& scope : scopes) {
            scope->print(depth + 1);
        }
    }

    void updateChildsNamespaces(){
        for(Scope* scope : scopes){
            scope->namespaces = this->namespaces;
            scope->updateChildsNamespaces();
            //scope->print();
        }
    }

    void processOperator() {
            if(expectingElse){
                exprStack.pop();
            }
            expectingElse = false; 
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
    
    void addExpected(Token& token, std::queue<pair> &expected){
        if (token.type == ARRAY) {
            expected.push(pair(Token("[]", SCOPE), false));
            //std::cout<<expected[expected.size()-1].token.value;
        } else if (token.type == FUNCTION_DECLARATION) {
            expected.push(pair(Token("()", SCOPE), true));
            expected.push(pair(Token("{}", SCOPE), true));
        } else if (token.type == FUNCTION) {
            expected.push(pair(Token("()", SCOPE), true));
        // } else if (token.type == CLASS) {
        //     expected.push(pair(Token("()", SCOPE), false));
        //     expected.push(pair(Token("{}", SCOPE), true));
        // } else if (token.type == OBJECT) {
        //     expected.push(pair(Token("()", SCOPE), false));
        //     expected.push(pair(Token(".", DOT), false));
        } else if(token.type == KEYWORD){
            if(token.value == "if"){
                expected.push(pair(Token("()", SCOPE), true));
                expected.push(pair(Token("{}", SCOPE), true));
            } else if(token.value == "else"){
                expected.push(pair(Token("if", KEYWORD), false));
                expected.push(pair(Token("{}", SCOPE), true));
            }
        } else if (token.type == IDENTIFIER){
            addExpected(*findInNamespace(token.value), expected);
        }
    }

    void processExpected(Token& token, Token& last, std::queue<pair> &expected, bool& expectingElse){
        //std::cout <<"Token: ";displayToken(token);
        //std::cout <<"Last: ";displayToken(last);
        if(token.type == NUL || last.type == NUL){
            std::cerr<<"ERROR unexpected null value";
            return;
        } else if(last.type == ARRAY){
            //std::cout<<"processing array\n";
            if(token.type == SCOPE && token.value == "[]"){
                Token scope = scopes[token.weight] -> interpret();
                if(scope.type == INT){
                    if((long long unsigned int)stoi(scope.value) < Arrays[last.weight].size()){
                        //exprStack.pop();
                        //exprStack.push(new Node(Arrays[last.weight][stoi(scope.value)]));
                        exprStack.top()->token = Arrays[last.weight][stoi(scope.value)];
                        //displayToken(Arrays[last.weight][stoi(scope.value)]);
                    } else{
                        std::cerr<<"ERROR out of array scope";
                        exprStack.pop();
                        exprStack.push(new Node(Token("null", NUL)));//to do pusing random thing from void
                    }
                }
            }
        } else if(last.type == FUNCTION_DECLARATION){
            if(token.type == SCOPE && token.value == "()"){
                scopes[token.weight]->ScopeNamespace.insert({"~~arguments~~", scopes[token.weight]->interpret()});
                scopes[token.weight]->type = "function";
                functions.push_back(*scopes[token.weight]);
                last.weight = functions.size()-1;
            } else if(token.type == SCOPE && token.value == "{}"){
                if(last.weight == -1){
                    std::cerr<<"ERROR expected () after function name\n";
                } else{
                    functions[last.weight].copy(scopes[token.weight]);
                }
                ScopeNamespace.insert({last.value, Token(last.value, FUNCTION, last.weight)});
                exprStack.pop();
            }
        } else if(last.type == FUNCTION){
            if(token.type == SCOPE && token.value == "()"){
                //std::cout<<"==========================================================function call\n";
                Token scope = scopes[token.weight] -> interpret();
                //displayToken(scope);
                displayToken(*functions[last.weight].findInNamespace("~~arguments~~"));
                displayToken(scope);
                if(functions[last.weight].findInNamespace("~~arguments~~")->type == NUL){
                    std::cerr<<"function takes no arguments but arguments are given\n";
                } else {
                    if(scope.type == ARRAY){
                        if(functions[last.weight].findInNamespace("~~arguments~~")->type == ARRAY){
                            if(Arrays[scope.weight].size()>Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight].size()){
                                std::cerr<<"ERROR too many arguments passed to function. Number of arguments passed: "<<Arrays[scope.weight].size()<<" expected: "<<functions[last.weight].ScopeNamespace.size()<<"\n";
                            }
                            for(long long unsigned int i = 0; i < std::min(Arrays[scope.weight].size(), Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight].size()); i++){
                                if(Arrays[scope.weight][i].type == IDENTIFIER){
                                    *functions[last.weight].findInNamespace(Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight][i].value) = *findInNamespace(Arrays[scope.weight][i].value);
                                } else{
                                    *functions[last.weight].findInNamespace(Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight][i].value) = Arrays[scope.weight][i];
                                }
                            }
                        } else if(functions[last.weight].findInNamespace("~~arguments~~")->type == IDENTIFIER){
                            if(Arrays[scope.weight][0].type == IDENTIFIER){
                                *functions[last.weight].findInNamespace(functions[last.weight].findInNamespace("~~arguments~~")->value) = *findInNamespace(Arrays[scope.weight][0].value);
                            } else{
                                *functions[last.weight].findInNamespace(functions[last.weight].findInNamespace("~~arguments~~")->value) = Arrays[scope.weight][0];
                            }
                        }
                    } else if(scope.type == IDENTIFIER){
                        if(functions[last.weight].findInNamespace("~~arguments~~")->type == ARRAY){
                            *functions[last.weight].findInNamespace(Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight][0].value) = *findInNamespace(scope.value);
                        } else{
                            *functions[last.weight].findInNamespace(functions[last.weight].findInNamespace("~~arguments~~")->value) = *findInNamespace(scope.value);
                        }
                    } else if(scope.type == INT || scope.type == FLOAT){
                        if(functions[last.weight].findInNamespace("~~arguments~~")->type == ARRAY){
                            *functions[last.weight].findInNamespace(Arrays[functions[last.weight].findInNamespace("~~arguments~~")->weight][0].value) = scope;
                        } else{
                            *functions[last.weight].findInNamespace(functions[last.weight].findInNamespace("~~arguments~~")->value) = scope;
                        }
                    }
                }
                exprStack.pop();
                functions[last.weight].updateChildsNamespaces();
                //functions[last.weight].print();
                Token temp = functions[last.weight].interpret();//error here     terminate called after throwing an instance of 'std::logic_error'      what():  basic_string: construction from null is not valid
                //displayToken(temp);
                exprStack.push(new Node(temp));//todo oneliner of inserting
                //std::cout<<"==========================================================done\n";
            }
        // } else if(last.type == CLASS){
            
        // } else if(last.type == OBJECT){

        } else if(last.type == KEYWORD){
            if(last.value == "if"){
                if(token.type == SCOPE && token.value == "()" && last.weight == 0){
                    Token scope = scopes[token.weight] -> interpret();
                    if(stoi(scope.value) > 0){
                        last.weight = 1;
                    } else{
                        last.weight = -1;
                    }
                } else if(token.type == SCOPE && token.value == "{}"){
                    if(last.weight){
                        exprStack.pop();
                        exprStack.push(new Node(scopes[token.weight] -> interpret()));
                    }else{
                        expectingElse = true;
                    }
                }
            } else if(last.value == "else"){
                exprStack.pop();
                last = (*exprStack.top()).token;
                if(last.type == KEYWORD && last.value == "if" && last.weight == -1){
                    exprStack.pop();
                    if(token.type == KEYWORD && token.value == "if"){
                        exprStack.push(new Node(token));
                        expected.pop();
                    } else if(token.type == SCOPE && token.value == "{}"){
                        exprStack.push(new Node(scopes[token.weight] -> interpret()));
                    }
                } else{
                    if(token.type == KEYWORD && token.value == "if"){
                        token.weight = -1;
                        expected.pop();
                    }
                }
            } else if(last.value == "for"){

            } else if(last.value == "while"){

            }
        } else if(last.type == IDENTIFIER){
            //std::cout<<"getting identifier value\n";
            //displayToken(*findInNamespace(last.value));
            processExpected(token, *findInNamespace(last.value), expected, expectingElse);
        }
    }
    bool expectingElse = false;
    Node* parse(std::vector<Token> statement){
        std::queue<pair> expected = {};
        bool skip = false;
        //bool expectingElse = false;
        for (Token& token : statement) {
            //std::cout <<"parsing "<<token.value<<" "<<displayTokenType(token.type)<<" "<<token.weight<<"\n";
            if(expectingElse && token.value != "else" && token.type != KEYWORD){
                exprStack.pop();
            }
            expectingElse = false;
            while(!expected.empty()){
                if(token.value == expected.front().token.value && token.type == expected.front().token.type){
                    Token last = (*exprStack.top()).token;
                    processExpected(token, (*exprStack.top()).token, expected, expectingElse);
                    expected.pop();
                    skip = true;
                    break;
                } else if(expected.front().required){
                    std::cerr<<"ERROR token expected: ";displayToken(expected.front().token);
                    std::cerr<<"ERROR token given: ";displayToken(token);
                    std::cerr<<"ERROR token last: ";displayToken((*exprStack.top()).token);
                    return nullptr;
                }else{
                    std::cout<<"token not required\n";
                    expected.pop();
                }
            }
            if(skip){
                skip = false;
            } else{
                if (token.type == SCOPE) {
                    Token scope = scopes[token.weight] -> interpret();
                    addExpected(scope, expected);
                    exprStack.push(new Node(scope));
                } else if(token.type == KEYWORD){
                    if(token.value == "if"){
                        addExpected(token, expected);
                        exprStack.push(new Node(token));
                    } else if(token.value == "else"){
                        addExpected(token, expected);
                        exprStack.push(new Node(token));
                    }

                } else if (token.type == ARRAY || token.type == FUNCTION_DECLARATION || token.type == FUNCTION || token.type == CLASS || token.type == OBJECT || token.type == IDENTIFIER) {
                    addExpected(token, expected);
                    exprStack.push(new Node(token));
                } else if (token.type == INT || token.type == FLOAT || token.type == STRING || token.type == CHAR) {
                    exprStack.push(new Node(token));
                } else if (token.type == ARITMETIC_OPERATOR || token.type == ASSIGN || token.type == AMPERSAND || token.type == LOGICAL_OPERATOR) {
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
        }
        if(expectingElse){
            exprStack.pop();
        }
        expectingElse = false;  
        // Process remaining operators
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

    Token* findInNamespace(std::string key){
        for(std::map<std::string, Token>* ns : namespaces){
            if((*ns).count(key)){
                return &(*ns)[key];
            }
        }
        return &nullvalue;
    }

    Token interpretTree(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
        //printTree(node);
        //std::cout<<"("<<depth<<","<<node->token.value<<","<<displayTokenType(node->token.type)<<")\n";
        if(node->token.type == IDENTIFIER){
            //displayToken(node->token);

            if(findInNamespace(node->token.value) == &nullvalue){
                //std::cout<<"========================\n";
                //printNamespaces();
                //std::cout<<"new identifier ";displayToken(node->token);
                ScopeNamespace.insert({node->token.value, Token("null", NUL)});
                //printNamespaces();
                //std::cout<<"========================\n";
            }
            return node->token;
        // } else if(node->token.type == ARRAY){
        //     return node->token;
        } else if(node->token.type == INT || node->token.type == FLOAT || node->token.type == STRING || node->token.type == CHAR || node->token.type == ARRAY){
            return node->token;
        } else{
            //std::cout<<"else";
            Token right, left;
            if (node->right) {
                right = interpretTree(node->right);
            } else{
                std::cout<<"ERROR interpreting no righr\n";
                return Token("null", NUL);
            }
            if (node->left) {
                left = interpretTree(node->left);
            } else{
                std::cout<<"ERROR interpreting no left\n";
                return Token("null", NUL);
            }
            if(node->token.type == ASSIGN){
                if(left.type == IDENTIFIER){
                    *findInNamespace(left.value) = (right.type == IDENTIFIER) ? *findInNamespace(right.value) : right;
                    return *findInNamespace(left.value);
                } else{
                    std::cout<<"assigning error\n";
                }
            } else if(node->token.type == AMPERSAND){
                return *findInNamespace(right.value);
            } else if(node->token.type == ARITMETIC_OPERATOR){
                if(left.type == IDENTIFIER){
                    left = *findInNamespace(left.value);
                    if(left.type == NUL){
                        left = Token(0,INT);
                    }
                }
                if(right.type == IDENTIFIER){
                    right = *findInNamespace(right.value);
                    if(right.type == NUL){
                        right = Token(0,INT);
                    }
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
        std::vector<Token> output;
        if(!statements.empty()){
            for(Token& statement : statements){
                // Node* node = parse(statementsTokens[statement.weight]);
                // printTree(node);
                // output.push_back(interpretTree(node));
                output.push_back(interpretTree(parse(statementsTokens[statement.weight])));
            }
        }

        if(!tokens.empty()){
            // Node* node = parse(tokens);
            // printTree(node);
            // output.push_back(interpretTree(node));
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

Scope makeScopeTree(std::vector<Token>& tokens) {
    Scope root("{}", BuildInNamespaces);
    root.ScopeNamespace = GlobalNamespace;
    Scope* currentScope = &root;
    std::stack<Scope*> scopeStack;
    for (const Token& token : tokens) {
        if (token.type != BRACKET_OPEN && token.type != BRACKET_CLOSE && token.type != END && token.type != COMMA) {
            currentScope->appendToken(token);
        } else if (token.type == END || token.type == COMMA){
            currentScope->makeStatement((token.type == END)?";":",");
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