#include "scopes.cpp"

void interpreter(std::string& code){
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    print(tokens);
    Scope root = makeScopeTree(tokens);
    root.print();
    Token output = root.interpret();
    std::cout << "\n";
    std::cout << "(" << output.value << "," << displayTokenType(output.type) << "," << output.weight <<")\n";
    if(output.type == ARRAY){
        for(Token token : Arrays[output.weight]){
            std::cout << "(" << token.value << "," << displayTokenType(token.type) << "," << token.weight <<")\n";
        }
    }
    root.print();
    std::cout<<"done\n";
}