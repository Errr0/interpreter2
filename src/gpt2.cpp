#include <iostream>
#include <string>
#include <vector>

enum TokenType {
    END,
    ASSIGN,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    MODULO,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBTRACT,
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
    IDENTIFIER,
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
    INCREMENT,
    DECREMENT,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    TERNARY
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.length()) {
        char ch = input[i];

        // Skip whitespace
        if (isspace(ch)) {
            i++;
            continue;
        }

        // Match numbers
        if (isdigit(ch)) {
            std::string num;
            while (i < input.length() && isdigit(input[i])) {
                num += input[i++];
            }
            tokens.push_back({NUMBER, num});
            continue;
        }

        // Match identifiers
        if (isalpha(ch) || ch == '_') {
            std::string ident;
            while (i < input.length() && (isalnum(input[i]) || input[i] == '_')) {
                ident += input[i++];
            }
            tokens.push_back({IDENTIFIER, ident});
            continue;
        }

        // Match operators and symbols
        switch (ch) {
            case '+':
                if (i + 1 < input.length() && input[i + 1] == '+') {
                    tokens.push_back({INCREMENT, "++"});
                    i += 2;
                } else if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({ADD_ASSIGN, "+="});
                    i += 2;
                } else {
                    tokens.push_back({PLUS, "+"});
                    i++;
                }
                break;

            case '-':
                if (i + 1 < input.length() && input[i + 1] == '-') {
                    tokens.push_back({DECREMENT, "--"});
                    i += 2;
                } else if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({SUB_ASSIGN, "-="});
                    i += 2;
                } else {
                    tokens.push_back({MINUS, "-"});
                    i++;
                }
                break;

            case '*':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({MUL_ASSIGN, "*="});
                    i += 2;
                } else {
                    tokens.push_back({ASTERISK, "*"});
                    i++;
                }
                break;

            case '/':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({DIV_ASSIGN, "/="});
                    i += 2;
                } else if (i + 1 < input.length() && input[i + 1] == '/') {
                    tokens.push_back({DOUBLESLASH, "//"});
                    i += 2;
                } else {
                    tokens.push_back({SLASH, "/"});
                    i++;
                }
                break;

            case '%':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({MOD_ASSIGN, "%="});
                    i += 2;
                } else {
                    tokens.push_back({MODULO, "%"});
                    i++;
                }
                break;

            case '?': tokens.push_back({TERNARY, "?"}); i++; break;
            case '(': tokens.push_back({BRACKETOPEN, "("}); i++; break;
            case ')': tokens.push_back({BRACKETCLOSE, ")"}); i++; break;
            case '{': tokens.push_back({CURLYBRACKETOPEN, "{"}); i++; break;
            case '}': tokens.push_back({CURLYBRACKETCLOSE, "}"}); i++; break;
            case '[': tokens.push_back({SQUAREBRACKETOPEN, "["}); i++; break;
            case ']': tokens.push_back({SQUAREBRACKETCLOSE, "]"}); i++; break;
            case ',': tokens.push_back({COMMA, ","}); i++; break;
            case ':': tokens.push_back({COLON, ":"}); i++; break;
            case '=':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({EQUAL, "=="});
                    i += 2;
                } else {
                    tokens.push_back({ASSIGN, "="});
                    i++;
                }
                break;

            case '<':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({LESSEQUAL, "<="});
                    i += 2;
                } else {
                    tokens.push_back({LESSTHAN, "<"});
                    i++;
                }
                break;

            case '>':
                if (i + 1 < input.length() && input[i + 1] == '=') {
                    tokens.push_back({GREATEREQUAL, ">="});
                    i += 2;
                } else {
                    tokens.push_back({GREATERTHAN, ">"});
                    i++;
                }
                break;

            default:
                // Handle unknown characters
                std::cerr << "Unknown character: " << ch << std::endl;
                i++;
                break;
        }
    }

    return tokens;
}

int main() {
    std::string input = "int a = 1 + 3 * 5 / 5; int b = 3 + -2 - 2 - 2 - 2 - 1 + 4 * 4; if (a += b >= 2) return b;";
    auto tokens = tokenize(input);

    for (const auto& token : tokens) {
        std::cout << "Token: " << token.value << " Type: " << token.type << std::endl;
    }

    return 0;
}
