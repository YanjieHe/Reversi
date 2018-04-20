#include "Board.hpp"
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
