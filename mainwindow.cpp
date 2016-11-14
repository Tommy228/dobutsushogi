#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "definitions.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setWindowTitle("Yokai No Mori");

    game = new Game;
    game->setBoardSize(QPoint(3, 4));
    addDefaultPlayers(true);
    addDefaultPlayers(false);
    game->enableAutoplay(true);
    connect(game, SIGNAL(userWins(bool)), this, SLOT(onUserWins(bool)));
    connect(game, SIGNAL(playerMoved(bool)), this, SLOT(updateStatusBar()));
    connect(game, SIGNAL(sendAIProgress(int)), this, SLOT(getAIProgress(int)));
    connect(game, SIGNAL(startedDecision()), this, SLOT(onDecisionStart()));
    connect(game, SIGNAL(endedDecision()), this, SLOT(onDecisionEnd()));
    game->newGame();

    board = new Board(this);
    board->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->mainLayout->addWidget(board);
    board->setGame(game);

    connect(ui->actionMake_decision, SIGNAL(triggered(bool)), game, SLOT(AIDecision()));
    connect(ui->actionRandom_decision, SIGNAL(triggered(bool)), game, SLOT(randomAIMove()));
    connect(ui->actionAutoplay, SIGNAL(triggered(bool)), game, SLOT(enableAutoplay(bool)));
    connect(ui->actionNew_Game, SIGNAL(triggered(bool)), this, SLOT(startNewGame()));
    connect(ui->actionEvaluer, SIGNAL(triggered(bool)), this, SLOT(showEvaluation()));

    messageLabel = new QLabel(this);
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->statusBar->addWidget(messageLabel);
    ui->statusBar->addWidget(progressBar);
    updateStatusBar();

    setFixedSize(325, 570);
}

void MainWindow::startNewGame()
{
    game->clearGame();
    addDefaultPlayers(true);
    addDefaultPlayers(false);
    game->newGame();
    board->update();
    progressBar->setValue(0);
    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete game;
    delete ui;
}

void MainWindow::addDefaultPlayers(bool opponent)
{
    // Koropokkuru, le roi
    Player *koro = new Player;
    koro->setType(TYPE_KORO);
    koro->setImage(":/images/koro.png");
    koro->setKing(true);
    koro->setReserve(false);
    koro->setOpponent(opponent);
    if (opponent)
    {
        koro->setPos(QPoint(2, 1));
    }
    else
    {
        koro->setPos(QPoint(2, 4));
    }
    koro->setPower(0);

    QList<QPoint> koroPattern;
    koroPattern << QPoint(-1, -1); // en haut à gauche
    koroPattern << QPoint(0, -1); // en haut
    koroPattern << QPoint(1, -1); // en haut à droite
    koroPattern << QPoint(1, 0); // à droite
    koroPattern << QPoint(1, 1); // en bas à droite
    koroPattern << QPoint(0, 1); // en bas
    koroPattern << QPoint(-1, 1); // en bas à gauche
    koroPattern << QPoint(-1, 0); // à gauche
    koro->setMovePattern(koroPattern);

    game->addPlayer(koro);

    // Kitsune, le renard
    Player *kitsune = new Player;
    kitsune->setType(TYPE_KITSUNE);
    kitsune->setImage(":/images/kitsune.png");
    kitsune->setKing(false);
    kitsune->setReserve(false);
    kitsune->setOpponent(opponent);
    if (opponent)
    {
        kitsune->setPos(QPoint(3, 1));
    }
    else
    {
        kitsune->setPos(QPoint(1, 4));
    }
    kitsune->setPower(AI_KITSUNE_POWER);

    QList<QPoint> kitsunePattern;
    kitsunePattern << QPoint(-1, -1); // en haut à gauche
    kitsunePattern << QPoint(1, -1); // en haut à droite
    kitsunePattern << QPoint(1, 1); // en bas à droite
    kitsunePattern << QPoint(-1, 1); // en bas à gauche
    kitsune->setMovePattern(kitsunePattern);

    game->addPlayer(kitsune);

    // Takuni, le raton-laveur
    Player *tanuki = new Player;
    tanuki->setType(TYPE_TANUKI);
    tanuki->setImage(":/images/tanuki.png");
    tanuki->setKing(false);
    tanuki->setReserve(false);
    tanuki->setOpponent(opponent);
    if (opponent)
    {
        tanuki->setPos(QPoint(1, 1));
    }
    else
    {
        tanuki->setPos(QPoint(3, 4));
    }
    tanuki->setPower(AI_TANUKI_POWER);

    QList<QPoint> tanukiPattern;
    tanukiPattern << QPoint(0, -1); // en haut
    tanukiPattern << QPoint(1, 0); // à droite
    tanukiPattern << QPoint(0, 1); // en bas
    tanukiPattern << QPoint(-1, 0); // à gauche
    tanuki->setMovePattern(tanukiPattern);

    game->addPlayer(tanuki);

    // Kodoma, esprit de l'arbre
    Player *kodoma = new Player;
    kodoma->setType(TYPE_KODOMA);
    kodoma->setImage(":/images/kodoma.png");
    kodoma->setKing(false);
    kodoma->setReserve(false);
    kodoma->setOpponent(opponent);
    if (opponent)
    {
        kodoma->setPos(QPoint(2, 2));
    }
    else
    {
        kodoma->setPos(QPoint(2, 3));
    }
    kodoma->setPower(AI_KODOMA_POWER);

    QList<QPoint> kodomaPattern;
    kodomaPattern << QPoint(0, -1); // en haut
    kodoma->setMovePattern(kodomaPattern);

    game->addPlayer(kodoma);

}

void MainWindow::onUserWins(bool opponent)
{
    QString str;
    if (opponent)
    {
        str = "L'intelligence artificielle gagne!";
    }
    else
    {
        str = "Vous gagnez!";
    }
    QMessageBox::information(this, "Fin du match", str);
}

void MainWindow::updateStatusBar(void)
{
    if (!game->isGameValid())
    {
        QString msg("Fin du jeu");
        if (game->isOpponentWinner())
        {
            msg += " (victoire de l'intelligence artificielle).";
        }
        else
        {
            msg += " (victoire du joueur).";
        }
        messageLabel->setText(msg);
        progressBar->setVisible(false);
    }
    else
    {
        if (!game->isOpponentTurn())
        {
            messageLabel->setText("En attente de la décision de l'utilisateur.");
        }
        else
        {
            if (!ui->actionAutoplay->isChecked())
            {
                messageLabel->setText("Tour de l'intelligence artificielle. Autoplay désactivé.");
                progressBar->setVisible(false);
            }
        }
    }
}

void MainWindow::getAIProgress(int progress)
{
    progressBar->setValue(progress);
}

void MainWindow::onDecisionStart(void)
{
    progressBar->setVisible(true);
    messageLabel->setText("En attende de l'intelligence artificielle.");
    progressBar->setValue(0);
    ui->actionMake_decision->setDisabled(true);
    ui->actionRandom_decision->setDisabled(true);
    ui->actionNew_Game->setDisabled(true);
}

void MainWindow::onDecisionEnd()
{
    ui->actionMake_decision->setDisabled(false);
    ui->actionRandom_decision->setDisabled(false);
    ui->actionNew_Game->setDisabled(false);
}

void MainWindow::showEvaluation(void)
{
    QVector<QVector<int>> humanSafePos = game->getSafePositions(true);
    for (int i = 0; i < humanSafePos.size(); i++)
    {
        QVector<int> line = humanSafePos.at(i);
        QString str;
        for (int j = 0; j < line.size(); j++)
        {
            str += QString::number(line.at(j)) + " ";
        }
       // qDebug() << str;
    }
}
