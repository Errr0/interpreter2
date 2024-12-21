#include "parser.cpp"

class Scope{
    public:
    std::string type;
    std::vector<Token> tokens;
    std::vector<Scope> scopes;
    Scope(std::string type = "{}"){
        this->type = type;
    }
    void appendToken(Token token){
        tokens.push_back(token);
    }
    void appendScope(Scope scope){
        tokens.push_back(Token(std::to_string(scopes.size()), SCOPE));
        scopes.push_back(scope);
    }
    void print(int depth = 0){
        for(Token token : tokens){
            std::cout<<std::string(depth, ' ')<<token.value<<displayType(token.type);
        }
        std::cout<<"\n";
        for(Scope scope : scopes){
            print(depth+1);
        }
    }
};

// void printScopes(Scope* node, int depth = 0, bool isLeft = true) {
//     if (!node) return;
//     if (node->right) {
//         printScopes(node->right, depth + 1, false);
//     }
//     std::cout << std::string(std::max(depth-1, 0) * 10, ' ');
//     if (depth > 0) {
//         std::cout << (!isLeft ? "/" : "\\");
//     }
//     std::cout << std::string(depth * 10, '-') << node->scope << "\n";
//     if (node->left) {
//         printScopes(node->left, depth + 1, true);
//     }
// }

// Scope* parseScopes(std::vector<std::string>& scopes) {
//     std::stack<std::string> operatorStack;
//     std::stack<Scope*> scopeStack;

//     auto processOperator = [&]() {
//         if (operatorStack.empty() || scopeStack.size() < 2) {
//             std::cerr << "PARSER ERROR: Invalid operator/operand stack state!\n";
//             return;
//         }
//         std::string scope = operatorStack.top();
//         operatorStack.pop();

//         Scope* right = scopeStack.top(); // Right operand
//         scopeStack.pop();

//         Scope* left = scopeStack.top(); // Left operand
//         scopeStack.pop();

//         scopeStack.push(new Scope(scope, left, right));
//     };

//     for (const std::string& scope : scopes) {
//         // if (scope == "{") {
//         //     operatorStack.push(scope);
//         // } else 
//         if (scope == "}") {
//             while (!operatorStack.empty() && operatorStack.top() != "{") {
//                 processOperator();
//             }
//             if (operatorStack.empty() || operatorStack.top() != "{") {
//                 std::cerr << "PARSER ERROR: Mismatched brackets!\n";
//                 return nullptr;
//             }
//             operatorStack.pop(); // Pop the opening bracket
//         } else {
//             operatorStack.push(scope);
//             // std::cerr << "PARSER ERROR: Unknown token type!\n";
//             // return nullptr;
//         }
//     }

//     // Process remaining operators
//     while (!operatorStack.empty()) {
//         if (operatorStack.top() == "{") {
//             std::cerr << "PARSER ERROR: Mismatched brackets!\n";
//             return nullptr;
//         }
//         processOperator();
//     }

//     if (scopeStack.size() != 1) {
//         std::cerr << "PARSER ERROR: Malformed expression!\n";
//         return nullptr;
//     }

//     return scopeStack.top(); // Root of the expression tree
// }