#include "token.cpp"
#include <string>
#include <queue>

class Executor{
    //std::queue<Token> queue;

    //memory
    public:
    Executor(){

    }

    void run(std::queue<Token> queue){
        while(!queue.empty()){
            //do something
            queue.pop();
        }
    }
};