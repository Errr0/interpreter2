#include "parser.cpp"

std::vector<std::vector<Token>> Arrays;

std::map<std::string, Token> GlobalNamespace = {
    {"_", Token("0", INT)},
    {"a", Token("2", INT)}
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
    std::vector<Token> statements;
    std::vector<std::vector<Token>> statementsTokens;
    std::vector<std::unique_ptr<Scope>> scopes;

    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    std::map<std::string, Token>  ScopeNamespace;
    std::vector<std::map<std::string, Token>*> namespaces;
    
    Scope(std::string type = "{}", std::vector<std::map<std::string, Token>*> namespaces = {}){
        this->type = type;
        this->namespaces = namespaces;
        this->namespaces.insert(this->namespaces.begin(), &ScopeNamespace);
    }

    void appendToken(Token token){
        tokens.push_back(token);
    }

    void makeStatement(){
        statementsTokens.push_back(tokens);
        tokens.clear();
        statements.push_back(Token(";", STATEMENT, statementsTokens.size()-1));
        
    }

    Scope* appendScope(const std::string& scopeType) {
        scopes.emplace_back(std::make_unique<Scope>(scopeType, namespaces));
        tokens.push_back(Token(scopeType, SCOPE, scopes.size()-1));
        return scopes.back().get();
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
            //std::cout << std::string((depth) * 5, ' ')<< "Statements: ";
            for(Token statement : statements){
                std::cout<<statement.weight<<displayType(statement.type)<<" ";
            }
            for(std::vector<Token> statement : statementsTokens){
                std::cout << "\n";
                std::cout << std::string(depth * 5, ' ') <<"Tokens: ";
                for(Token token : statement){
                    std::cout<<token.value<<displayType(token.type)<<((token.type == SCOPE) ? std::to_string(token.weight) : "")<<" ";
                }
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

    Node* parse(std::vector<Token> statement, std::map<Token, bool> expected = {}){
        for (const Token& token : statement) {
            //std::cout <<"parsing "<<token.value<<" "<<displayTokenType(token.type)<<" "<<token.weight<<"\n";
            if(!expected.empty()){
                std::cout <<"clearing expected";
                expected = {};
            } else if (token.type == SCOPE) {
                exprStack.push(new Node(scopes[token.weight] -> interpret()));
            } else if (token.type == ARRAY) {
                std::cout<<"arr\n";
                expected.insert({Token("[]", SCOPE), false});
                exprStack.push(new Node(token));
            } else if (token.type == FUNCTION_DECLARATION) {
                expected.insert({Token("()", SCOPE), true});
                expected.insert({Token("{}", SCOPE), false});
                exprStack.push(new Node(token));
            } else if (token.type == CLASS) {
                expected.insert({Token("()", SCOPE), false});
                expected.insert({Token("{}", SCOPE), true});
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
            processOperator();
        }
        if (exprStack.size() != 1) {
            std::cerr << "PARSER ERROR: Malformed expression!\n";
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
        //std::cout<<"()"<<depth<<","<<node->token.value<<","<<displayTokenType(node->token.type)<<")\n";
        if(node->token.type == IDENTIFIER){
            if(findInNamespace(node->token.value) == &nullvalue){
                ScopeNamespace.insert({node->token.value, Token("null", NUL)});
            }
            return node->token;
        } else if(node->token.type == ARRAY){
            return node->token;
        } else if(node->token.type == INT || node->token.type == FLOAT){
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
                    *findInNamespace(left.value) = (right.type == IDENTIFIER) ? *findInNamespace(right.value) : right;
                    return *findInNamespace(left.value);
                } else{
                    std::cout<<"assigning error\n";
                }
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
                output.push_back(interpretTree(parse(statementsTokens[statement.weight])));
            }
        } 

        if(!tokens.empty()){
            output.push_back(interpretTree(parse(tokens)));
        }

        if(output.size() == 1){
            return output.back();
        } else if(output.size() > 1){
            Arrays.push_back(output);
            return Token("array", ARRAY, Arrays.size()-1);
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
        if (token.type != BRACKET_OPEN && token.type != BRACKET_CLOSE && token.type != END) {
            currentScope->appendToken(token);
        } else if (token.type == END){
            currentScope->makeStatement();
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