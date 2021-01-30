#include <iostream> 
#include <vector>
#include <algorithm> // for all_of function
#include <string> 

using namespace std;

// trim leading and trailing spaces from string s
string trim(string s) {
    string whitespace = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(whitespace) + 1);
    s.erase(0, s.find_first_not_of(whitespace));
    return s;
}

// split string s by the given delimiter and returns vector containing strings split
vector<string> split(string s, string delimiter) {
    size_t pos = 0;
    string token;
    vector<string> result;
    while ((pos = s.find(delimiter)) < s.length()) {
        token = s.substr(0, pos);
        result.push_back(trim(token));
        s.erase(0, pos + delimiter.length());
    }
    s = trim(s);
    if (s.length() != 0) {
        result.push_back(s);
    }
    return result;
}

// checks LETTER(LETTER | DIGIT)* ​
bool checkName(string s) {
    bool isValid = false;
    if (isalpha(s[0])) {
        isValid = true;
        for (int i = 1; i <= s.length() - 1; i++)
        {
            isValid = (isalnum(s[i]));

            if (!isValid) {
                break;
            }
        }
    }
    return isValid;
}

//  checks DIGIT +
bool isNumber(string s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

bool checkNameWithQuotes(string s) {
    // am i suppose to check if it comforms to naming standards 
    // or if it exists in code or both?
    return 0;
}