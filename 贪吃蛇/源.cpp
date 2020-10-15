#include<graphics.h>
#include<conio.h>
#include<stack>
#include<memory.h>
#include<windows.h>
#include<vector>
#pragma warning(disable:4996)
const int mx = 2020;
const int my = 1620;
const int cx = 1320;
const int cy = 820;
using namespace std;


inline void trans(int, int);
void read();
void refresh();
void init();
void drawlittlemap();
inline void start();
inline void snakedirectionchange();
void snakemove();
inline bool gameendcheck();

int spx, spy;
int mpx = 1, mpy = 1;
int foodx = 5, foody = 5;
bool map[101][81];
int headdirect = 1;
IMAGE wall;
IMAGE snake;
IMAGE food;
IMAGE display(cx, cy);

//链表如下

class position {
public:
	int x; int y;; int direction;
};

class node {
public:
	position p;
	node* next = NULL;
};

class list {
public:

	node* head = NULL;

	void listinit(node* n) {
		head = n;
		head->next = NULL;
	}

	void insert(node* temp) {
		node* d = head;
		while (d->next != NULL) {
			d = d->next;
		}
		d->next = temp;
		temp->next = NULL;
		length++;
	}

	void pop() {
		node* d = head;
		node* prev = NULL;
		while (d->next != NULL) {
			prev = d;
			d = d->next;
		}
		delete d;
		if (prev != NULL) prev->next = NULL;
	}

	void delhalf() {
		node* p = head;
		if (length == 1) {
			delete head;
			length = 0;
			return;
		}
		node* temp = NULL;
		for (int i = length / 2; i > 0; i--) {
			temp = p;
			p = p->next;
		}
		length = length / 2;
		temp->next = NULL;
		temp = p->next;
		while (temp != NULL) {
			delete p;
			p = temp;
			temp = temp->next;
		}
	}

	void output() {
		node* p = head;
		while (p != NULL) {
			trans(p->p.x, p->p.y);
			putimage(spx, spy, &snake);
			p = p->next;
		}
	}

	void update(node* in){
		in->next = head;
		head = in;
		pop();
	}

	void increase(node* in) {
		in->next = head;
		head = in;
	}

	bool check() {
		int tempx, tempy;
		tempx = head->p.x;
		tempy = head->p.y;
		node* temp = head->next;
		while (temp != NULL) {
			if (tempx == temp->p.x && tempy == temp->p.y) return true;
		}
		return false;
	}

private:
	int length = 1;
};

//链表如上


list snakep;



int main() {
	init();
	start();
	closegraph();
	return 0;
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
	display = IMAGE(cx, cy);
	SetWorkingImage(&display);
	spx = foodx - mpx + 25;
	spy = foody - mpy + 20;
	if (spx >= 0 && spx <= 50 && spy >= 0 && spy <= 40) {
		trans(foodx, foody);
		putimage(spx, spy, &food);
	}
	for (int i = mpx - 25; i <= mpx + 25; i++) {
		for (int j = mpy - 20; j <= mpy + 20; j++) {
			if (i < 0 || j < 0||i > 100||j > 80) {
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
	snakep.output();
	SetWorkingImage();
	cleardevice();
	putimage(0, 0, &display);
}

void init()
{
	mpx = 25;
	mpy = 80;
	memset(map,0,sizeof(map));
}

void drawlittlemap()
{	
	//x-220 y-180
	const int x = 1020;

}

inline void start()
{
	initgraph(cx, cy);
	read();
	node* temp = new node;
	temp->p.x = mpx;
	temp->p.y = mpy;
	snakep.listinit(temp);
	while (1==1) {
		refresh();
		drawlittlemap();
		Sleep(500);
		snakedirectionchange();
		snakemove();
		if (gameendcheck()) {
			return;
		}
	}
}

inline void snakedirectionchange()
{
	char c;
	if (_kbhit())	c = _getch();
	else return;
	switch (headdirect)
	{
	case 0: case 2:
		if (c == 'w') headdirect = 1;
		if (c == 's') headdirect = 3;
		break;
	case 1: case 3:
		if (c == 'a') headdirect = 0;
		if (c == 'd') headdirect = 2;
		break;
	default:
		break;
	}
}

void snakemove() {
	switch (headdirect)
	{
	case 0: mpx--; break;
	case 1: mpy--; break;
	case 2: mpx++; break;
	case 3: mpy++; break;
	default:
		break;
	}
	node* temp = new node;
	temp->p.x = mpx;
	temp->p.y = mpy;
	if (mpx != foodx || mpy != foody) snakep.update(temp);
	else snakep.increase(temp);
}

inline bool gameendcheck()
{	
	return (snakep.check() || snakep.head->p.x < 0 || snakep.head->p.x > 100 || snakep.head->p.y < 0 || snakep.head->p.y > 80 || map[snakep.head->p.x][snakep.head->p.y]);
}

inline void trans(int lpx,int lpy) {
	//
	spx = lpx - mpx + 25;
	spy = lpy - mpy + 20;
	spx = spx * 20;
	spy = spy * 20;
	
}
