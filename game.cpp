#include "game.h"
#include "cli.h"
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;
using namespace minesweeperCLI;

#define DEBUG_ON 1

#if DEBUG_ON
ostream& debug = cout;
#else
struct Dstream: ostream {
    template <typename T>
    Dstream& operator <<(const T&) {return *this;}
};
Dstream debug;

#endif

const short ZERO = 0;

#define cord(x, y) "(" << x << ", " << y << ")"

inline short middle(short source, short upBound, short lowBound) {
    short tmp = min(source, upBound);
    tmp = max(source, lowBound);
    return tmp;
}

#define trim( x ) { \
    if (x > MAX_SIZE) x = MAX_SIZE; \
    if (x < MIN_SIZE) x = MIN_SIZE; } 

Game::Game(const string& filename)
    : map(NULL), 
      mines(0), 
      intacts(0),
      initialized(false),
      win(false)
{
    readFrom(filename);        
}

void Game::allocate() {
    map = new Cell* [rows];
    for (short i = 0; i < rows; ++i) {
        map[i] = new Cell[columns];
    }
}

Game::~Game() {
    for (short i = 0; i < rows; ++i) {
        delete[] map[i];
        map[i] = NULL;
    }
}

bool Game::readFrom(const string& filename) {
    ifstream fin(filename.c_str());
    if (fin.fail()) {
        debug << "Unable to find file: " << filename << ". Aborted.\n";
        return false;
    }
    fin >> rows >> columns;
    allocate();

    char ch;
    short num = 0;
    while (!fin.eof()) {
        if (fin.fail()) break;
        ch = fin.get();
        if (!isspace(ch) && ch != -1) {
            //debug << "Get a character: " << (short)ch << endl;
            const short row = num / columns;
            const short column = num % columns;
            Cell& cell = map [row] [column];
            if (ch == '+') {
                cell.mined = true;
                ++mines;
            }
            else {
                ++intacts;
            }
            cell.around = getCellsAdjacent(row, column).size();
            ++num;
        }
    }
    //debug << "Totally " << num << " non-empty characters.\n";

    fin.close();
    evalEachCell();
    initialized = true;
    return true;
}

void Game::evalEachCell() {
    for (short i = 0; i < rows; ++i) {
        for (short j = 0; j < columns; ++j) {
            Cell& cell = map[i][j];
            if (cell.around == -1) continue;
            cell.around = 0;
            Cells adjs = getCellsAdjacent(i, j);
            for (Cells::iterator iter = adjs.begin(); iter != adjs.end(); ++iter) {
                if ( (*iter)->mined)
                ++cell.around;
            }
        }
    }

}

void Game::print() const {
    assert(initialized);
    cout << "Board state\n    ";
    for (short k = 0; k < columns; ++k) {
        if (k < 10) cout << ' ';
        cout << k << " ";
    }
    cout << '\n';

    for (short i = 0; i < rows; ++i) {
        if (i < 10) cout << ' ';
        cout << i << "|  ";

        for (short j = 0; j < columns; ++j) {
            switch (map[i][j].state) {
            case INTACT:
                cout << "-  ";
                break;
            case MARKED:
                cout << "M  ";
                break;
            case QUESTIONED:
                cout << "?  ";
                break;
            case CLEARED:
                cout << map[i][j].around << "  ";
                break;
            default:;

            }

        }
        cout << '\n';

    }
    cout << "Mines not marked: " << mines << 
            ". Intact: " << intacts << endl;
}

void Game::printMine() const {
    assert(initialized);
    cout << "Mine state: \n    ";
    for (short k = 0; k < columns; ++k) {
        if (k < 10) cout << ' ';
        cout << k << " ";
    }
    cout << '\n';

    for (short i = 0; i < rows; ++i) {
        if (i < 10) cout << ' ';
        cout << i << "|  ";

        for (short j = 0; j < columns; ++j) {
            if (map[i][j].mined) {
                if (map[i][j].state == CLEARED)
                    cout << "*! ";
                else
                    cout << "*  ";
            }
            else {
                cout << map[i][j].around << "  ";
            }

        }
        cout << '\n';

    }
}

void Game::printFinal() const {
    assert(initialized);
    cout << "Map";
    for (short k = 0; k < columns; ++k) {
        if (k < 10) cout << ' ';
        cout << k << " ";
    }
    cout << '\n';

    for (short i = 0; i < rows; ++i) {
        if (i < 10) cout << ' ';
        cout << i << "|  ";

        for (short j = 0; j < columns; ++j) {
            if (map[i][j].mined) {
                if (map[i][j].state == CLEARED)
                    cout << "*! ";
                else
                    cout << "*  ";
            }
            else {
                cout << map[i][j].around << "  ";
            }

        }
        cout << '\n';

    }
}

Game::Cells Game::getCellsAdjacent(short row, short column) const {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);

    const bool atTop = (row == 0);
    const bool atBottom = (row == rows - 1);
    const bool atLeft = (column == 0);
    const bool atRight = (column == columns - 1);

    Cells adjacent;
    if (!atLeft) {
        adjacent.push_back( &map[row][column-1] );
        if (!atTop)
            adjacent.push_back( &map[row-1][column-1]);
        if (!atBottom)
            adjacent.push_back( &map[row+1][column-1]);

    }

    if (!atRight) {
        adjacent.push_back( &map[row][column+1] );
        if (!atTop)
            adjacent.push_back( &map[row-1][column+1]);
        if (!atBottom)
            adjacent.push_back( &map[row+1][column+1]);
    }

    if (!atTop)
        adjacent.push_back( &map[row-1][column]);
    if (!atBottom)
        adjacent.push_back( &map[row+1][column]);


    return adjacent;

}

Game::Cords Game::getCordsAdjacent(short row, short column) const {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);

    const bool atTop = (row == 0);
    const bool atBottom = (row == rows - 1);
    const bool atLeft = (column == 0);
    const bool atRight = (column == columns - 1);

    Cords adjacent;
    typedef pair<short, short> Pair;
    if (!atLeft) {
        adjacent.push_back( Pair(row, column-1) );
        if (!atTop)
            adjacent.push_back( Pair(row-1, column-1) );
        if (!atBottom)
            adjacent.push_back( Pair(row+1, column-1) );
    }

    if (!atRight) {
        adjacent.push_back( Pair(row, column+1) );
        if (!atTop)
            adjacent.push_back( Pair(row-1, column+1) );
        if (!atBottom)
            adjacent.push_back( Pair(row+1, column+1) );
    }

    if (!atTop)
        adjacent.push_back( Pair(row-1, column) );
    if (!atBottom)
        adjacent.push_back( Pair(row+1, column) );

    return adjacent;

}

void Game::mark(short row, short column) {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);
    CellState& state = map[row][column].state;
    if (state == INTACT || state == QUESTIONED) {
        state = MARKED;
        --mines;
    }
}

void Game::question(short row, short column) {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);
    CellState& state = map[row][column].state;
    switch (state) {
    case INTACT:
        state = QUESTIONED;
        break;
    case MARKED:
        state = QUESTIONED;
        ++mines;
        break;
    default: break;
    }
}

void Game::unmark(short row, short column) {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);
    CellState& state = map[row][column].state;
    switch (state) {
    case MARKED:
        state = INTACT;
        ++mines;
        break;
    case QUESTIONED:
        state = INTACT;
        break;
    default: break;
    }
}


void Game::click(short row, short column) {
    row = middle(row, rows - 1, ZERO);
    column = middle(column, columns - 1, ZERO);
    Cell& cell = map[row][column];
    //debug << "Clicking " << cord(row, column) << endl;
    if (cell.state != INTACT) {
        cout << "Cell " << row << ", " << column << " is already cleared, marked, or questioned.\n";
        return;
    }
    if (cell.mined) {
        cout << "You reveal a mine at " << cord(row, column) << endl;
        cell.state = CLEARED;
        printMine();
        return;
    }
    cell.state = CLEARED;
    --intacts;

    if (intacts <= 0) {
        printFinal();
        cout << "You win!\n";
        win = true;
        return;
    }

    if (cell.around == 0) {
        //debug << "Cell " << cord(row, column) << " has no mines around.\n";
        Cords adj = getCordsAdjacent(row, column);
        for (Cords::const_iterator iter = adj.begin(); iter != adj.end(); ++iter) {
            short rr = iter->first;
            short cc = iter->second;
            if (map[rr][cc].state == INTACT) {
                //debug << cord(rr, cc) << endl;
                click(rr, cc);
            }
        }
    }
}

void Game::start() {
    for (;;) {
        cout << "Enter you command: ";
        Command cmd = prompt();
        short row = cmd.row;
        short column = cmd.column;
        if (cmd.type == SURRENDER) {
            cout << "Are you sure you want to surrender (Y/N)?: ";
            char c = cin.get();
            if (c == 'Y' || c == 'y') {
                cout << "You choose to surrender.\n";
                printMine();
                break;
            }
        }
        else if (cmd.type == PRINT) {
            print();
        }
        else if (cmd.type == CLICK) {
            cout << "Command: Click " << cord(row, column) << endl;
            click(row, column);
        }
        else if (cmd.type == MARK) {
            cout << "Command: Mark " << cord(row, column) << endl;
            mark(row, column);
        }
        else if (cmd.type == QUESTION) {
            cout << "Command: Question " << cord(row, column) << endl;
            question(row, column);
        }
        else if (cmd.type == UNMARK) {
            cout << "Command: Unmark " << cord(row, column) << endl;
            unmark(row, column);
        }
        else if (cmd.type == UNKNOWN) {
            cout << "Invalid command.\n";
        }
        else {}


        if (win) break;
    } // for
}
