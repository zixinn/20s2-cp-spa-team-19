#include <fstream>

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
    std::ifstream theFile;
    theFile.open(filename);
    if (!theFile) {
        valid = false;
    }
    std::string input;
    std::string line;
    while (getline(theFile, line)) {
        input += line + "\n";
    }
    theFile.close();
   // cout << input << endl;
    try {
        std::vector<sp::Token> actual_tok;
        std::vector<sp::Token*> tok_ptrs;
        ParserUtils::StringToTokenPtrs(input, actual_tok, tok_ptrs);
        auto l = new LexerStub(tok_ptrs);
        auto p = Parser(l);
        valid = p.parse();
    } catch (...) {
        valid = false;
    }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...
    if (valid) {
        QueryPreprocessor qp = QueryPreprocessor();
        Query q = qp.process(trim(query));

        QueryEvaluator qe = QueryEvaluator();
        results = qe.evaluate(q);
    }
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
}
