#include "parser.cpp"

std::map<std::string, Token> variables = {
    {"_", Token("0", INT)},
    {"a", Token("0", INT)}
};
//variables.insert();
//variables["b"] = Variable("b");

Token interpret(Node* node, int depth = 0){
    //std::cout<<"()"<<depth<<","<<node->token.value<<","<<displayTokenType(node->token.type)<<")\n";
    if(node->token.type == IDENTIFIER){
        if(!variables.count(node->token.value)){
            variables.insert({node->token.value, Token("0", INT)});
        }
        return variables[node->token.value];
    } else if(node->token.type == INT || node->token.type == FLOAT){
        return node->token;
    } else{
        Token right, left;
        if (node->right) {
            right = interpret(node->right);
        } else{
            //return Token();
        }
        if (node->left) {
            left = interpret(node->left);
        } else{
            //return Token();
        }
        
        if(node->token.type == ASSIGN){
            if(left.type == IDENTIFIER){
                if(variables.count(left.value)){
                    variables[left.value] = right;
                } else{
                    variables.insert({left.value, right});
                } 
                return right;
            } else{
                std::cout<<"assigning error\n";
            }
        } else if(node->token.type == ARITMETIC_OPERATOR){
            if(node->token.value == "+"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) + std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "-"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) - std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "*"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stof(left.value) * std::stof(right.value)), FLOAT);
                }
            } else if(node->token.value == "/"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), INT);
                } else if((left.type == FLOAT && right.type == FLOAT) || (left.type == FLOAT && right.type == INT)){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) / std::stoi(right.value) : 0), FLOAT);
                }
            } else if(node->token.value == "%"){
                if(left.type == INT && right.type == INT){
                    return Token(std::to_string(std::stoi(right.value) != 0 ? std::stoi(left.value) % std::stoi(right.value) : 0), INT);
                }
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
    std::cout<<"Variables:\n";
    for(auto variable : variables){
        std::cout<< "(" <<variable.first<<","<< variable.second.value << "," << displayTokenType(variable.second.type)<<")\n";
    }
}