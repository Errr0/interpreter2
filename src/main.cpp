#include "parser.cpp" 

void printTokens(std::vector<Token> &tokens){
    for(Token token : tokens){
        std::cout << token.value;// << displayType(token.type);// << token.weight;
    }
}

// void printTree(Node root){
//     if(root.left) printTree((*root.left));
//     std::cout<<root.token.value<<displayTokenType(root.token.type)<<"\n";
//     if(root.right) printTree((*root.right));
// }

// void printTree(Node* root) {
//     if (root == nullptr) return; // Base case: return if the root is null

//     // Recursively print the left subtree
//     if (root->left) {
//         printTree(root->left);
//     }

//     // Print the value of the current node (token value and type)
//     std::cout << root->token.value << displayTokenType(root->token.type) << "\n";

//     // Recursively print the right subtree
//     if (root->right) {
//         printTree(root->right);
//     }
// }


bool readfile(std::string filename, std::string &str){
    std::fstream file(filename, std::ios::in);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 0;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    str = buffer.str();
    file.close();
    return 1;
}

int main(int argc, char* argv[]){
    if(argc<2) {
        std::cerr << "Error no path argument!" << std::endl;
        return 1;
    }
    std::string code;
    if(!readfile(argv[1], code)) return 2;
    std::vector<Token> tokens;
    tokenizer(code,tokens);
    //printTokens(tokens);
    Node root = parse(tokens);
    std::cout << "done\n";
    return 0;
}

