#include "tokenizer.cpp"

class Node{
    public:
    Token token;
    Node* left;
    Node* right;
    Node(Token token, Node* left = nullptr, Node* right = nullptr){
        this->token = token;
        this->left = left;
        this->right = right;
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
