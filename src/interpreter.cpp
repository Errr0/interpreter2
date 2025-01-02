#include "scopes.cpp"

void interpreter(std::string& code){
    code+="\n";
    eraseBetween(code, "/*", "*/"); 
    eraseBetween(code, "//", "\n");
    //std::cout<<code<<"|\n";
    std::string stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    while(stringLiteral!=""){
        //std::cout<<stringLiteral<<"\n";
        StringsLiterals.push(stringLiteral);
        stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    }
    //std::cout<<"\n"<<code<<"\n";

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
    for(Scope function:functions){
        function.print();
    }
}