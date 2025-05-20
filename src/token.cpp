#include <string>
#include <vector>

enum Actions {
    END,
    ADD,
    VARIABLE,
    FOR,
    WHILE,
    IF,
    PRINT

};

class Token{
    public:
    enum Actions action;
    std::vector<void*> args;
    int priority;
    Token(enum Actions action = Actions::END, std::vector<void*> args, int priority = 0){
        this->action = action;
        this->args = args;
        this->priority = priority;
    }
};




// switch (type) {
//     case NUL:
//         std::cout << "Null token\n";
//         break;

//     case OPERATOR:
//         std::cout << "Operator token\n";
//         break;

//     default:
//         std::cout << "Unknown token type\n";
//         break;
// }



// class Token{
//     int value_i;
//     double value_d;
//     std::string value_s;
//     enum TokenType type;
//     void *ptr;
//     int priority;
//     public:
//     Token(int value_i = 0, enum TokenType type = TokenType::NUL, int priority = 0, void *ptr = nullptr){
//         this->value_i = value_i;
//         this->value_d = 0.0;
//         this->value_s = "";
//         this->type = type;
//         this->ptr = ptr;
//         this->priority = priority;
//     }

//     Token(double value_d = 0.0, enum TokenType type = TokenType::NUL, int priority = 0, void *ptr = nullptr){
//         this->value_i = 0;
//         this->value_d = value_d;
//         this->value_s = "";
//         this->type = type;
//         this->ptr = ptr;
//         this->priority = priority;
//     }

//     Token(std::string value_s = "", enum TokenType type = TokenType::NUL, int priority = 0, void *ptr = nullptr){
//         this->value_i = 0;
//         this->value_d = 0.0;
//         this->value_s = value_s;
//         this->type = type;
//         this->ptr = ptr;
//         this->priority = priority;
//     }
// };