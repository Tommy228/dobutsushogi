#include "playerstat.h"

PlayerStat::PlayerStat(QWidget *parent) : QWidget(parent)
{
    opponent = false;
    activeUser = false;

    titleLabel = new QLabel(this);
    titleLabel->setScaledContents(true);
    QFont sansFont("Helvetica [Cronyx]", 10);
    titleLabel->setFont(sansFont);
    imageX = 0;
    imageY = 10;

    QWidget *widgetScroll = new QWidget;

    frame = new QScrollArea(this);
    frame->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    mLayout = new QHBoxLayout(frame);
    mLayout->setMargin(2);
    widgetScroll->setLayout(mLayout);

    frame->setWidget(widgetScroll);
    frame->setWidgetResizable(true);

    dot = QImage(":/images/greendot.png").scaled(12, 12);
}

void PlayerStat::setTitle(QString title)
{
    titleLabel->setText(title);
}

void PlayerStat::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    QRectF rectangle(0, 0, width(), height());
    painter.drawRect(rectangle);
    painter.drawImage(imageX, imageY, image);

    if (activeUser)
    {
        painter.drawImage(width()-14, 2, dot);
    }
}

void PlayerStat::resizeEvent(QResizeEvent *event)
{
    int labelWidth = titleLabel->fontMetrics().boundingRect(titleLabel->text()).width();
    int labelHeight = titleLabel->fontMetrics().boundingRect(titleLabel->text()).height();
    int totalSize = labelWidth + image.width();
    imageX = width()/2 - totalSize/2 - 10;
    titleLabel->move(imageX + image.width(), imageY + image.height()/2 - labelHeight/2);
    frame->setGeometry(5, imageY + image.height() + 5, width() - 10, height() - imageY - image.height() - 10);
}

void PlayerStat::setImage(QString img)
{
    image = QImage(img).scaled(25, 25);
}

void PlayerStat::updateReserve(QList<Player *> players)
{
    QLayoutItem* child;
    while(mLayout->count() !=0)
    {
        child = mLayout->takeAt(0);
        if(child->widget() != 0)
        {
            delete child->widget();
        }
        delete child;
    }
    for (int i = 0; i < players.size(); i++)
    {
        Player *player = players.at(i);
        if (player->isOpponent() == opponent && player->isReserve())
        {
            ReserveButton *but = new ReserveButton(this);
            but->setFixedSize(55, 55);
            but->setPlayer(player);
            connect(but, SIGNAL(buttonClicked(Player*)), this, SLOT(onButtonClicked(Player*)));
            mLayout->addWidget(but);
        }
    }
}

void PlayerStat::onButtonClicked(Player* player)
{
    emit playerSelected(player);
}

void PlayerStat::setOpponent(bool isOpponent)
{
    opponent = isOpponent;
}

void PlayerStat::setActiveUser(bool isActive)
{
    activeUser = isActive;
}
