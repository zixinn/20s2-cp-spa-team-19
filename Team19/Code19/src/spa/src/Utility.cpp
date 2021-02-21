#include "Utility.h" 

using namespace std;

// Trim leading and trailing spaces from string s
string trim(string s) {
    string whitespace = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(whitespace) + 1);
    s.erase(0, s.find_first_not_of(whitespace));
    return s;
}

// Split string s by the given delimiter and returns vector containing strings split
// Strings in vector will be trimmed
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

// Checks LETTER(LETTER | DIGIT)*
// Returns true if name is valid, false otherwise
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

// Checks DIGIT +
// Returns true if integer is valid, false otherwise
bool checkInteger(string s) {
    if (s.length() > 1 && s.at(0) == '0') {
        return false;
    }
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}
