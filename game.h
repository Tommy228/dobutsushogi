#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "move.h"

#include <QObject>
#include <QTime>

class AI;
struct Situation;

class Game : public QObject
{

    Q_OBJECT

public:
    explicit Game();
    explicit Game(const Game& prevGame);
    ~Game();
    void setBoardSize(QPoint size);
    QPoint getBoardSize(void) const;
    void newGame();
    QList<Player*>* getAllPlayers(void) const;
    QList<QPoint> getMovePoints(Player* player) const;
    bool isPlayerOnPoint(QPoint pos, int &id) const;
    bool isValidPosition(QPoint pos) const;
    void makeMove(Move &move, bool simulation);
    void addPlayer(Player *player);
    bool isOpponentTurn(void) const;
    QList<Move> getAllMoves(bool opponent) const;
    bool isPlayerSafe(Player *ply) const;
    Situation getUserSituation(bool opponent) const;
    bool isGameValid(void) const;
    bool isOpponentWinner(void) const;
    void clearGame(void);
    QVector<QVector<int>> getSafePositions(bool opponent) const;
    static int randInt(int low, int high);
    int evaluate(void) const;

private:
    void checkForWin(bool simulation);
    void deletePlayers(void);
    QPoint boardSize;
    QList<Player*>* players;
    bool gameValid;
    bool opponentTurn;
    bool opponentWinner;
    AI* ai;
    bool autoplayEnabled;

public slots:
    void AIDecision(void);
    void randomAIMove(void);
    void enableAutoplay(bool enable);

signals:
    void playerMoved(bool isOpponent);
    void userWins(bool isOpponent);
    void sendAIProgress(int progress);
    void startedDecision();
    void endedDecision();

private slots:
    void doAIDecision(Move move);
    void AIProgress(int progress);

};

#endif // GAME_H
