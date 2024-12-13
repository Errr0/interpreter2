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
    if(isDatatype(str)){
        tokens.push_back(Token(str,DATATYPE));
    } else{
        tokens.push_back(Token(str,KEYWORD));
    }
}

void addIdentifier(std::string str, std::vector<Token> &tokens){
    if(isKeyword(str)){
        addKeyword(str, tokens);    
    } else {
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

void lexer(std::string &str, std::vector<Token> &arr){
    std::vector<std::string> words;
    split(str,words);
    for (std::string& symbol : symbols) {
        std::vector<std::string> temp;
        for (std::string& substr : words) {
            if(substr.find(symbol) != std::string::npos){
                split(substr, temp, symbol, true, true, true);
            } else {
                temp.push_back(substr);
            }
        }
        words = std::move(temp);
    }
    tokenize(words, arr);
}

void tokenizer(std::string &str, std::vector<Token> &tokens){
    std::vector<std::string> statements;
    split(str, statements, "\n", true, true, true);
    for (std::string& statement : statements) {
        std::vector<std::string> splittedStatement;
        split(statement,splittedStatement);
        lexer(statement, tokens);
    }
}