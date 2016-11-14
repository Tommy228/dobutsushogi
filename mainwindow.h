#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QProgressBar>

#include "board.h"
#include "game.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Board *board;
    Game *game;
    void addDefaultPlayers(bool opponent);
    QLabel *messageLabel;
    QProgressBar *progressBar;

private slots:
    void onUserWins(bool opponent);
    void updateStatusBar(void);
    void getAIProgress(int progress);
    void onDecisionStart(void);
    void onDecisionEnd(void);
    void startNewGame(void);
    void showEvaluation(void);
};

#endif // MAINWINDOW_H
