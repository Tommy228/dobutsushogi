#include "aithread.h"
#include "game.h"
#include "move.h"

AIThread::AIThread(Game *gameInit, Move *moveInit, int IDInit) : game(gameInit), id(IDInit)
{
    move = new Move;
    move->id = moveInit->id;
    move->newPos = moveInit->newPos;
    done = false;
    running = false;
}

AIThread::~AIThread()
{
    delete move;
}

bool AIThread::isRunning(void) const
{
    return running;
}

Move AIThread::getMove(void) const
{
    Move newMove;
    newMove.id = move->id;
    newMove.newPos = move->newPos;
    return newMove;
}

bool AIThread::isDone(void) const
{
    return done;
}

void AIThread::setDone(bool isDone)
{
    done = isDone;
}

void AIThread::setRunning(bool isRunning)
{
    running = isRunning;
}

void AIThread::run()
{
    int result = alphaBeta(game, 6, -10000, 10000, false);
    emit resultFound(result, id);
}


int AIThread::alphaBeta(Game *gameSimu, int depth, int alpha, int beta, bool maximising)
{
    if (depth == 0 || !gameSimu->isGameValid())
    {
        return gameSimu->evaluate();
    }
    int v = maximising ? -10000 : 10000;
    QList<Move> allMoves = gameSimu->getAllMoves(gameSimu->isOpponentTurn());
    for (int i = 0; i < allMoves.size(); i++)
    {
        Move move = allMoves.at(i);
        Game newGameSimu(*gameSimu);
        newGameSimu.makeMove(move, true);
        if (maximising)
        {
            v = qMax(v, alphaBeta(&newGameSimu, depth-1, alpha, beta, false));
            alpha = qMax(alpha, v);
        }
        else
        {
            v = qMin(v, alphaBeta(&newGameSimu, depth-1, alpha, beta, true));
            beta = qMin(beta, v);
        }
        if (beta <= alpha)
        {
            break;
        }
    }
    return v;
}

