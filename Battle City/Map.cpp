#include "Map.h"

Map::Map(int x, int y) : sizeX(x), sizeY(y)
{
	MAPS.insert(make_pair("Карта 1", "Maps/map1.txt" ));
	MAPS.insert(make_pair("Карта 2", "Maps/map2.txt"));
	MAPS.insert(make_pair("Карта 3", "Maps/map3.txt"));
	MAPS.insert(make_pair("Карта 4", "Maps/map4.txt"));
	
	map = new char*[sizeY];
	for (int i = 0;i < sizeY;i++)
	{
		map[i] = new char[sizeX];
		for (int j = 0;j < sizeX;j++)
		{
			map[i][j] = ' ';
		}
	}
}

void Map::fillFromFile()
{
	ifstream file;
	while (true)
	{
		cout << "Введите номер карты " << endl;
		char count = '1';
		vector <string> adress;
		for (auto map : MAPS)
		{
			cout << count << ": " << map.first.c_str() << endl;
			adress.push_back(map.second);
			count++;
		}
		cin >> count;
		if (count<'1' || count>'0' + MAPS.size())
			cout << "Нет карты с таким номером" << endl << endl;
		else
		{
			file.open(adress[count - '1']);
			break;
		}
	}
	cin.ignore(10, '\n');
	if (!file)
		exit(0);
	//check map's Y size
	int height = 0;
	constexpr int MAX = 100;
	while (true)
	{
		char line[MAX];
		file.getline(line,MAX,'\n');
		if (line[0] != '_')
			height++;
		else
			break;
	}
	file.seekg(0, ios::beg);
	//check map X size
	vector<char>lineToCheckSize;
	char ch = ' ';
	for (int i = 0;file.get(ch);i++)
	{
		if (ch == '|')
			break;
		else
			lineToCheckSize.push_back(ch);
	}
	int length = lineToCheckSize.size();
	file.seekg(0, ios::beg);

	double iD = 0.0; 
	int i = 0;
	while (i < sizeY)
	{
		char* line = new char[length + 5];
		file.getline(line, length + 5, '\n');
		do
		{
			double jD = 0.0;
			int jI = 0;
			for (;jI < sizeX && line[(int)jD] != '|';jD += (double)length / sizeX, jI++)
			{
				if (line[(int)jD] == 'c')
					map[(int)i][jI] = ENVIRONMENT_CH;
			}
			if ((int)(iD + (double)height / sizeY) == (int)iD + 2) //if new iD is old iD+2, skip a line
				file.getline(line, length + 5, '\n');
			iD += (double)height / sizeY;
			i++;
		} while ((int)(iD + (double)height / sizeY) == (int)iD);//if line's (int)iD number doesn't change, print the same next line  
		delete[] line;
	}

}

void Map::print() const
{
	system("cls");
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,0 });
	for (int i = 0;i < sizeY + 2;i++)
	{
		if (i == 0 || i == sizeY + 1)
		{
			cout << " ";
			for (int j = 0;j < sizeX;j++)
				cout << "-";
		}
		else
		{
			cout << "|";
			for (int j = 0;j < sizeX;j++)
				cout << map[i - 1][j];
			cout << "|";
		}
		cout << endl;
	}
}

void Map::printEveryTurn()
{
	for (int i = 0;i < sizeY + 2;i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (short)(sizeX + 2),(short)i });
		if (i == 0 || i == sizeY + 1)
		{
			cout << " ";
			for (int j = 0;j < sizeX;j++)
				cout << "-";
		}
		else
		{
			cout << "|";
			for (int j = 0;j < sizeX;j++)
				cout << map[i - 1][j];
			cout << "|";
		}
		cout << endl;
	}
}

bool Map::spaceIsEmpty(int x, int y, int size) const
{
	for (int i = y;i < y + size;i++)
	{
		for (int j = x;j < x + size;j++)
		{
			if (i < 0 || j < 0 || i >= sizeY || j >= sizeX)
				return false;
			if (map[i][j] != ' ')
				return false;
		}
	}
	return true;
}

void Map::cleanSpace(int x, int y, int size)
{
	for (int i = x;i < x + 3;i++)
	{
		for (int j = y;j < y + 3;j++)
		{
			map[j][i] = ' ';
		}
	}
}

void Map::addArmor(int x, int y)
{
	for (int i = y;i < y + 3;i++)
	{
		for (int j = x;j < x + 3;j++)
		{
			map[i][j] = ARMOR_CH;
		}
	}
}

Hit Map::missileInteraction(int x, int y)
{
	if (map[y][x] == ' ')
	{
		return NOTHING;
	}
	else if (map[y][x] == ARMOR_CH)
	{
		return ARMOR;
	}
	else if (map[y][x] == ENVIRONMENT_CH)
	{
		map[y][x] = ' ';
		return ENVIRONMENT;
	}
}

Map::~Map()
{
	for (int i = 0;i < sizeY;i++)
		delete[]map[i];
	delete[] map;
}
