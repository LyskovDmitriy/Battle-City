#include "Game.h"

Game::Game(int x, int y) : map(x, y), isWon(false), type(gameType::SINGLEPLAYER), AINumber(0)
{
	map.fillFromFile();
}

void Game::playSinglePlayerGame(int AInumber)
{
	type = gameType::SINGLEPLAYER;
	AINumber = AInumber;
	map.print();	
	printRules();
	player = createPlayer(DOWN);
	secondPlayer = nullptr;
	for (int i = 0;i < AINumber;i++)
	{
		createAI();
		this_thread::sleep_for(0.1s);
	}
	//map.printEveryTurn();//for debug
	thread{ startPlayerAction, this }.join();
}

void Game::playMultiPlayerGame()
{
	type = gameType::MULTIPLAYER;
	map.print();
	printRules();
	player = createPlayer(DOWN);
	secondPlayer = createPlayer(UP);
	thread{ startPlayerAction, this }.join();
}

void Game::createAI()
{
	srand(time(NULL));
	int x = 0;
	int y = 0;
	while (true)
	{
		x = rand() % (map.getSizeX() - 2);
		y = rand() % (map.getSizeY() / 3);
		if (map.spaceIsEmpty(x, y, 3))
			break;
	}
	unique_lock<mutex> lc{ m };
	AIArmors.push_back(new Armor{ x + 1,y + 1,DOWN });
	AIArmors.back()->print();
	map.addArmor(x, y);
	lc.unlock();
	thread(AIHandling, AIArmors.back(), this).detach();
}

Armor * Game::createPlayer(Direction dir)
{
	srand(time(NULL));
	int X = 0, Y = 0;
	while (true)
	{
		X = rand() % (map.getSizeX() - 2);
		if (dir == DOWN)
			Y = map.getSizeY() - 2 - rand() % 5;
		else
			Y = 0 + rand() % 4;
		if (map.spaceIsEmpty(X, Y, 3))
			break;
	}
	Armor* player = new Armor{ X + 1,Y + 1, dir == UP ? DOWN : UP };
	player->print();
	map.addArmor(player->getX() - 1, player->getY() - 1);
	return player;
}

void Game::printRules() const
{
	int x = map.getSizeX();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (short)(x + 3),0 });
	cout << "Управление: ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (short)(x + 3),1 });
	if (type == gameType::SINGLEPLAYER)
		cout << "Перемещение на WASD и стрелки. Стрельба на Q или пробел";
	else
	{
		cout << "Первый игрок     Перемещение на WASD. Стрельба на Q или пробел";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (short)(x + 3),2 });
		cout << "Второй игрок     Перемещение на стрелки. Стрельба на 0";
	}
}

void startPlayerAction(Game* game)
{
	while (true)
	{
		Missile missile = playerInteraction(game);
		if (!game->player->checkAlive() || (game->secondPlayer!=nullptr && !game->secondPlayer->checkAlive()) || game->isWon)
			return;
		thread f(missileHandling, missile, game);
		f.detach();
	}
}

Missile playerInteraction(Game* game)
{
	char ch = 'a';
	Direction dir = UP;
	Armor* activePlayer = nullptr;
	while (true)
	{
		if (!game->player->checkAlive()||game->isWon)
			return Missile();
		if (_kbhit())
			ch = _getch();
		else
		{
			ch = 'g';
			this_thread::sleep_for(0.02s);
		}
		if (ch == 'q' || ch == ' ')
		{
			this_thread::sleep_for(0.2s);
			return game->player->getMissile();
		}
		else if (ch == '0' && game->type==gameType::MULTIPLAYER)
		{
			this_thread::sleep_for(0.2s);
			return game->secondPlayer->getMissile();
		}
		switch (ch)
		{
		case 'w':
			activePlayer = game->player;
			dir = UP;
			break;
		case 72:
			activePlayer = game->secondPlayer;
			dir = UP;
			break;
		case 'a':
			activePlayer = game->player;
			dir = LEFT;
			break;
		case 75:
			activePlayer = game->secondPlayer;
			dir = LEFT;
			break;
		case 's':
			activePlayer = game->player;
			dir = DOWN;
			break;
		case 80:
			activePlayer = game->secondPlayer;
			dir = DOWN;
			break;
		case 'd':
			activePlayer = game->player;
			dir = RIGHT;
			break;
		case 77:
			activePlayer = game->secondPlayer;
			dir = RIGHT;
			break;
		default:
			continue;
		}
		if (game->type == gameType::SINGLEPLAYER)
			movementHandling(game->player, dir, game->map);
		else
			movementHandling(activePlayer, dir, game->map);
		if (game->isWon)
			return Missile();
	}
}

Direction findPlayer(Armor * armor, Armor* player) //compares the directions of player and armor
{
	int x = armor->getX();
	int y = armor->getY();
	int playerX = player->getX();
	int playerY = player->getY();
	Direction dir = NOWHERE;
	if ((x < playerX + 2 && x > playerX - 2) && y > playerY)
		dir = UP;
	else if ((x < playerX + 2 && x > playerX - 2) && y < playerY)
		dir = DOWN;
	else if (x < playerX && (y > playerY - 2 && y < playerY + 2))
		dir = RIGHT;
	else if (x > playerX && (y > playerY - 2 && y < playerY + 2))
		dir = LEFT;
	return dir;
}

void AIHandling(Armor* armor, Game* game)
{
	while (armor->checkAlive() && !game->isWon)
	{
		bool playerFound = false;
		Direction dir = findPlayer(armor, game->player);
		if (dir == NOWHERE) //if player was not found, move in random direction
		{
			srand(time(NULL) + armor->getX());
			dir = static_cast<Direction>(rand() % 4);
		}
		else
			playerFound = true;
		int maxTurns = rand() % 7;
		for (int i = 0;i < maxTurns && armor->checkAlive();i++)
		{
			bool moved = false;
			int oldX = armor->getX();
			int oldY = armor->getY();
			movementHandling(armor, dir, game->map);
			if (playerFound) //shoots if player was found
			{
				thread f(missileHandling, armor->getMissile(), game);
				f.detach();
			}
			else
			{
				if (oldX == armor->getX() && oldY == armor->getY()) //doesn't wait if just turned, not moved
					break;
			}
			chrono::duration<double> sleep{ (2.0 + rand() % 2) / 10 };
			if (!armor->checkAlive() || game->isWon)
				return;
			this_thread::sleep_for(sleep);
		}
	}
}

void movementHandling(Armor* armor, Direction dir, Map& map)
{
	bool turned = true;
	unique_lock<mutex> lc{ m };
	if (!armor->turn(dir)) //moves if didn't turn
	{
		turned = false;
		int moveX = 0;
		int moveY = 0;
		switch (dir)
		{
		case UP:
			moveY--;
			break;
		case DOWN:
			moveY++;
			break;
		case RIGHT:
			moveX++;
			break;
		case LEFT:
			moveX--;
		}
		map.cleanSpace(armor->getX() - 1, armor->getY() - 1, 3);
		if (map.spaceIsEmpty(armor->getX() - 1 + moveX, armor->getY() - 1 + moveY, 3))
		{
			armor->erase();
			armor->move();
		}
		map.addArmor(armor->getX() - 1, armor->getY() - 1);
	}
	//map.printEveryTurn();//for debug
	armor->print();
	lc.unlock();
	if (!turned)
		this_thread::sleep_for(0.1s);
}

void missileHandling(Missile missile, Game* game)
{
	while (!game->isWon)
	{
		int x = missile.getX();
		int y = missile.getY();
		unique_lock<mutex> lc{ m };
		if (x == 0 || x >= game->map.getSizeX() + 1 || y == 0 || y >= game->map.getSizeY() + 1)
			return;
		auto hitObject = game->map.missileInteraction(x - 1, y - 1);

		switch (hitObject)
		{
		case ARMOR:
			if (game->player->checkHit(x, y))
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,(short int)(game->map.getSizeY() + 2) });
				cout << "Игрок 1 проиграл";
				lc.unlock();
				game->isWon = true;
				return;
			}
			else if (game->secondPlayer!=nullptr && game->secondPlayer->checkHit(x,y))
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,(short int)(game->map.getSizeY() + 2) });
				cout << "Игрок 2 проиграл";
				lc.unlock();
				game->isWon = true;
				return;
			}
			for (int i = 0;i < game->AIArmors.size();i++)
			{
				if (game->AIArmors[i]->checkHit(x, y))
				{
					game->AIArmors[i]->erase();
					game->map.cleanSpace(game->AIArmors[i]->getX() - 1, game->AIArmors[i]->getY() - 1, 3);
					game->AINumber--;
				}
			}
			if (game->AINumber == 0)
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,(short int)(game->map.getSizeY() + 2) });
				cout << "Вы победили";
				lc.unlock();
				game->isWon = true;
				return;
			}
		case ENVIRONMENT:
			missile.erase();
			return;
		case NOTHING:
			missile.print();
			lc.unlock();
			this_thread::sleep_for(0.07s);
			lc.lock();
			missile.erase();
			lc.unlock();
			missile.move();
		}
	}
}

Game::~Game()
{
	delete player;
	for (auto armor : AIArmors)
		delete armor;
	if (secondPlayer != nullptr)
		delete secondPlayer;
}
