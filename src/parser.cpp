#include "tokenizer.cpp"

class Node{
    public:
    Token token;
    Node* left;
    Node* right;
    Node(Token token, Node* e1 = nullptr, Node* e2 = nullptr){
        this -> token = token;
        this -> left = e1;
        this -> right = e2;
    }
};

// Parser
Node parse(std::vector<Token>& tokens){
    std::stack<Token> operatorStack;
    std::stack<Node> exprStack;
    for(Token token : tokens){
        //std::cout<<"2 "<< displayType(token.type) << token.value <<"\n";
        if(token.type == BRACKET_OPEN){
            operatorStack.push(token);
        } else if(token.type == INT){
            exprStack.push(Node(token));
        } else if(token.type == ARITMETIC_OPERATOR){
            if(operatorStack.empty())continue;
            while(operatorStack.top().weight >= token.weight){
                Token op = operatorStack.top();
                operatorStack.pop();
                if(operatorStack.empty())continue;
                Node e2 = exprStack.top();
                exprStack.pop();
                if(operatorStack.empty())continue;
                Node e1 = exprStack.top();
                exprStack.pop();
                exprStack.push(Node(op, &e1, &e2));
            }
            operatorStack.push(token);
        } else if(token.type == BRACKET_CLOSE){
            if(operatorStack.empty()) continue;
            while(operatorStack.top().type != BRACKET_OPEN){
                Token op = operatorStack.top();
                operatorStack.pop();
                if(exprStack.empty()) continue;
                Node e2 = exprStack.top();
                exprStack.pop();
                if(operatorStack.empty()) continue;
                Node e1 = exprStack.top();
                exprStack.pop();
                exprStack.push(Node(op, &e1, &e2));
            }
            operatorStack.pop();
        } else{
            std::cout<<"PARSER ERROR!!!\n";
            
        }
    }
    return exprStack.top();
}