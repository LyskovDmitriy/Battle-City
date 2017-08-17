#include "Game.h"
#include <conio.h>
#include <thread>
#include <mutex>

int main()
{
	setlocale(LC_ALL, "rus");
	//deletes cursor
	CONSOLE_CURSOR_INFO* info = new CONSOLE_CURSOR_INFO;
	info->bVisible = false;
	info->dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), info);
	delete info;

	srand(time(NULL));
	while (true)
	{
		short int sizeX = 30 + rand() % 71; // [30,100]
		short int sizeY = 25 + rand() % 6; // [25,30]
		cout << "Выберите режим (1 - одиночная игра, 2 - многопользовательская): ";
		char answer = '1';
		cin >> answer;
		cin.ignore(10, '\n');
		char AIs = '1';
		while (true)
		{
			cout << "Введите количество вражеских танков (1-6): ";
			cin >> AIs;
			cin.ignore(10, '\n');
			if (AIs<'1' || AIs>'6')
				cout << "Ошибка ввода" << endl << endl;
			else
				break;
		}
		Game game(sizeX, sizeY, AIs - '0');
		if (answer == '2')
			game.playMultiPlayerGame();
		else
			game.playSinglePlayerGame();
		this_thread::sleep_for(1s);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,(short)(sizeY + 2) });
		answer = 'n';
		cout << "Сыграть еще раз (y/n)? ";
		cin >> answer;
		system("cls");
		if (answer != 'y')
			return 0;
	}
}
