#include "token.cpp"
#include <string>
#include <vector>

class Lexer{
    std::string code;
    std::vector<Token> tokens;
    public:
    Lexer(std::string code){
        this->code = code;
        
    }
    void tokenize(){

    }
};