#include "reservebutton.h"

ReserveButton::ReserveButton(QWidget *parent) : QPushButton(parent)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClick()));
    setText("");
}

void ReserveButton::setPlayer(Player *ply)
{
    player = ply;
    QPixmap pixmap = QPixmap::fromImage(player->getImage());
    QIcon buttonIcon(pixmap);
    setIcon(buttonIcon);
    setIconSize(QSize(45, 45));
}

Player* ReserveButton::getPlayer(void)
{
    return player;
}

void ReserveButton::onClick()
{
    emit buttonClicked(player);
}
