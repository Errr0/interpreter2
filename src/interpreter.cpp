#include "scopes.cpp"

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