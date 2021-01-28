#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <string> 

using namespace std;

string trim(string s);

vector<string> split(string s, string delimiter);

bool checkName(string s);

bool isNumber(string s);

bool checkNameWithQuotes(string s);
