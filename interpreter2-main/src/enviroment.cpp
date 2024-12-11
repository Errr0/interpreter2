#include "include.cpp"
#include "checks.cpp"
#include "strings.cpp"
#include "tokenizer.cpp"

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
