#include "strings.cpp"

void addNumberToken(std::string str, std::vector<Token> &tokens){
    if(tokens.size()>=2){
        if((tokens[tokens.size() - 1].type == DOT && tokens[tokens.size() - 2].type == INT) || tokens[tokens.size() - 1].type == INT){
            if(tokens[tokens.size() - 1].type == DOT)
                tokens.pop_back();
            std::string temp = tokens.back().value;
            tokens.pop_back();
            tokens.push_back(Token(temp+"."+str,FLOAT));
            return;
        }
    }
    tokens.push_back(Token(str,INT));
}

void addAssign(Token token, std::vector<Token> &tokens){
    if(!tokens.empty()){
        Token temp = tokens.back();
        tokens.push_back(locked["~ASSIGN"]);
        tokens.push_back(temp);
        tokens.push_back(token);
    }
}

void addKeyword(std::string str, std::vector<Token> &tokens){
    // if(str == "def"){
    //     tokens.push_back(Token(str,FUNCTION_DECLARATION));
    // } else if(str == "class"){
    //     tokens.push_back(Token(str,CLASS));
    //} else if(isDatatype(str)){
        //tokens.push_back(Token(str,DATATYPE));
    //} else{
        tokens.push_back(Token(str,KEYWORD));
    //}
}

void addIdentifier(std::string str, std::vector<Token> &tokens){
    if(isKeyword(str)){
        addKeyword(str, tokens);    
    } else {
        if(!tokens.empty()){
            if(tokens.back().type == KEYWORD){
                if(tokens.back().value == "def"){
                    tokens.pop_back();
                    tokens.push_back(Token(str,FUNCTION_DECLARATION));
                    return;
                } else if(tokens.back().value == "class"){
                    tokens.pop_back();
                    tokens.push_back(Token(str,CLASS));
                    return;
                }
            }
        }
        tokens.push_back(Token(str,IDENTIFIER));
    }
}

void tokenize(std::vector<std::string> arr, std::vector<Token> &tokens){
    for (std::string str : arr) {
        if(str[0]=='~'){
            if(str=="~INCREMENT"){
                tokens.push_back(locked["~ADDASSIGN"]);
                tokens.push_back(Token("1", INT));
            } else if(str=="~DECREMENT"){
                tokens.push_back(locked["~SUBASSIGN"]);
                tokens.push_back(Token("1", INT));
            } else if(str=="~ADDASSIGN"){
                addAssign(locked["~ADDASSIGN"], tokens);
            } else if(str=="~SUBASSIGN"){
                addAssign(locked["~SUBASSIGN"], tokens);
            } else if(str=="~MULASSIGN"){
                addAssign(locked["~MULASSIGN"], tokens);
            } else if(str=="~DIVASSIGN"){
                addAssign(locked["~DIVASSIGN"], tokens);
            } else if(str=="~MODASSIGN"){
                addAssign(locked["~MODASSIGN"], tokens);
            } else if(str=="~SPACE" || str=="~TAB" || str=="~ENDLINE"){
                continue;
            } else{
                tokens.push_back(locked[str]);
            }
        } else if(isNumber(str)){
            addNumberToken(str, tokens);
        } else if(isIdentifier(str)){
            addIdentifier(str, tokens);
        }
    }
}

void print(std::vector<Token> &tokens){
    for(Token token : tokens){
        std::cout << token.value << displayType(token.type)<<" ";//<< "|" << token.weight << "|";
    }
    std::cout<<"\n";
}

void tokenizer(std::string &input, std::vector<std::vector<Token>> &output){
    std::vector<std::string> statements;
    split(input, statements, ";");
    for (std::string& statement : statements) {
        std::vector<Token> tokens;
        std::vector<std::string> strings;
        splitString(statement, operators, strings);
        tokenize(strings, tokens);
        print(tokens);
        if(!tokens.empty())
            output.push_back(tokens);
    }
}