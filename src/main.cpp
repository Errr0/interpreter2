#include "parser.cpp" 

void printTokens(std::vector<Token> &tokens){
    for(Token token : tokens){
        std::cout << token.value << displayType(token.type)<< "|" << token.weight << "|";
    }
    std::cout<<"\n";
}

bool readfile(std::string filename, std::string &str){
    std::fstream file(filename, std::ios::in);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 0;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    str = buffer.str();
    file.close();
    return 1;
}

int main(int argc, char* argv[]){
    if(argc<2) {
        std::cerr << "Error no path argument!" << std::endl;
        return 1;
    }
    std::string code;
    if(!readfile(argv[1], code)) return 2;
    std::vector<Token> tokens;
    tokenizer(code,tokens);
    //printTokens(tokens);
    Node* root = parse(tokens);
    printTree(root);
    std::cout << "done\n";
    return 0;
}

