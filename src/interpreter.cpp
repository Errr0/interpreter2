#include "scopes.cpp"

void interpreter(std::string& code){
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    print(tokens);
    Scope scopes = makeScopeTree(tokens);
    scopes.print();
    Token output = scopes.interpret();
    std::cout << "(" << output.value << "," << displayTokenType(output.type) << "," << output.weight <<")\n";
    std::cout<<"done\n";
}