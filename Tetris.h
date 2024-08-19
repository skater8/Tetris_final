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
	//构造函数
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init();//初始化
	void play();//开始游戏
	void startGame();
	


private://私有方法
	void keyEvent();
	void updateWindow();

	//返回距离上一次调用这个函数间隔多少ms
	//第一次调用该函数，返回0
	int getDelay();
	void drop();
	void clearLine();
	void moveLeftRight(int offset);//Block.h里面也有
	void rotate();//旋转
	void drawScore();//绘制当前分数
	void checkOver();//检查游戏是否结束
	void saveScore();//保存最高分
	void displayOver();//绘制游戏结束画面
	void pause();
	//void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);
	void button(int x, int y, TCHAR* text);//封装按钮绘制函数
	void startAgain();  



private://私有数据
	int delay;
	bool update;//是否更新
	int rows;//游戏地图的行
	int cols;//游戏地图的列
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	//int map[20][20];//map是变量名，需求改变时不方便改游戏地图的行列数
	//c++方式
	vector<vector<int>> map; //vector是长度可以动态变化的数组,这里是vector的二维数组
	//0：空白，没有任何方块
	//5:第5种俄罗斯方块
	Block* curBlock;//当前方块
	Block* nextBlock;//预告方块
	Block backBlock;//当前方块降落过程中，用来备份上一个合法位置
	//备用方块,类型不能用指针了，因为备用方块要指向=curBlock的
	//如果是指针，到时候curBlock改了，备用backBlock方块也会变

	int score;//当前的分数
	int level;//当前关卡
	int lineCount;//当前已经消除了几行
	int highestScore;//闯关模式最高分
	bool gameOver;//游戏是否结束状态
	bool clear;//是否有消行
	IMAGE imgOver;//easyx-IMAGE图像对象
	IMAGE imgWin;
	IMAGE imgPause;
	IMAGE imgClear[12] = { 0 };
	IMAGE imgBegin;
	int returnLines[20];//存有那些行被消除，用于消除特效
	bool isButtonHovered; // 用于标记鼠标是否悬停在按钮上
	int hoveredButtonIndex;//用于记录悬停的按钮索引,判断是应该进闯关模式还是进无尽模式
	bool endless;//是否是无尽模式
	int endlesshScore;//无尽模式最高分
};
