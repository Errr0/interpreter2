#include "checks.cpp"

bool eraseBetween(std::string& str, const std::string& start, const std::string& end) {
    size_t startPos = str.find(start);  // Find the starting substring
    size_t endPos = str.find(end, startPos);  // Find the ending substring after the start
    if (startPos != std::string::npos && endPos != std::string::npos) {
        str.erase(startPos, (endPos + end.length()) - startPos);
        return true;
    }
    return false;
} 

std::string replaceBetween(std::string& str, const std::string& start, const std::string& end, const std::string& replacement) {
    std::string replacedPart = "";
    size_t startPos = str.find(start);
    
    if (startPos != std::string::npos) {
        size_t endPos = str.find(end, startPos + start.length());

        if (endPos != std::string::npos) {
            replacedPart = str.substr(startPos+start.length(), endPos - startPos - end.length());
            str.replace(startPos, (endPos + end.length()) - startPos, replacement);
        }
    }
    return replacedPart;
}

void split(std::string str, std::vector<std::string> &arr, std::string symbol = " ", bool include = true, bool separate = true, bool lock = true) {
    size_t start = 0, end = str.find(symbol);
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
    arr.push_back(str.substr(start));
}

void splitString(std::string &str, std::vector<std::string> symbols, std::vector<std::string>& output){
    if(str!=""){
        output.push_back(str);
    }
    for (std::string& symbol : symbols) {
        std::vector<std::string> temp;
        for (std::string& substr : output) {
            if(substr.find(symbol) != std::string::npos){
                split(substr, temp, symbol, true, true, true);
            } else {
                if(substr != ""){
                    temp.push_back(substr);
                }
            }
        }
        output = std::move(temp);
    }
}

std::string processBackSlash(std::string input){
    if(input[1]=='n'){
        return std::string(0, '\n');
    // } else if(input[1]=='n'){
    //     return '\n';
    // } else if(input[1]=='n'){
    //     return '\n';
    // } else if(input[1]=='n'){
    //     return '\n';
    // } else if(input[1]=='n'){
    //     return '\n';
    } else {
        return std::string(1,input[1]);
    }
}

void processChar(std::string string){
    if(string.length()>=1){
        if(string.length()==1){
            Chars.push(string);
        } else if(string.length()==2 && string[0]=='\\'){
            Chars.push(processBackSlash(string));
        } else{
            std::cerr<<"char too long "<<string.length()<<":"<<string<<"\n";
        }
    } else {
        std::cerr<<"char too short '"<<string<<"'\n";
        Chars.push("");
    }
}

void processInput(std::string& code){
    code+="\n";
    while(eraseBetween(code, "/*", "*/")); 
    while(eraseBetween(code, "//", "\n"));
    //std::cout<<code<<"~~\n";
    std::string stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    while(stringLiteral!=""){
        //std::cout<<stringLiteral<<"\n";
        StringsLiterals.push(stringLiteral);

        stringLiteral = replaceBetween(code, "\"", "\"", " ~STRING ");
    }
    stringLiteral = replaceBetween(code, "'", "'", " ~CHAR ");
    while(stringLiteral!=""){
        //std::cout<<stringLiteral<<"\n";
        //Chars.push(stringLiteral);
        processChar(stringLiteral);
        stringLiteral = replaceBetween(code, "'", "'", " ~CHAR ");
    }
    //std::cout<<"\n"<<code<<"\n";
}


