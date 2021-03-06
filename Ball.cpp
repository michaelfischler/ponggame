#include "Ball.h"

#include <iostream>

using namespace std;

extern void PlaceCursor(const int x, const int y);

Ball::Ball()
{
	x = 11;
	y = 40;
}

Ball::~Ball()
{
}

void Ball::draw(int xVal, int yVal)
{
	char ball = 254; 
	PlaceCursor(x, y);
	cout << ball;
}
void Ball::move(int playerCoord, int botCoord)
{
	// are we at the top?
	if (y == 1)
	{
		// are we going up-right ^ >
		if (direction == 1) 
		{
			// then bounce, move down and to the right
			y++;
			x++;
			direction = 2;
			goto paddleCheck; // break out of nested loop to see if we've hit a paddle
		}
		// are we going up-left < ^
		else if (direction == 3) 
		{
			// then bounce, move down and to the left
			y++;
			x--;
			direction = 4;
			goto paddleCheck;
		}
	}
	// are we at the bottom?
	else if (y == 21)
	{
		// are we going bot-right v >
		if (direction == 2) 
		{
			// then bounce, move up and to the right
			x++;
			y--;
			direction = 1;
			goto paddleCheck;
		}
		// are we going bot-left < v
		else if (direction == 4)
		{
			// then bounce, move up and to the left
			x--;
			y--;
			direction = 3;
			goto paddleCheck;
		}
	}
	// ^ >
	if (direction == 1) 
	{
		x++;
		y--;
	}
	// v >
	else if (direction == 2)
	{
		x++;
		y++;
	}
	// < ^
	else if (direction == 3) 
	{
		x--;
		y--;
	}
	// < v
	else if (direction == 4) 
	{
		x--;
		y++;
	}

	paddleCheck:
	// are we all the way to the left (computer side) ?
	if (x == 0)
	{
		// check if we've hit bot paddle (botCoord is top of paddle, increment for 3 more paddle chars)
		if (y == botCoord || y == botCoord + 1 || y == botCoord + 2 || y == botCoord + 3) 
		{
			// we've hit the paddle, so bounce off to the right
			x += 2;
			// adjust direction based on trajectory
			if (direction == 3)
			{
				direction = 1;
			}
			else if (direction == 4)
			{
				direction = 2;
			}
		}
	}
	// are we all the way to the right (player side) ?
	else if (x == 79)
	{
		// check if we've hit player paddle (playerCoord is top of paddle, increment for 3 more paddle chars)
		if (y == playerCoord || y == playerCoord + 1 || y == playerCoord + 2 || y == playerCoord + 3)
		{
			// we've hit the paddle, so bounce off to the left
			x -= 2;
			// adjust direction based on trajectory
			if (direction == 1)
			{
				direction = 3;
			}
			else if (direction == 2)
			{
				direction = 4;
			}
		}
	}
	
}

void Ball::setXandYandDir(int xVal, int yVal, int dirVal)
{
	x = xVal;
	y = yVal;
	direction = dirVal;
}

int Ball::getX()
{
	return x;
}

int Ball::getY()
{
	return y;
}

int Ball::getDir()
{
	return direction;
}