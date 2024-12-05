#include "include.cpp"
#include "checks.cpp"
#include "strings.cpp"
#include "tokenizer.cpp"

class node{
    public:
    Token value;
    node *left;
    node *right;
    node(Token value = Token()){
        this->value = value;
    }
    ~node(){
        delete this->left;
        delete this->right;
    }
    void addLeftNode(node *left){
        this->left = left;
    }
    void addRightNode(node *right){
        this->right = right;
    }
};

class AST{
    public:
    AST(){

    }
};

void parse(std::vector<Token> &tokens){

}
