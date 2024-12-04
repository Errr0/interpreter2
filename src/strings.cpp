#include "include.cpp"

std::map<std::string, std::string> lockSymbol = {
    {";", "~END"},
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
    {"'", "~APOSTROPHE"},
    {"\"", "~QUOTATION"},
    {"?", "~QUESTIONMARK"},
    {",", "~COMMA"},
    {":", "~COLON"},
    {".", "~DOT"},
    {"\\", "~BACKSLASH"},
    {"//", "~DOUBLESLASH"},
    {"#", "~HASHTAG"},
    {" ", "~SPACE"}
};

int numberOfOcurrences(std::string str, std::string substr){
    int count = 0;
    size_t start = 0, end = str.find(substr);
    while (end != std::string::npos) {
        count++;
        start = end + 1;
        end = str.find(substr, start);
    }
    //std::cout<<"|nr of ocur "<<substr<<" in "<<str<<" = "<<count<<"|";
    return count;
}

void split(std::string str, std::vector<std::string> &arr, std::string symbol = " ", bool include = true, bool separate = true, bool lock = true) {
    size_t start = 0, end = str.find(symbol);//int with different name
    while (end != std::string::npos) {
        if(start != end || (start==end && str.substr(start, symbol.length()) == symbol)){
            arr.push_back(str.substr(start, (include && !separate ? end - start + symbol.length() : end - start)));
            if(include && separate){
                arr.push_back((lock ? lockSymbol[symbol] : symbol));
            }
        }
        start = end + (include ? symbol.length() : 1);
        end = str.find(symbol, start);
    }
    if(str.substr(start) != " " && str.substr(start) != "")
    arr.push_back(str.substr(start)); // Add the last word
}

std::string merge(std::vector<std::string> &arr){
    std::string str = "";
    for (std::string& s : arr) {
        str+=" "+s;
    }
    return str;
}

void replace(std::string &str, std::string from, std::string to) {
    str = std::regex_replace(str, std::regex(from), to);
}
