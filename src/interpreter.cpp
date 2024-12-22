#include "scopes.cpp"


//variables.insert();
//variables["b"] = Variable("b");



void interpreter(std::string& code){
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    print(tokens);
    Scope scopes = makeScopeTree(tokens);
    scopes.print();
    
    //replace(code, "\n", "|n|");
    //replace(code, "    ", "|t|");
    //std::vector<std::string> scopes = {"{"};
    //std::vector<std::string> scopeSymbols = {"{", "}"};
    //splitString(code, scopeSymbols, scopes);
    //scopes.push_back("~CURLYBRACKETCLOSE");

    // for(std::string scope : scopes){
    //     std::cout << scope <<"\n";
    // } std::cout << "\n";
    // //Scope* rootScope = parseScopes(scopes);
    //printScopes(rootScope);
    // for (std::string& statement : scopes) {
    //     std::vector<Token> tokens;
    //     std::vector<std::string> strings;
    //     lexer(statement, strings, operators);
    //     tokenize(strings, tokens);
    //     //print(tokens);
    //     if(!tokens.empty())
    //         tokensArr.push_back(tokens);
    // }


    // std::vector<std::vector<Token>> statements;
    // for(std::vector<Token> tokens : statements){
    //     Node* root = parse(tokens);
    //     //std::cout << "\n";
    //     Token output = interpret(root);
    //     std::cout << "(" << output.value << "," << displayTokenType(output.type) << ")\n";
    // }
    // std::cout<<"Variables:\n";
    // for(auto variable : variables){
    //     std::cout<< "(" <<variable.first<<","<< variable.second.value << "," << displayTokenType(variable.second.type)<<")\n";
    // }
}