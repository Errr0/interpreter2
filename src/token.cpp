#include <iostream>
#include <string>
#include <any>

enum TokenType {
    NUL,
    OPERATOR

};



class Token{
    std::any value; // std::variant<int, double, std::string>
    enum TokenType type;
    int priority;
    public:
    Token(std::any value,enum TokenType type = TokenType::NUL, int priority = 0){
        this->value = value;
        this->type = type;
        this->priority = priority;
    }

    void print(){
        std::cout << this->value.type().name() << "\n";
        
        std::cout << "value: " << std::any_cast<std::string>(this->value) << " type: " << this->type << " priority: " << this->priority << "\n";
    }
};