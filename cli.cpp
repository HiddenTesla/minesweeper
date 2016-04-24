#include "cli.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace minesweeperCLI;

const char* firstNonSpace(const char* str) {
   if (!str) return NULL;
   while ( *str == ' ' || *str == '\t') ++str; 
   return str;
}

inline bool isEmpty(const string& str) {
    return *firstNonSpace(str.c_str()) == '\0';
}

const char* findNext(const char* source, char key) {
    if(!source) return NULL;
    while (*source != key && *source != '\0') ++source;
    return source;
}

namespace minesweeperCLI
{

Command prompt() {
    string line;
    do {
        getline(cin, line);
    }
    while ( isEmpty(line) );

    cout << "You entered: " << line << endl;
    Command outcome;
    const char* ptr = line.c_str();
    ptr = firstNonSpace(ptr);
    while (*ptr == ',') ++ptr;
    
    bool compound = false;
    switch (*ptr) {
    case 'p': case 'P':
        outcome.type = PRINT;
        break;
    case 's': case 'S':
        outcome.type = SURRENDER;
        break;
    case 'c': case 'C':
        outcome.type = CLICK;
        compound = true;
        break;
    case 'm': case 'M':
        outcome.type = MARK;
        compound = true;
        break;
    case 'q': case 'Q':
        outcome.type = QUESTION;
        compound = true;
        break;
    case 'u': case 'U':
        outcome.type = UNMARK;
        compound = true;
        break;
    default:
        outcome.type = UNKNOWN;
        break;
    }
    
    if (compound) {
        const char* comma = findNext(ptr, ',');
        if ( *comma == '\0' ) {
            cout << "Error: Click, mark, question, or unmark command must have one comma.\n";
            outcome.type = UNKNOWN;
            return outcome;
        }
        const char* head = ptr;
        while (!isspace(*head)) ++head;
        const char* end = comma;
        ++end;
        if (*end == ',' || *firstNonSpace(end) == '\0') {
            cout << "Error: second parameter missing.\n";
            outcome.type = UNKNOWN;
            return outcome;
        }
        while (*end != ',' && *end != '\0') ++end;

        string para1(head, comma);
        string para2(comma + 1, end);

        outcome.row = atoi(para1.c_str());
        outcome.column = atoi(para2.c_str());
    }


    return outcome;
}

}
