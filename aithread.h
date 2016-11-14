#ifndef AITHREAD_H
#define AITHREAD_H

#include <QObject>
#include <QThread>

class Game;
struct Move;

class AIThread : public QThread
{

    Q_OBJECT

public:
    AIThread(Game* gameInit, Move *moveInit, int IDInit);
    ~AIThread();
    Move getMove(void) const;
    bool isDone(void) const;
    void setDone(bool isDone);
    bool isRunning(void) const;
    void setRunning(bool isRunning);

private:
    void run();
    Move* move;
    Game *game;
    bool done;
    int id;
    int alphaBeta(Game *gameSimu, int depth, int alpha, int beta, bool maximising);
    bool running;

signals:
    void resultFound(int result, int id);
};

#endif // AITHREAD_H
