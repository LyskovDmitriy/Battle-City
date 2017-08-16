#include "MovingObject.h"

MovingObject::MovingObject() : X(0), Y(0), direction(UP)
{
}

MovingObject::MovingObject(int x, int y, Direction dir): X(x), Y(y), direction(dir)
{
}

void MovingObject::move()
{
	switch (direction)
	{
	case UP:
		Y--;
		break;
	case DOWN:
		Y++;
		break;
	case RIGHT:
		X++;
		break;
	case LEFT:
		X--;
		break;
	}
}

MovingObject::~MovingObject()
{
}
