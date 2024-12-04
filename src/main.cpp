#include "include.cpp"
#include "tokenizer.cpp"

int main(int argc, char* argv[]){
    if(argc<2) {
        std::cerr << "Error no path argument!" << std::endl;
        return 1;
    }
    std::string code;
    if(!readfile(argv[1], code)) return 2;
    std::vector<Token> tokens;
    parse(code,tokens);
    for (Token& token : tokens){
        if(token.type == IDENTIFIER || token.type == NUMBER || token.type == INT || token.type == FLOAT || token.type == KEYWORD){
            std::cout <<token.value << "("<< display(token.type) <<")";
        } else{
            std::cout << display(token.type);
        }
    }
    //interpret
    return 0;
}

