#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QLabel>
#include <QList>
#include <QTimer>
class GameStatus;
class Chesspiece;
enum class ChesspieceKind
{
    Black,
    White
};
extern const int BLACK;
extern const int EMPTY;
extern const int WHITE;
static const int pixelSize = 60;
class GameScene : public QGraphicsScene
{
public:
    Board board;
    vector<Chesspiece*> chesspieces;
    GameStatus* status;
    GameScene();
    void initializeView();
    void putBlack(int i, int j);
    void putWhite(int i, int j);
    void putChesspiece(int kind, int i, int j);
    void removeChessPiece(int i, int j);
    void refresh();
    void computerMove();
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

class GameStatus : public QGraphicsTextItem
{
public:
    GameStatus();
    void showScore(int white, int black, QString text);
};

class Chesspiece : public QGraphicsEllipseItem
{
public:
    ChesspieceKind kind;
    Chesspiece();
    void setWhite();
    void setBlack();
};
class Game : public QGraphicsView
{
public:
    GameScene* scene;
    Game();
};

#endif // GAME_HPP
