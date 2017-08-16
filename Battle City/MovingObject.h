#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

enum Direction: char { UP, DOWN, LEFT, RIGHT, NOWHERE };

//a parent object for armors and missiles
class MovingObject
{
protected:
	short int X; 
	short int Y;
	Direction direction;
public:
	MovingObject();
	MovingObject(int x, int y, Direction dir);
	virtual void print() = 0;
	virtual void erase() = 0;
	void move(); 
	int getX() const { return X; }
	int getY() const { return Y; }
	virtual ~MovingObject();
};

