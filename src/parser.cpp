#include "tokenizer.cpp"


class Node{
    public:
    Token value;
    Node* left;
    Node* right;
    // Node(Token value){
    //     this -> value = value;
    //     left = right = nullptr;
    // }
    Node(Token value, Node* e1 = nullptr, Node* e2 = nullptr){
        this -> value = value;
        this -> left = e1;
        this -> right = e2;
    }
};

void printAST(Node* root, int indent = 0) {
    if (root == nullptr) return;

    // Print the right child first, so it appears on the top (right subtree)
    if (root->right) {
        printAST(root->right, indent + 4);
        std::cout << std::string(indent + 2, ' ') << " /" << std::endl; // Print the "/" for the right child
    }

    // Print the current node value with appropriate indentation
    if (indent > 0) {
        std::cout << std::string(indent, ' ');
    }
    std::cout << root->value.value << std::endl;

    // Print the left child (left subtree)
    if (root->left) {
        std::cout << std::string(indent + 2, ' ') << " \\" << std::endl; // Print the "\" for the left child
        printAST(root->left, indent + 4);
    }
}


// Parser
Node parse(std::vector<Token>& tokens){
    std::stack<Token> operatorStack;
    std::stack<Node> exprStack;
    //std::cout<<"1";
    for(Token token : tokens){
        std::cout<<"2 "<< displayType(token.type) << token.value <<"\n";
        if(token.type == BRACKET_OPEN){
            std::cout<<"3!\n";
            operatorStack.push(token);
        } else if(token.type == INT){
            exprStack.push(Node(token));std::cout<<"4!\n";
        } else if(token.type == ARITMETIC_OPERATOR){std::cout<<"5!\n";
            if(operatorStack.top().weight >= token.weight){std::cout<<"5!4!\n";}else{std::cout<<"5!5!\n";}///IT BREAKS HERE
            std::cout<<"5!6!\n";
            while(operatorStack.top().weight >= token.weight){std::cout<<"5!1!\n";
                Token op = operatorStack.top();
                operatorStack.pop();

                Node e2 = exprStack.top();
                exprStack.pop();
                Node e1 = exprStack.top();
                exprStack.pop();
                exprStack.push(Node(op, &e1, &e2));
            }std::cout<<"5!2!\n";
            operatorStack.push(token);
            std::cout<<"5!3!\n";
        } else if(token.type == BRACKET_CLOSE){std::cout<<"6!\n";
            while(operatorStack.top().weight >= token.weight){
                Token op = operatorStack.top();
                operatorStack.pop();

                Node e2 = exprStack.top();
                exprStack.pop();
                Node e1 = exprStack.top();
                exprStack.pop();
                exprStack.push(Node(op, &e1, &e2));
            }
            operatorStack.push(token);
        } else{
            std::cout<<"PARSER ERROR!!!\n";
            
        }
    }
    // There should only be one item on exprStack.
    // It's the root node, so we return it.
    return exprStack.top();
}


 /* 
class node {
public:
    Token value;
    node* left;
    node* right;

    node(Token value = Token()) {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }

    ~node() {
        delete this->left;
        delete this->right;
    }

    void addLeftNode(node* left) {
        this->left = left;
    }

    void addRightNode(node* right) {
        this->right = right;
    }
};

node* createNode(const Token& token) {
    return new node(token);
}

// Function to print the AST (In-order traversal) with proper indentation
void printAST(node* root, int indent = 0) {
    if (root == nullptr) return;

    // Print the right child first, so it appears on the top (right subtree)
    if (root->right) {
        printAST(root->right, indent + 4);
        std::cout << std::string(indent + 2, ' ') << " /" << std::endl; // Print the "/" for the right child
    }

    // Print the current node value with appropriate indentation
    if (indent > 0) {
        std::cout << std::string(indent, ' ');
    }
    std::cout << root->value.value << std::endl;

    // Print the left child (left subtree)
    if (root->left) {
        std::cout << std::string(indent + 2, ' ') << " \\" << std::endl; // Print the "\" for the left child
        printAST(root->left, indent + 4);
    }
}

node* buildAST(const std::vector<Token>& tokens) {
    std::stack<node*> operandStack;
    std::stack<node*> operatorStack;

    for (const Token& token : tokens) {
        if (token.type == INT) {
            operandStack.push(createNode(token)); // Push operands directly
        } else if (token.type == ARITMETIC_OPERATOR) {
            // Handle operators based on precedence
            while (!operatorStack.empty() && operatorStack.top()->value.weight >= token.weight) {
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

void parse(std::vector<Token>& tokens) {
    node* astRoot = buildAST(tokens);
    printAST(astRoot); // Print the AST
    delete astRoot;    // Cleanup after printing
}*/
/*





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

// Function to print the AST (In-order traversal)
// void printAST(node* root) {
//     if (root == nullptr) return;
//     if (root->left) printAST(root->left);
//     std::cout << root->value.value;
//     if (root->right) printAST(root->right);
// }

void printAST(node* root, int indent = 0) {
    if (root == nullptr) return;

    // Print the right child first, so it appears on the top
    if (root->right) {
        printAST(root->right, indent + 4);
    }

    // Print the current node value with appropriate indentation
    if (indent > 0) {
        std::cout << std::string(indent, ' ');
    }
    std::cout << root->value.value << std::endl;

    // Print the left child
    if (root->left) {
        printAST(root->left, indent + 4);
    }
}


// Example function to build an AST from a list of tokens (infix expression)
node* buildAST(const std::vector<Token>& tokens) {
    std::stack<node*> operandStack;
    std::stack<node*> operatorStack;

    for (const Token& token : tokens) {
        if (token.type == INT) {
            operandStack.push(createNode(token)); // Push operands directly
        } else if (token.type == ARITMETIC_OPERATOR) {
            // Handle operators based on precedence
            while (!operatorStack.empty() && operatorStack.top()->value.weight >= token.weight) {
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

void parse(std::vector<Token> &tokens){
    node* astRoot = buildAST(tokens);
    printAST(astRoot);
    delete astRoot;
}*/
