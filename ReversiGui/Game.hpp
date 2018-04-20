#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QMouseEvent>
class Game : public QGraphicsView
{
public:
    static const int BLACK = 1;
    static const int EMPTY = 0;
    static const int WHITE = -1;
    static const int pixelSize = 60;
    Board board;
    QGraphicsScene* scene;
    vector<QGraphicsEllipseItem*> chesspieces;
    Game();
    void putBlack(int i, int j);
    void putWhite(int i, int j);
    void putChessPiece(int kind, int i, int j);
    void removeChessPiece(int i, int j);
    void mousePressEvent(QMouseEvent* event);
    void refresh();
};

#endif // GAME_HPP
