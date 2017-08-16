#pragma once
#include "MovingObject.h"

class Missile: public MovingObject
{
public:
	Missile();
	Missile(int x, int y, Direction dir);
	void print();
	void erase();
	Direction getDirection() const { return direction; }
	~Missile();
};

