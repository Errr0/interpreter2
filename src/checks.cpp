#include "data.cpp"
 
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
    return keywords.find(str)!=keywords.end();
}