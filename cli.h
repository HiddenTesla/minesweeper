#ifndef _CLI_H_
#define _CLI_H_

#include <string>
using std::string;

namespace minesweeperCLI
{

enum CommandType {
    UNKNOWN,
    HELP,
    PRINT,
    CLICK,
    MARK,
    QUESTION,
    UNMARK,
    SURRENDER,
};


struct Command {
    CommandType type;
    short row;
    short column;
};

Command prompt();

} //end of namespace minesweeperCLI


#endif
