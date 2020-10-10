#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <ctime>

//�����ͼ��С
#define WIDTH	18			//�����ͼ���
#define HIGHT	30			//�����ͼ�߶�

enum direction {DOWN, LEFT, RIGHT};
struct position				//C++������cһ�� "typedef struct position{} Position;"
{
	int x;
	int y;
};


class Tetris
{
public:
	position pos[4];				//�ĸ�С���������
	int kind = 0;					//����7�ַ��飬C++11�������ڳ�ʼ��
	static const position style[7][4];
	
	Tetris();						//Ĭ�Ϲ��캯��
	Tetris(int n);
	Tetris(const Tetris & t) = default;
	~Tetris() = default;			//��������
	
	void deformation();				//�������
	void move(direction d);			//�����ƶ�
	void show(int a = 0, int b = 0);	//��ӡ����
	void clear(int a = 0, int b = 0);	//��������
};
extern bool game_flag;				//��Ϸ��־
extern int Canvas[HIGHT][WIDTH];	//����
extern Tetris currentTetris;		//��ǰ����
extern Tetris nextTetris;			//��һ�鷽��
extern bool down_flag;				//�ܷ�����
extern bool left_flag;				//�ܷ�����
extern bool right_flag;				//�ܷ�����
extern bool deformation_flag;		//�ܷ����
//extern int speed;					//���������ٶ�

void HideCursor(int n);  			//�����Ƿ����ع��
void gotoxy(int x, int y);			//���ù��λ��
void setColor(int color);			//�����ı���ɫ

void initialized();					//��ʼ������
void show();						//��ʾ����
void updateWithoutInput();			//���û��޹ص�����
void updateWithInput();				//���û��йص�����

bool isDown(Tetris & t);			//�ж��ܷ�����
bool isLeft(Tetris & t);			//�ж��ܷ�����
bool isRight(Tetris & t);			//�ж��ܷ�����
bool isDeformation(Tetris & t);		//�ж��ܷ����
Tetris createTetris();				//�����������

#endif