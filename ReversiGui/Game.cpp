#include "Game.hpp"
#include <QBrush>
#include <QDebug>
Game::Game()
    : QGraphicsView()
    , scene{new GameScene}
{
    setScene(scene);
}

void GameScene::putBlack(int i, int j)
{
    Chesspiece* chesspiece = chesspieces.at(i * board.cols + j);
    if (chesspiece == nullptr)
    {
        putChesspiece(BLACK, i, j);
    }
    else if (chesspiece->kind == ChesspieceKind::White)
    {
        chesspiece->setBlack();
    }
}

void GameScene::putWhite(int i, int j)
{
    Chesspiece* chesspiece = chesspieces.at(i * board.cols + j);
    if (chesspiece == nullptr)
    {
        putChesspiece(WHITE, i, j);
    }
    else if (chesspiece->kind == ChesspieceKind::Black)
    {
        chesspiece->setWhite();
    }
}

void GameScene::putChesspiece(int kind, int i, int j)
{
    board.at(i, j) = kind;
    Chesspiece* chesspiece = new Chesspiece;
    chesspiece->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
    if (kind == BLACK)
    {
        chesspiece->setBlack();
    }
    else
    {
        chesspiece->setWhite();
    }
    chesspieces.at(i * board.cols + j) = chesspiece;
    addItem(chesspiece);
}

void GameScene::removeChessPiece(int i, int j)
{
    Chesspiece* chesspiece = chesspieces.at(i * board.cols + j);
    if (chesspiece != nullptr)
    {
        removeItem(chesspiece);
        delete chesspiece;
        chesspieces.at(i * board.cols + j) = nullptr;
    }
}

void GameScene::refresh()
{
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.cols; j++)
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
                removeChessPiece(i, j);
            }
        }
    }
}

void GameScene::computerMove()
{
}

GameScene::GameScene()
    : QGraphicsScene()
    , chesspieces(board.rows * board.cols)
    , status{new GameStatus}
{
    for (int i = 0; i < board.rows; i++)
    {
        for (int j = 0; j < board.cols; j++)
        {
            QGraphicsRectItem* rect = new QGraphicsRectItem;
            rect->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
            addItem(rect);
            rect->setBrush(QBrush(Qt::GlobalColor::gray));
        }
    }
    for (int i = 0, n = chesspieces.size(); i < n; i++)
    {
        chesspieces.at(i) = nullptr;
    }

    initializeView();
}

void GameScene::initializeView()
{
    putBlack(4, 3);
    putWhite(3, 3);
    putBlack(3, 4);
    putWhite(4, 4);
    addItem(status);
    status->setPos(0, pixelSize * 8);
    status->showScore(board.whiteCount(), board.blackCount(),
                      QString("White moves"));
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    static int player = WHITE;
    if (player == BLACK)
    {
        if (!board.canMove(player))
        {
            player = -player;
        }
        else
        {
            status->showScore(board.whiteCount(), board.blackCount(),
                              player == WHITE ? QString("White moves")
                                              : QString("Black moves"));
            int move = chooseMove(board, player);
            qDebug() << "move = " << move << endl;
            if (move != -1)
            {
                board.reverse(move / board.cols, move % board.cols, player);
                refresh();
            }
            player = -player;

            status->showScore(board.whiteCount(), board.blackCount(),
                              player == WHITE ? QString("White moves")
                                              : QString("Black moves"));
            return;
        }
    }
    QPointF position = event->scenePos();
    int x = static_cast<int>(position.x());
    int y = static_cast<int>(position.y());
    int i = x / pixelSize;
    int j = y / pixelSize;
    if (board.isVaildPosition(i, j))
    {
        if (!board.canMove(player))
        {
            player = -player;
        }
        else if (board.isLegalMove(i, j, player))
        {
            board.reverse(i, j, player);
            refresh();
            player = -player;
        }
        status->showScore(board.whiteCount(), board.blackCount(),
                          player == WHITE ? QString("White moves")
                                          : QString("Black moves"));
    }
}

GameStatus::GameStatus()
    : QGraphicsTextItem()
{
}

void GameStatus::showScore(int white, int black, QString text)
{
    QString message = QString("white: ") + QString::number(white) +
                      QString(" black: ") + QString::number(black) +
                      QString(" ") + text;
    setPlainText(message);
}

Chesspiece::Chesspiece()
    : QGraphicsEllipseItem()
    , kind{ChesspieceKind::Black}
{
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
