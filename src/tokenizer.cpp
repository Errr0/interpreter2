#include "include.cpp"
#include "checks.cpp"
#include "strings.cpp"

enum TokenType {
    END,
    ASSIGN,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    MODULO,
    INCREMENT,   // ++
    DECREMENT,   // --
    ADDASSIGN,  // +=
    SUBASSIGN,  // -=
    MULASSIGN,  // *=
    DIVASSIGN,  // /=
    MODASSIGN,  // %=
    NOT,
    AND,
    OR,
    EQUAL,
    NOTEQUAL,
    LESSTHAN,
    LESSEQUAL,
    GREATERTHAN,
    GREATEREQUAL,
    NUMBER,
    INT,
    FLOAT,
    IDENTIFIER,
    KEYWORD,
    BRACKETOPEN,
    BRACKETCLOSE,
    SQUAREBRACKETOPEN,
    SQUAREBRACKETCLOSE,
    CURLYBRACKETOPEN,
    CURLYBRACKETCLOSE,
    APOSTROPHE,
    QUOTATION,
    QUESTIONMARK,
    COMMA,
    COLON,
    DOT,
    BACKSLASH,
    DOUBLESLASH,
    HASHTAG,
    SPACE
    
};

std::string displayTokenType(TokenType token) {
    switch (token) {
        case END: return "END";
        case ASSIGN: return "ASSIGN";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case ASTERISK: return "ASTERISK";
        case SLASH: return "SLASH";
        case MODULO: return "MODULO";
        case INCREMENT: return "INCREMENT";
        case DECREMENT: return "DECREMENT";
        case ADDASSIGN: return "ADDASSIGN";
        case SUBASSIGN: return "SUBASSIGN";
        case MULASSIGN: return "MULASSIGN";
        case DIVASSIGN: return "DIVASSIGN";
        case MODASSIGN: return "MODASSIGN";
        case NOT: return "NOT";
        case AND: return "AND";
        case OR: return "OR";
        case EQUAL: return "EQUAL";
        case NOTEQUAL: return "NOTEQUAL";
        case LESSTHAN: return "LESSTHAN";
        case LESSEQUAL: return "LESSEQUAL";
        case GREATERTHAN: return "GREATERTHAN";
        case GREATEREQUAL: return "GREATEREQUAL";
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case BRACKETOPEN: return "BRACKETOPEN";
        case BRACKETCLOSE: return "BRACKETCLOSE";
        case SQUAREBRACKETOPEN: return "SQUAREBRACKETOPEN";
        case SQUAREBRACKETCLOSE: return "SQUAREBRACKETCLOSE";
        case CURLYBRACKETOPEN: return "CURLYBRACKETOPEN";
        case CURLYBRACKETCLOSE: return "CURLYBRACKETCLOSE";
        case APOSTROPHE: return "APOSTROPHE";
        case QUOTATION: return "QUOTATION";
        case QUESTIONMARK: return "QUESTIONMARK";
        case COMMA: return "COMMA";
        case COLON: return "COLON";
        case DOT: return "DOT";
        case BACKSLASH: return "BACKSLASH";
        case DOUBLESLASH: return "DOUBLESLASH";
        case HASHTAG: return "HASHTAG";
        case SPACE: return "SPACE";
        default: return "UNKNOWN";
    }
}

std::string display(TokenType token) {
    switch (token) {
        case END: return ";";
        case ASSIGN: return "=";
        case PLUS: return "+";
        case MINUS: return "-";
        case ASTERISK: return "*";
        case SLASH: return "/";
        case MODULO: return "%";
        case INCREMENT: return "++";
        case DECREMENT: return "--";
        case ADDASSIGN: return "+=";
        case SUBASSIGN: return "-=";
        case MULASSIGN: return "*=";
        case DIVASSIGN: return "/=";
        case MODASSIGN: return "%=";
        case NOT: return "!";
        case AND: return "&&";
        case OR: return "||";
        case EQUAL: return "==";
        case NOTEQUAL: return "!=";
        case LESSTHAN: return "<";
        case LESSEQUAL: return "<=";
        case GREATERTHAN: return ">";
        case GREATEREQUAL: return ">=";
        case NUMBER: return "number";
        case IDENTIFIER: return "identifier";
        case INT: return "int";
        case FLOAT: return "float";
        case KEYWORD: return "keyword";
        case BRACKETOPEN: return "(";
        case BRACKETCLOSE: return ")";
        case SQUAREBRACKETOPEN: return "[";
        case SQUAREBRACKETCLOSE: return "]";
        case CURLYBRACKETOPEN: return "{";
        case CURLYBRACKETCLOSE: return "}";
        case APOSTROPHE: return "'";
        case QUOTATION: return "\"";
        case QUESTIONMARK: return "?";
        case COMMA: return ",";
        case COLON: return ":";
        case DOT: return ".";
        case BACKSLASH: return "\\";
        case DOUBLESLASH: return "//";
        case HASHTAG: return "#";
        case SPACE: return " ";
        default: return "UNKNOWN";
    }
}


std::map<std::string, TokenType> locked = {
    {"~END", END},
    {"~ASSIGN", ASSIGN},
    {"~PLUS", PLUS},
    {"~MINUS", MINUS},
    {"~ASTERISK", ASTERISK},
    {"~SLASH", SLASH},
    {"~MODULO", MODULO},
    {"~INCREMENT", INCREMENT},
    {"~DECREMENT", DECREMENT},
    {"~ADDASSIGN", ADDASSIGN},
    {"~SUBASSIGN", SUBASSIGN},
    {"~MULASSIGN", MULASSIGN},
    {"~DIVASSIGN", DIVASSIGN},
    {"~MODASSIGN", MODASSIGN},
    {"~NOT", NOT},
    {"~AND", AND},
    {"~OR", OR},
    {"~EQUAL", EQUAL},
    {"~NOTEQUAL", NOTEQUAL},
    {"~LESSTHAN", LESSTHAN},
    {"~LESSEQUAL", LESSEQUAL},
    {"~GREATERTHAN", GREATERTHAN},
    {"~GREATEREQUAL", GREATEREQUAL},
    {"~BRACKETOPEN", BRACKETOPEN},
    {"~BRACKETCLOSE", BRACKETCLOSE},
    {"~SQUAREBRACKETOPEN", SQUAREBRACKETOPEN},
    {"~SQUAREBRACKETCLOSE", SQUAREBRACKETCLOSE},
    {"~CURLYBRACKETOPEN", CURLYBRACKETOPEN},
    {"~CURLYBRACKETCLOSE", CURLYBRACKETCLOSE},
    {"~APOSTROPHE", APOSTROPHE},
    {"~QUOTATION", QUOTATION},
    {"~QUESTIONMARK", QUESTIONMARK},
    {"~COMMA", COMMA},
    {"~COLON", COLON},
    {"~DOT", DOT},
    {"~BACKSLASH", BACKSLASH},
    {"~DOUBLESLASH", DOUBLESLASH},
    {"~HASHTAG", HASHTAG},
    {"~SPACE", SPACE}
};

std::array<std::string, 39> symbols = {
    "==",
    "!=",
    "<=",
    ">=",
    "&&",
    "||",
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "//",
    ";",
    " ",
    "=",
    "+",
    "*",
    "/",
    "!",
    "-",
    "<",
    ">",
    "%",
    "(",
    ")",
    "[",
    "]",
    "{",
    "}",
    "'",
    "\"",
    "?",
    ",",
    ":",
    ".",
    "\\",
    "#"};

class Token{
    public:
    std::string value;
    enum TokenType type;
    Token(std::string value = ";",enum TokenType type = END){
        this -> value = value;
        this -> type = type;
    }
};

void addSpaceToken(std::vector<Token> &tokens){
    if(!tokens.empty()){
        if(tokens.back().type == SPACE){
            return;
        }
    }
    tokens.push_back(Token("",SPACE));
}

void addNumberToken(std::string str, std::vector<Token> &tokens){
    if(tokens.size()>=2){
        if(tokens[tokens.size() - 1].type == DOT && (tokens[tokens.size() - 2].type == NUMBER || tokens[tokens.size() - 2].type == INT)){
            tokens.pop_back();
            std::string temp = tokens.back().value;
            tokens.pop_back();
            if(!tokens.empty()){
                if(tokens.back().type == MINUS){
                    tokens.pop_back();
                    tokens.push_back(Token("-"+temp+"."+str,FLOAT));
            return;
                }
            }
            tokens.push_back(Token(temp+"."+str,FLOAT));
            return;
        }
    }
    if(tokens.back().type == MINUS){
        tokens.pop_back();
        tokens.push_back(Token("-"+str,INT));
    }
    tokens.push_back(Token(str,INT));
}

void tokenize(std::vector<std::string> arr, std::vector<Token> &tokens){
    for (std::string str : arr) {
        if(str[0]=='~'){
            if(str=="~SPACE"){
                addSpaceToken(tokens);
            } else{
                tokens.push_back(Token("",locked[str]));
            }
        } else if(isNumber(str)){
            addNumberToken(str, tokens);
        } else if(isIdentifier(str)){
            if(isKeyword(str)){
                tokens.push_back(Token(str,KEYWORD));
            } else {
                tokens.push_back(Token(str,IDENTIFIER));
            }
        } else{
            //std::cout <<" |no valid token: "<< str <<"| ";
        }
    }
}

void deconstructStatement(std::string &str, std::vector<Token> &arr){
    std::vector<std::string> words;
    split(str,words);
    for (std::string& symbol : symbols) {
        std::vector<std::string> temp;
        for (std::string& substr : words) {
            if(substr.find(symbol) != std::string::npos){
                split(substr, temp, symbol, true, true, true);
            } else {
                temp.push_back(substr); // Keep unchanged if no symbol found
            }
        }
        words = std::move(temp); // Replace help with the updated vector
    }
    tokenize(words, arr);
}


std::string interpret(std::vector<Token> &tokens){
    Token temp;
    while(!tokens.empty()){
        switch(tokens[0].type) {
        case END:
            return;
            break;
        //case NUMBER:
            //break;
        case INT:
            break;
        case FLOAT:
            break;
        case IDENTIFIER:
            break;
        case KEYWORD:
            if(tokens[0].value == "include"){

            } else if(dataTypes.find(tokens[0].value) != dataTypes.end()){
                if(tokens[1].type == IDENTIFIER){

                } else if(tokens[1].type == BRACKETOPEN){
                    
                } else{
                    return "type";
                }
            } else if(tokens[0].value == ""){

            } else if(tokens[0].value == ""){
                
            } else if(tokens[0].value == ""){
                
            } else if(tokens[0].value == ""){
                
            }
            break;
        case ASSIGN:
            break;
        case PLUS:
            break;
        case MINUS:
            break;
        case ASTERISK:
            break;
        case SLASH:
            break;
        case MODULO:
            break;
        case INCREMENT:
            break;
        case DECREMENT:
            break;
        case ADDASSIGN:
            break;
        case SUBASSIGN:
            break;
        case MULASSIGN:
            break;
        case DIVASSIGN:
            break;
        case MODASSIGN:
            break;
        case NOT:
            break;
        case AND:
            break;
        case OR:
            break;
        case EQUAL:
            break;
        case NOTEQUAL:
            break;
        case LESSTHAN:
            break;
        case LESSEQUAL:
            break;
        case GREATERTHAN:
            break;
        case GREATEREQUAL:
            break;
        case BRACKETOPEN:
            break;
        case BRACKETCLOSE:
            break;
        case SQUAREBRACKETOPEN:
            break;
        case SQUAREBRACKETCLOSE:
            break;
        case CURLYBRACKETOPEN:
            break;
        case CURLYBRACKETCLOSE:
            break;
        case APOSTROPHE:
            break;
        case QUOTATION:
            break;
        case QUESTIONMARK:
            break;
        case COMMA:
            break;
        case COLON:
            break;
        case DOT:
            break;
        case BACKSLASH:
            break;
        case DOUBLESLASH:
            break;
        case HASHTAG:
            break;
        case SPACE:
            break;
        default:
            break;
        }
    }
}


void parse(std::string &str, std::vector<Token> &tokens){
    std::vector<std::string> statements;
    replace(str, "\n", " ");
    split(str, statements, ";", true, true, true);
    for (std::string& statement : statements) {
        std::vector<std::string> splittedStatement;
        split(statement,splittedStatement);
        deconstructStatement(statement, tokens);
        interpret(tokens);
    }
}

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
