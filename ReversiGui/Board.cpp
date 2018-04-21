#include "Board.hpp"
#include <climits>
#include <iostream>
using namespace std;
Board::Board()
    : rows{8}
    , cols{8}
    , data(rows * cols)
{
    for (int i = 0, n = data.size(); i < n; i++)
    {
        data.at(i) = 0;
    }
}

int& Board::at(int row, int col)
{
    return data.at(row * cols + col);
}

bool Board::isLegalMove(int row, int col, int player)
{
    if (at(row, col) == EMPTY)
    {
        // check eight directions
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (!(i == 0 && j == 0))
                {
                    if (isLegalDirection(row, col, i, j, player))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

bool Board::isLegalDirection(int row, int col, int drow, int dcol, int player)
{
    row += drow;
    col += dcol;
    if (!isVaildPosition(row, col) || at(row, col) != -player)
    {
        return false;
    }
    row += drow;
    col += dcol;
    while (isVaildPosition(row, col))
    {
        if (at(row, col) == player)
        {
            return true;
        }
        else if (at(row, col) == EMPTY)
        {
            return false;
        }
        row += drow;
        col += dcol;
    }
    return false;
}

bool Board::isVaildPosition(int row, int col)
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Board::reverse(int row, int col, int player)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0))
            {
                if (isLegalDirection(row, col, i, j, player))
                {
                    reverseDirection(row, col, i, j, player);
                }
            }
        }
    }
    at(row, col) = player;
}

void Board::reverseDirection(int row, int col, int drow, int dcol, int player)
{
    row += drow;
    col += dcol;
    while (isVaildPosition(row, col))
    {
        if (at(row, col) == -player)
        {
            at(row, col) = player;
            row += drow;
            col += dcol;
        }
        else
        {
            return;
        }
    }
    throw "error";
}

bool Board::canMove(int player)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (at(i, j) == EMPTY)
            {
                if (isLegalMove(i, j, player))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

vector<int> Board::legalMoves(int player)
{
    vector<int> positions;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (at(i, j) == EMPTY && isLegalMove(i, j, player))
            {
                positions.push_back(i * cols + j);
            }
        }
    }
    return positions;
}

bool Board::isFull()
{
    for (int i = 0, n = data.size(); i < n; i++)
    {
        if (data.at(i) == EMPTY)
        {
            return false;
        }
    }
    return true;
}

int Board::score()
{
    // evaluate when the game is finished
    int blackCount = 0;
    int whiteCount = 0;
    for (int i = 0, n = data.size(); i < n; i++)
    {
        if (data.at(i) == BLACK)
        {
            blackCount++;
        }
        else if (data.at(i) == WHITE)
        {
            whiteCount++;
        }
    }
    if (blackCount > whiteCount)
    {
        return 9999;
    }
    else if (blackCount < whiteCount)
    {
        return -9999;
    }
    else
    {
        return 0;
    }
}

int Board::blackCount()
{
    int count = 0;
    for (int i = 0, n = data.size(); i < n; i++)
    {
        if (data.at(i) == BLACK)
        {
            count++;
        }
    }
    return count;
}

int Board::whiteCount()
{
    int count = 0;
    for (int i = 0, n = data.size(); i < n; i++)
    {
        if (data.at(i) == WHITE)
        {
            count++;
        }
    }
    return count;
}

BoardRecord::BoardRecord()
    : record(64)
    , current{-1}
{
}

void BoardRecord::push(Board& board)
{
    current++;
    Board& currentBoard = record.at(current);
    for (int i = 0, n = board.data.size(); i < n; i++)
    {
        currentBoard.data.at(i) = board.data.at(i);
    }
}

void BoardRecord::pop()
{
    current--;
}

Board& BoardRecord::peek()
{
    return record.at(current);
}

Board& BoardRecord::next()
{
    current++;
    return peek();
}

int negativeMax(BoardRecord& boardRecord, int player, int depth)
{
    Board& board = boardRecord.peek();
    if (board.isFull()) // The game is finished
    {
        int score = board.score();
        return player == BLACK ? score : -score;
    }
    else if (!board.canMove(player))
    {
        if (board.canMove(-player))
        {
            return -negativeMax(boardRecord, -player, depth - 1);
        }
        else
        {
            // both the players cannot make a move
            int score = board.score();
            return player == BLACK ? score : -score;
        }
    }
    else if (depth <= 0) // reach the required depth
    {
        int score = evaluate(board, player);
        return player == BLACK ? score : -score;
    }
    else
    {
        int best = INT_MIN;
        vector<int> moves = board.legalMoves(player);
        for (int move : moves)
        {
            int x = move / board.cols;
            int y = move % board.cols;
            boardRecord.push(board);
            Board& nextBoard = boardRecord.peek();
            nextBoard.reverse(x, y, player);
            int value = -negativeMax(boardRecord, -player, depth - 1);
            boardRecord.pop();
            if (value > best)
            {
                best = value;
            }
        }
        return best;
    }
}

int evaluate(Board& board, int player)
{
    return evaluateByPositions(board, player);
}

int evaluateByPositions(Board& board, int player)
{
    static vector<int> valueBoard = {
        90,  -60, 10, 10, 10, 10, -60, 90,  /* line 1 */
        -60, -80, 5,  5,  5,  5,  -80, -60, /* line 2 */
        10,  5,   1,  1,  1,  1,  5,   10,  /* line 3 */
        10,  5,   1,  1,  1,  1,  5,   10,  /* line 4 */
        10,  5,   1,  1,  1,  1,  5,   10,  /* line 5 */
        10,  5,   1,  1,  1,  1,  5,   10,  /* line 6 */
        -60, -80, 5,  5,  5,  5,  -80, -60, /* line 7 */
        90,  -60, 10, 10, 10, 10, -60, 90 /* line 8 */};
    int sum = 0;
    for (int i = 0, n = board.data.size(); i < n; i++)
    {
        if (board.data.at(i) == player)
        {
            sum = sum + valueBoard.at(i);
        }
    }
    return sum;
}

int chooseMove(Board& board, int player)
{
    vector<int> moves = board.legalMoves(player);
    BoardRecord record;
    record.push(board);
    int best = INT_MIN;
    int bestMove = -1;
    for (int move : moves)
    {
        record.push(board);
        Board& nextBoard = record.peek();
        int x = move / board.cols;
        int y = move % board.cols;

        nextBoard.reverse(x, y, player);
        int value = -negativeMax(record, -player, 5);
        record.pop();
        cout << "value = " << value << endl;
        if (value > best)
        {
            best = value;
            bestMove = move;
        }
    }
    return bestMove;
}
