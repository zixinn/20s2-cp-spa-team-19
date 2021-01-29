#include "ConstTable.h"

ConstTable::ConstTable() = default;

int ConstTable::getConstValue(STRING input) {
    try {
        return constMap.at(input);
    } catch (exception e) {
        std::cerr << input << " was not found in the ConstTable.\n";
        return -1;
    }
}

bool ConstTable::hasConst(STRING input) {
    return constMap.find(input) != constMap.end();
}

int ConstTable::getSize() {
    return constMap.size();
}

bool ConstTable::storeConst(STRING input) {
    if (constMap.count(input) == 1) {
        return false;
    } else {
        // Changes input from string to integer form.
        try {
            int inputValue = stoi(input);
            constMap[input] = inputValue;
            return  true;
        } catch (exception e) {
            std::cerr << input << " could not be converted into an integer.\n";
            throw;
        }
    }
}
