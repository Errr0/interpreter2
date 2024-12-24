#include "scopes.cpp"

void displayToken(Token token, int depth = 0){
    std::cout << std::string(depth * 5, ' ') << "(" << token.value << "," << displayTokenType(token.type) << "," << token.weight <<")\n";
     if(token.type == ARRAY){
        for(Token token : Arrays[token.weight]){
            displayToken(token, depth+1);
        }
    }
}

void interpreter(std::string& code){
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);

    Scope root = makeScopeTree(tokens);
    root.print();
    Token output = root.interpret();

    displayToken(output);
    std::cout<<"\n";

    std::cout<<"done\n";
}