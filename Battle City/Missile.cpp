#include "Missile.h"

Missile::Missile() : MovingObject()
{
}

Missile::Missile(int x, int y, Direction dir) : MovingObject(x, y, dir)
{
}

void Missile::print()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{X,Y});
	cout << '*';
}

void Missile::erase()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ X,Y });
	cout << ' ';
}

Missile::~Missile()
{
}
