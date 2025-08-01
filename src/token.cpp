#include <string>
#include <vector>
#include <iostream>

enum Actions {
    PROCESSING,
    END,
    INT,
    FLOAT,
    IDENTIFIER,
    ADD,
    VARIABLE,
    FOR,
    WHILE,
    IF,
    PRINT

};

unsigned char* raw(int input){
    unsigned char* data = new unsigned char[4];

    data[0] = input & 0xFF;
    data[1] = (input >> 8) & 0xFF;
    data[2] = (input >> 16) & 0xFF;
    data[3] = (input >> 24) & 0xFF;

    return data;
}

unsigned char* raw(float input) {
    unsigned char* data = new unsigned char[4];
    unsigned int* p = reinterpret_cast<unsigned int*>(&input);// WTF like in quake III isqrt algorithm

    data[0] = (*p >> 0) & 0xFF;
    data[1] = (*p >> 8) & 0xFF;
    data[2] = (*p >> 16) & 0xFF;
    data[3] = (*p >> 24) & 0xFF;

    return data;
}

unsigned char* raw(char input){
    unsigned char* data = new unsigned char[1];
    data[0] = input;
    return data;
}

unsigned char* raw(std::string input){
    const int size = input.length();
    unsigned char* data = new unsigned char[size+1];
    for (int i = 0; i < size; i++)
    {
        data[i] = input[i];
    }
    data[size] = '\0';
    return data;
}

class Token{
    public:
    enum Actions type;
    unsigned char* data;
    int priority;
    Token(enum Actions type = Actions::END, unsigned char* data, int priority = 0){
        this->type = type;
        this->data = data;
        this->priority = priority;
    }
    ~Token(){
        delete[] this->data;
    }
};

int retrieve_int(unsigned char* input){
    int output = 0;
    for (int i = 0; i < 4; i++)
    {
        int temp = int(input[i])<<(i*8);
        output += temp;
    }
    return output;
}

float power_float(int a, int b){
    for(int)
}

int retrieve_flo(unsigned char* input){
    float output = 0;
    int sign = input[3] & 0x80;
    int exponent = input[3] & 0x7f << 1 + input[2] & 0x80;
    int mantissa = input[2] & 0x7f << 3 + input[1] << 2 << input[0];
    output = (sign==0 ? 1 : -1) * 
    return output;
}

std::string retrieve_str(unsigned char* input){
    std::string output = "";
    int i = 0;
    while(input[i] != '\0')
    {
        output += char(input[i]);
        i++;
    }
    return output;
}

void print(Token token, bool value = true, bool type, bool priority){
    switch (token.type) {
    case INT:
        std::cout << "" << (value ? std::to_string(retrieve_int(token.data)) : "") << "" << (type ? "int" : "") << "" << (priority ? std::to_string(token.priority) : "") << "";
        break;

    case FLOAT:
        
        break;

    case IDENTIFIER:
        std::cout << "" << (value ? retrieve_str(token.data) : "") << "" << (type ? "str" : "") << "" << (priority ? std::to_string(token.priority) : "") << "";
        break;

    default:
        
        break;
}
}



// class Token{
//     public:
//     enum Actions action;
//     std::vector<void*> args;
//     int priority;
//     Token(enum Actions action = Actions::END, std::vector<void*> args = {}, int priority = 0){
//         this->action = action;
//         this->args = args;
//         this->priority = priority;
//     }
// };




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