#include "game.h"
#include "ai.h"
#include "definitions.h"
#include "situation.h"

#include <QtAlgorithms>

Game::Game()
{
    players = new QList<Player*>;
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
    opponentTurn = false;
    ai = new AI(this);
    connect(ai, SIGNAL(foundMove(Move)), this, SLOT(doAIDecision(Move)));
    connect(ai, SIGNAL(sendProgress(int)), this, SLOT(AIProgress(int)));
}

Game::Game(const Game &prevGame)
{
    players = new QList<Player*>;
    qsrand(static_cast<uint>(QTime::currentTime().msec()));
    opponentTurn = prevGame.opponentTurn;

    QList<Player*> *prevPlayers = prevGame.getAllPlayers();
    for (int i=0; i < prevPlayers->size(); i++)
    {
        Player *prevPlayer = prevPlayers->at(i);
        Player *playerCopy = new Player(*prevPlayer);
        addPlayer(playerCopy);
    }
    setBoardSize(prevGame.getBoardSize());
    newGame();
}

Game::~Game()
{
    deletePlayers();
    delete players;
}

void Game::deletePlayers()
{
    for (int i = 0; i < players->size(); i++)
    {
        Player *player = players->at(i);
        delete player;
    }
}

void Game::clearGame(void)
{
    deletePlayers();
    players->clear();
    gameValid = false;
}

void Game::setBoardSize(QPoint size)
{
    boardSize = size;
}

QPoint Game::getBoardSize(void) const
{
    return boardSize;
}

void Game::newGame(void)
{
    gameValid = true;
}

void Game::enableAutoplay(bool enable)
{
    autoplayEnabled = enable;
}

void Game::addPlayer(Player *player)
{
    players->append(player);
}

bool Game::isGameValid(void) const
{
    return gameValid;
}

QList<Player*>* Game::getAllPlayers(void) const
{
    return players;
}

bool Game::isPlayerOnPoint(QPoint pos, int &id) const
{
    for (int i = 0; i < players->size(); i++)
    {
        Player *ply = players->at(i);
        if (!ply->isReserve() && ply->GetPos().x() == pos.x() && ply->GetPos().y() == pos.y())
        {
            id = i;
            return true;
        }
    }
    return false;
}

bool Game::isValidPosition(QPoint pos) const
{
    int x = pos.x();
    int y = pos.y();
    return x >= 1 && y >= 1 && x <= boardSize.x() && y <= boardSize.y();
}

QList<QPoint> Game::getMovePoints(Player *player) const
{
    QList<QPoint> movePoints;
    if (player->isReserve())
    {
        for (int x = 1; x <= boardSize.x(); x++)
        {
            for (int y = 1; y <= boardSize.y(); y++)
            {
                QPoint movePoint(x, y);
                int id;
                bool playerOnPoint = isPlayerOnPoint(movePoint, id);
                if (!playerOnPoint)
                {
                    movePoints << movePoint;
                }
            }
        }
    }
    else
    {
        QList<QPoint> pattern = player->getMovePattern();
        int posX = player->GetPos().x();
        int posY = player->GetPos().y();
        for (int i = 0; i < pattern.size(); i++)
        {
            QPoint move;
            if (player->isOpponent())
            {
                QPoint prevMove = pattern.at(i);
                move = QPoint(-prevMove.x(), -prevMove.y());
            }
            else
            {
                move = pattern.at(i);
            }
            int relativeX = posX + move.x();
            int relativeY = posY + move.y();
            QPoint relativePos(relativeX, relativeY);
            if (!isValidPosition(relativePos)) continue;
            bool playerOnPoint;
            int id;
            playerOnPoint = isPlayerOnPoint(relativePos, id);
            if (playerOnPoint)
            {
                Player *ply = players->at(id);
                if (player->isSameTeam(ply)) continue;
            }
            movePoints << relativePos;
        }
        return movePoints;
    }
    return movePoints;
}

Situation Game::getUserSituation(bool opponent) const
{
    Situation sit;
    sit.reservePower = 0;
    sit.globalPower = 0;
    sit.kingwin = false;
    sit.noking = true;
    sit.zones = 0;
    for (int i = 0; i < players->size(); i++)
    {
        Player *player = players->at(i);
        if (player->isOpponent() == opponent)
        {
            if (player->isKing())
            {
                sit.noking = false;
                QPoint kingPos = player->GetPos();
                int winpos = opponent ? getBoardSize().y() : 1;
                if (kingPos.y() == winpos && isPlayerSafe(player))
                {
                    sit.kingwin = true;
                }
            }
            else
            {
                int power = player->getPower();
                if (player->isReserve())
                {
                    sit.reservePower += power;
                }
                else
                {
                    sit.globalPower += power;
                }
            }
        }
    }
    QVector<QVector<int>> safePoints = getSafePositions(opponent);
    for (int i = 0; i < safePoints.size(); i++)
    {
        QVector<int> line = safePoints.at(i);
        for (int j = 0; j < line.size(); j++)
        {
            sit.zones += line.at(j);
        }
    }
    return sit;
}

void Game::checkForWin(bool simulation)
{
    Situation humanSit = getUserSituation(false);
    Situation AISit = getUserSituation(true);
    if (AISit.noking || humanSit.kingwin)
    {
        gameValid = false;
        opponentWinner = false;
        if (!simulation)
        {
            emit userWins(false);
        }
    }
    else if (humanSit.noking || AISit.kingwin)
    {
        gameValid = false;
        opponentWinner = true;
        if (!simulation)
        {
            emit userWins(true);
        }
    }
}

bool Game::isOpponentWinner(void) const
{
    return opponentWinner;
}

void Game::makeMove(Move& move, bool simulation)
{
    Player *movePly = players->at(move.id);
    QList<QPoint> movePoints = getMovePoints(movePly);
    for (int i = 0; i < movePoints.size(); i++)
    {
        QPoint movePoint = movePoints.at(i);
        if (move.newPos.x() == movePoint.x() && move.newPos.y() == movePoint.y())
        {
            if (movePly->isOpponent() && opponentTurn)
            {
                opponentTurn = false;
            }
            else if (!movePly->isOpponent() && !opponentTurn)
            {
                opponentTurn = true;
            }
            else
            {
                return;
            }
            bool playerOnPoint;
            int id;
            playerOnPoint = isPlayerOnPoint(movePoint, id);
            if (playerOnPoint)
            {
                Player *ply = players->at(id);
                ply->setReserve(true);
                ply->setOpponent(movePly->isOpponent());
                if (ply->getType() == TYPE_SAMURAI)
                {
                    ply->setType(TYPE_KODOMA);
                    if (!simulation)
                    {
                        ply->setImage(":/images/kodoma.png");
                    }
                    QList<QPoint> kodomaPattern;
                    kodomaPattern << QPoint(0, -1); // en haut
                    ply->setMovePattern(kodomaPattern);
                    ply->setPower(AI_KODOMA_POWER);
                }
            }
            movePly->setPos(move.newPos);
            if (!movePly->isReserve() && movePly->getType() == TYPE_KODOMA)
            {
                int winpos = movePly->isOpponent() ? boardSize.y() : 1;
                if (move.newPos.y() == winpos)
                {
                    movePly->setType(TYPE_SAMURAI);
                    if (!simulation)
                    {
                        movePly->setImage(":/images/samurai.png");
                    }
                    QList<QPoint> samuraiPattern;
                    samuraiPattern << QPoint(0, -1); // en haut
                    samuraiPattern << QPoint(1, 0); // à droite
                    samuraiPattern << QPoint(1, -1); // en haut à droite
                    samuraiPattern << QPoint(0, 1); // en bas
                    samuraiPattern << QPoint(-1, -1); // en haut à gauche
                    samuraiPattern << QPoint(-1, 0); // à gauche
                    movePly->setMovePattern(samuraiPattern);
                    movePly->setPower(AI_SAMURAI_POWER);
                }
            }
            movePly->setReserve(false);
            checkForWin(simulation);
            if (!simulation)
            {
                emit playerMoved(movePly->isOpponent());
                if (autoplayEnabled && opponentTurn)
                {
                    AIDecision();
                }
            }
        }
    }
}

bool Game::isOpponentTurn(void) const
{
    return opponentTurn;
}

int Game::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

QList<Move> Game::getAllMoves(bool opponent) const
{
    QList<Move> moves;
    for (int i = 0; i < players->size(); i++)
    {
        Player *ply = players->at(i);
        if (ply->isOpponent() == opponent)
        {
            QList<QPoint> movePoints = getMovePoints(ply);
            for (int j = 0; j < movePoints.size(); j++)
            {
               Move newMove;
               newMove.id = i;
               newMove.newPos = movePoints.at(j);
               moves.append(newMove);
            }
        }
    }
    return moves;
}

QVector<QVector<int>> Game::getSafePositions(bool opponent) const
{
    QVector<QVector<int>> result;

    if (!opponent)
    {
        result = {
            { 10, 10, 10 },
            { 5, 7, 5 },
            { 3, 5, 3 },
            { 1, 1, 1 }
        };
    }
    else
    {
        result = {
            { 1, 1, 1 },
            { 3, 5, 3 },
            { 5, 7, 5 },
            { 10, 10, 10 }
        };
    }
    for (int i = 0; i < players->size(); i++)
    {
        Player *ply = players->at(i);
        if (!ply->isReserve() && ply->isOpponent() != opponent)
        {
            QPoint playerPos = ply->GetPos();
            result[playerPos.y()-1][playerPos.x()-1] = 0;
            QList<QPoint> movePoints = getMovePoints(ply);
            for (int j = 0; j < movePoints.size(); j++)
            {
                QPoint point = movePoints.at(j);
                result[point.y()-1][point.x()-1] = 0;
            }
        }
    }
    return result;
}

bool Game::isPlayerSafe(Player *ply) const
{
    for (int i = 0; i < players->size(); i++)
    {
        Player *opponent = players->at(i);
        if (opponent->isOpponent() != ply->isOpponent())
        {
            QList<QPoint> movePoints = getMovePoints(opponent);
            for (int j = 0; j < movePoints.size(); j++)
            {
                QPoint point = movePoints.at(j);
                if (point.x() == ply->GetPos().x() && point.y() == ply->GetPos().y())
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void Game::randomAIMove(void)
{
    if (!opponentTurn) return;
    QList<Move> allMoves = getAllMoves(true);
    int size = allMoves.size();
    Move move = allMoves.at(randInt(0, size-1));
    makeMove(move, false);
}

void Game::AIDecision()
{
    emit startedDecision();
    ai->setTeam(true);
    ai->run();
}

void Game::doAIDecision(Move move)
{
    makeMove(move, false);
    emit endedDecision();
}

void Game::AIProgress(int progress)
{
    emit sendAIProgress(progress);
}

int Game::evaluate(void) const
{
    Situation humanSit = getUserSituation(false);
    Situation AISit = getUserSituation(true);
    if (AISit.noking || humanSit.kingwin)
    {
        return -9999;
    }
    else if (humanSit.noking || AISit.kingwin)
    {
        return 9999;
    }
    int eval = 0;
    eval += AISit.globalPower * 10;
    eval += AISit.reservePower * 7;
    eval -= humanSit.globalPower * 10;
    eval -= humanSit.reservePower * 7;
    eval += AISit.zones * 0.4;
    eval -= humanSit.zones * 0.4;
    return eval;
}
