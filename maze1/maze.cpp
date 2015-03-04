/* Перебор всех возможных вариантов, в минуту укладывается поле максимум 5 x 5  */

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>

using namespace std;

#define NOT_INIT	65535

bool error(char error[])
{
	cout << error << endl;
	system("pause");
	return false;
}

int max(int (&a)[4])
{
	int max = 0;
	for (int i = 1; i < 4; i++)
	{
		if (a[i] == NOT_INIT)	continue;
		if (a[max] == NOT_INIT || a[max] != NOT_INIT && a[i] > a[max])	max = i;
	}	
	return max;
}

int maze(const vector<string>& field, const int n, const int m, vector<bool>& visited, string& patch, int cur_n, int cur_m, int level)
{
	if (field[cur_n][cur_m] == 'e')
	{
		return 2000;
	}

	vector<bool> new_visited = visited;
	new_visited[(cur_n * m) + cur_m] = true;
	
	string new_patch[4] = { patch, patch, patch, patch };

	int points[4] = { NOT_INIT, NOT_INIT, NOT_INIT, NOT_INIT };
	//	вверх
	int new_n = cur_n - 1, new_m = cur_m;
	if (field[new_n][new_m] != '#' && new_visited.at((new_n * m) + new_m) == false)
	{
		new_patch[0].push_back('u');
		points[0] = maze(field, n, m, new_visited, new_patch[0], new_n, new_m, level + 1);
	}
	//	вниз
	new_n = cur_n + 1, new_m = cur_m;
	if (field[new_n][new_m] != '#' && new_visited.at((new_n * m) + new_m) == false)
	{
		new_patch[1].push_back('d');
		points[1] = maze(field, n, m, new_visited, new_patch[1], new_n, new_m, level + 1);
	}
	//	влево
	new_n = cur_n, new_m = cur_m - 1;
	if (field[new_n][new_m] != '#' && new_visited.at((new_n * m) + new_m) == false)
	{
		new_patch[2].push_back('l');
		points[2] = maze(field, n, m, new_visited, new_patch[2], new_n, new_m, level + 1);
	}
	//	вправо
	new_n = cur_n, new_m = cur_m + 1;
	if (field[new_n][new_m] != '#' && new_visited.at((new_n * m) + new_m) == false)
	{
		new_patch[3].push_back('r');
		points[3] = maze(field, n, m, new_visited, new_patch[3], new_n, new_m, level + 1);
	}
	int add_points = 0;
	if ('1' <= field[cur_n][cur_m] && field[cur_n][cur_m] <= '9')
	{
		add_points = ((int)(field[cur_n][cur_m] - 48)) * 100;
	}
	int num = max(points);
	if (points[num] == NOT_INIT)
	{
		// в тупике
		return NOT_INIT;	//	пока никак не обрабатываем, а просто обрываем эту ветвь
	}
	else
	{
		patch = new_patch[num];
	}
	return (-10 + points[num] + add_points);
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "russian");

	ifstream F;
	F.open("maze.txt", ios::in);
	if (F)
	{  
		vector<string> field;
		string s;
		getline(F, s);
		field.push_back(s);
		int n = 1, m = s.size(),
			start_n = -1, start_m = string::npos,
			finish_n = -1, finish_m = string::npos;
		for (; F.eof() == false; n++)
		{
			getline(F, s);
			field.push_back(s);
			if (start_m == string::npos && (start_m = s.find("*")) != string::npos)		start_n = n;
			if (finish_m == string::npos && (finish_m = s.find("e")) != string::npos)	finish_n = n;
		}
		if (start_m == string::npos)		return error("В лабиринте не задан старт");
		else if (finish_m == string::npos)	return error("В лабиринте не задан финиш");
		
		vector<bool> visited(n * m);
		string patch;
		int points = maze(field, n, m, visited, patch, start_n, start_m, 0);
		cout << "Наилучший путь: " << patch << ", кол-во очков: " << ((float)points/100) << endl;
	}
	else cout << "Файл с лабиринтом не существует" << endl;
	system("pause");
	return false;
}
