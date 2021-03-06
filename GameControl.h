#pragma once
#include "Bot.h"
#include "Player.h"

#include <vector>

class GameControl
{
public:
	GameControl();
	~GameControl();

	void saveLeaderB();

	void loadLeaderB();
	
	void options();

	void start();

	void sortLB();

	void DisplayGame(Ball myBall, Player myPlayer, Bot myBot, int x, int y);

	int checkWinCondition(int xCord);

private:
	char board[30][120];
	int vict;
	int playerScore;
	int botScore;
	int leaderboard[11];
	int difficulty;
};

