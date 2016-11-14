#ifndef PLAYERSTAT_H
#define PLAYERSTAT_H

#include "player.h"
#include "reservebutton.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>

class PlayerStat : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerStat(QWidget *parent = 0);
    void setTitle(QString title);
    void setImage(QString img);
    void updateReserve(QList<Player*> players);
    void setOpponent(bool isOpponent);
    void setActiveUser(bool isActive);

private:
    QLabel *titleLabel;
    QImage image;
    QImage dot;
    int imageX;
    int imageY;
    QScrollArea *frame;
    QHBoxLayout *mLayout;
    QList<Player*> reserve;
    bool opponent;
    bool activeUser;

private slots:
    void onButtonClicked(Player *player);

signals:
    void playerSelected(Player*);

public slots:
     void paintEvent(QPaintEvent *e);
     void resizeEvent(QResizeEvent *event);
};

#endif // PLAYERSTAT_H
