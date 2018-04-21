#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
using std::vector;
static const int BLACK = 1;
static const int EMPTY = 0;
static const int WHITE = -1;
class Board
{
public:
    int rows;
    int cols;
    vector<int> data;

    Board();
    int& at(int row, int col);
    bool isLegalMove(int row, int col, int player);
    bool isLegalDirection(int row, int col, int drow, int dcol, int player);
    bool isVaildPosition(int row, int col);
    void reverse(int row, int col, int player);
    void reverseDirection(int row, int col, int drow, int dcol, int player);
    bool canMove(int player);
    vector<int> legalMoves(int player);
    bool isFull();
    int score();
    int blackCount();
    int whiteCount();
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
#endif // GRID_HPP
