#include "Board.hpp"
#include <climits>
#include <iostream>
using namespace std;
Board::Board()
    : xBound{8}
    , yBound{8}
    , data(xBound * yBound)
{
    for (int i = 0, n = data.size(); i < n; i++)
    {
        data.at(i) = EMPTY;
    }
}

int& Board::at(int x, int y)
{
    return data.at(y * yBound + x);
}

bool Board::isLegalMove(int x, int y, int player)
{
    if (at(x, y) == EMPTY)
    {
        // check eight directions
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (!(i == 0 && j == 0))
                {
                    if (isLegalDirection(x, y, i, j, player))
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

bool Board::isLegalDirection(int x, int y, int dx, int dy, int player)
{
    x += dx;
    y += dy;
    if (!isVaildPosition(x, y) || at(x, y) != -player)
    {
        return false;
    }
    x += dx;
    y += dy;
    while (isVaildPosition(x, y))
    {
        if (at(x, y) == player)
        {
            return true;
        }
        else if (at(x, y) == EMPTY)
        {
            return false;
        }
        x += dx;
        y += dy;
    }
    return false;
}

bool Board::isVaildPosition(int x, int y)
{
    return x >= 0 && x < xBound && y >= 0 && y < yBound;
}

void Board::reverse(int x, int y, int player)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0))
            {
                if (isLegalDirection(x, y, i, j, player))
                {
                    reverseDirection(x, y, i, j, player);
                }
            }
        }
    }
    at(x, y) = player;
}

void Board::reverseDirection(int x, int y, int dx, int dy, int player)
{
    x += dx;
    y += dy;
    while (isVaildPosition(x, y))
    {
        if (at(x, y) == -player)
        {
            at(x, y) = player;
            x += dx;
            y += dy;
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
    for (int i = 0; i < xBound; i++)
    {
        for (int j = 0; j < yBound; j++)
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
    for (int x = 0; x < xBound; x++)
    {
        for (int y = 0; y < yBound; y++)
        {
            if (at(x, y) == EMPTY && isLegalMove(x, y, player))
            {
                positions.push_back(y * yBound + x);
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

GameState Board::state(int player)
{
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
    if (blackCount == 0)
    {
        return GameState::WhiteWins;
    }
    else if (whiteCount == 0)
    {
        return GameState::BlackWins;
    }
    else if (blackCount + whiteCount == xBound * yBound)
    {
        return finalState(blackCount, whiteCount);
    }
    else
    {
        if (player == BLACK)
        {
            if (!canMove(BLACK))
            {
                if (canMove(WHITE))
                {
                    return GameState::BlackPasses;
                }
                else
                {
                    return finalState(blackCount, whiteCount);
                }
            }
            else
            {
                return GameState::Ongoing;
            }
        }
        else
        {
            if (!canMove(WHITE))
            {
                if (canMove(BLACK))
                {
                    return GameState::WhitePasses;
                }
                else
                {
                    return finalState(blackCount, whiteCount);
                }
            }
            else
            {
                return GameState::Ongoing;
            }
        }
    }
}

GameState Board::finalState(int blackCount, int whiteCount)
{
    if (blackCount > whiteCount)
    {
        return GameState::BlackWins;
    }
    else if (blackCount < whiteCount)
    {
        return GameState::WhiteWins;
    }
    else
    {
        return GameState::Draw;
    }
}

BoardRecord::BoardRecord()
    : record(8 * 8)
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
    GameState currentState = board.state(player);
    switch (currentState)
    {
    case GameState::BlackWins:
        return 9999;
    case GameState::WhiteWins:
        return -9999;
    case GameState::Draw:
        return 0;
    case GameState::BlackPasses:
    case GameState::WhitePasses:
        return -negativeMax(boardRecord, -player, depth - 1);
    default:
    {
        if (depth <= 0) // reach the required depth
        {
            return evaluate(board, player);
        }
        else
        {
            int best = INT_MIN;
            vector<int> moves = board.legalMoves(player);
            for (int move : moves)
            {
                int x = move % board.yBound;
                int y = move / board.yBound;
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
    int blackScore = 0;
    int whiteScore = 0;
    for (int i = 0, n = board.data.size(); i < n; i++)
    {
        if (board.data.at(i) == BLACK)
        {
            blackScore = blackScore + valueBoard.at(i);
        }
        else if (board.data.at(i) == WHITE)
        {
            whiteScore = whiteScore + valueBoard.at(i);
        }
    }
    if (player == BLACK)
    {
        return blackScore - whiteScore;
    }
    else
    {
        return whiteScore - blackScore;
    }
}

int chooseMove(Board& board, int player)
{
    vector<int> moves = board.legalMoves(player);
    BoardRecord record;
    record.push(board);

    int best = INT_MIN;
    int bestMove = -1;
    cout << "=========================" << endl;
    for (int move : moves)
    {
        record.push(board);
        Board& nextBoard = record.peek();
        int x = move % board.yBound;
        int y = move / board.yBound;

        nextBoard.reverse(x, y, player);
        int value = -negativeMax(record, -player, 4);
        record.pop();
        cout << (player == BLACK ? "black " : "white ");
        cout << "(" << (x + 1) << ", " << (y + 1) << ") ";
        cout << "value = " << value << endl;
        if (value > best)
        {
            best = value;
            bestMove = move;
        }
    }
    return bestMove;
}

Position::Position(int x, int y)
    : x{x}
    , y{y}
{
}
