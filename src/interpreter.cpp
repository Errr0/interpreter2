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
    std::cout << "(" << output.value << "," << displayTokenType(output.type) << "," << output.weight <<")\n";
    // for(Token statement : root.statements){
    //     Token output = root.scopes[statement.weight]->interpret();
    //     std::cout << "(" << output.value << "," << displayTokenType(output.type) << "," << output.weight <<")\n";
    // }
    //Token output = scopes.interpret();
    
    std::cout<<"done\n";
}