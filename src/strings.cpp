#include "checks.cpp"

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

void eraseBetween(std::string& str, const std::string& start, const std::string& end) {
    size_t startPos = str.find(start);  // Find the starting substring
    size_t endPos = str.find(end, startPos);  // Find the ending substring after the start
    if (startPos != std::string::npos && endPos != std::string::npos) {
        str.erase(startPos, (endPos + end.length()) - startPos);
    }
} 

std::string replaceBetween(std::string& str, const std::string& start, const std::string& end, const std::string& replacement) {
    std::string replacedPart = ""; // To store the replaced substring
    size_t startPos = str.find(start);  // Find the starting substring
    
    if (startPos != std::string::npos) {
        size_t endPos = str.find(end, startPos + start.length());  // Find the ending substring after the start

        if (endPos != std::string::npos) {
            // Extract the replaced part
            replacedPart = str.substr(startPos, (endPos + end.length()) - startPos);
            // Replace the part from startPos to the end of end with the replacement string
            str.replace(startPos, (endPos + end.length()) - startPos, replacement);
        }
    }
    return replacedPart;  // Return the replaced part
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
