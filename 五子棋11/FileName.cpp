#include <graphics.h>
#define margin 30
#define bwidth 35
int chess[15][15];
int turn;
int px = -1, py = -1;
int Score[15][15];
int MaxScore, Maxx, Maxy;

//...........................................................................................................
void DrawBoard()
{
	settextstyle(30, 0, _T("宋体"));
	setcolor(WHITE);
	outtextxy(400, 0, _T("ESC退出游戏"));
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 14; j++)
		{
			int x = margin + i * bwidth, y = margin + j * bwidth;
			rectangle(x, y, x + bwidth, y + bwidth);
		}
}
void DrawChess(int x, int y, int type)
{
	if (type == 1) //WHITE
	{
		int xx = margin + x * bwidth, yy = margin + y * bwidth;
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(xx, yy, 16);
	}
	if (type == 2) //BLACK
	{
		int xx = margin + x * bwidth, yy = margin + y * bwidth;
		setcolor(WHITE);
		setfillcolor(WHITE);
		fillcircle(xx, yy, 16);
	}
}
int CheckClick(int x, int y, int type)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			int xx = margin + i * bwidth, yy = margin + j * bwidth;
			if ((xx - x) * (xx - x) < 177 && (yy - y) * (yy - y) < 177 && chess[i][j] == 0)
			{
				chess[i][j] = type;
				px = i, py = j;
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
			int res = chess[i][j] * chess[i][j + 1] * chess[i][j + 2] * chess[i][j + 3] * chess[i][j + 4];
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 0; i < 15; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[j][i] * chess[j + 1][i] * chess[j + 2][i] * chess[j + 3][i] * chess[j + 4][i];
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 0; i <= 10; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[i][j] * chess[i + 1][j + 1] * chess[i + 2][j + 2] * chess[i + 3][j + 3] * chess[i + 4][j + 4];
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}
	for (int i = 4; i < 15; i++)
		for (int j = 0; j <= 10; j++)
		{
			int res = chess[i][j] * chess[i - 1][j + 1] * chess[i - 2][j + 2] * chess[i - 3][j + 3] * chess[i - 4][j + 4];
			if (res == 1)
				return 1;
			if (res == 32)
				return 2;
		}

	return 0;
}

void restartGame()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			chess[i][j] = 0;
	turn = 0;
	cleardevice();
	loadimage(NULL, _T("bg.jpg"));
	DrawBoard();
}

//......................................unsafe function
void regret()
{
	if (px != -1)
	{
		chess[px][py] = 0;
		turn--, px = -1, py = -1;
		loadimage(NULL, _T("bg.jpg"));
		DrawBoard();
	}
}
bool isValid(int a)
{
	return (a >= 0 && a < 15);
}
int lineScore(int num, int emp)
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
void calculateScore()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chess[i][j] == 0)
			{
				// x y xy yx

				//human score  变量有h前缀
				//x轴方向
				int hxemp = 0;
				int hlx = i - 1; int hrx = i + 1;
				while (isValid(hrx) && hrx != i + 5 && chess[hrx][j] == 1) hrx++;
				while (isValid(hlx) && hlx != i - 5 && chess[hlx][j] == 1) hlx--;
				if (isValid(hrx) && chess[hrx][j] == 0)hxemp++;
				if (isValid(hlx) && chess[hlx][j] == 0)hxemp++;
				int hxlen = hrx - hlx - 1;
				Score[i][j] = lineScore(hxlen, hxemp);
				//y轴方向
				int hyemp = 0;
				int hly = j + 1; int hry = j - 1;
				while (isValid(hly) && hly != j + 5 && chess[i][hly] == 1) hly++;
				while (isValid(hry) && hry != j - 5 && chess[i][hry] == 1) hry--;
				if (isValid(hly) && chess[i][hly] == 0)hyemp++;
				if (isValid(hry) && chess[i][hry] == 0)hyemp++;
				int hylen = hly - hry - 1;
				Score[i][j] += lineScore(hylen, hyemp);
				//xy方向
				int hxyemp = 0;
				int hlxy = -1; int hrxy = 1;
				while (isValid(i + hrxy) && isValid(j + hrxy) && hrxy != 5 && chess[i + hrxy][j + hrxy] == 1) hrxy++;
				while (isValid(i + hlxy) && isValid(j + hlxy) && hlxy != -5 && chess[i + hlxy][j + hlxy] == 1) hlxy--;
				if (isValid(i + hrxy) && isValid(j + hrxy) && chess[i + hrxy][j + hrxy] == 0) hxyemp++;
				if (isValid(i + hlxy) && isValid(j + hlxy) && chess[i + hlxy][j + hlxy] == 0) hxyemp++;
				int hxylen = hrxy - hlxy - 1;
				Score[i][j] += lineScore(hxylen, hxyemp);
				//yx方向
				int hyxemp = 0;
				int hlyx = -1; int hryx = 1;
				while (isValid(i + hryx) && isValid(j - hryx) && hryx != 5 && chess[i + hryx][j - hryx] == 1) hryx++;
				while (isValid(i + hlyx) && isValid(j - hlyx) && hlyx != -5 && chess[i + hlyx][j - hlyx] == 1) hlyx--;
				if (isValid(i + hryx) && isValid(j - hryx) && chess[i + hryx][j - hryx] == 0) hyxemp++;
				if (isValid(i + hlyx) && isValid(j - hlyx) && chess[i + hlyx][j - hlyx] == 0) hyxemp++;
				int hyxlen = hryx - hlyx - 1;
				Score[i][j] += lineScore(hyxlen, hyxemp);


				//ai score  无前缀
				//x轴方向
				int xemp = 0;
				int lx = i - 1; int rx = i + 1;
				while (isValid(rx) && rx != i + 5 && chess[rx][j] == 2) rx++;
				while (isValid(lx) && lx != i - 5 && chess[lx][j] == 2) lx--;
				if (isValid(rx) && chess[rx][j] == 0)hxemp++;
				if (isValid(lx) && chess[lx][j] == 0)hxemp++;
				int xlen = rx - lx - 1;
				Score[i][j] += lineScore(xlen, xemp);
				//y轴方向
				int yemp = 0;
				int ly = j + 1; int ry = j - 1;
				while (isValid(ly) && ly != j + 5 && chess[i][ly] == 2) ly++;
				while (isValid(ry) && ry != j - 5 && chess[i][ry] == 2) ry--;
				if (isValid(ly) && chess[i][ly] == 0) yemp++;
				if (isValid(ry) && chess[i][ry] == 0)yemp++;
				int ylen = ly - ry - 1;
				Score[i][j] += lineScore(ylen, yemp);
				//xy方向
				int xyemp = 0;
				int lxy = -1; int rxy = 1;
				while (isValid(i + rxy) && isValid(j + rxy) && rxy != 5 && chess[i + rxy][j + rxy] == 2) rxy++;
				while (isValid(i + lxy) && isValid(j + lxy) && lxy != -5 && chess[i + lxy][j + lxy] == 2) lxy--;
				if (isValid(i + rxy) && isValid(j + rxy) && chess[i + rxy][j + rxy] == 0) xyemp++;
				if (isValid(i + lxy) && isValid(j + lxy) && chess[i + lxy][j + lxy] == 0) xyemp++;
				int xylen = rxy - lxy - 1;
				Score[i][j] += lineScore(xylen, xyemp);
				//yx方向
				int yxemp = 0;
				int lyx = -1; int ryx = 1;
				while (isValid(i + ryx) && isValid(j - ryx) && ryx != 5 && chess[i + ryx][j - ryx] == 2) ryx++;
				while (isValid(i + lyx) && isValid(j - lyx) && lyx != -5 && chess[i + lyx][j - lyx] == 2) lyx--;
				if (isValid(i + ryx) && isValid(j - ryx) && chess[i + ryx][j - ryx] == 0) yxemp++;
				if (isValid(i + lyx) && isValid(j - lyx) && chess[i + lyx][j - lyx] == 0) yxemp++;
				int yxlen = ryx - lyx - 1;
				Score[i][j] += lineScore(yxlen, yxemp);

			}
		}
}
void getMaxScore()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chess[i][j] == 0 && Score[i][j] >= MaxScore)
			{
				MaxScore = Score[i][j];
				Maxx = i;
				Maxy = j;
			}
		}
}
void aiPlay()
{
	calculateScore();
	getMaxScore();
	chess[Maxx][Maxy] = 2; turn++;
	Score[Maxx][Maxy] = -10000;
	MaxScore = -10000;
	Maxx = -1;
	Maxy = -1;
}
//............................................................................................................
int main()
{
	initgraph(770, 660);
	loadimage(NULL, _T("bg.jpg"));
	DrawBoard();
	ExMessage m;
	while (true)
	{
		m = getmessage(EM_MOUSE | EM_KEY);
		//m.message = WM_KEYDOWN;
		switch (m.message)
		{
		case WM_LBUTTONDOWN:
			if (CheckClick(m.x, m.y, turn % 2 + 1))
				aiPlay();

			break;

		case WM_KEYDOWN:
			if (m.vkcode == VK_ESCAPE)
				return 0;
			if (m.vkcode == VK_SPACE)
				regret();
		}
		//Draw every chess
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
			{
				if (chess[i][j])
					DrawChess(i, j, chess[i][j]);
			}

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
				break;
		}
	}

}