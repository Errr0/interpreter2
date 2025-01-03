#include "scopes.cpp"

void interpreter(std::string& code){
    processInput(code);
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    Scope root = makeScopeTree(tokens);
    Token output = root.interpret();
    //displayToken(output);
    root.print();
    std::cout<<"\nprocess returned: ";
    displayToken(output);
}