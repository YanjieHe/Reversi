#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
using std::vector;
static const int BLACK = 1;
static const int EMPTY = 0;
static const int WHITE = -1;

enum class GameState
{
    BlackWins,
    WhiteWins,
    Draw,
    BlackPasses,
    WhitePasses,
    Ongoing
};

class Position
{
public:
    int x;
    int y;
    Position(int x, int y);
};
class Board
{
public:
    int xBound;
    int yBound;
    vector<int> data;

    Board();
    int& at(int x, int y);
    bool isLegalMove(int x, int y, int player);
    bool isLegalDirection(int x, int y, int dx, int dy, int player);
    bool isVaildPosition(int x, int y);
    void reverse(int x, int y, int player);
    void reverseDirection(int x, int y, int dx, int dy, int player);
    bool canMove(int player);
    vector<int> legalMoves(int player);
    bool isFull();
    int blackCount();
    int whiteCount();
    GameState state(int player);
    GameState finalState(int blackCount, int whiteCount);
};

class BoardRecord
{
public:
    vector<Board> record;
    int current;
    BoardRecord();
    void push(Board& board);
    void pop();
    Board& peek();
    Board& next();
};

int negativeMax(BoardRecord& boardRecord, int player, int depth);
int evaluate(Board& board, int player);
int evaluateByPositions(Board& board, int player);
int chooseMove(Board& board, int player);

#endif // BOARD_HPP
