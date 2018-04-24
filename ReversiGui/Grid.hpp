#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
using std::vector;
template <typename T>
class Grid
{
public:
    int xBound;
    int yBound;
    vector<T> data;
    Grid(int xBound, int yBound);
    T& at(int x, int y);
};

template <typename T>
T& Grid<T>::at(int x, int y)
{
    return data.at(y * yBound + x);
}

template <typename T>
Grid<T>::Grid(int xBound, int yBound)
    : xBound{xBound}
    , yBound{yBound}
    , data(xBound * yBound)
{
}

#endif // GRID_HPP
