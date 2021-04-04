#include "Utility.h"

using namespace std;

STRING trim(STRING s) {
    STRING whitespace = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(whitespace) + 1);
    s.erase(0, s.find_first_not_of(whitespace));
    return s;
}

vector<STRING> split(STRING s, STRING delimiter) {
    size_t pos = 0;
    STRING token;
    vector<STRING> result;
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

bool checkName(STRING s) {
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

bool checkInteger(STRING s) {
    if (s.length() > 1 && s.at(0) == '0') {
        return false;
    }
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}
