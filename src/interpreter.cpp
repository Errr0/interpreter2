#include "parser.cpp"

Token interpret(Node* node, int depth = 0, bool isLeft = true){
    if(node->token.type == INT || node->token.type == FLOAT || node->token.type == IDENTIFIER){
        return node->token;
    } else if(node->token.type == ARITMETIC_OPERATOR){
        Token right, left;
        if (node->left) {
            left = interpret(node->left, depth + 1, false);
        } else{
            //return Token();
        }
        if (node->right) {
            right = interpret(node->right, depth + 1, false);
        } else{
            //return Token();
        }
        if(node->token.value == "+"){
            if(left.type == INT && right.type == INT){
                return Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), INT);
            } else if(left.type == FLOAT && right.type == FLOAT){
                return Token(std::to_string(std::stof(left.value) + std::stof(right.value)), FLOAT);
            }
        } else if(node->token.value == "-"){
            if(left.type == INT && right.type == INT){
                return Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), INT);
            } else if(left.type == FLOAT && right.type == FLOAT){
                return Token(std::to_string(std::stof(left.value) - std::stof(right.value)), FLOAT);
            }
        } else if(node->token.value == "*"){
            if(left.type == INT && right.type == INT){
                return Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), INT);
            } else if(left.type == FLOAT && right.type == FLOAT){
                return Token(std::to_string(std::stof(left.value) * std::stof(right.value)), FLOAT);
            }
        } else if(node->token.value == "/"){
            if(left.type == INT && right.type == INT){
                return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), INT);
            } else if(left.type == FLOAT && right.type == FLOAT){
                return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), FLOAT);
            }
        } else if(node->token.value == "%"){
            if(left.type == INT && right.type == INT){
                return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) % std::stoi(right.value) : 0), INT);
            // } else if(left.type == FLOAT){
            //     return Token(std::to_string(std::stof(left.value) + std::stof(right.value)), FLOAT);
            }
        }
    }
}

void interpreter(std::vector<std::vector<Token>> &statements){
    for(std::vector<Token> tokens : statements){
        Node* root = parse(tokens);
        std::cout << "\n";
        Token output = interpret(root);
        std::cout << "(" << output.value << "," << displayTokenType(output.type) << ")\n";
    }
}