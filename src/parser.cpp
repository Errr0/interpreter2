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
        this->left = nullptr;
        this->right = nullptr;
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

node* createNode(const Token& token) {
    return new node(token);
}

// Helper function to define precedence of operators (higher number = higher precedence)
int precedence(const std::string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0; // Default for non-operators
}

// Function to print the AST (In-order traversal)
void printAST(node* root) {
    if (root == nullptr) return;
    if (root->left) printAST(root->left);
    std::cout << root->value.value << " ";
    if (root->right) printAST(root->right);
}

// Example function to build an AST from a list of tokens (infix expression)
node* buildAST(const std::vector<Token>& tokens) {
    std::stack<node*> operandStack;
    std::stack<node*> operatorStack;

    for (const Token& token : tokens) {
        if (token.type == NUMBER) {
            operandStack.push(createNode(token)); // Push operands directly
        } else if (token.type == PLUS/*operator*/) {
            // Handle operators based on precedence
            while (!operatorStack.empty() && precedence(operatorStack.top()->value.value) >= precedence(token.value)) {
                node* right = operandStack.top(); operandStack.pop();
                node* left = operandStack.top(); operandStack.pop();
                node* opNode = operatorStack.top(); operatorStack.pop();
                opNode->addLeftNode(left);
                opNode->addRightNode(right);
                operandStack.push(opNode);
            }
            operatorStack.push(createNode(token)); // Push the operator to the stack
        }
    }

    // Process remaining operators in the stack
    while (!operatorStack.empty()) {
        node* right = operandStack.top(); operandStack.pop();
        node* left = operandStack.top(); operandStack.pop();
        node* opNode = operatorStack.top(); operatorStack.pop();
        opNode->addLeftNode(left);
        opNode->addRightNode(right);
        operandStack.push(opNode);
    }

    return operandStack.top(); // The root of the AST
}

// class AST{
//     public:
//     AST(){
        
//     }
// };

void parse(std::vector<Token> &tokens){
    node* astRoot = buildAST(tokens); // Build the AST

    printAST(astRoot); // Output: 3 + 4 * 5 (in-order)
    delete astRoot; // Clean up the allocated memory
}
