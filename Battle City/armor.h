#pragma once
#include "MovingObject.h"
#include "Missile.h"

class Armor: public MovingObject
{
private:
	bool isAlive;
public:
	Armor(int x, int y, Direction dir);
	void print(); //prints itself in the console
	void erase(); //erases itself from the console
	Missile getMissile(); //creates a missile from armor's coordinates and direction
	bool checkHit(int x, int y);//if armor has those coordinates - it was hit
	bool checkAlive() { return isAlive; }
	bool turn(Direction); //true - if turned, false - if will be moved
	~Armor();
};

