#ifndef RESERVEBUTTON_H
#define RESERVEBUTTON_H

#include "player.h"

#include <QPushButton>

class ReserveButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ReserveButton(QWidget *parent = 0);
    Player* getPlayer(void);
    void setPlayer(Player* ply);

private:
    Player *player;

private slots:
    void onClick();

signals:
    void buttonClicked(Player*);

};
#endif // RESERVEBUTTON_H
