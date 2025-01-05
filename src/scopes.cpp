#include "parser.cpp"

class Scope{//TODO parsing rework.... again
    public:
    Node* root;
    std::string type;
    std::vector<Token> tokens;
    //std::vector<Token> statements;//to delete
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
    }

    ~Scope(){

    }

    void appendToken(Token token){
        tokens.push_back(token);
    }

    void makeStatement(std::string type){
        statementsTokens.push_back(tokens);
        tokens.clear();
        //statements.push_back(Token(type, STATEMENT, statementsTokens.size()-1));
        
    }

    Scope* appendScope(const std::string& scopeType) {
        scopes.push_back(new Scope(scopeType, namespaces));
        tokens.push_back(Token(scopeType, SCOPE, scopes.size()-1));
        return scopes.back();
    }

    void copy(Scope* second){
        this->tokens = second->tokens;
        this->statementsTokens = second->statementsTokens;
        this->scopes = second->scopes;
    }

    void updateChildsNamespaces(){
        for(Scope* scope : scopes){
            scope->namespaces = this->namespaces;
            scope->updateChildsNamespaces();
        }
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
            std::cout <<"parsing ";displayToken(token);
            std::cout <<"next ";displayToken(*(&token+1));
                if (token.type == SCOPE) {
                    Token scope = scopes[token.weight] -> interpret();
                    exprStack.push(new Node(scope));
                } else if (token.type == IDENTIFIER) {
                    if((&token+1)->type == SCOPE && (&token+1)->value == "()"){
                        if(functions.count(token.value)){
                            if((&token+2)->type == SCOPE && (&token+2)->value == "{}"){
                                scopes[(&token+2)->weight]->ScopeNamespace.insert({"~~arguments~~", scopes[(&token+1)->weight]->interpret()});
                                scopes[(&token+2)->weight];
                                functions.insert({token.value, *scopes[(&token+2)->weight]});
                            } else{
                                std::cerr<<"";
                            }
                        } else{
                            exprStack.push(new Node(token));
                        }
                        //token.type == FUNCTION;
                    }
                    
                } else if (token.type == ARRAY){
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
        return &nullToken;
    }

    Token interpretTree(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
        if(node->token.type == IDENTIFIER){
            if(findInNamespace(node->token.value) == &nullToken){
                ScopeNamespace.insert({node->token.value, Token("null", NUL)});
            }
            return node->token;
        } else if(node->token.type == INT || node->token.type == FLOAT || node->token.type == STRING || node->token.type == CHAR || node->token.type == ARRAY){
            return node->token;
        } else{
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

