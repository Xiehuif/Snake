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
//inline void wrin();

int mpx=25, mpy=20;
int spx,spy;
bool map[100][80];
IMAGE wall;
IMAGE snake;

class pos {
public:
	int x;
	int y;
};

vector<pos> snakep;

int main() {
	initgraph(cx, cy);
	read();
	refresh();
	getch();
}

void read() {
	memset(map, 0, sizeof(map));
	freopen("E:\\m\\level1.txt","r",stdin);
	int tempx,tempy;
	loadimage(&wall, _T("E:\\m\\wall.bmp"));
	loadimage(&snake, _T("E:\\m\\snake.png"));
	while (scanf("%d %d ", &tempx, &tempy) != EOF) {
		map[tempx][tempy] = true;
	}
}

void refresh()
{
	cleardevice();
	for (int i = mpx - 25; i <= mpx + 25; i++) {
		for (int j = mpy - 20; j <= mpy + 20; j++) {
			if (i < 0 || j < 0) continue;
			if (map[i][j]) {
				trans(i, j);
				putimage(spx, spy, &wall);
			}
		}
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
/*
inline void wrin() {
	int temp[2];
	temp[0] = spx;
	temp[1] = spy;
	p.push(temp);
}
*/