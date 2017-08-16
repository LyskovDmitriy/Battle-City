#pragma once
#include "Map.h"
#include <conio.h>
#include <thread>
#include <mutex>

static mutex m;//for all threads

enum class gameType{SINGLEPLAYER, MULTIPLAYER};

class Game
{
private:
	Map map;
	Armor* player;
	Armor* secondPlayer;
	vector <Armor*> AIArmors;
	bool isWon;
	gameType type;
	int AINumber;
public:
	Game(int x, int y, int AInumber = 3);

	void playSinglePlayerGame();
	void playMultiPlayerGame();

	void createAI();
	Armor* createPlayer(Direction);

	void printRules() const;

	friend void startPlayerAction(Game* game); //begins a game for the player
	friend Direction findPlayer(Armor* armor, Armor* player); //searches for the player and returns direction to him
	friend Missile playerInteraction(Game* player); //for keyboard interaction
	friend void AIHandling(Armor* armor, Game* game); //makes AI armor move and shoot
	friend void movementHandling(Armor* armor, Direction dir, Map& map); //turnes or moves armor depending on it's direction
	friend void missileHandling(Missile missile, Game* game); //is responsible for missile movement and collision

	~Game();
};



