#include "TestWrapper.h"
#include "SP/Parser.h"
#include "PQL/QueryPreprocessor.h"
#include "PQL/QueryEvaluator.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
    //std::vector<string> stubTokens{ "procedure", "main", "{", "x", "=", "10", ";", "}", "EOF" };
    //auto l = new LexerStub(stubTokens);     //new keyword gets me a ptr to LexerStub
    //Parser p = Parser(l);
		//2, 11, 30, 11, 10, 11, 13, 12, 14, 0, 27, 31
	/*
	vector<Token> tokens;
	bool valid = Lexer::tokenise("procedure q {\n z = x + 1-    a1;}", tokens);
	for (int i = 0; i < tokens.size(); i++) {
		cout << tokens[i].getLiteral() << " " << int(tokens[i].getType()) <<endl;
	}
	cout << valid << endl;
	*/
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...
    if (valid) {
        QueryPreprocessor qp = QueryPreprocessor();
        Query q = qp.process(query);

        QueryEvaluator qe = QueryEvaluator();
        results = qe.evaluate(q);
    }
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
}
