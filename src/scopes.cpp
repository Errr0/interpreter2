#include "parser.cpp"

class Scope{
    public:
    Node* root;
    std::string type;
    std::vector<Token> tokens;
    //Scope* parent;
    std::vector<std::vector<Token>> statementsTokens;
    std::vector<Scope*> scopes;
int trash;
    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    std::map<std::string, Token>  ScopeNamespace;
    std::vector<std::map<std::string, Token>*> namespaces;
    
    Scope(std::string type = "{}", std::vector<std::map<std::string, Token>*> namespaces = {}){//, Scope* parent = nullptr
        this->type = type;
        this->namespaces = namespaces;
        this->namespaces.insert(this->namespaces.begin(), &ScopeNamespace);
        // this->parent = parent;
    }

    ~Scope(){
        for (Scope* scope : scopes) {
            delete scope;
        }
        for (auto* namespaceMap : namespaces) {
            delete namespaceMap;
        }
        delete root;
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
        scopes.push_back(new Scope(scopeType, namespaces));//, this
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
            std::cerr << "  operStackSize: "<<operatorStack.size()<<"\n";
            displayToken(operatorStack.top());
            std::cerr <<"\n";
            std::cerr << "  exprStackSize: "<<exprStack.size()<<"\n";
            displayToken(exprStack.top()->token);
            std::cerr <<"\n";
            std::cin >> trash;
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

    void updateArguments(Token arguments, std::vector<std::map<std::string, Token>*>& callNamespace){
        //std::cout<<"\n\n////////////////////////////////////////////////////////////////////////////\n";  
        if(findInNamespace("~~arguments_names~~")->type == ARRAY){
            if(arguments.type == ARRAY){
                for(size_t i = 0;i<std::min(Arrays[findInNamespace("~~arguments_names~~")->weight].size(), Arrays[arguments.weight].size());i++){
                    //std::cout<<Arrays[findInNamespace("~~arguments_names~~")->weight][i].value<<"\n";
                    //displayToken(((Arrays[arguments.weight][i].type == IDENTIFIER)?*findInNamespace(Arrays[arguments.weight][i].value, &callNamespace):Arrays[arguments.weight][i]));
                    //std::cout<<"\n";
                    *findInNamespace(Arrays[findInNamespace("~~arguments_names~~")->weight][i].value) = ((Arrays[arguments.weight][i].type == IDENTIFIER)?*findInNamespace(Arrays[arguments.weight][i].value, &callNamespace):Arrays[arguments.weight][i]);
                }
            } else{
                *findInNamespace(Arrays[findInNamespace("~~arguments_names~~")->weight][0].value) = (arguments.type == IDENTIFIER)?*findInNamespace(arguments.value, &callNamespace):arguments;
            }
        } else if(findInNamespace("~~arguments_names~~")->type == IDENTIFIER){
            *findInNamespace(findInNamespace("~~arguments_names~~")->value) = (arguments.type == IDENTIFIER)?*findInNamespace(arguments.value, &callNamespace):arguments;
        }      
        
        //displayToken(arguments);
        //print();
        //std::cout<<"\n\n\n";  
        updateChildsNamespaces();
        //print();
        //std::cout<<"\n////////////////////////////////////////////////////////////////////////////\n\n"; 
    }

    Token callExternalFunction(std::string name, Token& arguments = nullToken){
        if(name == "print"){
            if(arguments.type == ARRAY){
                for(Token arg : Arrays[arguments.weight]){
                    std::cout<<arg.value;
                }
            } else{//if(arguments.type == STRING)
                std::cout<<arguments.value;
            }
            return nullToken;
        } else if(name == "input"){
            callExternalFunction("print", arguments);
            std::string input;
            std::cin>>input;
            std::vector<Token> string;
            for(char c: input){
                string.push_back(Token(std::string(1, c), CHAR));
            }
            Arrays.push_back(string);
            return Token(input, STRING, Arrays.size()-1);
        } else if(name == ""){

        } else if(name == ""){

        } else if(name == ""){

        }
        return nullToken;
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
                        if(externalFunctions.find(token.value) != externalFunctions.end()){
                            callExternalFunction(token.value);
                            skip=1;
                        } else 
                        if((&token+2)->type == SCOPE && (&token+2)->value == "{}"){
                            scopes[(&token+1)->weight]->ScopeNamespace["~~arguments_names~~"] = scopes[(&token+1)->weight]->interpret();
                            scopes[(&token+1)->weight]->copy(scopes[(&token+2)->weight]);
                            scopes[(&token+1)->weight]->type = "function";
                            functions[token.value] = *scopes[(&token+1)->weight];
                            skip = 2;
                        } else{
                            if(functions.count(token.value)){
                                functions[token.value].updateArguments(scopes[(&token+1)->weight]->interpret(), scopes[(&token+1)->weight]->namespaces);
                                exprStack.push(new Node(functions[token.value].interpret()));
                            } else{
                                std::cerr << "FUNCTION ERROR: function doesent exist"<<displayTokenType(token.type)<<"\n";
                            }
                            skip = 1;
                        }
                    }
                } else if(token.type == KEYWORD){
                    if(token.value == "if"){
                        if((&token+1)->type == SCOPE && (&token+1)->value == "()"){ 
                            if((&token+2)->type == SCOPE && (&token+2)->value == "{}"){
                                skip = 2;
                                Token condition = scopes[(&token+1)->weight]->interpret();
                                //bool isTrue = false;
                                if(condition.type == INT && stoi(condition.value) > 0){//maybe to combine to one BIG if with ||
                                    exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                } else if(condition.type == FLOAT && stof(condition.value) > 0){
                                    exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                } else if(condition.type == CHAR && condition.value != ""){
                                    exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                } else if((condition.type == ARRAY || condition.type == STRING) && Arrays[condition.weight].size() > 0){
                                    exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                } else{
                                }
                                //scopes[(&token+1)->weight];
                                //exprStack.push(new Node(scopes[(&token+2)->weight]->interpret()));
                                
                            } else{
                                std::cerr << "IF ERROR: body doesent exist"<<displayTokenType(token.type)<<"\n";
                                skip = 1;
                                std::cin >> trash;
                            }
                        } else{
                                std::cerr << "IF ERROR: condition doesent exist"<<displayTokenType(token.type)<<"\n";
                                std::cin >> trash;
                                //skip = 1;
                            }
                    } else if((&token+1)->type == SCOPE && (&token+1)->value == "{}"){
                        scopes[(&token+1)->weight]->type = "class";
                        classes.insert({token.value, *scopes[(&token+1)->weight]});
                        skip=1;
                    } else{
                        exprStack.push(new Node(token));
                    }
                } else if (token.type == ARRAY){
                    exprStack.push(new Node(token));
                } else if (token.type == INT || token.type == FLOAT || token.type == STRING || token.type == CHAR) {
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
                    std::cin >> trash;
                    return nullptr;
                }
            }
        //std::cout<<"D\n";
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
        //std::cout<<"U\n";
        return return_value;
    }
    
    Token* findInNamespace(std::string key, std::vector<std::map<std::string, Token>*>* NS = nullptr){
        if(NS){
            for(std::map<std::string, Token>* ns : *NS){
                if((*ns).count(key)){
                    return &(*ns)[key];
                }
            }
        } else{
            for(std::map<std::string, Token>* ns : namespaces){
                if((*ns).count(key)){
                    return &(*ns)[key];
                }
            }
        }
        return &nullToken;
    }

    Token interpretTree(Node* node, int depth = 0, bool retrurning = false, Token retrurningValue = Token()){
        //std::cout<<"interpreting: ";displayToken(node->token.value);
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
                    *findInNamespace(left.value) = (right.type == IDENTIFIER) ? *findInNamespace(right.value) : right;
                    return left;
                } else{
                    std::cout<<"assigning error\n";
                }
            } else if(node->token.type == AMPERSAND){
                return *findInNamespace(right.value);
            } else if(node->token.type == OPERATOR){
                if(left.type == IDENTIFIER){
                    left = *findInNamespace(left.value);
                    //std::cout<<"left value: ";displayToken(left);
                }
                if(right.type == IDENTIFIER){
                    right = *findInNamespace(right.value);
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

