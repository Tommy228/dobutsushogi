#include "board.h"

Board::Board(QWidget *parent) : QWidget(parent)
{
    boardX = 0;
    boardY = 0;
    game = 0;
    selectedPlayer = 0;
    selectedPlayerID = 0;
    setMouseTracking(true);
    HumanStats = new PlayerStat(this);
    HumanStats->setTitle("HUMAN");
    HumanStats->setImage(":/images/human.png");
    HumanStats->setOpponent(false);
    HumanStats->setActiveUser(true);
    connect(HumanStats, SIGNAL(playerSelected(Player*)), this, SLOT(onReserveClick(Player*)));
    AIStats = new PlayerStat(this);
    AIStats->setTitle(" COMPUTER");
    AIStats->setImage(":/images/computer.png");
    AIStats->setOpponent(true);
    AIStats->setActiveUser(false);
    opponentTurn = false;
}

void Board::updatePaintValues(void)
{
    relativeX = 0;
    relativeY = 130;
    relativeW = width() - relativeX;
    relativeH = height() - relativeY;
    margin = 5;
    caseX = relativeW/boardX - 10;
    caseY = relativeH/boardY - 10;
}

void Board::paintEvent(QPaintEvent *e)
{
    if (game == 0) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor background(0xcc,0xcc,0xcc);
    painter.fillRect(QRect(relativeX, relativeY, relativeW, relativeH), background);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    QRectF rectangle(relativeX, relativeY, relativeW, relativeH);
    painter.drawRect(rectangle);

    QColor line(Qt::black);
    for (int i = 1; i <= boardY; i++)
    {
        int y = i * relativeH/(boardY);
        painter.fillRect(QRect(relativeX, relativeY + y, relativeW, 3), line);
    }
    for (int i = 1; i <= boardX; i++)
    {
        int x = i * relativeW/(boardX);
        painter.fillRect(QRect(relativeX + x, relativeY, 3, relativeH), line);
    }

    QList<Player*>* players = game->getAllPlayers();
    QListIterator<Player*> i(*players);
    while(i.hasNext())
    {
        Player *player = i.next();
        paintPlayer(*player, e);
    }
}

void Board::paintPlayer(Player &player, QPaintEvent *e)
{
    QPoint pos = player.GetPos();
    if (!player.isReserve())
    {
        int x = (pos.x()-1) * relativeW/(boardX);
        int y = (pos.y()-1) * relativeH/(boardY);
        QImage image = player.getImage().scaled(caseX, caseY);
        if (player.isOpponent())
        {
            image = image.mirrored(false, true);
        }
        QPainter painter(this);
        painter.drawImage(relativeX + x + 7, relativeY + y + 7, image);
    }
    if (selectedPlayer == &player)
    {
        if (!player.isReserve()) paintPosition(pos, Qt::red, e);
        for (int i = 0; i < movePoints.size(); i++)
        {
            QPoint point = movePoints.at(i);
            paintPosition(point, Qt::yellow, e);
        }
    }
}

void Board::paintPosition(QPoint pos, QColor color, QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(Qt::NoBrush);
    int x = (pos.x()-1) * relativeW/(boardX);
    int y = (pos.y()-1) * relativeH/(boardY);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(2);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    QRectF rectangle(relativeX + x + 7, relativeY + y + 7, caseX, caseY);
    painter.drawRoundedRect(rectangle, 15.0, 10.0);
}

void Board::mousePressEvent(QMouseEvent *e)
{
    if (game == 0) return;
    if (!game->isGameValid()) return;
    if (game->isOpponentTurn()) return;
    QPoint pos = getPosFromMouse(e->x(), e->y());
    int id;
    if (game->isPlayerOnPoint(pos, id))
    {

        QList<Player*>* players = game->getAllPlayers();
        Player *player = players->at(id);
        if (!player->isOpponent())
        {
            selectedPlayer = player;
            selectedPlayerID = id;
            updateSelectablePoints();
            update();
        }
    }
    if (selectedPlayer != 0)
    {
        Move move;
        move.id = selectedPlayerID;
        move.newPos = pos;
        game->makeMove(move, false);
    }
}

QPoint Board::getPosFromMouse(int mouseX, int mouseY)
{
    int xInterval = relativeW/boardX;
    int posX = (mouseX - relativeX)/xInterval + 1;
    int yInterval = relativeH/boardY;
    int posY = (mouseY - relativeY)/yInterval + 1;
    return QPoint(posX, posY);
}

void Board::updateSelectablePoints()
{
    selectablePoints.clear();
    if (game->isOpponentTurn()) return;
    if (selectedPlayer != 0)
    {
        movePoints = game->getMovePoints(selectedPlayer);
    }
    for (int i = 0; i < movePoints.size(); i++)
    {
        QPoint point = movePoints.at(i);
        selectablePoints << point;
    }
    QList<Player*>* players = game->getAllPlayers();
    for (int i = 0; i < players->size(); i++)
    {
        Player *player = players->at(i);
        if (!player->isOpponent())
        {
            selectablePoints << player->GetPos();
        }
    }
}

void Board::mouseMoveEvent(QMouseEvent *e)
{
    if (game == 0) return;
    if (!game->isGameValid())
    {
        if (cursor().shape() != Qt::ArrowCursor)
        {
            setCursor(Qt::ArrowCursor);
        }
        return;
    }
    QPoint pos = getPosFromMouse(e->x(), e->y());
    for (int i = 0; i < selectablePoints.size(); i++)
    {
        QPoint compPos = selectablePoints.at(i);
        if (compPos.x() == pos.x() && compPos.y() == pos.y())
        {
            setCursor(Qt::PointingHandCursor);
            return;
        }
    }
    setCursor(Qt::ArrowCursor);
}

void Board::onPlayerMove(bool isOpponent)
{
    selectedPlayer = 0;
    selectedPlayerID = 0;
    updateSelectablePoints();
    update();
    HumanStats->updateReserve(*game->getAllPlayers());
    HumanStats->setActiveUser(isOpponent);
    AIStats->updateReserve(*game->getAllPlayers());
    AIStats->setActiveUser(!isOpponent);
    opponentTurn = !isOpponent;
}

void Board::setGame(Game *newGame)
{
    game = newGame;
    QPoint size = game->getBoardSize();
    boardX = size.x();
    boardY = size.y();
    updateSelectablePoints();
    connect(game, SIGNAL(playerMoved(bool)), this, SLOT(onPlayerMove(bool)));
    update();
}

void Board::resizeEvent(QResizeEvent *event)
{
    updatePaintValues();
    HumanStats->setGeometry(0, 0, width()/2 - margin, relativeY - margin*2);
    AIStats->setGeometry(width()/2 + margin, 0, width()/2 - margin, relativeY - margin*2);
}

void Board::onReserveClick(Player *ply)
{
    if (!game->isGameValid()) return;
    if (opponentTurn) return;
    selectedPlayer = ply;
    QList<Player*>* players = game->getAllPlayers();
    for (int i = 0; i < players->size(); i++)
    {
        if (selectedPlayer == players->at(i))
        {
            selectedPlayerID = i;
            break;
        }
    }
    updateSelectablePoints();
    update();
}
