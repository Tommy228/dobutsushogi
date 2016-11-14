#ifndef MOVE_H
#define MOVE_H

#include <QPoint>

struct Move
{
    int id;
    QPoint newPos;
};

Q_DECLARE_METATYPE(Move)

#endif // MOVE_H
