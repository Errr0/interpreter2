#include "scopes.cpp"

void interpreter(std::string& code){
    eraseBetween(code, "/*", "*/");
    eraseBetween(code, "//", "\n");
    std::string stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    while(stringLiteral!=""){
        StringsLiterals.push(stringLiteral);
        std::string stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    }
    std::cout<<"\n"<<code<<"\n";

    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    Scope root = makeScopeTree(tokens);
    Token output = root.interpret();
    //displayToken(output);
    root.print();
    std::cout<<"\nprocess returned:\n";
    displayToken(output);
}