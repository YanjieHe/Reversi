#ifndef GAME_HPP
#define GAME_HPP
#include "Board.hpp"
#include "Grid.hpp"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QLabel>
#include <QList>
#include <QTimer>

extern const int BLACK;
extern const int EMPTY;
extern const int WHITE;
static const int pixelSize = 80;

class GameStatus;
class Chesspiece;
class Block;
enum class ChesspieceKind
{
    Black,
    White,
    Transparent
};
enum class BlockKind
{
    Green,
    LightGreen
};
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    Board board;
    Grid<Chesspiece*> chesspieces;
    Grid<Block*> blocks;
    GameStatus* status;
    int player = WHITE;
    bool boardLock = false;
    GameScene();
    void initializeBoard();
    void initializeView();
    void putBlack(int x, int y);
    void putWhite(int x, int y);
    void putChesspiece(int kind, int i, int j);
    void removeChessPiece(int i, int j);
    void refresh();
    void computerMove();
    bool humanMove(int i, int j);
    void showStatus();
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void switchPlayer();
    void showChoices();
    void closeChoices();
    void wait(int millisecond);
    void putComputerChess(int computerChoice);
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
    void setTransparent();
};
class Block : public QGraphicsRectItem
{
public:
    BlockKind kind;
    Block();
    void setGreen();
    void setLightGreen();
};
class Game : public QGraphicsView
{
public:
    GameScene* scene;
    Game();
};

#endif // GAME_HPP
