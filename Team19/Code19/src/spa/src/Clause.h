#pragma once

#include <string>
#include <vector>

using namespace std;

class Clause {
public:
    Clause();
    Clause(string rel, vector<string> args);

    string getRel();
    vector<string> getArgs();

    ~Clause();

private:
    string rel;
    vector<string> args;
};
