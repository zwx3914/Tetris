#include "Tetris.h"
#include <windows.h>

using std::cout;

bool game_flag;					//游戏标志
int Canvas[HIGHT][WIDTH] = {0};	//画布初始化
Tetris currentTetris;			//当前方块
Tetris nextTetris;				//下一块方块
bool down_flag;					//能否下移
bool left_flag;					//能否左移
bool right_flag;				//能否右移
bool deformation_flag;			//能否变形

/**
*函数定义
*/
void HideCursor(int n)  			//设置是否隐藏光标
{   
	n %= 2;
	CONSOLE_CURSOR_INFO cursor_info={1,n};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info); 
}

void gotoxy(int x, int y)			//设置光标位置
{
	COORD r;
	r.X = x;
	r.Y = y;
	
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleCursorPosition(h, r);
}

void setColor(int color)			//设置文本颜色
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(h, color);
}

void initialized() 					//初始化数据
{
	system("title 俄罗斯方块");
	system("mode con cols=40 lines=40");
	HideCursor(0);
	
	game_flag = true;
	
	//初始化边框
	gotoxy(0, 0);
	int i;
	for (i = 0; i < HIGHT; i++)
	{
		gotoxy(0,i);
		cout << "│";
		gotoxy(WIDTH*2+2, i);
		cout << "│";
	}
	gotoxy(0,i);
	cout << " ";
	for (int j = 0; j < WIDTH+1; j++)
		cout<< "─";

	
	
	down_flag = true;
	left_flag = true;
	right_flag = true;
	deformation_flag = true;
	
	currentTetris = createTetris();
	nextTetris = createTetris();
}

void show() 						//显示画面
{
	currentTetris.show();
	nextTetris.show(3, 5+HIGHT);
	
	
	for (int i = 0; i < 4; i++)
	{
		//显示四小方块的落点
		gotoxy(currentTetris.pos[i].x*2, HIGHT+1);
		cout << "─";
		//显示四小方块的坐标
		gotoxy(0,HIGHT+5+i);
		cout << "x" << i << ": ";
		cout.width(2);
		cout << currentTetris.pos[i].x;
		cout << "\ty" << i << ": ";
		cout.width(2);
		cout << currentTetris.pos[i].y;
		
		
	}
	
	
}

void updateWithoutInput() 			//与用户无关的输入
{	
	
	if (down_flag = isDown(currentTetris))
	{
		currentTetris.clear();
		currentTetris.move(DOWN);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (currentTetris.pos[i].y < 0)
				continue;
			Canvas[currentTetris.pos[i].y][currentTetris.pos[i].x -1] = currentTetris.kind+1;
			
			//擦除四小方块的落点
			gotoxy(currentTetris.pos[i].x*2, HIGHT+1);
			cout << "  ";
		}
		nextTetris.clear(3, HIGHT+5);
		currentTetris = nextTetris;
		nextTetris = createTetris();
	}
	
	//检测消行和游戏结束
	int count = 0;		//一行有几个方块
	for (int i = HIGHT - 1; i > 0; i--)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			if (Canvas[i][j] != 0)
			{
				count++;
			}
		}
		if (count == WIDTH)
		{
			
			//下移
			for (int m = i; m > 0; m--)
			{
				for (int n = 0; n < WIDTH; n++)
				{
					Canvas[m][n] = Canvas[m-1][n];
					
					setColor(Canvas[m][n]);
					gotoxy(n*2+2, m);
					if (Canvas[m][n] == 0)
						cout << "  ";
					else
						cout << "■";
					
					setColor(7);
				}
			}
			for (int k = 0; k < WIDTH; k++)
			{
				Canvas[0][k] = 0;
				gotoxy(k*2+2, 0);
				cout << "  ";
			}
			i++;		//因为下移
		}
		
		if(count == 0)
		{
			break;
		}
		
		count = 0;		//重置
	}
	
	for(int j = 0; j < WIDTH; j++)
	{
		if (Canvas[0][j] != 0)
		{
			count++;
		}
	}
	if (count > 0)
	{
		gotoxy(WIDTH-5,HIGHT/2);
		cout << "game over!";
		Sleep(3000);
		game_flag = false;
	}
	
}

void updateWithInput() 				//与用户有关的输入
{
	currentTetris.clear();
	for (int i = 0; i < 4; i++)
	{
		//擦除四小方块的落点
		gotoxy(currentTetris.pos[i].x*2, HIGHT+1);
		cout << "  ";
	}	
	
	if (GetAsyncKeyState(VK_UP)& 0x8000 && (deformation_flag = isDeformation(currentTetris)) )
	{	
		currentTetris.deformation();
	}	
	else if (GetAsyncKeyState(VK_LEFT)& 0x8000 && (left_flag = isLeft(currentTetris)) )
	{
        currentTetris.move(LEFT);
	}
	else if (GetAsyncKeyState(VK_RIGHT)& 0x8000 && (right_flag = isRight(currentTetris)) )
	{
		currentTetris.move(RIGHT);
	}
	else if (GetAsyncKeyState(VK_DOWN)& 0x8000 && (down_flag = isDown(currentTetris)) ) 
	{
		currentTetris.move(DOWN);
	}
	else if (GetAsyncKeyState(VK_SPACE)& 0x8000)
	{
		while (down_flag = isDown(currentTetris))
		{
			currentTetris.move(DOWN); 
		}
	}
}

bool isDown(Tetris & t) 			//判断能否下移
{
	for (int i = 0; i < 4; i++)
	{
		if(t.pos[i].y < 0)
			continue;
		
		if (t.pos[i].y == HIGHT -1)
		{
			return false;
		}
		
		if (Canvas[t.pos[i].y+1][t.pos[i].x-1] != 0)
		{
			return false;
		}
		
	}
	
	return true;
}

bool isLeft(Tetris & t) 			//判断能否左移
{
	for (int i = 0; i < 4; i++)
	{
		if(t.pos[i].y < 0)
			continue;
		
		if (t.pos[i].x == 1)
		{
			return false;
		}
		
		if (Canvas[t.pos[i].y][t.pos[i].x-1 - 1] != 0)
		{
			return false;
		}
		
	}
	
	return true;
}

bool isRight(Tetris & t) 			//判断能否右移
{
	for (int i = 0; i < 4; i++)
	{
		if(t.pos[i].y < 0)
			continue;
		if (t.pos[i].x == WIDTH-1 + 1)
		{
			return false;
		}
		if (Canvas[t.pos[i].y][t.pos[i].x-1 + 1] != 0)
		{
			return false;
		}
		
	}
	
	return true;
}

bool isDeformation(Tetris & t) 		//判断能否变形
{
	Tetris temp = t;
	temp.deformation();
	
	for (int i = 0; i < 4; i++)
	{
		if(temp.pos[i].y < 0)
			continue;
		if (temp.pos[i].x > WIDTH)
		{
			return false;
		}
		if (temp.pos[i].x < 1)
		{
			return false;
		}
		if (Canvas[temp.pos[i].y][temp.pos[i].x -1] != 0)
		{
			return false;
		}
		
	}
	return true;
}

Tetris createTetris() 				//生产随机方块
{
	std::srand((int)std::time(0));
	//随机生产第n种方块
	Tetris temp(std::rand() % 7);
	
	//随机变形n次
	for (int i = 0; i < std::rand() % 3; i++)
	{
		temp.deformation();
	}
	
	//移动方块的最低点到-1
	int h = temp.pos[0].y;
	for (int i = 1; i < 4; i++)
	{
		if (h > temp.pos[i].y)
		{
			h = temp.pos[i].y;
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		temp.pos[i].y += -1 - h;
	}
	
	return temp;
}



/**
Tetris类
*/
const position Tetris::style[7][4] = {
		/*
			□ □
			□ □
		*/
		{
			{WIDTH/2, -2},
			{WIDTH/2+1, -2},
			{WIDTH/2, -1},
			{WIDTH/2+1, -1}
		},
		/**
			□ □
			□
			□
		*/
		{
			{WIDTH/2, -3},
			{WIDTH/2+1, -3},
			{WIDTH/2, -2},
			{WIDTH/2, -1}
		},
		/**
			□ □
			  □
			  □
		*/
		{
			{WIDTH/2, -3},
			{WIDTH/2+1, -3},
			{WIDTH/2+1, -2},
			{WIDTH/2+1, -1}
		},
		/**
			□
			□ □
			□
		*/
		{
			{WIDTH/2, -3},
			{WIDTH/2+1, -2},
			{WIDTH/2, -2},
			{WIDTH/2, -1}
		},	
		/**
			□
			□ □
			  □
		*/
		{
			{WIDTH/2, -3},
			{WIDTH/2+1, -2},
			{WIDTH/2, -2},
			{WIDTH/2+1, -1}
		},
		/**
			  □
			□ □
			□
		*/
		{
			{WIDTH/2+1, -3},
			{WIDTH/2+1, -2},
			{WIDTH/2, -2},
			{WIDTH/2, -1}
		}, 
		/**
			□
			□ 
			□
			□
		*/
		{
			{WIDTH/2, -4},
			{WIDTH/2, -3},
			{WIDTH/2, -2},
			{WIDTH/2, -1}
		}
	};

Tetris::Tetris()					//默认构造函数
{
	kind = 0;
	for (int i = 0; i < 4; i++)
	{
		pos[i] = style[kind][i];
	}
}
Tetris::Tetris(int n) : kind(n%7)
{
	for (int i = 0; i < 4; i++)
	{
		pos[i] = style[kind][i];
	}
}
	
void Tetris::deformation()			//方块变形，逆时针旋转
{
	/*
		□ □
		□ □	不用变形
	*/
	if (kind == 0)
	{
		return;
	}
	
	for (int i = 1; i < 4; i++)
	{
		position temp = pos[i];
		pos[i].x = pos[0].x + (temp.y - pos[0].y);
		pos[i].y = pos[0].y - (temp.x - pos[0].x);
		
	}
}

void Tetris::move(direction d)				//方块移动
{
	switch (d)
	{
		case DOWN:
			for (int i = 0; i < 4; i++)
			{
				pos[i].y++;
			}
			break;
		case LEFT:
			for (int i = 0; i < 4; i++)
			{
				pos[i].x--;
			}
			break;
		case RIGHT:
			for (int i = 0; i < 4; i++)
			{
				pos[i].x++;
			}
			break;
	}
}

void Tetris::show(int a, int b) 			//打印方块
{
	setColor(kind+1);
	for (int i = 0; i < 4; i++)
	{
		if (pos[i].y +b >= 0)
		{
			gotoxy(pos[i].x * 2 + a, pos[i].y + b);
			cout << "■";
		}
	}
	setColor(7);
}
void Tetris::clear(int a, int b) 			//擦除方块
{
	for (int i = 0; i < 4; i++)
	{
		gotoxy(pos[i].x * 2 + a, pos[i].y + b);
		cout << "  ";
	}
}