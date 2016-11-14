#ifndef BOARD_H
#define BOARD_H

#include "game.h"
#include "playerstat.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    void setGame(Game *newGame);

private:
    int boardX;
    int boardY;
    Game *game;
    void paintPlayer(Player &player, QPaintEvent *e);
    Player *selectedPlayer;
    int selectedPlayerID;
    void updateSelectablePoints(void);
    QList<QPoint> movePoints;
    void paintPosition(QPoint pos, QColor color, QPaintEvent *e);
    QList<QPoint> selectablePoints;
    QPoint getPosFromMouse(int mouseX, int mouseY);
    void updatePaintValues(void);
    int relativeW;
    int relativeH;
    int relativeX;
    int relativeY;
    int caseX;
    int caseY;
    int margin;
    PlayerStat *HumanStats;
    PlayerStat *AIStats;
    bool opponentTurn;

private slots:
    void onPlayerMove(bool isOpponent);
    void onReserveClick(Player *ply);

public slots:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *event);
};

#endif // BOARD_H
