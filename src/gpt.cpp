#include <iostream>
#include <string>
#include <vector>
#include <regex>

enum TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    ID,
    ASSIGN,
    SEMI,
    COMMA,
    FUNCTION,
    IF,
    ELSE,
    WHILE,
    END,
    TRUE,
    FALSE,
    AND,
    OR,
    NOT,
    EQUAL,
    NOTEQUAL,
    LESSTHAN,
    LESSEQUAL,
    GREATERTHAN,
    GREATEREQUAL,
    UNKNOWN
};

// Token structure
struct Token {
    std::string value;
    TokenType type;
};

// Function to convert TokenType to string
std::string tokenTypeToString(TokenType type) {
    switch(type) {
        case INTEGER: return "INTEGER";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case ID: return "ID";
        case ASSIGN: return "ASSIGN";
        case SEMI: return "SEMI";
        case COMMA: return "COMMA";
        case FUNCTION: return "FUNCTION";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case END: return "END";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case EQUAL: return "EQUAL";
        case NOTEQUAL: return "NOTEQUAL";
        case LESSTHAN: return "LESSTHAN";
        case LESSEQUAL: return "LESSEQUAL";
        case GREATERTHAN: return "GREATERTHAN";
        case GREATEREQUAL: return "GREATEREQUAL";
        default: return "UNKNOWN";
    }
}

class Lexer {
public:
    Lexer(const std::string& input) : input(input), currentPos(0) {}

    // Lexical analysis function
    Token getNextToken() {
        std::cout << currentPos << std::endl;
        while (currentPos < input.length()) {
            char currentChar = input[currentPos];
            if (isspace(currentChar)) {
                // Skip whitespaces
                currentPos++;
                continue;
            } else if (isdigit(currentChar)) {
                return lexInteger();
            } else if (isalpha(currentChar)) {
                return lexIdentifier();
            } else {
                // Handle single character tokens
                switch (currentChar) {
                    case '+': return createToken(PLUS);
                    case '-': return createToken(MINUS);
                    case '*': return createToken(MUL);
                    case '/': return createToken(DIV);
                    case '(': return createToken(LPAREN);
                    case ')': return createToken(RPAREN);
                    case '=': return createToken(ASSIGN);
                    case ';': return createToken(SEMI);
                    case ',': return createToken(COMMA);
                    default: 
                        // For simplicity, ignore unknown characters
                        return createToken(UNKNOWN);
                }
            }
        }
        // Return an EOF token when the end of the input is reached
        return createToken(END);
    }

private:
    std::string input;
    size_t currentPos;

    Token lexInteger() {
        std::string integerValue;
        while (currentPos < input.length() && isdigit(input[currentPos])) {
            integerValue += input[currentPos];
            currentPos++;
        }
        return {integerValue, INTEGER};
    }

    Token lexIdentifier() {
        std::string identifier;
        while (currentPos < input.length() && (isalnum(input[currentPos]) || input[currentPos] == '_')) {
            identifier += input[currentPos];
            currentPos++;
        }
        // Check if the identifier matches any keyword
        if (identifier == "function") return createToken(FUNCTION);
        else if (identifier == "if") return createToken(IF);
        else if (identifier == "else") return createToken(ELSE);
        else if (identifier == "while") return createToken(WHILE);
        else if (identifier == "true" || identifier == "True") return createToken(TRUE);
        else if (identifier == "false" || identifier == "False") return createToken(FALSE);
        else if (identifier == "and") return createToken(AND);
        else if (identifier == "or") return createToken(OR);
        else if (identifier == "not") return createToken(NOT);
        else return {identifier, ID};
    }

    Token createToken(TokenType type) {
        currentPos++;
        return {"", type};
    }
};

int main() {
    std::string code = "int main { int x = 5; int y = 10; int z = x + y; std::cout << z; return 0; }";
    Lexer lexer(code);

    Token token;
    do {
        token = lexer.getNextToken();
        std::cout << "Token: " << token.value << ", Type: " << tokenTypeToString(token.type) << std::endl;
    } while (token.type != END);

    return 0;
}
/*
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <sstream>

bool splitChunks(std::string str, std::vector<std::string>& arr) {
    std::array<std::string, 34> symbols = {
        "==", "!=", "<=", ">=", "&&", "||", "+=", "-=", "*=", "/=", "//", 
        "=", "+", "*", "/", "!", "-", "<", ">", "%", "(", ")", "[", "]", 
        "{", "}", "'", "\"", "?", ",", ":", ".", "\\", "#"
    };

    size_t start = 0, end = 0;
    bool change = false;

    while (start < str.size()) {
        bool wasFound = false;
        
        // Try to find any symbol in the string from the current position
        for (const std::string& s : symbols) {
            end = str.find(s, start);
            if (end == start) {  // If the symbol is found at the start
                if (str.substr(start, s.length()) != " ") {  // Avoid empty spaces
                    arr.push_back(s);  // Push the symbol as a token
                    change = true;
                }
                start = end + s.length();  // Move start after the symbol
                wasFound = false;
                break;
            }
        }
        
        if (!wasFound) {
            // If no symbol was found, it must be part of an operand (number or variable)
            end = str.find_first_of(" \t\n", start);  // Find the next whitespace or end of string
            if (end == std::string::npos) end = str.length();  // Until the end if no space found
            
            std::string token = str.substr(start, end - start);
            if (!token.empty() && token != " ") {
                arr.push_back(token);  // Push operand as a token
                change = true;
            }
            start = end;  // Move the start index to the next position
        }
    }

    return change;
}

int main() {
    std::string expr = "a=9-3+10/2+8*2;a+=3*2%2;if(a==b)a=1";
    std::vector<std::string> arr;

    if (splitChunks(expr, arr)) {
        std::cout << "Tokens: \n";
        for (const std::string& token : arr) {
            std::cout << "'" << token << "'\n";
        }
    } else {
        std::cout << "No tokens found.\n";
    }

    return 0;
}*/