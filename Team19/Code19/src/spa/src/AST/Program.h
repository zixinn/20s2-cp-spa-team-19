#pragma once
#include <vector>
#include "AST/Proc.h"

// AST representing a Program
namespace ast {
    class Program //: public Node
    {
        Proc* main;
        std::vector<Proc*> procedures; // default initialization is called when not mentioned in constructor
    public:
        Program() { main = NULL; }
        Program(Proc* main, std::vector<Proc*> procedures) : main{ main }, procedures { procedures }{}
        STRING getTokenLiteral() //override
        {
            if (procedures.size() > 0) {
                return (*procedures.front()).getTokenLiteral();
            }
            return "";
        };

        std::vector<Proc*> getProcedures() { return procedures; }
        //std::vector<Proc*>::iterator getIterator() { return procedures.begin(); };

    };
}
