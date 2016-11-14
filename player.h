#ifndef PLAYER_H
#define PLAYER_H

#include <QImage>
#include <QList>

class Player
{
public:
    Player();
    Player(const Player& ply);
    QPoint GetPos(void) const;
    void setPos(QPoint position);
    bool isReserve() const;
    void setReserve(bool res);
    void setImage(QString src);
    QImage& getImage(void);
    bool isKing(void) const;
    void setKing(bool uKing);
    bool isOpponent(void) const;
    void setOpponent(bool uOpponent);
    void setMovePattern(QList<QPoint> pattern);
    QList<QPoint> getMovePattern(void) const;
    void setType(int newType);
    int getType(void) const;
    bool isSameTeam(Player *player);
    int getPower() const;
    void setPower(int pow);

private:
    QPoint pos;
    bool reserve;
    QImage image;
    bool king;
    bool opponent;
    QList<QPoint> movePattern;
    int type;
    int power;
};

#endif // PLAYER_H
