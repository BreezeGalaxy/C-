#include <graphics.h>
#include <vector>
using namespace std;
#define margin 30
#define bwidth 35

//Global Variables
int mode = IDNO;
int turn;

class Chess {
public:
	int x = 0;
	int y = 0;
	int type = 0;
	int score = 0;
	bool mark = true;
	void draw();
};
Chess chess[15][15];

void initGame()
{
	loadimage(NULL, _T("bg.jpg"));
	settextstyle(30, 0, _T("宋体"));
	setcolor(WHITE);
	outtextxy(600, 0, _T("ESC退出游戏"));
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 14; j++)
		{
			int x = margin + i * bwidth, y = margin + j * bwidth;
			rectangle(x, y, x + bwidth, y + bwidth);
		}
	for(int i=0; i<15; i++)
		for (int j = 0; j < 15; j++)
		{
			chess[i][j].x = i, chess[i][j].y = j;
			chess[i][j].type = 0;
			chess[i][j].mark = true;
			chess[i][j].score = 0;
		}
	HWND hWnd = GetHWnd();
	mode = MessageBox(hWnd, L"要和电脑挑战吗？否则和玩家对战", L"模式选择", MB_YESNO | MB_ICONASTERISK);
}

void Chess::draw()
{
	if (type == 1)
	{
		int xx = margin + x * bwidth, yy = margin + y * bwidth;
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(xx, yy, 16);
		if (mark)
		{
			setcolor(RED);
			line(xx - 5, yy, xx + 5, yy);
			line(xx, yy - 5, xx, yy + 5);
			mark = false;
		}
	}
	if (type == 2)
	{
		int xx = margin + x * bwidth, yy = margin + y * bwidth;
		setcolor(WHITE);
		setfillcolor(WHITE);
		fillcircle(xx, yy, 16);
		if (mark)
		{
			setcolor(RED);
			line(xx - 5, yy, xx + 5, yy);
			line(xx, yy - 5, xx, yy + 5);
			mark = false;
		}
	}
}

int CheckClick(int x, int y, int type)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			int xx = margin + i * bwidth, yy = margin + j * bwidth;
			if ((xx - x) * (xx - x) < 177 && (yy - y) * (yy - y) < 177 && chess[i][j].type == 0)
			{
				chess[i][j].type = type;
				turn++;
				return 1;
			}
		}
	return 0;
}
int judgeWin()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[i][j].type * chess[i][j + 1].type * chess[i][j + 2].type * chess[i][j + 3].type * chess[i][j + 4].type;
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 0; i < 15; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[j][i].type * chess[j + 1][i].type * chess[j + 2][i].type * chess[j + 3][i].type * chess[j + 4][i].type;
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 0; i <= 10; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[i][j].type * chess[i + 1][j + 1].type * chess[i + 2][j + 2].type * chess[i + 3][j + 3].type * chess[i + 4][j + 4].type;
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 4; i < 15; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[i][j].type * chess[i - 1][j + 1].type * chess[i - 2][j + 2].type * chess[i - 3][j + 3].type * chess[i - 4][j + 4].type;
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}

	return 0;
}
void restartGame()
{
	turn = 0;
	cleardevice();
	initGame();
}
//
int lineScore(int num, int emp)//输入连子数和两边空余数得出一个分数
{
	int ans = 0;
	if (num >= 5)
	{
		ans = 100000;
	}
	if (num == 4)
	{
		switch (emp)
		{
		case 0:
			ans = 7; break; //dead four -_-
		case 1:
			ans = 7777; break;
		case 2:
			ans = 77777; break;
		}
	}
	if (num == 3)
	{
		switch (emp)
		{
		case 0:
			ans = 1; break; //dead three -_-
		case 1:
			ans = 77; break;
		case 2:
			ans = 7777; break;
		}
	}
	if (num == 2)
	{
		switch (emp)
		{
		case 0:
			ans = -7; break; //dead two -_-
		case 1:
			ans = 0; break;
		case 2:
			ans = 77; break;
		}
	}
	if (num == 1) ans = 0;
	return ans;
}
bool isValid(int a)
{
	return (a >= 0 && a < 15);
}
void calculateScore()//直接将双方分数相加判断空点分数
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chess[i][j].type == 0)
			{
				// x y xy yx

				//x轴方向
				int hxemp = 0;
				int hlx = i - 1; int hrx = i + 1;
				while (isValid(hrx) && hrx != i + 5 && chess[hrx][j].type == 1) hrx++;
				while (isValid(hlx) && hlx != i - 5 && chess[hlx][j].type == 1) hlx--;
				if (isValid(hrx) && chess[hrx][j].type == 0)hxemp++;
				if (isValid(hlx) && chess[hlx][j].type == 0)hxemp++;
				int hxlen = hrx - hlx - 1;
				chess[i][j].score += lineScore(hxlen, hxemp);
				//y轴方向
				int hyemp = 0;
				int hly = j + 1; int hry = j - 1;
				while (isValid(hly) && hly != j + 5 && chess[i][hly].type == 1) hly++;
				while (isValid(hry) && hry != j - 5 && chess[i][hry].type == 1) hry--;
				if (isValid(hly) && chess[i][hly].type == 0)hyemp++;
				if (isValid(hry) && chess[i][hry].type == 0)hyemp++;
				int hylen = hly - hry - 1;
				chess[i][j].score += lineScore(hylen, hyemp);
				//xy方向
				int hxyemp = 0;
				int hlxy = -1; int hrxy = 1;
				while (isValid(i + hrxy) && isValid(j + hrxy) && hrxy != 5 && chess[i + hrxy][j + hrxy].type == 1) hrxy++;
				while (isValid(i + hlxy) && isValid(j + hlxy) && hlxy != -5 && chess[i + hlxy][j + hlxy].type == 1) hlxy--;
				if (isValid(i + hrxy) && isValid(j + hrxy) && chess[i + hrxy][j + hrxy].type == 0) hxyemp++;
				if (isValid(i + hlxy) && isValid(j + hlxy) && chess[i + hlxy][j + hlxy].type == 0) hxyemp++;
				int hxylen = hrxy - hlxy - 1;
				chess[i][j].score += lineScore(hxylen, hxyemp);
				//yx方向
				int hyxemp = 0;
				int hlyx = -1; int hryx = 1;
				while (isValid(i + hryx) && isValid(j - hryx) && hryx != 5 && chess[i + hryx][j - hryx].type == 1) hryx++;
				while (isValid(i + hlyx) && isValid(j - hlyx) && hlyx != -5 && chess[i + hlyx][j - hlyx].type == 1) hlyx--;
				if (isValid(i + hryx) && isValid(j - hryx) && chess[i + hryx][j - hryx].type == 0) hyxemp++;
				if (isValid(i + hlyx) && isValid(j - hlyx) && chess[i + hlyx][j - hlyx].type == 0) hyxemp++;
				int hyxlen = hryx - hlyx - 1;
				chess[i][j].score += lineScore(hyxlen, hyxemp);


				//x轴方向
				int xemp = 0;
				int lx = i - 1; int rx = i + 1;
				while (isValid(rx) && rx != i + 5 && chess[rx][j].type == 2) rx++;
				while (isValid(lx) && lx != i - 5 && chess[lx][j].type == 2) lx--;
				if (isValid(rx) && chess[rx][j].type == 0)hxemp++;
				if (isValid(lx) && chess[lx][j].type == 0)hxemp++;
				int xlen = rx - lx - 1;
				chess[i][j].score += lineScore(xlen, xemp);
				//y轴方向
				int yemp = 0;
				int ly = j + 1; int ry = j - 1;
				while (isValid(ly) && ly != j + 5 && chess[i][ly].type == 2) ly++;
				while (isValid(ry) && ry != j - 5 && chess[i][ry].type == 2) ry--;
				if (isValid(ly) && chess[i][ly].type == 0) yemp++;
				if (isValid(ry) && chess[i][ry].type == 0)yemp++;
				int ylen = ly - ry - 1;
				chess[i][j].score += lineScore(ylen, yemp);
				//xy方向
				int xyemp = 0;
				int lxy = -1; int rxy = 1;
				while (isValid(i + rxy) && isValid(j + rxy) && rxy != 5 && chess[i + rxy][j + rxy].type == 2) rxy++;
				while (isValid(i + lxy) && isValid(j + lxy) && lxy != -5 && chess[i + lxy][j + lxy].type == 2) lxy--;
				if (isValid(i + rxy) && isValid(j + rxy) && chess[i + rxy][j + rxy].type == 0) xyemp++;
				if (isValid(i + lxy) && isValid(j + lxy) && chess[i + lxy][j + lxy].type == 0) xyemp++;
				int xylen = rxy - lxy - 1;
				chess[i][j].score += lineScore(xylen, xyemp);
				//yx方向
				int yxemp = 0;
				int lyx = -1; int ryx = 1;
				while (isValid(i + ryx) && isValid(j - ryx) && ryx != 5 && chess[i + ryx][j - ryx].type == 2) ryx++;
				while (isValid(i + lyx) && isValid(j - lyx) && lyx != -5 && chess[i + lyx][j - lyx].type == 2) lyx--;
				if (isValid(i + ryx) && isValid(j - ryx) && chess[i + ryx][j - ryx].type == 0) yxemp++;
				if (isValid(i + lyx) && isValid(j - lyx) && chess[i + lyx][j - lyx].type == 0) yxemp++;
				int yxlen = ryx - lyx - 1;
				chess[i][j].score += lineScore(yxlen, yxemp);

			}
			else
			{
				chess[i][j].score = 0;
			}
		}
}
vector<int> getMaxScore()
{
	vector<int> maxchess(2);
	int maxscore = 0;
	for(int i=0; i<15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chess[i][j].score >= maxscore && chess[i][j].type == 0)
			{
				maxscore = chess[i][j].score;
				maxchess[0] = i;
				maxchess[1] = j;
			}
		}
	return maxchess;
}
void aiPlay()
{
	calculateScore();
	vector<int> pos = getMaxScore();

	chess[pos[0]][pos[1]].type = 2;
	chess[pos[0]][pos[1]].draw();
	turn++;
}
//
int main()
{
	initgraph(770, 660);
	initGame();
	ExMessage msg;
	while (1)
	{
		msg = getmessage(EM_MOUSE | EM_KEY);
		switch (msg.message)
		{
		case WM_KEYDOWN:
			if (msg.vkcode == VK_ESCAPE)
				return 0;
		case WM_LBUTTONDOWN:
			if (CheckClick(msg.x, msg.y, turn % 2 + 1))
			{
				for (int i = 0; i < 15; i++)
					for (int j = 0; j < 15; j++)
						chess[i][j].draw();
				if (int state = judgeWin())
				{
					HWND hWnd = GetHWnd();
					int ret = 0;
					if (state == 1)
						ret = MessageBox(hWnd, L"黑棋赢了，要再来一局吗？", L"游戏结束", MB_YESNO | MB_ICONASTERISK);
					else
						ret = MessageBox(hWnd, L"白棋赢了，要再来一局吗？", L"游戏结束", MB_YESNO | MB_ICONASTERISK);
					if (ret == IDYES)
						restartGame();
					else
						return 0;
				}
				if (mode == IDYES && turn)
				{
					aiPlay();
					if (int state = judgeWin())
					{
						HWND hWnd = GetHWnd();
						int ret = 0;
						if (state == 1)
							ret = MessageBox(hWnd, L"黑棋赢了，要再来一局吗？", L"游戏结束", MB_YESNO | MB_ICONASTERISK);
						else
							ret = MessageBox(hWnd, L"白棋赢了，要再来一局吗？", L"游戏结束", MB_YESNO | MB_ICONASTERISK);
						if (ret == IDYES)
							restartGame();
						else
							return 0;
					}

				}
			}
			break;
		}
		

	}
}