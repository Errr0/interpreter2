#include "tokenizer.cpp"

class Node{
    public:
    Token token;
    Node* left;
    Node* right;
    Node(Token token, Node* e1 = nullptr, Node* e2 = nullptr){
        this->token = token;
        this->left = e1;
        this->right = e2;
    }
    ~Node(){
        delete this->left;
        delete this->right;
    }
};

void printTree(Node* node, int depth = 0, bool isLeft = true) {
    if (!node) return;
    if (node->right) {
        printTree(node->right, depth + 1, false);
    }
    std::cout << std::string(std::max(depth-1, 0) * 10, ' ');
    if (depth > 0) {
        std::cout << (!isLeft ? "/" : "\\");
    }
    std::cout << std::string(depth * 10, '-') << node->token.value << " " << displayTokenType(node->token.type) << "\n";
    if (node->left) {
        printTree(node->left, depth + 1, true);
    }
}

Node* parse(std::vector<Token>& tokens) {
    std::stack<Token> operatorStack;
    std::stack<Node*> exprStack;

    auto processOperator = [&]() {
        if (operatorStack.empty() || exprStack.size() < 2) {
            std::cerr << "PARSER ERROR: Invalid operator/operand stack state!\n";
            return;
        }
        Token op = operatorStack.top();
        operatorStack.pop();

        Node* e2 = exprStack.top(); // Right operand
        exprStack.pop();

        Node* e1 = exprStack.top(); // Left operand
        exprStack.pop();

        exprStack.push(new Node(op, e1, e2));
    };

    for (const Token& token : tokens) {
        if (token.type == BRACKET_OPEN && token.value == "(") {
            operatorStack.push(token);
        } else if (token.type == INT || token.type == FLOAT) {
            exprStack.push(new Node(token));
        } else if (token.type == INT || token.type == IDENTIFIER/* || token.type == DECLARATION*/) {
            exprStack.push(new Node(token));
        } else if (token.type == ARITMETIC_OPERATOR || token.type == ASSIGN) {
            while (!operatorStack.empty() && 
                   operatorStack.top().type != BRACKET_OPEN &&
                   operatorStack.top().weight >= token.weight) {
                processOperator();
            }
            operatorStack.push(token);
        } else if (token.type == BRACKET_CLOSE && token.value == ")") {
            while (!operatorStack.empty() && operatorStack.top().type != BRACKET_OPEN) {
                processOperator();
            }
            if (operatorStack.empty() || operatorStack.top().type != BRACKET_OPEN) {
                std::cerr << "PARSER ERROR: Mismatched brackets!\n";
                return nullptr;
            }
            operatorStack.pop(); // Pop the opening bracket
        } else {
            std::cerr << "PARSER ERROR: Unknown token type! "<<token.type<<"\n";
            return nullptr;
        }
    }

    // Process remaining operators
    while (!operatorStack.empty()) {
        if (operatorStack.top().type == BRACKET_OPEN) {
            std::cerr << "PARSER ERROR: Mismatched brackets!\n";
            return nullptr;
        }
        processOperator();
    }

    if (exprStack.size() != 1) {
        std::cerr << "PARSER ERROR: Malformed expression!\n";
        return nullptr;
    }

    return exprStack.top(); // Root of the expression tree
}


// Parser
// Node* parse(std::vector<Token>& tokens){
//     std::stack<Token> operatorStack;
//     std::stack<Node*> exprStack;
//     for(Token token : tokens){
//         //std::cout<<"2 "<< displayType(token.type) << token.value <<"\n";
//         if(token.type == BRACKET_OPEN){
//             operatorStack.push(token);
//         } else if(token.type == INT){
//             exprStack.push(new Node(token));
//         } else if(token.type == ARITMETIC_OPERATOR){
//             if(operatorStack.empty())continue;
//             while(operatorStack.top().weight >= token.weight){
//                 Token op = operatorStack.top();
//                 operatorStack.pop();
//                 if(operatorStack.empty())continue;
//                 Node* e2 = exprStack.top();
//                 exprStack.pop();
//                 if(operatorStack.empty())continue;
//                 Node* e1 = exprStack.top();
//                 exprStack.pop();
//                 exprStack.push(new Node(op, e1, e2));
//             }
//             operatorStack.push(token);
//         } else if(token.type == BRACKET_CLOSE){
//             if(operatorStack.empty()) continue;
//             while(operatorStack.top().type != BRACKET_OPEN){
//                 Token op = operatorStack.top();
//                 operatorStack.pop();
//                 if(exprStack.empty()) continue;
//                 Node* e2 = exprStack.top();
//                 exprStack.pop();
//                 if(operatorStack.empty()) continue;
//                 Node* e1 = exprStack.top();
//                 exprStack.pop();
//                 exprStack.push(new Node(op, e1, e2));
//             }
//             operatorStack.pop();
//         } else{
//             std::cout<<"PARSER ERROR!!!\n";
            
//         }
//     }
//     return exprStack.top();
// }
