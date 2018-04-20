#include "Game.hpp"
#include <QBrush>

Game::Game()
    : scene{new QGraphicsScene}
    , chesspieces(board.rows * board.cols)
{
    setScene(scene);
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.cols; j++)
        {
            QGraphicsRectItem* rect = new QGraphicsRectItem;
            rect->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
            scene->addItem(rect);
            rect->setBrush(QBrush(Qt::GlobalColor::gray));
        }
    }
    for (int i = 0, n = chesspieces.size(); i < n; i++)
    {
        chesspieces.at(i) = nullptr;
    }
    putBlack(4, 3);
    putWhite(3, 3);
    putBlack(3, 4);
    putWhite(4, 4);
}

void Game::putBlack(int i, int j)
{
    if (board.at(i, j) == BLACK)
    {
        return;
    }
    else if (board.at(i, j) == WHITE)
    {
        removeChessPiece(i, j);
    }
    putChessPiece(BLACK, i, j);
}

void Game::putWhite(int i, int j)
{
    if (board.at(i, j) == WHITE)
    {
        return;
    }
    else if (board.at(i, j) == BLACK)
    {
        removeChessPiece(i, j);
    }
    putChessPiece(WHITE, i, j);
}

void Game::putChessPiece(int kind, int i, int j)
{
    board.at(i, j) = kind;
    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem;
    ellipse->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
    if (kind == BLACK)
    {
        ellipse->setBrush(QBrush(Qt::GlobalColor::black));
    }
    else
    {
        ellipse->setBrush(QBrush(Qt::GlobalColor::white));
    }
    chesspieces.at(i * board.cols + j) = ellipse;
    scene->addItem(ellipse);
}

void Game::removeChessPiece(int i, int j)
{
    QGraphicsEllipseItem* item = chesspieces.at(i * board.cols + j);
    if (item != nullptr)
    {
        scene->removeItem(item);
        delete item;
        chesspieces.at(i * board.cols + j) = nullptr;
    }
}

void Game::mousePressEvent(QMouseEvent* event)
{
    static int player = WHITE;
    QPoint position = event->pos();
    int i = position.x() / pixelSize;
    int j = position.y() / pixelSize;
    if (!board.canMove(player))
    {
        player = -player;
    }
    if (board.isLegalMove(i, j, player))
    {
        board.reverse(i, j, player);
        refresh();
        player = -player;
    }
}

void Game::refresh()
{
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.cols; j++)
        {
            if (board.at(i, j) == BLACK)
            {
                removeChessPiece(i, j);
                putChessPiece(BLACK, i, j);
            }
            else if (board.at(i, j) == WHITE)
            {
                removeChessPiece(i, j);
                putChessPiece(WHITE, i, j);
            }
            else
            {
                removeChessPiece(i, j);
            }
        }
    }
}
