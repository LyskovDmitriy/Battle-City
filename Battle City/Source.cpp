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
		short int sizeX = 25 + rand() % 76; // [25,100]
		short int sizeY = 25 + rand() % 6; // [25,30]
		cout << "�������� ����� (1 - ��������� ����, 2 - ���������������������): ";
		char answer = '1';
		cin >> answer;
		cin.ignore(10, '\n');
		Game game(sizeX, sizeY, 4);
		if (answer == '2')
			game.playMultiPlayerGame();
		else
			game.playSinglePlayerGame();
		this_thread::sleep_for(1s);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,(short)(sizeY + 2) });
		answer = 'n';
		cout << "������� ��� ��� (y/n)? ";
		cin >> answer;
		system("cls");
		if (answer != 'y')
			return 0;
	}
}
