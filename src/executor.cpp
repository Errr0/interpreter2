#include "token.cpp"
#include <string>
#include <queue>
#include <iostream>

class Executor{
    //std::queue<Token> queue;

    //memory
    public:
    Executor(){

    }

    void run(std::queue<Token> queue){
        while(!queue.empty()){
            Token token = queue.front();
            switch(token.action){
                case Actions::END:

                    break;
                case Actions::ADD:

                    break;
                case Actions::PRINT:
                    std::cout<<*static_cast<std::string*>(token.args[0]);
                    break; 
                case Actions::FOR:
                    int times = *static_cast<int*>(token.args[0]);
                    for(int i = 0; i < times ; i++){
                        this->run(*static_cast<std::queue<Token>*>(token.args[1]));
                    }
                    break;
                case Actions::WHILE:
                    while(*static_cast<bool*>(token.args[0])){
                        this->run(*static_cast<std::queue<Token>*>(token.args[1]));
                    }
                    break;
                case Actions::IF:
                    if(*static_cast<bool*>(token.args[0])){
                        this->run(*static_cast<std::queue<Token>*>(token.args[1]));
                    } else {
                        if(token.args[2]){
                            this->run(*static_cast<std::queue<Token>*>(token.args[2]));
                        }
                    }
                    break;
                default:
                    break;
            }
            queue.pop();
        }
    }
};