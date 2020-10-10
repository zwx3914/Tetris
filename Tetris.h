#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <ctime>

//定义地图大小
#define WIDTH	18			//定义地图宽度
#define HIGHT	30			//定义地图高度

enum direction {DOWN, LEFT, RIGHT};
struct position				//C++不用像c一样 "typedef struct position{} Position;"
{
	int x;
	int y;
};


class Tetris
{
public:
	position pos[4];				//四个小方块的坐标
	int kind = 0;					//共有7种方块，C++11可以类内初始化
	static const position style[7][4];
	
	Tetris();						//默认构造函数
	Tetris(int n);
	Tetris(const Tetris & t) = default;
	~Tetris() = default;			//析构函数
	
	void deformation();				//方块变形
	void move(direction d);			//方块移动
	void show(int a = 0, int b = 0);	//打印方块
	void clear(int a = 0, int b = 0);	//擦除方块
};
extern bool game_flag;				//游戏标志
extern int Canvas[HIGHT][WIDTH];	//画布
extern Tetris currentTetris;		//当前方块
extern Tetris nextTetris;			//下一块方块
extern bool down_flag;				//能否下移
extern bool left_flag;				//能否左移
extern bool right_flag;				//能否右移
extern bool deformation_flag;		//能否变形
//extern int speed;					//方块下落速度

void HideCursor(int n);  			//设置是否隐藏光标
void gotoxy(int x, int y);			//设置光标位置
void setColor(int color);			//设置文本颜色

void initialized();					//初始化数据
void show();						//显示画面
void updateWithoutInput();			//与用户无关的输入
void updateWithInput();				//与用户有关的输入

bool isDown(Tetris & t);			//判断能否下移
bool isLeft(Tetris & t);			//判断能否左移
bool isRight(Tetris & t);			//判断能否右移
bool isDeformation(Tetris & t);		//判断能否变形
Tetris createTetris();				//生产随机方块

#endif