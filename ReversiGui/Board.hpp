#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
using std::vector;
class Board
{
public:
    static const int BLACK = 1;
    static const int EMPTY = 0;
    static const int WHITE = -1;
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
};

#endif // GRID_HPP
