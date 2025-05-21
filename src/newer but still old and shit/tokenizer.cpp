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

void addMinus(std::vector<Token> &tokens){
    if(!tokens.empty()){
        if(tokens.back().type == OPERATOR || tokens.back().type == ASSIGN){
            tokens.push_back(Token("0", INT));
            tokens.push_back(Token("-", OPERATOR, values[3]));

        } else{
            tokens.push_back(Token("-", OPERATOR, values[1]));
        }       
    }
}

void addAssign(Token token, std::vector<Token> &tokens){
    if(!tokens.empty()){
        Token temp = tokens.back();
        tokens.push_back(locked["~ASSIGN"]);
        tokens.push_back(temp);
        tokens.push_back(token);
    }
}

void tokenize(std::vector<std::string> arr, std::vector<Token> &tokens){
    for (std::string str : arr) {
        if(str[0]=='~'){
            if(str=="~STRING"){
                std::vector<Token> string;
                for(char c: StringsLiterals.front()){
                    string.push_back(Token(std::string(1, c), CHAR));
                }
                Arrays.push_back(string);
                tokens.push_back(Token(StringsLiterals.front(), STRING, Arrays.size()-1));
                StringsLiterals.pop();
            } else if(str=="~CHAR"){
                tokens.push_back(Token(std::string(1, Chars.front()[1]), CHAR));
                Chars.pop();
            } else if(str=="~INCREMENT"){
                int temp = tokens.size()-1;
                addAssign(locked["~ADDASSIGN"], tokens);
                tokens.push_back(Token("1", INT));
                tokens.insert(tokens.begin()+temp, locked["~BRACKETOPEN"]);
                tokens.push_back(locked["~BRACKETCLOSE"]);
            } else if(str=="~DECREMENT"){
                int temp = tokens.size()-1;
                addAssign(locked["~SUBASSIGN"], tokens);
                tokens.push_back(Token("1", INT));
                tokens.insert(tokens.begin()+temp, locked["~BRACKETOPEN"]);
                tokens.push_back(locked["~BRACKETCLOSE"]);
            } else if(str.substr(3, 6) == "ASSIGN"){
                addAssign(locked[str], tokens);
            } else if(str=="~MINUS"){
                addMinus(tokens);
            // } else if(str=="~AMPERSAND"){
            //     tokens.push_back(locked[str]);
            } else if(str=="~SPACE" || str=="~TAB" || str=="~ENDLINE"){
                continue;
            } else{
                tokens.push_back(locked[str]);
            }
        } else if(isNumber(str)){
            addNumberToken(str, tokens);
        } else if(isIdentifier(str)){
            tokens.push_back(Token(str,isKeyword(str)?KEYWORD:IDENTIFIER));
        }
    }
}

void tokenizer(std::string &input, std::vector<std::vector<Token>> &output){
    std::vector<std::string> statements;
    split(input, statements, ";");
    for (std::string& statement : statements) {
        std::vector<Token> tokens;
        std::vector<std::string> strings;
        splitString(statement, operators, strings);
        tokenize(strings, tokens);
        if(!tokens.empty())
            output.push_back(tokens);
    }
}