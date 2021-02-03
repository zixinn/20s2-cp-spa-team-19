#pragma once

#include <vector>
#include "AST/Proc.h"

namespace ast {
	class Program //: public Node
	{
		Proc* main;
		std::vector<Proc*> procedures; // default initialization is called when not mentioned in constructor
	public:
		Program() { main = NULL; }
		std::string getTokenLiteral() //override 
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
