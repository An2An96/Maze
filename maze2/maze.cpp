#include <iostream>
#include <fstream>
#include <vector>

#define NOT_INIT 0xFFFF

using namespace std;

struct pos
{
	int pos_n;
	int pos_m;
};

pos createpos(int n, int m)
{
	pos p;
	p.pos_n = n, p.pos_m = m;
	return p;
}

struct s_bonus
{
	int pos_n;
	int pos_m;
	int cost;
};

s_bonus create(int n, int m, int cost)
{
	s_bonus b;
	b.pos_n = n, b.pos_m = m, b.cost = cost;
	return b;
}

struct s_comb
{
	vector<pos> patch;
	int points;
	s_bonus* e;
};

s_comb ccomb(vector<pos> p, int points, s_bonus* e)
{
	s_comb c;
	c.patch = p, c.points = points, c.e = e;
	return c;
}

bool error(char error[])
{
	cerr << "[Ошибка]: " << error << endl;
	return false;
}

void recovery_path(const vector<int>& wave, int n, int m, int cur_n, int cur_m, vector<pos>& patch)
{	
	while (true)
	{
		patch.push_back(createpos(cur_n, cur_m));
		int new_pos[4][3];
		//	вверх
		new_pos[0][0] = cur_n - 1, new_pos[0][1] = cur_m;
		if (new_pos[0][0] > 0 && wave[m * new_pos[0][0] + new_pos[0][1]])
		{
			new_pos[0][2] = wave[m * new_pos[0][0] + new_pos[0][1]];
		}
		else    new_pos[0][2] = NOT_INIT;
		//	вниз
		new_pos[1][0] = cur_n + 1, new_pos[1][1] = cur_m;
		if (new_pos[1][0] < n && wave[m * new_pos[1][0] + new_pos[1][1]])
		{
			new_pos[1][2] = wave[m * new_pos[1][0] + new_pos[1][1]];
		}
		else	new_pos[1][2] = NOT_INIT;
		//	влево
		new_pos[2][0] = cur_n, new_pos[2][1] = cur_m - 1;
		if (new_pos[2][1] > 0 && wave[m * new_pos[2][0] + new_pos[2][1]])
		{
			new_pos[2][2] = wave[m * new_pos[2][0] + new_pos[2][1]];
		}
		else	new_pos[2][2] = NOT_INIT;
		//	вправо
		new_pos[3][0] = cur_n, new_pos[3][1] = cur_m + 1;
		if (new_pos[3][1] < m && wave[m * new_pos[3][0] + new_pos[3][1]])
		{
			new_pos[3][2] = wave[m * new_pos[3][0] + new_pos[3][1]];
		}
		else	new_pos[3][2] = NOT_INIT;

		int min = 0;
		for (int i = 1; i < 4; i++)
		{
			if (0 < new_pos[i][2] && new_pos[i][2] < new_pos[min][2])
			{
				min = i;
			}
		}
		if (wave[m * cur_n + cur_m] == 1)	break;
		else cur_n = new_pos[min][0], cur_m = new_pos[min][1];
	}
}

bool wave(const vector<bool>& field, int n, int m, vector<int>& result, int cur_n, int cur_m, int finish_n, int finish_m)
{	
	int step = 1, size = 1, new_n, new_m;
	vector<pos> list(1, createpos(cur_n, cur_m));
	vector<pos>::iterator cur = list.begin();
	bool is_finish = false;
	while (cur < list.end())
	{
		cur_n = (*cur).pos_n, cur_m = (*cur).pos_m;
		list.erase(cur);	//	удаляем текущий
		//	если не финиш
		if (cur_n != finish_n || cur_m != finish_m)
		{	
			//	вверх
			new_n = cur_n - 1, new_m = cur_m;
			if (new_n > 0 && field[m * new_n + new_m] == false && result[m * new_n + new_m] == 0)
			{
				result[m * new_n + new_m] = step + 1;
				list.push_back(createpos(new_n, new_m));
			}
			//	вниз
			new_n = cur_n + 1, new_m = cur_m;
			if (new_n < n && field[m * new_n + new_m] == false && result[m * new_n + new_m] == 0)
			{
				result[m * new_n + new_m] = step + 1;
				list.push_back(createpos(new_n, new_m));
			}	
			//	влево
			new_n = cur_n, new_m = cur_m - 1;
			if (new_m > 0 && field[m * new_n + new_m] == false && result[m * new_n + new_m] == 0)
			{
				result[m * new_n + new_m] = step + 1;
				list.push_back(createpos(new_n, new_m));
			}	
			//	вправо
			new_n = cur_n, new_m = cur_m + 1;
			if (new_m < m && field[m * new_n + new_m] == false && result[m * new_n + new_m] == 0)
			{
				result[m * new_n + new_m] = step + 1;
				list.push_back(createpos(new_n, new_m));
			}
		}
		else	is_finish = true;
		//	если закончилась прошлая волна	
		if (--size == 0)
		{
			step++;
			size = list.size();
		}
		cur = list.begin();
	}
	return is_finish;
}

bool patch(const vector<bool>& field, int n, int m, vector<pos>& patch, int start_n, int start_m, int finish_n, int finish_m)
{
	vector<int> result(n * m);
	result.at(m * start_n + start_m) = 1;
	//	пускаем волну которая заполняет кол-во шагов
	if (wave(field, n, m, result, start_n, start_m, finish_n, finish_m))
	{	
		//	пускаем волну обратно, которая соберет путь
		vector<pos> buf_patch;
		recovery_path(result, n, m, finish_n, finish_m, buf_patch);

		for (vector<pos>::iterator cur = buf_patch.end() - 1; cur > buf_patch.begin(); cur--)
		{
			patch.push_back(*cur);
		}
		patch.push_back(*(buf_patch.begin()));
		return true;
	}
	return false;
}

void comb(vector<s_bonus>& bonus, vector<s_bonus*> visited, const vector<bool>& field, int n, int m, vector<pos>& curpatch, int& points, int start_n, int start_m, int finish_n, int finish_m, int count)
{
	if (count-- == 0)
	{
		if (patch(field, n, m, curpatch, start_n, start_m, finish_n, finish_m))
		{
			points += 200 - curpatch.size();
		}
	}
	else
	{
		vector<s_bonus*>::iterator it;
		int buf = (points == NOT_INIT ? 0 : points);
		vector<pos> buf_patch = curpatch;
		for (vector<s_bonus>::iterator curbonus = bonus.begin(); curbonus < bonus.end(); curbonus++)
		{
			it = find(visited.begin(), visited.end(), &(*curbonus));
			if (it != visited.end())	continue;

			s_comb c = ccomb(buf_patch, buf, &(*curbonus));
			if (patch(field, n, m, c.patch, start_n, start_m, (*curbonus).pos_n, (*curbonus).pos_m))
			{
				vector<s_bonus*> new_visited = visited;
				new_visited.push_back(&(*curbonus));
				c.points += (*curbonus).cost;
				
				comb(bonus, new_visited, field, n, m, c.patch, c.points, (*curbonus).pos_n, (*curbonus).pos_m, finish_n, finish_m, count);

				if (points == NOT_INIT || c.points > points)
				{
					points = c.points;
					curpatch = c.patch;
				}
			}
			else continue;
		}
	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "russian");
	if (argc < 2 || argv[1] == NULL)
		return error("Не задано имя файла");
	ifstream F(argv[1], ios::in);
	if (F)
	{
		vector<bool> field;
		vector<s_bonus> bonus;
		int n = 0, m = 0,
			start_n = 0, start_m = 0,
			finish_n = 0, finish_m = 0;
		char ch;
		for (int i = 0; (ch = (char)F.get()) != EOF; i++)
		{
			if (ch == '#')			field.push_back(true);
			else if (ch == '\n')
			{
				if (n == 0)	m = i;
				else if (i != m || field[n * m + i - 1] == false)
				{
					return error("Неверный формат лабиринта");
				}
				i = -1, n++;
			}
			else
			{
				if (n == 0 || i == 0 || i >= m)
				{
					return error("Неверный формат лабиринта");
				}
				if (ch >= '1' && ch <= '9')
				{
					bonus.push_back(create(n, i, atoi(&ch) * 10));
				}		
				else if (ch == '*')	start_n = n, start_m = i;
				else if (ch == 'e')	finish_n = n, finish_m = i;
				field.push_back(false);
			}
		}
		for (int i = n * m; i < n * m + m; i++)
		{
			if (field[i] == false)
			{
				return error("Неверный формат лабиринта");
			}
		}
		if (start_m == 0)	return error("В лабиринте не задан старт");
		if (finish_m == 0)	return error("В лабиринте не задан финиш");

		vector<pos> best_patch;
		if (patch(field, n, m, best_patch, start_n, start_m, finish_n, finish_m))
		{
			int points = 200 - best_patch.size();

			vector<pos> cur_patch;
			int cur_points = NOT_INIT;
			//	кобинируем
			for (int i = 1, s = (int)bonus.size(); i <= s; i++)
			{	
				vector<s_bonus*> visited;
				comb(bonus, visited, field, n, m, cur_patch, cur_points, start_n, start_m, finish_n, finish_m, i);
				//	если этот путь лучше прошлого
				if (cur_points != NOT_INIT && cur_points > points)
				{
					points = cur_points;
					best_patch = cur_patch;
				}
				cur_patch.clear();
				cur_points = NOT_INIT;
			}

			//	Выводим конечный путь
			cout << "Лучшие очки: " << ((float)points / 10.0) << endl;
			for (vector<pos>::iterator cur = best_patch.begin(); cur < best_patch.end() - 1; cur++)
			{
				if ((*cur).pos_n > (*(cur + 1)).pos_n)		cout << "u";
				else if ((*cur).pos_n < (*(cur + 1)).pos_n)	cout << "d";
				else if ((*cur).pos_m > (*(cur + 1)).pos_m)	cout << "l";
				else if ((*cur).pos_m < (*(cur + 1)).pos_m)	cout << "r";
			}
			cout << endl;
		}
		else 
			return error("Пути не существует.");
	}
	else cout << "Файл с лабиринтом не существует" << endl;
	return false;
}
