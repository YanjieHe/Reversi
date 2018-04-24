#include "Game.hpp"
#include <QBrush>
#include <QCoreApplication>
#include <QPen>
#include <QTime>
#include <iostream>
using std::cout;
using std::endl;
void GameScene::putBlack(int x, int y)
{
    Chesspiece* chesspiece = chesspieces.at(x, y);
    if (chesspiece->kind != ChesspieceKind::Black)
    {
        chesspieces.at(x, y)->setBlack();
    }
}

void GameScene::putWhite(int x, int y)
{
    Chesspiece* chesspiece = chesspieces.at(x, y);
    if (chesspiece->kind != ChesspieceKind::White)
    {
        chesspieces.at(x, y)->setWhite();
    }
}

void GameScene::refresh()
{
    for (int i = 0; i < board.xBound; i++)
    {
        for (int j = 0; j < board.yBound; j++)
        {
            if (board.at(i, j) == BLACK)
            {
                putBlack(i, j);
            }
            else if (board.at(i, j) == WHITE)
            {
                putWhite(i, j);
            }
            else
            {
                chesspieces.at(i, j)->setTransparent();
            }
        }
    }
}

void GameScene::computerMove()
{
    if (!board.canMove(player))
    {
        switchPlayer();
    }
    else
    {
        int move = chooseMove(board, player);
        if (move != -1)
        {
            boardLock = true;
            putComputerChess(move);
            boardLock = false;
            int x = move % board.yBound;
            int y = move / board.yBound;
            board.reverse(x, y, player);
            cout << "computer(black) choose (" << (x + 1) << ", " << (y + 1)
                 << ")" << endl;
            refresh();
        }
        switchPlayer();
    }
}

void GameScene::putComputerChess(int computerChoice)
{
    for (int i = 0; i < 2; i++)
    {
        if (player == BLACK)
        {
            chesspieces.data.at(computerChoice)->setBlack();
        }
        else
        {
            chesspieces.data.at(computerChoice)->setWhite();
        }
        wait(300);
        chesspieces.data.at(computerChoice)->setTransparent();
        wait(300);
    }
}

bool GameScene::humanMove(int i, int j)
{
    if (board.isVaildPosition(i, j))
    {
        if (!board.canMove(player))
        {
            switchPlayer();
            return true;
        }
        else if (board.isLegalMove(i, j, player))
        {
            board.reverse(i, j, player);
            refresh();
            switchPlayer();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void GameScene::showStatus()
{
    int whiteCount = board.whiteCount();
    int blackCount = board.blackCount();
    if (player == BLACK)
    {
        status->showScore(whiteCount, blackCount, tr("Black to move"));
    }
    else
    {
        status->showScore(whiteCount, blackCount, tr("White to move"));
    }
}

GameScene::GameScene()
    : QGraphicsScene()
    , chesspieces(board.xBound, board.yBound)
    , blocks(board.xBound, board.yBound)
    , status{new GameStatus}
{
    initializeBoard();

    initializeView();
    showChoices();
}

void GameScene::initializeBoard()
{
    for (int i = 0; i < board.xBound; i++)
    {
        for (int j = 0; j < board.yBound; j++)
        {
            Block* block = new Block;
            block->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
            addItem(block);
            blocks.at(i, j) = block;
            Chesspiece* chesspiece = new Chesspiece;
            chesspiece->setRect(i * pixelSize + 10, j * pixelSize + 10,
                                pixelSize - 20, pixelSize - 20);
            addItem(chesspiece);
            chesspieces.at(i, j) = chesspiece;
        }
    }
}

void GameScene::initializeView()
{
    putBlack(4, 3);
    putWhite(3, 3);
    putBlack(3, 4);
    putWhite(4, 4);
    board.at(4, 3) = BLACK;
    board.at(3, 3) = WHITE;
    board.at(3, 4) = BLACK;
    board.at(4, 4) = WHITE;
    addItem(status);
    status->setPos(0, pixelSize * 8);
    showStatus();
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!boardLock)
    {
        if (player == WHITE)
        {
            QPointF position = event->scenePos();
            int x = static_cast<int>(position.x());
            int y = static_cast<int>(position.y());
            int i = x / pixelSize;
            int j = y / pixelSize;
            if (humanMove(i, j))
            {
                showStatus();
                closeChoices();
                GameState state = board.state(player);
                if (state == GameState::Ongoing)
                {
                    QCoreApplication::processEvents();
                }
                computerMove();
                showStatus();
                showChoices();
            }
        }
    }
}

void GameScene::switchPlayer()
{
    player = -player;
}

void GameScene::showChoices()
{
    vector<int> moves = board.legalMoves(player);
    for (int move : moves)
    {
        int x = move % board.yBound;
        int y = move / board.yBound;
        if (blocks.at(x, y)->kind == BlockKind::Green)
        {
            blocks.at(x, y)->setLightGreen();
        }
    }
}

void GameScene::closeChoices()
{
    for (int i = 0; i < blocks.xBound; i++)
    {
        for (int j = 0; j < blocks.yBound; j++)
        {
            if (blocks.at(i, j)->kind == BlockKind::LightGreen)
            {
                blocks.at(i, j)->setGreen();
            }
        }
    }
}

void GameScene::wait(int millisecond)
{
    QTime t;
    t.start();
    while (t.elapsed() < millisecond)
    {
        QCoreApplication::processEvents();
    }
}

GameStatus::GameStatus()
    : QGraphicsTextItem()
{
}

void GameStatus::showScore(int white, int black, QString text)
{
    QString message = QString("(white: ") + QString::number(white) +
                      QString(", black: ") + QString::number(black) +
                      QString(") ") + text;
    setPlainText(message);
}

Chesspiece::Chesspiece()
    : QGraphicsEllipseItem()
    , kind{ChesspieceKind::Transparent}
{
    setBrush(QBrush(Qt::GlobalColor::transparent));
    setPen(QPen(Qt::GlobalColor::transparent));
}

void Chesspiece::setWhite()
{
    kind = ChesspieceKind::White;
    setBrush(QBrush(Qt::GlobalColor::white));
}

void Chesspiece::setBlack()
{
    kind = ChesspieceKind::Black;
    setBrush(QBrush(Qt::GlobalColor::black));
}

void Chesspiece::setTransparent()
{
    kind = ChesspieceKind::Transparent;
    setBrush(QBrush(Qt::GlobalColor::transparent));
}

Game::Game()
    : QGraphicsView()
    , scene{new GameScene}
{
    setScene(scene);
}

Block::Block()
    : QGraphicsRectItem()
    , kind{BlockKind::Green}
{
    setGreen();
    setPen(QPen(Qt::GlobalColor::white));
}

void Block::setGreen()
{
    setBrush(QBrush(QColor::fromRgb(0, 102, 51)));
    kind = BlockKind::Green;
}

void Block::setLightGreen()
{
    setBrush(QBrush(QColor::fromRgb(0, 136, 68)));
    kind = BlockKind::LightGreen;
}
