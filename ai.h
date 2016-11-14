#ifndef AI_H
#define AI_H

#include <QObject>
#include <QVector>
#include <QTimer>

#include "move.h"
#include "aithread.h"

class Game;
struct Situation;

class AI : public QObject
{

    Q_OBJECT

public:
    AI(const Game *gameInit);
    ~AI();
    void setTeam(bool isOpponent);
    void run();

private:
    const Game *game;
    bool opponent;
    QVector<AIThread*> threads;
    QTimer* timer;
    QList<Move> bestMoves;
    int max_val;

signals:
    void foundMove(Move move);
    void sendProgress(int progress);

private slots:
    void handleResult(int result, int id);
    void update();

};

#endif // AI_H
