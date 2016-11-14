#include "ai.h"
#include "game.h"
#include "situation.h"

AI::AI(const Game *gameInit)
{
    qRegisterMetaType<Move>("Move");
    game = gameInit;
    opponent = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200);
}

AI::~AI()
{
    for (int i = 0; i < threads.size(); i++)
    {
        QThread *thread = threads.at(i);
        delete thread;
    }
}

void AI::setTeam(bool isOpponent)
{
    opponent = isOpponent;
}

void AI::run()
{
    for (int i = 0; i < threads.size(); i++)
    {
        QThread *thread = threads.at(i);
        delete thread;
    }
    threads.clear();
    max_val = -10000;
    QList<Move> allMoves = game->getAllMoves(opponent);
    int size = allMoves.size();
    for (int i = 0; i < size; i++)
    {
        Game *gameSimu = new Game(*game);
        Move move = allMoves.at(i);
        gameSimu->makeMove(move, true);
        AIThread *thread = new AIThread(gameSimu, &move, i);
        connect(thread, SIGNAL(resultFound(int,int)), this, SLOT(handleResult(int,int)));
        threads.append(thread);
    }
    int max = (size < 4) ? size : 4;
    for (int i = 0; i < max; i++)
    {
        threads.at(i)->setRunning(true);
        threads.at(i)->start();
    }
}

void AI::handleResult(int result, int id)
{
    AIThread *thread = threads.at(id);
    if (result > max_val)
    {
        max_val = result;
        bestMoves.clear();
        bestMoves.append(thread->getMove());
    }
    else if (result == max_val)
    {
        bestMoves.append(thread->getMove());
    }
    thread->setDone(true);
    int done_threads = 0;
    int size = threads.size();
    bool started_new = false;
    for (int i = 0; i < size; i++)
    {
        AIThread *t = threads.at(i);
        if (!t->isDone())
        {
            if (!started_new && !t->isRunning())
            {
                t->setRunning(true);
                t->start();
                started_new = true;
            }
        }
        else
        {
            done_threads++;
        }
    }
    if (done_threads == size)
    {
        Move bestMove = bestMoves.at(Game::randInt(0, bestMoves.size()-1));
        emit sendProgress(100);
        emit foundMove(bestMove);
    }
    else
    {
        emit sendProgress(100*done_threads/size);
    }
}

void AI::update()
{

}
