#include<graphics.h>
#include<conio.h>
#include<stack>
#include<memory.h>
#include<windows.h>
#include<vector>
#pragma warning(disable:4996)
const int mx = 2020;
const int my = 1620;
const int cx = 1020;
const int cy = 820;
using namespace std;

inline void trans(int,int);
void read();
void refresh();
void init();
void start();
void snakemove();

class posi {
public:
	int x;
	int y;
};

vector<posi> snakep;

int mpx=25, mpy=20;
int spx,spy;
int foodx = 5, foody = 5;
bool map[100][80];
int direct=1;
IMAGE wall;
IMAGE snake;
IMAGE food;



int main() {
	init();
	start();
	getch();
}

void read() {
	memset(map, 0, sizeof(map));
	freopen("E:\\m\\level1.txt","r",stdin);
	int tempx,tempy;
	loadimage(&wall, _T("E:\\m\\wall.bmp"));
	loadimage(&snake, _T("E:\\m\\snake.png"));
	loadimage(&food, _T("E:\\m\\food.png"));
	while (scanf("%d %d ", &tempx, &tempy) != EOF) {
		map[tempx][tempy] = true;
	}
}

void refresh()
{
	cleardevice();
	trans(foodx, foody);
	putimage(spx, spy, &food);
	for (int i = mpx - 25; i <= mpx + 25; i++) {
		for (int j = mpy - 20; j <= mpy + 20; j++) {
			if (i < 0 || j < 0) {
				trans(i, j);
				putimage(spx, spy, &wall);
				continue;
			}
			if (map[i][j]) {
				trans(i, j);
				putimage(spx, spy, &wall);
			}
		}
	}
}

void init()
{
	mpx = 25;
	mpy = 20;
	snakep.clear();
	posi t;
	t.x = mpx;
	t.y = mpy;
	snakep.push_back(t);
	memset(map,0,sizeof(map));
}

void start()
{
	initgraph(cx, cy);
	read();
	refresh();
}

void snakemovechange()
{
	char c;
	if (_kbhit())	c = _getch();
	else return;
	switch (direct)
	{
	case 0:
		if (c == 'w') direct = 1;
		if (c == 's') direct = 3;
	case 1:
		if (c == 'a') direct = 0;
	default:
		break;
	}

}

inline void trans(int lpx,int lpy) {
	//
	spx = lpx - mpx + 25;
	spy = lpy - mpy + 20;
	if (spx >= 0 && spx <= 50 && spy >= 0 && spy <= 40) {
		spx = spx * 20;
		spy = spy * 20;
	}
}

