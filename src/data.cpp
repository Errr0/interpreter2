#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <vector> 
#include <array>
#include <map>
#include <regex> 
#include <algorithm>
#include <set>
#include <memory>
#include <stack>

enum TokenType {
    END,
    ENDLINE,
    WHITESPACE,
    ARITMETIC_OPERATOR,
    ASSIGN, 
    LOGICAL_OPERATOR,
    VOID,
    INT,
    FLOAT,
    IDENTIFIER,
    KEYWORD,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    INCREMENT,
    DECREMENT,
    QUOTATION,
    QUESTIONMARK,
    COMMA,
    COLON,
    DOT,
    BACKSLASH,
    DOUBLESLASH,
    HASHTAG,
    AMPERSAND,
    SCOPE,
    STATEMENT,
    ARRAY,
    VARIABLE,
    FUNCTION,
    FUNCTION_DECLARATION,
    CLASS,
    OBJECT,
    NUL,
    DATATYPE,
    DECLARATION
};

class Token{
    public:
    std::string value;
    enum TokenType type;
    int weight;
    Token(std::string value = ";",enum TokenType type = END, unsigned int weight = 0){
        this -> value = value;
        this -> type = type;
        this -> weight = weight;
    }
};

Token nullvalue = Token("null", NUL);

std::string displayTokenType(TokenType token) {
    switch (token) {
        case END: return "END";
        case ENDLINE: return "ENDLINE";
        case ASSIGN: return "ASSIGN";
        case ARITMETIC_OPERATOR: return "ARITMETIC_OPERATOR";
        case LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";
        case VOID: return "VOID";
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case IDENTIFIER: return "IDENTIFIER";
        case KEYWORD: return "KEYWORD";
        case BRACKET_OPEN: return "BRACKET_OPEN";
        case BRACKET_CLOSE: return "BRACKET_CLOSE";
        case QUOTATION: return "QUOTATION";
        case QUESTIONMARK: return "QUESTIONMARK";
        case COMMA: return "COMMA";
        case COLON: return "COLON";
        case DOT: return "DOT";
        case BACKSLASH: return "BACKSLASH";
        case DOUBLESLASH: return "DOUBLESLASH";
        case HASHTAG: return "HASHTAG";
        case WHITESPACE: return "WHITESPACE";
        case DECLARATION: return "DECLARATION";
        case VARIABLE: return "VARIABLE";
        case NUL: return "NULL";
        case SCOPE: return "SCOPE";
        case STATEMENT: return "STATEMENT";
        case ARRAY: return "ARRAY";
        default: return "UNKNOWN";
    }
}

std::string displayType(TokenType token) {
    switch (token) {
        case END: return "";
        case ENDLINE: return "endl";
        case ASSIGN: return "";
        case ARITMETIC_OPERATOR: return "";
        case LOGICAL_OPERATOR: return "";
        case INT: return "(i)";
        case FLOAT: return "(f)";
        case IDENTIFIER: return "(id)";
        case KEYWORD: return "(kw)";
        case BRACKET_OPEN: return "";
        case BRACKET_CLOSE: return "";
        case QUOTATION: return "";
        case QUESTIONMARK: return "";
        case COMMA: return "";
        case COLON: return "";
        case DOT: return "";
        case BACKSLASH: return "";
        case DOUBLESLASH: return "";
        case HASHTAG: return "";
        case WHITESPACE: return "";
        case AMPERSAND: return "";
        case SCOPE: return "s";
        default: return "";
    }
}

std::array<int,6> values = {0, 1, 2, 3, 4, 5};

std::map<std::string, Token> locked = {
    {"~END", Token(";", END)},
    {"~ENDLINE", Token("\n", ENDLINE)},

    {"~STRONG_ASSIGN", Token("=", ASSIGN, -values[5])},
    {"~INCREMENT", Token("+", ARITMETIC_OPERATOR, -values[4])},
    {"~INCREMENT", Token("-", ARITMETIC_OPERATOR, -values[4])},

    {"~ASSIGN", Token("=", ASSIGN, -values[3])},
    {"~ADDASSIGN", Token("+", ARITMETIC_OPERATOR, -values[1])},
    {"~SUBASSIGN", Token("-", ARITMETIC_OPERATOR, -values[1])},
    {"~MULASSIGN", Token("*", ARITMETIC_OPERATOR, -values[2])},
    {"~DIVASSIGN", Token("/", ARITMETIC_OPERATOR, -values[2])},
    {"~MODASSIGN", Token("%", ARITMETIC_OPERATOR, -values[2])},

    {"~PLUS", Token("+", ARITMETIC_OPERATOR, values[1])},
    {"~MINUS", Token("-", ARITMETIC_OPERATOR, values[1])},
    {"~ASTERISK", Token("*", ARITMETIC_OPERATOR, values[2])},
    {"~SLASH", Token("/", ARITMETIC_OPERATOR, values[2])},
    {"~MODULO", Token("%", ARITMETIC_OPERATOR, values[2])},

    {"~NOT", Token("!", LOGICAL_OPERATOR)},
    {"~AND", Token("&&", LOGICAL_OPERATOR)},
    {"~OR", Token("||", LOGICAL_OPERATOR)},
    {"~EQUAL", Token("==", LOGICAL_OPERATOR)},
    {"~NOTEQUAL", Token("!=", LOGICAL_OPERATOR)},
    {"~LESSTHAN", Token("<", LOGICAL_OPERATOR)},
    {"~LESSEQUAL", Token("<=", LOGICAL_OPERATOR)},
    {"~GREATERTHAN", Token(">", LOGICAL_OPERATOR)},
    {"~GREATEREQUAL", Token(">=", LOGICAL_OPERATOR)},

    {"~BRACKETOPEN", Token("(", BRACKET_OPEN)},
    {"~BRACKETCLOSE", Token(")", BRACKET_CLOSE)},
    {"~SQUAREBRACKETOPEN", Token("[", BRACKET_OPEN)},
    {"~SQUAREBRACKETCLOSE", Token("]", BRACKET_CLOSE)},
    {"~CURLYBRACKETOPEN", Token("{", BRACKET_OPEN)},
    {"~CURLYBRACKETCLOSE", Token("}", BRACKET_CLOSE)},

    {"~APOSTROPHE", Token("\'", QUOTATION)},
    {"~QUOTATION", Token("\"", QUOTATION)},

    {"~QUESTIONMARK", Token("?", QUESTIONMARK)},
    {"~COMMA", Token(",", COMMA)},
    {"~COLON", Token(":", COLON)},
    {"~DOT", Token(".", DOT)},
    {"~BACKSLASH", Token("\\", BACKSLASH)},
    {"~DOUBLESLASH", Token("//", DOUBLESLASH)},
    {"~HASHTAG", Token("#", HASHTAG)},
    {"~AMPERSAND", Token("&", AMPERSAND)},

    {"~SPACE", Token(" ", WHITESPACE)},
    {"~TAB", Token("\t", WHITESPACE)}
};

std::map<std::string, std::string> lockSymbol = {
    {";", "~END"},
    {"\n", "~ENDLINE"},
    {"=", "~ASSIGN"},
    {"+", "~PLUS"},
    {"-", "~MINUS"},
    {"*", "~ASTERISK"},
    {"/", "~SLASH"},
    {"%", "~MODULO"},
    {"++", "~INCREMENT"},
    {"--", "~DECREMENT"},
    {"+=", "~ADDASSIGN"},
    {"-=", "~SUBASSIGN"},
    {"*=", "~MULASSIGN"},
    {"/=", "~DIVASSIGN"},
    {"%=", "~MODASSIGN"},
    {"!", "~NOT"},
    {"&&", "~AND"},
    {"||", "~OR"},
    {"==", "~EQUAL"},
    {"!=", "~NOTEQUAL"},
    {"<", "~LESSTHAN"},
    {"<=", "~LESSEQUAL"},
    {">", "~GREATERTHAN"},
    {">=", "~GREATEREQUAL"},
    {"(", "~BRACKETOPEN"},
    {")", "~BRACKETCLOSE"},
    {"[", "~SQUAREBRACKETOPEN"},
    {"]", "~SQUAREBRACKETCLOSE"},
    {"{", "~CURLYBRACKETOPEN"},
    {"}", "~CURLYBRACKETCLOSE"},
    {"\'", "~APOSTROPHE"},
    {"\"", "~QUOTATION"},
    {"?", "~QUESTIONMARK"},
    {",", "~COMMA"},
    {":", "~COLON"},
    {".", "~DOT"},
    {"\\", "~BACKSLASH"},
    {"//", "~DOUBLESLASH"},
    {"#", "~HASHTAG"},
    {" ", "~SPACE"},
    {"\t", "~TAB"},
    {"&", "~AMPERSAND"}
    
};

std::vector<std::string> operators = {
    "{",
    "}",
    ";",
    " ",
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
    "\n",
    //";",
    "\t",
    //" ",
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
    "'",
    "\"",
    "?",
    ",",
    ":",
    ".",
    "\\",
    "#",
    "&"
    };

std::set<std::string> keywords = {
    "def",
    "int",
    "float",
    "char",
    "bool",
    "void",

    "include",
    "return",
    "if",
    "else",
    "switch",
    "case",
    "for",
    "while",
    "do",
    "break",
    "continue",
    "struct",
    "class",
    "public",
    "private",
    "protected",
    "const",
    "new",
    "delete",
    };

std::map<std::string, int> enumerateKeywords = {
        {"include", 1},
        {"return", 2},

        {"int", 3},
        {"float", 4},
        {"char", 5},
        {"bool", 6},
        {"void", 7},

        {"if", 8},
        {"else", 9},
        {"switch", 10},
        {"case", 11},
        {"for", 12},
        {"while", 13},
        {"do", 14},

        {"break", 15},
        {"continue", 16},
        {"struct", 17},
        {"class", 18},
        {"public", 19},
        {"private", 20},
        {"protected", 21},
        {"const", 22},
        {"new", 23},
        {"delete", 24}
    };

std::set<std::string> dataTypes = {
    "int",
    "float",
    "char",
    "bool",
    "void"
};