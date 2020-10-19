#include<graphics.h>
#include<conio.h>
#include<stack>
#include<memory.h>
#include<windows.h>
#include<vector>
#include<random>
#include<ctime>
#include<cstring>
#include<tchar.h>
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
bool foodcheck(int,int,int);
bool potatocheck(int,int,int);
void placemenu();

int score = 0;
int spx, spy;
int mpx = 1, mpy = 1;
int foodx[5]; int foody[5];
int potatox[10]; int potatoy[10];
int grassx[30]; int grassy[30];
bool map[101][81];
int headdirect = 1;
IMAGE wall;
IMAGE snake;
IMAGE food;
IMAGE potato;
IMAGE grass;
IMAGE display(cx, cy);
IMAGE st1; IMAGE st2;
//链表如下

class position {
public:
	int x; int y;
};

class node {
public:
	position p;
	node* next = NULL;
};

class list {
public:

	node* head = NULL;
	int length = 1;
	void listinit(node* n) {
		length = 1;
		n->next = NULL;
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
		if (head->next == NULL) {
			length = 0;
			return;
		}
		node* d = head;
		node* prev = NULL;
		while (d->next != NULL) {
			prev = d;
			d = d->next;
		}
		delete d;
		length--;
		if (prev != NULL) prev->next = NULL;
	}

	void popp() {
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
		popp();
	}

	void increase(node* in) {
		in->next = head;
		head = in;
		length++;
		score++;
	}

	bool check() {
		int tempx, tempy;
		tempx = head->p.x;
		tempy = head->p.y;
		node* temp = head->next;
		while (temp != NULL) {
			if (tempx == temp->p.x && tempy == temp->p.y) return true;
			temp = temp->next;
		}
		return false;
	}
};

//链表如上


list snakep;




int main() {
	placemenu();
	return 0;
}

void placemenu() {
	MOUSEMSG m;
	loadimage(&st1, _T("start1.png"));
	closegraph();
	initgraph(500, 400);
	int i = 0;
	freopen("player.txt","r",stdin);
	int sc[3];
	for (int i = 0; i < 3; i++) {
		scanf("%d", &sc[i]);
	}
	outtextxy(400, 200, _T("排行榜"));
	for (int i = 0; i < 3; i++) {
		TCHAR s[50] = {};
		swprintf(s, L"NO%d:%d", i + 1, sc[i]);
		outtextxy(400, 200 + 50 * (i+1), s);
	}
	putimage(113, 179, &st1);
	settextstyle(50, 0, _T("Consolas"));
	outtextxy(180, 100, _T("贪吃蛇"));
	while (true) {
		m = GetMouseMsg();
		if (m.x > 113 && m.x < 387 && m.y>179 && m.y < 221) {
			Sleep(20);
			if (m.uMsg== WM_LBUTTONDOWN) {
				closegraph();
				srand((unsigned int)time(NULL));
				init();
				start();
				closegraph();
				initgraph(500, 400);
				putimage(113, 179, &st1);
				outtextxy(400, 200, _T("排行榜"));
				freopen("player.txt", "r", stdin);
				int sc[3];
				for (int i = 0; i < 3; i++) {
					scanf("%d", &sc[i]);
				}
				for (int i = 0; i < 3; i++) {
					TCHAR s[50] = {};
					swprintf(s, L"NO%d:%d", i + 1, sc[i]);
					outtextxy(400, 200 + 50 * (i + 1), s);
				}
				settextstyle(50, 0, _T("Consolas"));
				outtextxy(180, 100, _T("贪吃蛇"));
			}
		}
	}
}

void read() {
	memset(map, 0, sizeof(map));
	freopen("level1.txt","r",stdin);
	int tempx,tempy;
	loadimage(&wall, _T("wall.bmp"));
	loadimage(&snake, _T("snake.png"));
	loadimage(&food, _T("food.png"));
	loadimage(&potato, _T("potato.png"));
	loadimage(&grass, _T("grass.png"));
	while (scanf("%d %d ", &tempx, &tempy) != EOF) {
		map[tempx][tempy] = true;
	}
	;
}

void refresh()
{
	display = IMAGE(cx, cy);
	SetWorkingImage(&display);
	for (int i = 0; i < 5; i++) {
		spx = foodx[i] - mpx + 25;
		spy = foody[i] - mpy + 20;
		if (spx >= 0 && spx <= 50 && spy >= 0 && spy <= 40) {
			trans(foodx[i], foody[i]);
			putimage(spx, spy, &food);
		}
	}
	for (int i = 0; i < 10; i++) {
		spx = potatox[i] - mpx + 25;
		spy = potatoy[i] - mpy + 20;
		if (spx >= 0 && spx <= 50 && spy >= 0 && spy <= 40) {
			trans(potatox[i], potatoy[i]);
			putimage(spx, spy, &potato);
		}
	}
	for (int i = 0; i < 30; i++) {
		spx = grassx[i] - mpx + 25;
		spy = grassy[i] - mpy + 20;
		if (spx >= 0 && spx <= 50 && spy >= 0 && spy <= 40) {
			trans(grassx[i], grassy[i]);
			putimage(spx, spy, &grass);
		}
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
	drawlittlemap();
	SetWorkingImage();
	cleardevice();
	putimage(0, 0, &display);
}

void init()
{
	mpx = 10;
	mpy = 10;
	score = 0;
	memset(map,0,sizeof(map));
}

void drawlittlemap()
{	

	int tempx;
	int tempy;
	//x-204 y-184 cx-1320 block-2*2
	const int x = 1020+40;
	const int y = 0 + 40;
	setlinecolor(WHITE);
	line(1021, 0,1021, cy);
	line(1022, 0, 1022, cy);
	line(1023, 0, 1023, cy);
	line(1024, 0, 1024, cy);
	line(x, y, x + 204, y);	line(x, y+1, x + 204, y+1);
	line(x, y+164, x + 204, y+164); line(x, y+163, x + 204, y+163);
	line(x, y, x, y+164);	line(x+1, y , x+1 , y + 164);
	line(x+204 , y , x + 204, y+164);	line(x+203, y , x + 203, y + 164);
	for (int z = 0; z < 5; z++) {
		tempx = x + (foodx[z] + 1) * 2; tempy = y + (foody[z] + 1) * 2;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				putpixel(tempx + i, tempy + j, GREEN);
			}
		}
	}

	for (int z = 0; z < 30; z++) {
		tempx = x + (grassx[z] + 1) * 2; tempy = y + (grassy[z] + 1) * 2;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				putpixel(tempx + i, tempy + j, BROWN);
			}
		}
	}
	
	for (int z = 0; z < 10; z++) {
		tempx = x + (potatox[z] + 1) * 2; tempy = y + (potatoy[z] + 1) * 2;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				putpixel(tempx + i, tempy + j, WHITE);
			}
		}
	}
	for (int i = 0; i < 100;i++) {
		for (int j = 0; j < 80; j++) {
			if (map[i][j]) {
				tempx = x + (i + 1) * 2;
				tempy = y+(j + 1)*2;
				putpixel(tempx, tempy, RED);putpixel(tempx+1, tempy, RED); putpixel(tempx, tempy+1, RED); putpixel(tempx+1, tempy+1, RED);
			}
		}
	}
	node* temp = snakep.head;
	while (temp != NULL) {
		tempx = temp->p.x; tempx = x + (tempx + 1) * 2;
		tempy = temp->p.y; tempy = y + (tempy + 1) * 2;
		putpixel(tempx, tempy, BLUE); putpixel(tempx + 1, tempy, BLUE); putpixel(tempx, tempy + 1, BLUE); putpixel(tempx + 1, tempy + 1, BLUE);
		temp = temp->next;
	}
	settextstyle(50, 0, _T("Consolas"),0.1,0.1,0,true,0,0);
	TCHAR s[50] = {};
	swprintf(s, L"Score:%d", score);
	outtextxy(x, 300, s);
}


bool grasscheck(int x, int y, int control) {
	for (int i = 0; i < 30; i++) {
		if (grassx[i] == x && grassy[i] == y) {
			if (control == 1) {
				do {
					grassx[i] = rand() % 100;
					grassy[i] = rand() % 80;
				} while (map[grassx[i]][grassy[i]] || potatocheck(grassx[i], grassy[i], 0)|| foodcheck(grassx[i], grassy[i], 0));
			}
			return true;
		}

	}
	return false;
}

bool foodcheck(int x,int y,int control) {
	for (int i = 0; i < 5; i++) {
		if (foodx[i] == x && foody[i] == y) {
			if (control == 1) {
				do {
					foodx[i] = rand() % 100;
					foody[i] = rand() % 80;
				} while (map[foodx[i]][foody[i]]||potatocheck(foodx[i], foody[i], 0) || grasscheck(foodx[i], foody[i], 0));
			}
			return true;
		}
			
	}
	return false;
}

bool potatocheck(int x, int y, int control) {
	for (int i = 0; i < 10; i++) {
		if (potatox[i] == x && potatoy[i] == y) {
			if (control == 1) {
				do {
					potatox[i] = rand() % 100;
					potatoy[i] = rand() % 80;
				} while (map[potatox[i]][potatoy[i]]||foodcheck(potatox[i],potatoy[i],0) || grasscheck(potatox[i], potatoy[i], 0));
			}
			return true;
		}

	}
	return false;
}

inline void start()
{
	initgraph(cx, cy);
	read();
	node* temp = new node;
	temp->p.x = mpx;
	temp->p.y = mpy;
	snakep.listinit(temp);
	int tempx; int tempy;
	for (int i = 0; i < 5; i++) {
		tempx = rand() % 100;
		tempy = rand() % 80;
		if (map[tempx][tempy] || foodcheck(tempx, tempy, 0||grasscheck(tempx,tempy,0))){
			i--; continue;
		}
		else { foodx[i] = tempx; foody[i] = tempy; }

	}
	for (int i = 0; i < 10; i++) {
		tempx = rand() % 100;
		tempy = rand() % 80;
		if (map[tempx][tempy] || potatocheck(tempx, tempy, 0)||grasscheck(tempx,tempy,0)) {
			i--; continue;
		}
		else { potatox[i] = tempx; potatoy[i] = tempy; }

	}
	for (int i = 0; i < 30; i++) {
		tempx = rand() % 100;
		tempy = rand() % 80;
		if (map[tempx][tempy] || potatocheck(tempx, tempy, 0)||grasscheck(tempx,tempy,0)) {
			i--; continue;
		}
		else { grassx[i] = tempx; grassy[i] = tempy; }

	}
	while (1==1) {
		
		refresh();
		Sleep(50);
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
	if (potatocheck(mpx, mpy, 1))
		snakep.delhalf();
	else if (grasscheck(mpx, mpy, 1)) 
		snakep.pop();
	if (!foodcheck(mpx,mpy,1)) 
		snakep.update(temp);
	else 
		snakep.increase(temp);
}

inline bool gameendcheck()
{	
	if (snakep.check() || snakep.head->p.x < 0 || snakep.head->p.x > 100 || snakep.head->p.y < 0 || snakep.head->p.y > 80 || map[snakep.head->p.x][snakep.head->p.y] || snakep.length == 0) {
		int s[3]; int temp;
		fclose(stdin);
		fclose(stdout);
		freopen("player.txt", "r", stdin);
		for (int i = 0; i < 3; i++) {
			scanf("%d", &s[i]);
		}
		fclose(stdin);
		freopen("player.txt", "w", stdout);
		for (int i = 0; i < 3; i++) {
			if (score > s[i]) {
				temp = i;
				while (i < 3)
				{
					s[i+1] = s[i];
					i++;
				}
				s[temp] = score;
			}	
		}
		for (int i = 0; i < 3; i++) {
			printf("%d\n", s[i]);
		}
		fclose(stdout);
		return true;
	}
	return false;
}

inline void trans(int lpx,int lpy) {
	//
	spx = lpx - mpx + 25;
	spy = lpy - mpy + 20;
	spx = spx * 20;
	spy = spy * 20;
	
}
