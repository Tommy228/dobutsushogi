#include "player.h"

Player::Player() : pos(QPoint(1, 1)), power(0)
{
}

Player::Player(const Player& ply)
{
    setPos(ply.GetPos());
    setReserve(ply.isReserve());
    setKing(ply.isKing());
    setOpponent(ply.isOpponent());
    setMovePattern(ply.getMovePattern());
    setType(ply.getType());
    setPower(ply.getPower());
}

QPoint Player::GetPos(void) const
{
    return pos;
}

void Player::setPos(QPoint position)
{
    pos = position;
}

void Player::setReserve(bool res)
{
    reserve = res;
}

bool Player::isReserve() const
{
    return reserve;
}

void Player::setImage(QString src)
{
    image = QImage(src);
}

QImage& Player::getImage(void)
{
    return image;
}

bool Player::isKing(void) const
{
    return king;
}

void Player::setKing(bool uKing)
{
    king = uKing;
}

bool Player::isOpponent(void) const
{
    return opponent;
}

void Player::setOpponent(bool uOpponent)
{
    opponent = uOpponent;
}

void Player::setMovePattern(QList<QPoint> pattern)
{
    movePattern = pattern;
}

QList<QPoint> Player::getMovePattern(void) const
{
    return movePattern;
}

int Player::getType(void) const
{
    return type;
}

void Player::setType(int newType)
{
    type = newType;
}

bool Player::isSameTeam(Player *player)
{
    if (player->isOpponent() == opponent) return true;
    return false;
}

void Player::setPower(int pow)
{
    power = pow;
}

int Player::getPower() const
{
    return power;
}
