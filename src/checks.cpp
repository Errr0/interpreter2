#include "include.cpp"

std::set<std::string> keywords = {
    "include",
    "return",
    "int",
    "float",
    "char",
    "bool",
    "void",

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

bool isInt(std::string str){
    std::regex regex ("[-]?[0-9]+");
    return std::regex_match(str, regex);
}

bool isFloat(std::string str) {
    std::regex regex ("[-]?([0-9]+\\.[0-9]+|\\.[0-9]+)");
    return std::regex_match(str, regex);
}

bool isNumber(std::string str){
    std::regex regex ("[0-9]+");
    return std::regex_match(str, regex);
}

bool isIdentifier(std::string str) {
    std::regex regex ("[_a-zA-Z][_a-zA-Z0-9]*");
    return std::regex_match(str, regex);
}

bool isChar(std::string str) {
    std::regex regex ("[a-zA-Z]");
    return std::regex_match(str, regex);
}

bool isKeyword(std::string str) {
    return (keywords.find(str)!=keywords.end());
}