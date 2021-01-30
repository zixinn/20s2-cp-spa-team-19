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

    friend bool operator==(const Clause& c1, const Clause& c2);

    ~Clause();

private:
    string rel;
    vector<string> args;
};
