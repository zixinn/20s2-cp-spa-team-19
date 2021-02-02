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
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

// Checks if name with quotes conforms to naming standards
// Returns true if name is valid, false otherwise
bool checkNameWithQuotes(string s) {
    if (s[0] != '\"' || s[s.length() - 1] != '\"') {
        return false;
    }
    return checkName(trim(s.substr(1, s.length() - 2)));
}

// Check if expression is valid
// Returns true if expression is valid, false otherwise
bool checkExpression(string s) {
    // This is just a stub, SP will be implementing it
    if (s[0] != '\"' || s[s.length() - 1] != '\"') {
        return false;
    }
    string str = trim(s.substr(1, s.length() - 2));
    // TODO
    return true;
}

// Checks if expression with underscores is valid
// Returns true if expression is valid, false otherwise
bool checkExpressionWithUnderscores(string s) {
    if (s[0] != '_' || s[s.length() - 1] != '_') {
        return false;
    }
    return checkExpression(trim(s.substr(1, s.length() - 2)));
}

/* Pseudo code for comparing AST as SP hasn't finalise AST structure
// Compares two ASTs, returns true if AST1 == AST2
// This is a helper function for compareAst
bool areIdentical(AST* root1, AST*, root2) {
    // base cases
    if (root1 == NULL && root2 == NULL)
        return true;

    if (root1 == NULL || root2 == NULL)
        return false;

    // Check if the data of both roots is same and data of left and right
    // subtrees are also same
    return (root1->data == root2->data &&
        areIdentical(root1->left, root2->left) &&
        areIdentical(root1->right, root2->right));
}

// Compares two ASTs, returns true if AST2 is a subexpression of AST1
bool isSubtree(AST* root1, AST* root2) {
    // base cases
    if (root1 == NULL || root2 == NULL)
        return true;

    // Check the tree with root as current node
    if (areIdentical(root1, root2))
        return true;

    // If the tree with root as current node doesn't match then try left
    // and right subtrees one by one
    return isSubtree(root1->left, root2) ||
        isSubtree(root1->right, root2);
}
*/
