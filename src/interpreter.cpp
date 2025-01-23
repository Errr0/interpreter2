#include "scopes.cpp" 

Scope program = Scope("{}", nullptr, BuildInNamespace);

Scope makeScopeTree(std::vector<Token>& tokens) {
    Scope root("{}", &program);
    Scope* currentScope = &root;
    std::stack<Scope*> scopeStack;
    for (const Token& token : tokens) {
        if (token.type != BRACKET_OPEN && token.type != BRACKET_CLOSE && token.type != END && token.type != COMMA) {
            currentScope->appendToken(token);
        } else if (token.type == END || token.type == COMMA){
            currentScope->makeStatement((token.type == END)?";":",");
        } else if (token.type == BRACKET_OPEN) {
            std::string type;
            if (token.value == "{") type = "{}";
            else if (token.value == "(") type = "()";
            else if (token.value == "[") type = "[]";

            Scope newScope = currentScope->appendScope(type);
            scopeStack.push(currentScope);
            currentScope = &newScope;
        } else if (token.type == BRACKET_CLOSE) {
            char expectedClose = currentScope->type[1];
            if (token.value[0] != expectedClose) {
                std::cerr << "ERROR: Mismatched brackets " << token.value << " and " << currentScope->type << "\n";
                return root;
            }
            currentScope = scopeStack.top();
            scopeStack.pop();
        }
    }
    return root;
}

void interpreter(std::string& code){
    processInput(code);
    std::vector<std::string> strings;
    std::vector<Token> tokens;
    splitString(code, operators, strings);
    tokenize(strings, tokens);
    // for(Token token:tokens){
    //     displayToken(token);
    // }
    Scope root = makeScopeTree(tokens);
    Token output = root.interpret();
    //displayToken(output);
    root.print();
    std::cout<<"\nprocess returned: ";
    displayToken(output);
}