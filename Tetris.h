#pragma once
#include <graphics.h>
#include <vector>
#include "Block.h"
using namespace std;

struct ScoreEntry {
	int score;
};
class Tetris
{
public:
	//���캯��
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init();//��ʼ��
	void play();//��ʼ��Ϸ
	void startGame();
	


private://˽�з���
	void keyEvent();
	void updateWindow();

	//���ؾ�����һ�ε�����������������ms
	//��һ�ε��øú���������0
	int getDelay();
	void drop();
	void clearLine();
	void moveLeftRight(int offset);//Block.h����Ҳ��
	void rotate();//��ת
	void drawScore();//���Ƶ�ǰ����
	void checkOver();//�����Ϸ�Ƿ����
	void saveScore();//������߷�
	void displayOver();//������Ϸ��������
	void pause();
	//void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);
	void button(int x, int y, TCHAR* text);//��װ��ť���ƺ���
	void startAgain();  



private://˽������
	int delay;
	bool update;//�Ƿ����
	int rows;//��Ϸ��ͼ����
	int cols;//��Ϸ��ͼ����
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	//int map[20][20];//map�Ǳ�����������ı�ʱ���������Ϸ��ͼ��������
	//c++��ʽ
	vector<vector<int>> map; //vector�ǳ��ȿ��Զ�̬�仯������,������vector�Ķ�ά����
	//0���հף�û���κη���
	//5:��5�ֶ���˹����
	Block* curBlock;//��ǰ����
	Block* nextBlock;//Ԥ�淽��
	Block backBlock;//��ǰ���齵������У�����������һ���Ϸ�λ��
	//���÷���,���Ͳ�����ָ���ˣ���Ϊ���÷���Ҫָ��=curBlock��
	//�����ָ�룬��ʱ��curBlock���ˣ�����backBlock����Ҳ���

	int score;//��ǰ�ķ���
	int level;//��ǰ�ؿ�
	int lineCount;//��ǰ�Ѿ������˼���
	int highestScore;//����ģʽ��߷�
	bool gameOver;//��Ϸ�Ƿ����״̬
	bool clear;//�Ƿ�������
	IMAGE imgOver;//easyx-IMAGEͼ�����
	IMAGE imgWin;
	IMAGE imgPause;
	IMAGE imgClear[12] = { 0 };
	IMAGE imgBegin;
	int returnLines[20];//������Щ�б�����������������Ч
	bool isButtonHovered; // ���ڱ������Ƿ���ͣ�ڰ�ť��
	int hoveredButtonIndex;//���ڼ�¼��ͣ�İ�ť����,�ж���Ӧ�ý�����ģʽ���ǽ��޾�ģʽ
	bool endless;//�Ƿ����޾�ģʽ
	int endlesshScore;//�޾�ģʽ��߷�
};
