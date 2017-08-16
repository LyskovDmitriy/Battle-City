#include "armor.h"

Armor::Armor(int x, int y, Direction dir) : MovingObject(x, y, dir), isAlive(true)
{
}

bool Armor::turn(Direction dir)
{
	if (direction == dir)
		return false;
	else
	{
		direction = dir;
		return true;
	}
}

void Armor::print()
{
	//first draw a square of 'O'
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	for (int i = 0;i < 3;i++)
	{
		coord.X = X;
		coord.Y = Y+i;
		SetConsoleCursorPosition(hConsole, coord);
		for (int j = 0;j < 3;j++)
		{
			cout << 'O';
		}
	}
	//then depending on direction draw the gun and delete 2 'O' near it
	coord.X = X;
	coord.Y = Y;
	int moveX = 0;
	int moveY = 0;
	switch (direction)
	{
	case UP:
		moveX = 1;
		break;
	case LEFT:
		moveY = 1;
		break;
	case RIGHT:
		coord.X = X + 2;
		coord.Y = Y + 2;
		moveY = -1;
		break;
	case DOWN:
		coord.X = X + 2;
		coord.Y = Y + 2;
		moveX = -1;
	}
	for (int i = 0;i < 3;i++)
	{
		SetConsoleCursorPosition(hConsole, coord);
		if (i != 1)
			cout << " ";
		else
		{
			if (direction == UP || direction == DOWN)
				cout << '|';
			else
				cout << "-";
		}
		coord.X += moveX;
		coord.Y += moveY;
	}
}

void Armor::erase()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	for (int i = 0;i < 3;i++)
	{
		coord.X = X;
		coord.Y = Y + i;
		SetConsoleCursorPosition(hConsole, coord);
		for (int j = 0;j < 3;j++)
		{
			cout << ' ';
		}
	}
}

Missile Armor::getMissile()
{
	int missileX = X;
	int missileY = Y;
	switch (direction)
	{
	case UP:
		missileX += 1;
		missileY -= 1;
		break;
	case DOWN:
		missileX += 1;
		missileY += 3;
		break;
	case LEFT:
		missileY += 1;
		missileX -= 1;
		break;
	case RIGHT:
		missileX += 3;
		missileY += 1;
	}
	return Missile(missileX,missileY,direction);
}

bool Armor::checkHit(int x, int y)
{
	if ((x <= X + 2 && x >= X) && (y <= Y + 2 && y >= Y))
	{
		isAlive = false;
		return true;
	}
	return false;
}

Armor::~Armor()
{
}
