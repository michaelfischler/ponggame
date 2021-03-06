#include "GameControl.h"
#include "Player.h"
#include "Ball.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

extern void SetColor(int);
extern void PlaceCursor(const int x, const int y);
extern void printArt(string fileName);


GameControl::GameControl()
{
	vict = 3;
	playerScore = 0;
	botScore = 0;
	difficulty = 2;

	for (int i = 0; i < 11;i++)
	{
		leaderboard[i] = 0;
	}
}

GameControl::~GameControl()
{
}

//move player with switch statement then move bot then move ball and display function at the end then check for win
//function called when user chooses to start playing the actual game

void GameControl::start()
{
	const char paddle = 219;
	const char ball = 254;
	Ball *myBall = new Ball();
	Player *myPlayer = new Player();
	Bot *myBot = new Bot();

	this->playerScore = 0;
	this->botScore = 0;

	//boolean value to check whether end program button was pressed and exit program
	bool check = true;

	system("mode 80"); // ensure console width is large enough
	system("CLS");

	// print how many points required to win
	PlaceCursor(30, 11);
	cout << "First to ";
	SetColor(0x0D);
	cout << vict;
	SetColor(10);
	cout << " wins!" << endl;
	Sleep(1500);

	while (check == true)
	{
		int winner = 0, ballDir = 0, ballX = 0, ballY = 0;

		srand(time(NULL)); 
		ballDir = (rand() % 4) + 1; // seed for random ball starting direction

		srand(time(NULL));
		ballY = (rand() % 10) + 6; // seed for random ball starting posY

		srand(time(NULL));
		ballX = (rand() % 20) + 31; // seed for random ball starting posX

		myBall->setXandYandDir(ballX, ballY, ballDir);
		DisplayGame(*myBall, *myPlayer, *myBot, ballX, ballY); // print game board

		while (1)
		{
			//checks if escape key was pressed
			if (myPlayer->getInput() == 27)
			{
				delete myBall;
				delete myPlayer;
				delete myBot;

				check = false;
				break;
			}

			// move player, ball, and bot
			myPlayer->move();
			myBot->move(myBall->getX(), myBall->getY(), myBall->getDir(), this->difficulty); // passing position and direction of ball
			myBall->move(myPlayer->getY(), myBot->getY());             // passing position of player and bot

			// print updated positions
			DisplayGame(*myBall, *myPlayer, *myBot, ballX, ballY);

			// check if someone has scored, 0 = nobody, 1 = player, 2 = bot
			winner = checkWinCondition(myBall->getX());

			//consider flushing user input stream here

			if (winner != 0) // triggered if some scored a point
			{
				if (winner == 1)
				{
					// add 1 to player score
					playerScore++;

					// print "PLAYER SCORED !" 
					system("CLS");
					SetColor(0x0B);
					PlaceCursor(0, 7);
					printArt("pScore.txt");
					SetColor(10);
					Sleep(1500);
				}
				else if (winner == 2)
				{
					// add 1 to bot score
					botScore++;

					// print "BOT SCORED !" 
					system("CLS");
					SetColor(0x0C);
					PlaceCursor(0, 7);
					printArt("bScore.txt");
					SetColor(10);
					Sleep(1500);
				}
				goto checkForVictory; // break out of nested loop to see if any has reached number of points for victory
			}

		}

		checkForVictory:

			// checking if victory condition has been met
			if (playerScore == vict || botScore == vict)
			{
				system("CLS");
				if (playerScore == vict)
				{
					SetColor(0xB0);
					PlaceCursor(0, 11);
					printArt("pWin.txt");
					SetColor(10);
					sortLB();
					Sleep(3000);
				}
				else
				{
					SetColor(0xC0);
					PlaceCursor(0, 3);
					printArt("bWin.txt");
					SetColor(10);
					sortLB();
					Sleep(3000);
				}

				delete myBall;
				delete myPlayer;
				delete myBot;

				break;
			}
	}
}

int GameControl::checkWinCondition(int xCord)
{
	if (xCord == 79) // computer scored
	{
		return 2;
	}
	if (xCord == 0) // player scored
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// calls player, bot, and ball draw functions, prints game border and updates the scoreboard
void GameControl::DisplayGame(Ball myBall, Player myPlayer, Bot myBot, int posx, int posy)
{
	system("CLS");

	//start drawing player + bot which just moved
	myPlayer.draw(79, myPlayer.getY());
	myBot.draw(0, myBot.getY());

	//start drawing ball which just moved
	myBall.draw(posx, posy);

	//top and bottom border, with spaces for the score cards
	char ch = 176;
	int x = 0;
	while (x >= 0 && x < 5)
	{
		PlaceCursor(x, 0);
		cout << ch;
		x++;
	}
	x = 19;
	while (x > 18 && x < 58)
	{
		PlaceCursor(x, 0);
		cout << ch;
		x++;
	}
	x = 75;
	while (x > 74 && x < 80)
	{
		PlaceCursor(x, 0);
		cout << ch;
		x++;
	}
	for (int x2 = 0; x2 < 80; x2++)
	{
		PlaceCursor(x2, 22);
		cout << ch;
	}

	//current score cards
	SetColor(0xA0);
	PlaceCursor(5, 0);
	cout << " BOT SCORE: " << botScore << " " << endl;
	PlaceCursor(58, 0);
	cout << " PLAYER SCORE: " << playerScore << " " << endl;
	SetColor(10);
}


//function to load the leaderboard, call this function at the beginning of every game to load the leaderboard list primarily and to show user potential goal to achieve
void GameControl::loadLeaderB()
{
	string fileName = "leaderboard.txt";
	string line;
	int score = 0;
	int i = 0;

	ifstream myFile(fileName);

	cout << "\nPONG MASTER DELUXE SUPER TRIPLE DELUXE MASTER EDITION VOLUME 17 Leaderboard!:" << endl << endl << endl;

	while (std::getline(myFile, line) && i < 13)
	{
		std::istringstream iss(line);

		cout << line << endl;		

		if (i > 3)
		{
			iss >> score;

			leaderboard[i] = score;
			
		}
		i++;
	}
	myFile.close();
}

void GameControl::options()
{
	int inp;
	cout << "\n1. Change Difficulty" << endl;
	cout << "2. Amount of Points for Victory" << endl;
	cout << "Press 9 to return to previous screen.\n" << endl;
	cin >> inp;

	
		if (inp == 1)
		{
			cout << "Choose difficulty 1 through 4." << endl;
			cout << "1. BEGINNER  2. INTERMEDIATE  3. CHALLENGING  4. INSANE" << endl;
			cout << "Press 9 to return to previous screen.\n" << endl;
			cin >> inp;

			if (inp <= 4)
			{
				cout << inp << " is the new difficulty for the game." << endl;
				Sleep(2500);
				this->difficulty = inp;
			}
			else
			{
				cout << "Invalid difficulty setting.\nReturning to main menu." << endl;
				Sleep(2500);
			}
		}
		else if (inp == 2)
		{
			cout << "Choose amount of Points to win." << endl;
			cout << "Press 9 to return to previous screen.\n" << endl;

			cin >> vict; 
			
			cout << endl << vict << " points required for victory." << endl;
			Sleep(2500);

		}		
		if(inp == 9)
		{
			cout << "Returning to previous menu..." << endl;
		}
}

void GameControl::saveLeaderB()
{
	string fileName = "leaderboard.txt";

	ofstream myFile(fileName);

	myFile << "PONG MASTER DELUXE SUPER TRIPLE DELUXE MASTER EDITION VOLUME 17 Leaderboard!:" << endl << endl << endl;

	for (int i = 0; i < 10; i++)
	{
		myFile << leaderboard[i];
	}

}


//call this function when game ends to input the new score
void GameControl::sortLB()
{
	leaderboard[10] = playerScore;

	//insertion sort algorithm to sort leaderboard and input the new score
	int i, key, j;


	//size of the array being sorted
	int n = 11;

	for (i = 1; i < n; i++)
	{
		key = leaderboard[i];
		j = i - 1;

		/* Move elements of leaderboard[0..i-1], that are
		greater than key, to one position ahead
		of their current position */
		while (j >= 0 && leaderboard[j] > key)
		{
			leaderboard[j + 1] = leaderboard[j];
			j = j - 1;
		}
		leaderboard[j + 1] = key;
	}
}