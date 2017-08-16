#pragma once
#include "armor.h"
#include <ctime>
#include <fstream>
#include <vector>
#include <map>

enum Hit {NOTHING, ENVIRONMENT, ARMOR};

class Map
{
private:
	int sizeX;
	int sizeY;
	char **map;
	const char ARMOR_CH = '8';
	const char ENVIRONMENT_CH = 'M';
	std::map<string, string>MAPS;
public:
	Map(int x, int y);
	void fillFromFile(); //for hand-made maps

	void cleanSpace(int x, int y, int size); //cleans a square with given coordinates and size
	void addArmor(int x, int y);
	Hit missileInteraction(int x, int y); //returnes the object it hits	
	
	bool spaceIsEmpty(int x, int y, int size) const;
	void print() const;
	void printEveryTurn();//is not used recently, was used for debug
	int getSizeX() const { return sizeX; }
	int getSizeY() const { return sizeY; }

	~Map();
};

