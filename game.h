#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::pair;

enum CellState {
    INTACT,
    MARKED,
    QUESTIONED,
    CLEARED
};

struct Cell {
    CellState state;
    short around;
    bool mined;
    Cell(): state(INTACT), around(-1), mined(false) {}
    ~Cell() {}
};

class Game {
private:
    Game(const Game&);
    Game& operator =(const Game&);

    Cell** map;
    short rows;
    short columns;
    short mines;
    short intacts;
    bool initialized;
    bool win;

    void allocate();

    void evalEachCell();

    typedef vector<Cell*> Cells;
    typedef pair<short, short> Cord;
    typedef vector<Cord> Cords;
    
public:
    static const short DEFAULT_SIZE = 20;
    static const short MAX_SIZE = 99;
    static const short MIN_SIZE = 3;

    Game(const string& filename = "mine.map");
    ~Game();
    bool readFrom(const string& filename);
    void print() const;
    void printMine() const;
    void printFinal() const;

    Cells getCellsAdjacent(short row, short column) const;
    Cords getCordsAdjacent(short row, short cloumn) const;
    void mark(short row, short column);
    void question(short row, short column);
    void unmark(short row, short column);
    void click(short row, short column);

    void start();
};

#endif
