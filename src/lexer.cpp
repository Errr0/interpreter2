// #include "token.cpp"
#include <string>
#include <vector>
#include <iostream>

class Lexer{
    std::string code;
    std::vector<std::string> strings;
    // std::vector<Token> tokens;
    public:
    Lexer(std::string code){
        this->strings.push_back(code);
        splitScopes('{', '}');
        //splitScopes('(', ')');
        //splitScopes('[', ']');
        print();
    }
    void tokenize(){

    }

    void splitScopes(char open = '[', char close = ']'){
        std::string code;
        for(long long unsigned int i = 0; i<strings.size(); i++){
            code = strings[i];
            size_t startPos = code.find(open);
            while (startPos != std::string::npos) {
            size_t endPos = code.find(close, startPos + 1);
                if (endPos != std::string::npos){
                    strings[i] = code.substr(0, startPos);
                    strings.insert(strings.begin()+i, code.substr(startPos, endPos));
                    strings.insert(strings.begin()+i+1, code.substr(endPos, 0));
                    i+=2;
                    //std::cout<<" | "<<strings[i]<<" | "<<strings[i+1]<<" | "<<strings[i+2]<<" | ";
                }
                startPos = code.find(open, endPos+1);
            }
        }
    }

    void print(){
        for(std::string s: strings){
            std::cout<<" | "<<s;
            std::cout<<" |\n";
        }
    }
};